/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�Motor.c
*�ļ���ʶ��
*ժ    Ҫ�����ýṹ��ֱ��������õ�			 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/9
*****************************************************************************/
#include "Motor.h"

int g_mmTargPulse = 0;
u32 g_mmActDFlg = 0;
u8 g_dirFlg = '+';

void MotorGPIOInit(void)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  
	//MainMotor PWM PA3 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Encoder PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	// DIR+ PA1 DIR- PA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
   
	//Curve Motor PWM PB0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
void MotorTIMInit(void)
{
		
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM3, ENABLE);
  
	//TIM4_CH4 ���� ����Ӧ�����
	TIM_TimeBaseStructure.TIM_Period = MM_DEFAULT_PERIOD;       
	TIM_TimeBaseStructure.TIM_Prescaler =MM_DEFAULT_PRE; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	TIM_ITConfig(  TIM4, TIM_IT_Update,  ENABLE);//����TIM4���ж�Դ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = MM_DEFAULT_SPD;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	
	
	//TIM3_CH3 ���� ����Ӧ��ֽ���
	TIM_TimeBaseStructure.TIM_Period = CM_DEFAULT_PERIOD;       
	TIM_TimeBaseStructure.TIM_Prescaler =CM_DEFAULT_PRE; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	//TIM_ITConfig(  TIM3, TIM_IT_Update,  ENABLE);//����TIM3���ж�Դ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CM_DEFAULT_SPD;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);	

	//���ö�ʱ���ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1�����ȼ��θ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  
	
}
void EncoderInit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure; 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0 ); //ָ���ⲿ�ж�������PA0

	//�ⲿ�жϳ�ʼ��
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		

	//�жϲ����趨
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						  
	NVIC_Init(&NVIC_InitStructure);  	  
}
void MotorInit(void)
{
	 MotorGPIOInit();
	 MotorTIMInit();
	 EncoderInit();	
}
void MMotMotion(float angleDeg, u8 spd,u8 dir)
{
	if (dir == '+')
	{
		MM_CW;
	}
	else
	{
		MM_CCW;
	}
	g_mmTargPulse = (u32)((1.0 * angleDeg / 360 * MM_REDUCT_RAT * MM_PULSE_R)+0.5);
	TIM_Cmd(TIM4,ENABLE);
	TIM4->CCR4=spd;
	TIM4->CCER |= TIM_CCER_CC4E;//ʹ��Time2 ͨ��4���������
	g_mmActDFlg = 0;
}
void CMotStart(u8 spd)
{
	TIM_Cmd(TIM3,ENABLE);	
	TIM3->CCR3=spd;
	TIM3->CCER|=TIM_CCER_CC3E;//ʹ��Time3 ͨ��3���������	
}
void CMotStop(void)
{
	TIM3->CCER &= ~TIM_CCER_CC3E;//�ر�Time3 ͨ��3���رյ��		
}

void MMotSetSpd(u8 spd)
{
	TIM4->CCR4=spd;	
}

void MMotStop(void)
{
	TIM4->CCER &= ~TIM_CCER_CC4E;//�ر�Time2 ͨ��4���رյ��	
}
u8 IsDCMotActDone(void)
{
	return g_mmActDFlg;
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	else
	{
		return;
	}
	if (g_dirFlg == '+')
	{
		
		g_mmTargPulse--;
	}
	else
	{
		g_mmTargPulse++;		
	}
}

void TIM4_IRQHandler(void)   //TIM4�ж�
{
	float spd = 0;
	static int  s_sum = 0;
	static int targPulseLast = 0;
	static u16 timCnt  = 0;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
	{	
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
	}
	else
	{
		return;
	}
	
	if (g_mmActDFlg == 1)
	{
		return ;
	}//�����Ѿ���ɣ�����Ҫ�ټ���
	else
	{;}
		
	s_sum += g_mmTargPulse; //����
		
	//�����޷�
	if (s_sum > MM_MAX_INTEGRA )
	{
		s_sum = MM_MAX_INTEGRA;
	}
	else if (s_sum < -MM_MAX_INTEGRA )
	{
		s_sum = -MM_MAX_INTEGRA;
	}
	
	//����ת��
	spd = (1.0*g_mmTargPulse * MM_PID_P  + 1.0 * s_sum * MM_PID_I);

	//�ٶ��޷�������
	if (spd < 0)
	{
		MM_CCW;
		spd = - spd;
		g_dirFlg = '-';
	}
	else
	{
		MM_CW;
		g_dirFlg = '+';		
	}
		
	if (spd < MM_MIN_SPD)
	{
		spd = MM_MIN_SPD;
	}
	else if (spd > MM_MAX_SPD)
	{
		spd = MM_MAX_SPD;
	}
	MMotSetSpd((u8)spd);	
	
	if ( g_mmTargPulse <= MM_DEAD_RANGE && g_mmTargPulse >= -MM_DEAD_RANGE )
	{
		g_mmActDFlg = 1;
		MMotStop();
		s_sum = 0;
	}

	printf("%0.1f %d %d\n",spd,s_sum,g_mmTargPulse);

	 
}


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
