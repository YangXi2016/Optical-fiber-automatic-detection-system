/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�StepperMotor.c
*�ļ���ʶ��
*ժ	   Ҫ���Բ����������ɲ����Դ��ʼ�������ṩ��������		 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/14
*****************************************************************************/
#include "StepperMotor.h"
#include "math.h"
#include "stm32f10x.h"
#include "usart.h"


static u16 gs_hAccPrescTab[H_ACC_NUM] = {0};//��ñ�����/����Ԥ��Ƶ��
static u16 gs_mtAccPrescTab[MT_ACC_NUM] = {0};//��ƽ��̨�����/����Ԥ��Ƶ��



u32 g_hStps = 0;//�м���ߵ����Ҫ��ת�Ĳ���
u16 g_thPresc = 0;//�м���ߵ��Ŀ���ٶȶ�Ӧ��Ԥ��Ƶ��
u8  g_nhActFlg = 0; //�м���ߵ���¶�����־λ
u8  g_hActDFlg = 0; //�м���ߵ������������־λ
u8  g_hClkFlg  = 0; //�м���ߵ���������ʹ�ܱ�־λ

u32 g_mtStps = 0;//���߼��ߵ����Ҫ��ת�Ĳ���
u16 g_tmtPresc = 0;//���߼��ߵ��Ŀ���ٶȶ�Ӧ��Ԥ��Ƶ��
u8  g_nmtActFlg = 0; //���߼��ߵ���¶�����־λ
u8  g_mtActDFlg = 0; //���߼��ߵ������������־λ
u8  g_mtClkFlg  = 0; //���߼��ߵ���������ʹ�ܱ�־λ



/*******************************************************************************
*�������ƣ�StepMotorGPIOInit
*����˵�������Ʋ��������GPIO���� 
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void StepMotorGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA
						  | RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PC�˿�ʱ��
	
	// ��ƽ��̨��� CLK - PB0   DIR - PB3   EN - PB4  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ��ñ��� CLK - PA8   DIR - PA11   EN - PA12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/*******************************************************************************
*�������ƣ�StepMotorTIMInit
*����˵�������Ʋ�������Ķ�ʱ�����ж����� 
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void StepMotorTIMInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
 	//���ʱ������ TIM2
	TIM_TimeBaseStructure.TIM_Period = DEFAULT_PERIOD - 1;	 
	TIM_TimeBaseStructure.TIM_Prescaler = DEFAULT_PRESCALE - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig( TIM2, TIM_IT_Update  |  TIM_IT_Trigger,  ENABLE);//����TIM2���ж�Դ�ʹ����ж�

	//���ö�ʱ���ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM1~3ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1�����ȼ��θ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 


}

/*******************************************************************************
*�������ƣ�StepMotorInit
*����˵�����������GPIO����ʱ����ʼ�������ٱ��ʼ��
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void StepMotorInit(void)
{
	u16 i = 0;
	double prescMin = 0;
	double prescMax = 0;
	double t_sx = 0; //
	double TIFreq = 0;
		
	StepMotorGPIOInit();//GPIO��ʼ��
	StepMotorTIMInit();//��ʱ�����жϳ�ʼ��
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);

	
	//���θ���Ԥ��Ƶ���е�ÿһ�� ���� f = fmin + (fmax - fmin)/(1 + e^(-a(i/num -1)))
	//���мӼ��٣��˴���fת������Ԥ��Ƶ�ϣ���˹�ʽ�б���
	TIFreq = 72000000.0 / DEFAULT_PERIOD /DEFAULT_PRESCALE;//ʱ���ж�Ƶ�ʼ���
	
	//���´�ñ����Ӽ���Ԥ��Ƶ��
	prescMin =  TIFreq / (H_MAX_SPD * 1.0 / 60 * H_MOTOR_DIV);//�����趨�������СԤ��Ƶ
	prescMax = TIFreq /  (H_MIN_SPD * 1.0 / 60 * H_MOTOR_DIV);
	for (i=0; i<H_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-HS_COEFF * (HA_COEFF * i / H_ACC_NUM - 1)));
		gs_hAccPrescTab[i] = (u16)(1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin) + 0.5);
		printf("hPrescTab[%d]:%d\n",i,gs_hAccPrescTab[i]);
	}	

	//������ƽ��̨����Ӽ���Ԥ��Ƶ��
	prescMin =  TIFreq / (MT_MAX_SPD * 1.0 / 60 * MT_MOTOR_DIV);//�����趨�������СԤ��Ƶ
	prescMax = TIFreq /  (MT_MIN_SPD * 1.0 / 60 * MT_MOTOR_DIV);
	for (i=0; i<MT_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-MTS_COEFF * (MTA_COEFF * i / MT_ACC_NUM - 1)));
		gs_mtAccPrescTab[i] = (u16)(1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin) + 0.5);
		printf("mtPrescTab[%d]:%d\n",i,gs_mtAccPrescTab[i]);
	}
	
}

/*******************************************************************************
*�������ƣ�MotorEN
*����˵�����������ʹ��
*���������motor : H��ñ��T��ƽ��̨
		   oper: E enable ; D disable
*�����������
*���ز������� 
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
		}//��ñ���
		
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
		}//ƽ��̨���
		default:break;
	}
}

/*******************************************************************************
*�������ƣ�MotorDir
*����˵�����������ת��
*���������motor : H��ñ��T��ƽ��̨
		   oper: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
*�����������
*���ز������� 
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
*�������ƣ�HMotion
*����˵������ñ��������������ýӿ�
*���������angleDeg : ���������Ҫת���Ķ��� ��λ��
		   dir: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
		   spd:�ٶȣ���λr/min
*�����������
*���ز������� 
*******************************************************************************/
void HMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //��ʱ�������жϵ�Ƶ��
	float spdFreq = 0; //�ﵽ��Ӧ�ٶ���Ҫ������Ƶ��	
	
	MotorEN('H','E');//ʹ���Ƽоߵ��
	MotorDir('H',dir);//�趨ת��
	
	//�ٶ�����
	if (spd > H_MAX_SPD)
	{
		spd = H_MAX_SPD;
	}
	else if (spd < H_MIN_SPD)
	{
		spd = H_MIN_SPD;
	}
	
	//����ȫ�ֱ���	
	//����ȫ�ֱ���
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//��ʱ���ж�Ƶ��
	spdFreq = spd * 1.0 / 60 * H_MOTOR_DIV;//�ﵽָ���ٶ���Ҫ������Ƶ��	
	
	g_thPresc = (u16)( TIFreq / spdFreq  +0.5);//��������ٶȶ�Ӧ��Ԥ��Ƶֵ		
	g_hStps = (u32)((angleDeg * 1.0 / 360 * H_MOTOR_DIV)+0.5);//���´��˶��Ĳ���
	g_nhActFlg = 1;
	g_hActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
 	printf("hSteps: %d\n",g_hStps);
 	printf("hPrec: %d\n",g_thPresc);
}

