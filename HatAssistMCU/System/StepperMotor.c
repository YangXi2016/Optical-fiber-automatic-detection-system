/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：StepperMotor.c
*文件标识：
*摘	   要：对步进电机及其刹车资源初始化，并提供驱动函数		 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/14
*****************************************************************************/
#include "StepperMotor.h"
#include "math.h"
#include "stm32f10x.h"
#include "usart.h"


static u16 gs_hAccPrescTab[H_ACC_NUM] = {0};//戴帽电机加/减速预分频表
static u16 gs_mtAccPrescTab[MT_ACC_NUM] = {0};//主平移台电机加/减速预分频表



u32 g_hStps = 0;//中间夹线电机需要运转的步数
u16 g_thPresc = 0;//中间夹线电机目标速度对应的预分频数
u8  g_nhActFlg = 0; //中间夹线电机新动作标志位
u8  g_hActDFlg = 0; //中间夹线电机动作结束标志位
u8  g_hClkFlg  = 0; //中间夹线电机脉冲输出使能标志位

u32 g_mtStps = 0;//两边夹线电机需要运转的步数
u16 g_tmtPresc = 0;//两边夹线电机目标速度对应的预分频数
u8  g_nmtActFlg = 0; //两边夹线电机新动作标志位
u8  g_mtActDFlg = 0; //两边夹线电机动作结束标志位
u8  g_mtClkFlg  = 0; //两边夹线电机脉冲输出使能标志位



/*******************************************************************************
*函数名称：StepMotorGPIOInit
*函数说明：控制步进电机的GPIO配置 
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void StepMotorGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA
						  | RCC_APB2Periph_GPIOB, ENABLE); // 使能PC端口时钟
	
	// 主平移台电机 CLK - PB0   DIR - PB3   EN - PB4  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 戴帽电机 CLK - PA8   DIR - PA11   EN - PA12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/*******************************************************************************
*函数名称：StepMotorTIMInit
*函数说明：控制步进电机的定时器、中断配置 
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void StepMotorTIMInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
 	//电机时钟配置 TIM2
	TIM_TimeBaseStructure.TIM_Period = DEFAULT_PERIOD - 1;	 
	TIM_TimeBaseStructure.TIM_Prescaler = DEFAULT_PRESCALE - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig( TIM2, TIM_IT_Update  |  TIM_IT_Trigger,  ENABLE);//开启TIM2的中断源和触发中断

	//配置定时器中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM1~3全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1，优先级次高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 


}

/*******************************************************************************
*函数名称：StepMotorInit
*函数说明：步进电机GPIO、定时器初始化，加速表初始化
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void StepMotorInit(void)
{
	u16 i = 0;
	double prescMin = 0;
	double prescMax = 0;
	double t_sx = 0; //
	double TIFreq = 0;
		
	StepMotorGPIOInit();//GPIO初始化
	StepMotorTIMInit();//定时器和中断初始化
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);

	
	//依次更新预分频表中的每一项 按照 f = fmin + (fmax - fmin)/(1 + e^(-a(i/num -1)))
	//进行加减速，此处将f转化到了预分频上，因此公式有变型
	TIFreq = 72000000.0 / DEFAULT_PERIOD /DEFAULT_PRESCALE;//时钟中断频率计算
	
	//更新戴帽电机加减速预分频表
	prescMin =  TIFreq / (H_MAX_SPD * 1.0 / 60 * H_MOTOR_DIV);//计算设定的最大最小预分频
	prescMax = TIFreq /  (H_MIN_SPD * 1.0 / 60 * H_MOTOR_DIV);
	for (i=0; i<H_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-HS_COEFF * (HA_COEFF * i / H_ACC_NUM - 1)));
		gs_hAccPrescTab[i] = (u16)(1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin) + 0.5);
		printf("hPrescTab[%d]:%d\n",i,gs_hAccPrescTab[i]);
	}	

	//更新主平移台电机加减速预分频表
	prescMin =  TIFreq / (MT_MAX_SPD * 1.0 / 60 * MT_MOTOR_DIV);//计算设定的最大最小预分频
	prescMax = TIFreq /  (MT_MIN_SPD * 1.0 / 60 * MT_MOTOR_DIV);
	for (i=0; i<MT_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-MTS_COEFF * (MTA_COEFF * i / MT_ACC_NUM - 1)));
		gs_mtAccPrescTab[i] = (u16)(1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin) + 0.5);
		printf("mtPrescTab[%d]:%d\n",i,gs_mtAccPrescTab[i]);
	}
	
}

/*******************************************************************************
*函数名称：MotorEN
*函数说明：步进电机使能
*输入参数：motor : H戴帽、T主平移台
		   oper: E enable ; D disable
*输出参数：无
*返回参数：无 
*******************************************************************************/
void MotorEN(u8 motor,u8 oper)
{
	switch(motor)
	{
		
		case 'H':
		{
			if (oper == 'D')
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_12);
			}
			else if (oper == 'E')
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);	
			}
			break;			
		}//戴帽电机
		
		case 'T':
		{
			if (oper == 'D')
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_4);
			}
			else if (oper == 'E')
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_4);	
			}
			break;			
		}//平移台电机
		default:break;
	}
}

