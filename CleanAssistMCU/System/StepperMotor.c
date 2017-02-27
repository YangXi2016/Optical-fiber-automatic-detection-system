/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�StepperMotor.c
*�ļ���ʶ��
*ժ	   Ҫ���Բ����������ɲ����Դ��ʼ�������ṩ��������		 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/9
*****************************************************************************/
#include "StepperMotor.h"
#include "math.h"
#include "stm32f10x.h"
#include "usart.h"


static u16 gs_pAccPrescTab[STEP_ACC_NUM] = {0};//�Ƽоߵ����/����Ԥ��Ƶ��

u32 g_pStps = 0;//�Ƽоߵ����Ҫ��ת�Ĳ���
u16 g_tpPresc = 0;//�Ƽоߵ��Ŀ���ٶȶ�Ӧ��Ԥ��Ƶ��
u8  g_npActFlg = 0; //�Ƽоߵ���¶�����־λ
u8  g_pActDFlg = 0; //�Ƽоߵ������������־λ
u8  g_pClkFlg  = 0; //�Ƽоߵ���������ʹ�ܱ�־λ

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
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // ʹ��PC�˿�ʱ��
	
	// ������ֽ��� CLK - PA8   DIR - PA11   EN - PA12
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
	
 	//�Ƽоߵ��ʱ������ TIM2
	TIM_TimeBaseStructure.TIM_Period = DEFAULT_PERIOD - 1;	 //PWM����
	TIM_TimeBaseStructure.TIM_Prescaler = DEFAULT_PRESCALE - 1;  //����Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig( TIM2, TIM_IT_Update  |  TIM_IT_Trigger,  ENABLE);//����TIM2���ж�Դ�ʹ����ж�

	//���ö�ʱ���ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2ȫ���ж�
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
	
	//���θ���Ԥ��Ƶ���е�ÿһ�� ���� f = fmin + (fmax - fmin)/(1 + e^(-a(i/num -1)))
	//���мӼ��٣��˴���fת������Ԥ��Ƶ�ϣ���˹�ʽ�б���
	TIFreq = 72000000.0 / DEFAULT_PERIOD /DEFAULT_PRESCALE;//ʱ���ж�Ƶ�ʼ���
	
	//�����Ƹ˵���Ӽ���Ԥ��Ƶ��
	prescMin =  TIFreq / (P_MAX_SPD * 1.0 / 60 * P_MOTOR_DIV);//�����趨�������СԤ��Ƶ
	prescMax = TIFreq /  (P_MIN_SPD * 1.0 / 60 * P_MOTOR_DIV);
	for (i=0; i<STEP_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-PS_COEFF * (PA_COEFF * i / STEP_ACC_NUM - 1)));
		gs_pAccPrescTab[i] = (u16)((1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin)) + 0.5);
		printf("pPrescTab[%d]:%d\n",i,gs_pAccPrescTab[i]);
	}
	
}

/*******************************************************************************
*�������ƣ�MotorEN
*����˵�����������ʹ��
*���������motor : P���ߵ����M�м���ߵ����S���߼��ߵ��
		   oper: E enable ; D disable
*�����������
*���ز������� 
*******************************************************************************/
void MotorEN(u8 motor,u8 oper)
{
	switch(motor)
	{
		case 'P':
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
		}//�Ƶ�����
		
		default:break;
	}
}

/*******************************************************************************
*�������ƣ�MotorDir
*����˵�����������ת��
*���������motor : P�Ƶ���
		   oper: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
*�����������
*���ز������� 
*******************************************************************************/
void MotorDir(u8 motor,u8 oper)
{
	switch(motor)
	{
		case 'P':
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
		}//�Ƶ�����
		default:break;
	}
}

/*******************************************************************************
*�������ƣ�StepMotion
*����˵�����Ƽо߲�������������ýӿ�
*���������angleDeg : ���������Ҫת���Ķ��� ��λ��
		   dir: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
		   spd:�ٶȣ���λr/min
*�����������
*���ز������� 
*******************************************************************************/
void StepMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //��ʱ�������жϵ�Ƶ��
	float spdFreq = 0; //�ﵽ��Ӧ�ٶ���Ҫ������Ƶ��
	
	MotorEN('P','E');//ʹ���Ƽоߵ��
	MotorDir('P',dir);//�趨ת��
	
	//�ٶ�����
	if (spd > P_MAX_SPD)
	{
		spd = P_MAX_SPD;
	}
	else if (spd < P_MIN_SPD)
	{
		spd = P_MIN_SPD;
	}
	
	//����ȫ�ֱ���
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//��ʱ���ж�Ƶ��
	spdFreq = spd * 1.0 / 60 * P_MOTOR_DIV;//�ﵽָ���ٶ���Ҫ������Ƶ��	
	
	g_tpPresc = (u16)( TIFreq / spdFreq  +0.5);//��������ٶȶ�Ӧ��Ԥ��Ƶֵ		
	g_pStps = (u32)((angleDeg * 1.0 / 360 * P_MOTOR_DIV)+0.5);//���´��˶��Ĳ���
	g_npActFlg = 1;
	g_pActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);

}

/*******************************************************************************
*�������ƣ�PMClkGen
*����˵��������ʱ���жϲ�����������������Ƽоߵ��
*�����������
*�����������
*���ز������� 
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
		s_presc = StepMotorDrive();
		s_clkCnt = 0;
	}
}

/*******************************************************************************
*�������ƣ�StepMotorDrive
*����˵��������pmClkGen�������ɵ������������
*�����������
*������������ڵ�������Ƶ�ʵķ�Ƶ
*���ز������� 
*******************************************************************************/
u16 StepMotorDrive(void)
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
	{;}//����������¶�������������е�״̬
	
	//printf("s : %d\n",s_state);
	switch (s_state)
	{
		case 0:
		{
			if (g_pStps > 2*STEP_ACC_NUM)
			{
				s_maxAccNum = STEP_ACC_NUM;
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
*�������ƣ�IsStepMotActDone
*����˵����������������Ƿ���ɱ�־λ
*���������motor P���Ƹ˵����M�м�гֵ����S���߼ӳֵ��
*�����������
*���ز��������״̬ 1 ��ɣ�0 δ��� 
*******************************************************************************/
u8 IsStepMotActDone(void)
{
	return g_pActDFlg;
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
	
	PMClkGen(); //����������������������ź�

}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