/*******************************************************************************
*�������ƣ�MTMotion
*����˵������ƽ��̨��������������ýӿ�
*���������angleDeg : ���������Ҫת���Ķ��� ��λ��
		   dir: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
		   spd:�ٶȣ���λr/min
*�����������
*���ز������� 
*******************************************************************************/
void MTMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //��ʱ�������жϵ�Ƶ��
	float spdFreq = 0; //�ﵽ��Ӧ�ٶ���Ҫ������Ƶ��	
	
	MotorEN('T','E');//ʹ���Ƽоߵ��
	MotorDir('T',dir);//�趨ת��
	
	//�ٶ�����
	if (spd > MT_MAX_SPD)
	{
		spd = MT_MAX_SPD;
	}
	else if (spd < MT_MIN_SPD)
	{
		spd = MT_MIN_SPD;
	}
	
	//����ȫ�ֱ���
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//��ʱ���ж�Ƶ��
	spdFreq = spd * 1.0 / 60 * MT_MOTOR_DIV;//�ﵽָ���ٶ���Ҫ������Ƶ��	
	
	g_tmtPresc = (u16)( TIFreq / spdFreq  +0.5);//��������ٶȶ�Ӧ��Ԥ��Ƶֵ		
	g_mtStps = (u32)((angleDeg * 1.0 / 360 * MT_MOTOR_DIV)+0.5);//���´��˶��Ĳ���
	g_nmtActFlg = 1;
	g_mtActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
 	printf("mtSteps: %d\n",g_mtStps);
 	printf("mtPrec: %d\n",g_tmtPresc);
}

/*******************************************************************************
*�������ƣ�MTMClkGen
*����˵��������ʱ���жϲ������������������ƽ��̨
*�����������
*�����������
*���ز������� 
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
*�������ƣ�HMClkGen
*����˵��������ʱ���жϲ���������������ڴ�ñ���
*�����������
*�����������
*���ز������� 
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
*�������ƣ�HMotorDrive
*����˵��������hmClkGen�������ɵ������������
*�����������
*������������ڵ�������Ƶ�ʵķ�Ƶ
*���ز������� 
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
	}//����������¶�������������е�״̬
	
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
*�������ƣ�MTMotorDrive
*����˵��������mtmClkGen�������ɵ������������
*�����������
*������������ڵ�������Ƶ�ʵķ�Ƶ
*���ز������� 
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
	}//����������¶�������������е�״̬
	
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
*�������ƣ�IsMotActDone
*����˵����������������Ƿ���ɱ�־λ
*���������motor H��ñ�����T��ƽ��̨���
*�����������
*���ز��������״̬ 1 ��ɣ�0 δ��� 
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
*�������ƣ�TIM2_IRQHandler
*����˵����TIM2�жϣ����ڲ���ʱ���ź������������
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
	else
	{
		return;
	}//ȷ����ָ���ж�Դ�����ж�
	

	HMClkGen();
	MTMClkGen();
	
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