/*******************************************************************************
*函数名称：MotorDir
*函数说明：步进电机转向
*输入参数：motor : H戴帽、T主平移台
		   oper: + 正向 ; -反向；规定面向电机出轴，顺时针为正
*输出参数：无
*返回参数：无 
*******************************************************************************/
void MotorDir(u8 motor,u8 oper)
{
	switch(motor)
	{
		case 'H':
		{
			if (oper == '+')
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_11);
			}
			else if (oper == '-')
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_11);	
			}
			break;			
		}
		
		case 'T':
		{
			if (oper == '+')
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_3);
			}
			else if (oper == '-')
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_3);	
			}
			break;			
		}
		default:break;
	}
}

/*******************************************************************************
*函数名称：HMotion
*函数说明：戴帽步进电机动作调用接口
*输入参数：angleDeg : 步进电机需要转过的度数 单位°
		   dir: + 正向 ; -反向；规定面向电机出轴，顺时针为正
		   spd:速度，单位r/min
*输出参数：无
*返回参数：无 
*******************************************************************************/
void HMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //定时器产生中断的频率
	float spdFreq = 0; //达到相应速度需要的脉冲频率	
	
	MotorEN('H','E');//使能推夹具电机
	MotorDir('H',dir);//设定转向
	
	//速度限制
	if (spd > H_MAX_SPD)
	{
		spd = H_MAX_SPD;
	}
	else if (spd < H_MIN_SPD)
	{
		spd = H_MIN_SPD;
	}
	
	//更新全局变量	
	//更新全局变量
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//定时器中断频率
	spdFreq = spd * 1.0 / 60 * H_MOTOR_DIV;//达到指定速度需要的脉冲频率	
	
	g_thPresc = (u16)( TIFreq / spdFreq  +0.5);//更新最大速度对应的预分频值		
	g_hStps = (u32)((angleDeg * 1.0 / 360 * H_MOTOR_DIV)+0.5);//更新待运动的步数
	g_nhActFlg = 1;
	g_hActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
 	printf("hSteps: %d\n",g_hStps);
 	printf("hPrec: %d\n",g_thPresc);
}

/*******************************************************************************
*函数名称：MTMotion
*函数说明：主平移台步进电机动作调用接口
*输入参数：angleDeg : 步进电机需要转过的度数 单位°
		   dir: + 正向 ; -反向；规定面向电机出轴，顺时针为正
		   spd:速度，单位r/min
*输出参数：无
*返回参数：无 
*******************************************************************************/
void MTMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //定时器产生中断的频率
	float spdFreq = 0; //达到相应速度需要的脉冲频率	
	
	MotorEN('T','E');//使能推夹具电机
	MotorDir('T',dir);//设定转向
	
	//速度限制
	if (spd > MT_MAX_SPD)
	{
		spd = MT_MAX_SPD;
	}
	else if (spd < MT_MIN_SPD)
	{
		spd = MT_MIN_SPD;
	}
	
	//更新全局变量
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//定时器中断频率
	spdFreq = spd * 1.0 / 60 * MT_MOTOR_DIV;//达到指定速度需要的脉冲频率	
	
	g_tmtPresc = (u16)( TIFreq / spdFreq  +0.5);//更新最大速度对应的预分频值		
	g_mtStps = (u32)((angleDeg * 1.0 / 360 * MT_MOTOR_DIV)+0.5);//更新待运动的步数
	g_nmtActFlg = 1;
	g_mtActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
 	printf("mtSteps: %d\n",g_mtStps);
 	printf("mtPrec: %d\n",g_tmtPresc);
}

