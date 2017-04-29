/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：StepperMotor.c
*文件标识：
*摘	   要：对步进电机及其刹车资源初始化，并提供驱动函数		 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/9
*****************************************************************************/
#include "StepperMotor.h"
#include "math.h"
#include "stm32f10x.h"
#include "usart.h"


static u16 gs_pAccPrescTab[PUSH_ACC_NUM] = {0};//推夹具电机加/减速预分频表
static u16 gs_mcAccPrescTab[MC_ACC_NUM] = {0};//中间夹线电机加/减速预分频表
static u16 gs_scAccPrescTab[SC_ACC_NUM] = {0};//两边夹线电机加/减速预分频表

u32 g_pStps = 0;//推夹具电机需要运转的步数
u16 g_tpPresc = 0;//推夹具电机目标速度对应的预分频数
u8  g_npActFlg = 0; //推夹具电机新动作标志位
u8  g_pActDFlg = 0; //推夹具电机动作结束标志位
u8  g_pClkFlg  = 0; //推夹具电机脉冲输出使能标志位

u32 g_mcStps = 0;//中间夹线电机需要运转的步数
u16 g_tmcPresc = 0;//中间夹线电机目标速度对应的预分频数
u8  g_nmcActFlg = 0; //中间夹线电机新动作标志位
u8  g_mcActDFlg = 0; //中间夹线电机动作结束标志位
u8  g_mcClkFlg  = 0; //中间夹线电机脉冲输出使能标志位

u32 g_scStps = 0;//两边夹线电机需要运转的步数
u16 g_tscPresc = 0;//两边夹线电机目标速度对应的预分频数
u8  g_nscActFlg = 0; //两边夹线电机新动作标志位
u8  g_scActDFlg = 0; //两边夹线电机动作结束标志位
u8  g_scClkFlg  = 0; //两边夹线电机脉冲输出使能标志位



/*******************************************************************************
*函数名称：StepMotorGPIOInit
*函数说明：控制步进电机的GPIO配置 
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void StepMotorGPIOInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; //外部中断配置结参数
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA
						  | RCC_APB2Periph_GPIOB, ENABLE); // 使能PA/PB端口时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	// 推夹具电机 CLK - PA1  DIR - PA2   EN  - PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
		
	// 左右夹持电机 CLK - PB0   DIR - PB3   EN - PB4  (刹车左)BARKE - PB6   (刹车右)BARKE - PB7
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 中夹持电机 CLK - PA8   DIR - PA11   EN - PA12   (刹车)BARKE - PB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //内部上拉
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//photogate IRQ
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB ,GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB ,GPIO_PinSource6);
	//外部触发设置，边沿
	EXTI_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line6 ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
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
	
 	//推夹具电机时钟配置 TIM2 - CH2 - PA1
	TIM_TimeBaseStructure.TIM_Period = DEFAULT_PERIOD - 1;	 //PWM周期
	TIM_TimeBaseStructure.TIM_Prescaler = DEFAULT_PRESCALE - 1;  //设置预分频
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
	
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	//GPIO_SetBits(GPIOA,GPIO_Pin_12);
	
	//依次更新预分频表中的每一项 按照 f = fmin + (fmax - fmin)/(1 + e^(-a(i/num -1)))
	//进行加减速，此处将f转化到了预分频上，因此公式有变型
	TIFreq = 72000000.0 / DEFAULT_PERIOD /DEFAULT_PRESCALE;//时钟中断频率计算
	
	//更新主推杆电机加减速预分频表
	prescMin =  TIFreq / (P_MAX_SPD * 1.0 / 60 * P_MOTOR_DIV);//计算设定的最大最小预分频
	prescMax = TIFreq /  (P_MIN_SPD * 1.0 / 60 * P_MOTOR_DIV);
	for (i=0; i<PUSH_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-PS_COEFF * (PA_COEFF * i / PUSH_ACC_NUM - 1)));
		gs_pAccPrescTab[i] = (u16)((1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin)) + 0.5);
		//printf("pPrescTab[%d]:%d\n",i,gs_pAccPrescTab[i]);
	}
	
	//更新中间夹线电机加减速预分频表
	prescMin =  TIFreq / (MC_MAX_SPD * 1.0 / 60 * MC_MOTOR_DIV);//计算设定的最大最小预分频
	prescMax = TIFreq /  (MC_MIN_SPD * 1.0 / 60 * MC_MOTOR_DIV);
	for (i=0; i<MC_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-MCS_COEFF * (MCA_COEFF * i / MC_ACC_NUM - 1)));
		gs_mcAccPrescTab[i] = (u16)(1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin) + 0.5);
		//printf("mcPrescTab[%d]:%d\n",i,gs_mcAccPrescTab[i]);
	}	

	//更新两边夹线电机加减速预分频表
	prescMin =  TIFreq / (SC_MAX_SPD * 1.0 / 60 * SC_MOTOR_DIV);//计算设定的最大最小预分频
	prescMax = TIFreq /  (SC_MIN_SPD * 1.0 / 60 * SC_MOTOR_DIV);
	for (i=0; i<SC_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-SCS_COEFF * (SCA_COEFF * i / SC_ACC_NUM - 1)));
		gs_scAccPrescTab[i] = (u16)(1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin) + 0.5);
		//printf("scPrescTab[%d]:%d\n",i,gs_scAccPrescTab[i]);
	}
	
}

/*******************************************************************************
*函数名称：MotorEN
*函数说明：步进电机使能
*输入参数：motor : P推线电机、M中间夹线电机、S两边夹线电机
		   oper: E enable ; D disable
*输出参数：无
*返回参数：无 
*******************************************************************************/
void MotorEN(u8 motor,u8 oper)
{
	switch(motor)
	{
		case 'P':
		{
			if (oper == 'D')
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_1);
			}
			else if (oper == 'E')
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
			}
			break;
		}//推夹具电机
		
		case 'M':
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
		}//中间夹持电机
		
		case 'S':
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
		}//两边夹持电机	
		default:break;
	}
}

/*******************************************************************************
*函数名称：MotorDir
*函数说明：步进电机转向
*输入参数：motor : P推线电机、M中间夹线电机、S两边夹线电机
		   oper: + 正向 ; -反向；规定面向电机出轴，顺时针为正
*输出参数：无
*返回参数：无 
*******************************************************************************/
void MotorDir(u8 motor,u8 oper)
{
	switch(motor)
	{
		case 'P':
		{
			if (oper == '+')
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_2);
			}
			else if (oper == '-')
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			}
			break;
		}//推夹具电机
		
		case 'M':
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
		}//中间夹持电机
		
		case 'S':
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
		}//两边夹持电机	
		default:break;
	}
}

/*******************************************************************************
*函数名称：PushMotion
*函数说明：推夹具步进电机动作调用接口
*输入参数：angleDeg : 步进电机需要转过的度数 单位°
		   dir: + 正向 ; -反向；规定面向电机出轴，顺时针为正
		   spd:速度，单位r/min
*输出参数：无
*返回参数：无 
*******************************************************************************/
void PushMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //定时器产生中断的频率
	float spdFreq = 0; //达到相应速度需要的脉冲频率
	
	MotorEN('P','E');//使能推夹具电机
	MotorDir('P',dir);//设定转向
	
	//速度限制
	if (spd > P_MAX_SPD)
	{
		spd = P_MAX_SPD;
	}
	else if (spd < P_MIN_SPD)
	{
		spd = P_MIN_SPD;
	}
	
	//更新全局变量
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//定时器中断频率
	spdFreq = spd * 1.0 / 60 * P_MOTOR_DIV;//达到指定速度需要的脉冲频率	
	
	g_tpPresc = (u16)( TIFreq / spdFreq  +0.5);//更新最大速度对应的预分频值		
	g_pStps = (u32)((angleDeg * 1.0 / 360 * P_MOTOR_DIV)+0.5);//更新待运动的步数
	g_npActFlg = 1;
	g_pActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
	//printf("psteps: %d\n",g_pStps);
 	//printf("pPrec: %d\n",g_tpPresc);

}