/*******************************************************************************
*函数名称：MTMClkGen
*函数说明：基于时钟中断产生脉冲输出，用于主平移台
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void MTMClkGen(void)
{
	static u16 s_clkCnt = 0;
	static u16 s_presc = MT_DEFAULT_PRE;
	
	s_clkCnt ++;
	
	if (g_mtClkFlg == 1)
	{
		if (s_clkCnt > (s_presc>>1))
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			return;
		}			
	}
	else
	{;}
	
	if (s_clkCnt == s_presc)
	{
		s_presc = MTMotorDrive();
		s_clkCnt = 0;
	}
}

/*******************************************************************************
*函数名称：HMClkGen
*函数说明：基于时钟中断产生脉冲输出，用于戴帽电机
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void HMClkGen(void)
{
	static u16 s_clkCnt = 0;
	static u16 s_presc = H_DEFAULT_PRE;

	s_clkCnt ++;
	
	if (g_hClkFlg == 1)
	{
		if (s_clkCnt >(s_presc>>1))
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
		}
		else
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			return;
		}			
	}
	else
	{;}
	
	if (s_clkCnt == s_presc )
	{
		s_presc = HMotorDrive();
		s_clkCnt = 0;
	}

}

/*******************************************************************************
*函数名称：HMotorDrive
*函数说明：基于hmClkGen函数生成的脉冲驱动电机
*输入参数：无
*输出参数：用于调节脉冲频率的分频
*返回参数：无 
*******************************************************************************/
u16 HMotorDrive(void)
{
	static u16 s_currPresc = H_DEFAULT_PRE ;
	static u8 s_state = 0;
	static u16 s_accCnt = 0;
	static u16 s_maxAccNum = 0;

	
	if (g_nhActFlg == 1)
	{
		s_accCnt = 0;
		s_state = 0;
		g_nhActFlg = 0;
	}
	else
	{
		;
	}//如果发现是新动作，则更新运行的状态
	
	//printf("s : %d\n",s_state);
	switch (s_state)
	{
		case 0:
		{
			if (g_hStps > 2*H_ACC_NUM)
			{
				s_maxAccNum = H_ACC_NUM;
			}
			else 
			{
				s_maxAccNum = g_hStps / 2;
			}
			
			s_accCnt = 0;
			s_currPresc = gs_hAccPrescTab[0];
			s_state = 1;
			
			if (g_hStps !=0)
			{
				g_hClkFlg = 1;
			}				
			//break;
		}
		case 1:
		{
			
			if (g_thPresc < s_currPresc && s_accCnt < s_maxAccNum)
			{
				s_currPresc = gs_hAccPrescTab[s_accCnt];
				g_hStps--;
				s_accCnt++;
				break;				
			}
			else
			{
				s_state = 2;
			}
		}
		case 2:
		{
			if (g_hStps > s_accCnt)
			{
				g_hStps --;
				break;				
			}
			else
			{
				s_state = 3;
			}		
		}
		case 3:
		{
			if (g_hStps != 0)
			{
				g_hStps --;
				s_accCnt--;
				s_currPresc = gs_hAccPrescTab[s_accCnt];
				break;
			}
			else
			{
				g_hActDFlg = 1;
				g_hClkFlg = 0;
			}
		}
	}
	return s_currPresc;	
}

/*******************************************************************************
*函数名称：MTMotorDrive
*函数说明：基于mtmClkGen函数生成的脉冲驱动电机
*输入参数：无
*输出参数：用于调节脉冲频率的分频
*返回参数：无 
*******************************************************************************/
u16 MTMotorDrive(void)
{
	static u16 s_currPresc = MT_DEFAULT_PRE;
	static u8 s_state = 0;
	static u16 s_accCnt = 0;
	static u16 s_maxAccNum = 0;
	
	
	if (g_nmtActFlg == 1)
	{
		s_accCnt = 0;
		s_state = 0;
		g_nmtActFlg = 0;
	}
	else
	{
		;
	}//如果发现是新动作，则更新运行的状态
	
	//printf("s : %d\n",s_state);
	switch (s_state)
	{
		case 0:
		{
			if (g_mtStps > 2*MT_ACC_NUM)
			{
				s_maxAccNum = MT_ACC_NUM;
			}
			else 
			{
				s_maxAccNum = g_mtStps / 2;
			}
			
			s_accCnt = 0;
			s_currPresc = gs_mtAccPrescTab[0];
			s_state = 1;
			
			if (g_mtStps !=0)
			{
				g_mtClkFlg = 1;
			}				
			//break;
		}
		case 1:
		{
			
			if (g_tmtPresc < s_currPresc && s_accCnt < s_maxAccNum)
			{
				s_currPresc = gs_mtAccPrescTab[s_accCnt];
				g_mtStps--;
				s_accCnt++;
				break;				
			}
			else
			{
				s_state = 2;
			}
		}
		case 2:
		{
			if (g_mtStps > s_accCnt)
			{
				g_mtStps --;
				break;				
			}
			else
			{
				s_state = 3;
			}		
		}
		case 3:
		{
			if (g_mtStps != 0)
			{
				g_mtStps --;
				s_accCnt--;
				s_currPresc = gs_mtAccPrescTab[s_accCnt];
				break;
			}
			else
			{
				g_mtActDFlg = 1;
				g_mtClkFlg = 0;
			}
		}
	}
	return s_currPresc;	
}

/*******************************************************************************
*函数名称：IsMotActDone
*函数说明：步进电机动作是否完成标志位
*输入参数：motor H戴帽电机，T主平移台电机
*输出参数：无
*返回参数：完成状态 1 完成，0 未完成 
*******************************************************************************/
u8 IsMotActDone(u8 motor)
{
	switch (motor)
	{
		case 'H':return g_hActDFlg;
		case 'T':return g_mtActDFlg;
		default:return 0;		
	}
}

/*******************************************************************************
*函数名称：TIM2_IRQHandler
*函数说明：TIM2中断，用于产生时钟信号驱动步进电机
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
	else
	{
		return;
	}//确保是指定中断源触发中断
	

	HMClkGen();
	MTMClkGen();
	
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