/*******************************************************************************
*函数名称：MCMotion
*函数说明：中间夹线步进电机动作调用接口
*输入参数：angleDeg : 步进电机需要转过的度数 单位°
		   dir: + 正向 ; -反向；规定面向电机出轴，顺时针为正
		   spd:速度，单位r/min
*输出参数：无
*返回参数：无 
*******************************************************************************/
void MCMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //定时器产生中断的频率
	float spdFreq = 0; //达到相应速度需要的脉冲频率	
	
	MotorEN('M','E');//使能推夹具电机
	MotorDir('M',dir);//设定转向
	
	//速度限制
	if (spd > MC_MAX_SPD)
	{
		spd = MC_MAX_SPD;
	}
	else if (spd < MC_MIN_SPD)
	{
		spd = MC_MIN_SPD;
	}
	
	//更新全局变量	
	//更新全局变量
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//定时器中断频率
	spdFreq = spd * 1.0 / 60 * MC_MOTOR_DIV;//达到指定速度需要的脉冲频率	
	
	g_tmcPresc = (u16)( TIFreq / spdFreq  +0.5);//更新最大速度对应的预分频值		
	g_mcStps = (u32)((angleDeg * 1.0 / 360 * MC_MOTOR_DIV)+0.5);//更新待运动的步数
	g_nmcActFlg = 1;
	g_mcActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
 	//printf("mcSteps: %d\n",g_mcStps);
 	//printf("mcPrec: %d\n",g_tmcPresc);
	
	
}

/*******************************************************************************
*函数名称：SCMotion
*函数说明：两边夹线步进电机动作调用接口
*输入参数：angleDeg : 步进电机需要转过的度数 单位°
		   dir: + 正向 ; -反向；规定面向电机出轴，顺时针为正
		   spd:速度，单位r/min
*输出参数：无
*返回参数：无 
*******************************************************************************/
void SCMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //定时器产生中断的频率
	float spdFreq = 0; //达到相应速度需要的脉冲频率	
	
	MotorEN('S','E');//使能推夹具电机
	MotorDir('S',dir);//设定转向
	
	//速度限制
	if (spd > SC_MAX_SPD)
	{
		spd = SC_MAX_SPD;
	}
	else if (spd < SC_MIN_SPD)
	{
		spd = SC_MIN_SPD;
	}
	
	//更新全局变量
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//定时器中断频率
	spdFreq = spd * 1.0 / 60 * SC_MOTOR_DIV;//达到指定速度需要的脉冲频率	
	
	g_tscPresc = (u16)( TIFreq / spdFreq  +0.5);//更新最大速度对应的预分频值		
	g_scStps = (u32)((angleDeg * 1.0 / 360 * SC_MOTOR_DIV)+0.5);//更新待运动的步数
	g_nscActFlg = 1;
	g_scActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
 	//printf("scSteps: %d\n",g_scStps);
 	//printf("scPrec: %d\n",g_tscPresc);
}

/*******************************************************************************
*函数名称：PMClkGen
*函数说明：基于时钟中断产生脉冲输出，用于推夹具电机
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void PMClkGen(void)
{
	static u16 s_clkCnt = 0;
	static u16 s_presc = P_DEFAULT_PRE;
	
	s_clkCnt ++;
	
	if (g_pClkFlg == 1)
	{
		if (s_clkCnt > (s_presc>>1))
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
		}
		else
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			return;
		}			
	}
	else
	{;}
	
	if (s_clkCnt == s_presc)
	{
		s_presc = PushMotorDrive();
		s_clkCnt = 0;
	}
}

/*******************************************************************************
*函数名称：MCMClkGen
*函数说明：基于时钟中断产生脉冲输出，用于中间夹线电机
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void MCMClkGen(void)
{
	static u16 s_clkCnt = 0;
	static u16 s_presc = MC_DEFAULT_PRE;
	
	s_clkCnt ++;
	
	if (g_mcClkFlg == 1)
	{
		if (s_clkCnt > (s_presc>>1))
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
	
	if (s_clkCnt == s_presc)
	{
		s_presc = MCMotorDrive();
		s_clkCnt = 0;
	}
}

/*******************************************************************************
*函数名称：SCMClkGen
*函数说明：基于时钟中断产生脉冲输出，用于两边夹线电机
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void SCMClkGen(void)
{
	static u16 s_clkCnt = 0;
	static u16 s_presc = SC_DEFAULT_PRE;

	s_clkCnt ++;
	
	if (g_scClkFlg == 1)
	{
		if (s_clkCnt >(s_presc>>1))
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
	
	if (s_clkCnt == s_presc )
	{
		s_presc = SCMotorDrive();
		s_clkCnt = 0;
	}

}

/*******************************************************************************
*函数名称：PushMotorDrive
*函数说明：基于pmClkGen函数生成的脉冲驱动电机
*输入参数：无
*输出参数：用于调节脉冲频率的分频
*返回参数：无 
*******************************************************************************/
u16 PushMotorDrive(void)
{
	static u16 s_currPresc = DEFAULT_PRESCALE ;
	static u8 s_state = 0;
	static u16 s_accCnt = 0;
	static u16 s_maxAccNum = 0;
		
	if (g_npActFlg == 1)
	{
		s_accCnt = 0;
		s_state = 0;
		g_npActFlg = 0;
	}
	else
	{;}//如果发现是新动作，则更新运行的状态
	
	//printf("s : %d\n",s_state);
	switch (s_state)
	{
		case 0:
		{
			if (g_pStps > 2*PUSH_ACC_NUM)
			{
				s_maxAccNum = PUSH_ACC_NUM;
			}
			else 
			{
				s_maxAccNum = g_pStps / 2;
			}
			
			s_accCnt = 0;
			s_currPresc = gs_pAccPrescTab[0];
			s_state = 1;
			
			if (g_pStps !=0)
			{
				g_pClkFlg = 1;
			}				
			//break;
		}
		case 1:
		{
			
			if (g_tpPresc < s_currPresc && s_accCnt < s_maxAccNum)
			{
				s_currPresc = gs_pAccPrescTab[s_accCnt];
				g_pStps--;
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
			if (g_pStps > s_accCnt)
			{
				g_pStps --;
				break;				
			}
			else
			{
				s_state = 3;
			}		
		}
		case 3:
		{
			if (g_pStps != 0)
			{
				g_pStps --;
				s_accCnt--;
				s_currPresc = gs_pAccPrescTab[s_accCnt];
				break;
			}
			else
			{
				g_pActDFlg = 1;
				g_pClkFlg = 0;
			}
		}
	}
	return s_currPresc;
}

/*******************************************************************************
*函数名称：MCMotorDrive
*函数说明：基于mcmClkGen函数生成的脉冲驱动电机
*输入参数：无
*输出参数：用于调节脉冲频率的分频
*返回参数：无 
*******************************************************************************/
u16 MCMotorDrive(void)
{
	static u16 s_currPresc = MC_DEFAULT_PRE ;
	static u8 s_state = 0;
	static u16 s_accCnt = 0;
	static u16 s_maxAccNum = 0;

	
	if (g_nmcActFlg == 1)
	{
		s_accCnt = 0;
		s_state = 0;
		g_nmcActFlg = 0;
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
			if (g_mcStps > 2*MC_ACC_NUM)
			{
				s_maxAccNum = MC_ACC_NUM;
			}
			else 
			{
				s_maxAccNum = g_mcStps / 2;
			}
			
			s_accCnt = 0;
			s_currPresc = gs_mcAccPrescTab[0];
			s_state = 1;
			
			if (g_mcStps !=0)
			{
				g_mcClkFlg = 1;
			}				
			//break;
		}
		case 1:
		{
			
			if (g_tmcPresc < s_currPresc && s_accCnt < s_maxAccNum)
			{
				s_currPresc = gs_mcAccPrescTab[s_accCnt];
				g_mcStps--;
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
			if (g_mcStps > s_accCnt)
			{
				g_mcStps --;
				break;				
			}
			else
			{
				s_state = 3;
			}		
		}
		case 3:
		{
			if (g_mcStps != 0)
			{
				g_mcStps --;
				s_accCnt--;
				s_currPresc = gs_mcAccPrescTab[s_accCnt];
				break;
			}
			else
			{
				g_mcActDFlg = 1;
				g_mcClkFlg = 0;
			}
		}
	}
	return s_currPresc;	
}

/*******************************************************************************
*函数名称：SCMotorDrive
*函数说明：基于scmClkGen函数生成的脉冲驱动电机
*输入参数：无
*输出参数：用于调节脉冲频率的分频
*返回参数：无 
*******************************************************************************/
u16 SCMotorDrive(void)
{
	static u16 s_currPresc = SC_DEFAULT_PRE;
	static u8 s_state = 0;
	static u16 s_accCnt = 0;
	static u16 s_maxAccNum = 0;
	
	
	if (g_nscActFlg == 1)
	{
		s_accCnt = 0;
		s_state = 0;
		g_nscActFlg = 0;
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
			if (g_scStps > 2*SC_ACC_NUM)
			{
				s_maxAccNum = SC_ACC_NUM;
			}
			else 
			{
				s_maxAccNum = g_scStps / 2;
			}
			
			s_accCnt = 0;
			s_currPresc = gs_scAccPrescTab[0];
			s_state = 1;
			
			if (g_scStps !=0)
			{
				g_scClkFlg = 1;
			}				
			//break;
		}
		case 1:
		{
			
			if (g_tscPresc < s_currPresc && s_accCnt < s_maxAccNum)
			{
				s_currPresc = gs_scAccPrescTab[s_accCnt];
				g_scStps--;
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
			if (g_scStps > s_accCnt)
			{
				g_scStps --;
				break;				
			}
			else
			{
				s_state = 3;
			}		
		}
		case 3:
		{
			if (g_scStps != 0)
			{
				g_scStps --;
				s_accCnt--;
				s_currPresc = gs_scAccPrescTab[s_accCnt];
				break;
			}
			else
			{
				g_scActDFlg = 1;
				g_scClkFlg = 0;
			}
		}
	}
	return s_currPresc;	
}

/*******************************************************************************
*函数名称：IsMotActDone
*函数说明：步进电机动作是否完成标志位
*输入参数：motor P主推杆电机，M中间夹持电机，S两边加持电机
*输出参数：无
*返回参数：完成状态 1 完成，0 未完成 
*******************************************************************************/
u8 IsMotActDone(u8 motor)
{
	switch (motor)
	{
		case 'P':return g_pActDFlg;
		case 'M':return g_mcActDFlg;
		case 'S':return g_scActDFlg;
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
	
	PMClkGen(); //生成驱动步进电机的脉冲信号
	MCMClkGen();
	SCMClkGen();
}




void Fixture_Stop(void){
	//MotorEN('P','D');
	//MotorEN('M','D');
	//MotorEN('S','D');
	TIM_Cmd(TIM2,DISABLE);
}

u8 Check_Position_C(void){
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
}

u8 Check_Position_M(void){
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);
}


u8 state;
u8 Position_Flag_C =0,Position_Flag_M =0;
void EXTI9_5_IRQHandler(void)
{
	//printf("handler\n");
	if(EXTI_GetITStatus(EXTI_Line5) == SET){
		state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
		if( state == 0){
			printf("Position_Flag_C = 1;\n");
			Position_Flag_C = 1;
		}else{
			printf("Position_Flag_C = 2;\n");
			Position_Flag_C = 2;
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
	if(EXTI_GetITStatus(EXTI_Line6) == SET){
		state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
		if( state == 0){
			printf("Position_Flag_M = 1;\n");
			Position_Flag_M = 1;
		}else{
			printf("Position_Flag_M = 2;\n");
			Position_Flag_M = 2;
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}

}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
