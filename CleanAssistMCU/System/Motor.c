/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：Motor.c
*文件标识：
*摘    要：擦拭结构的直流电机配置等			 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/9
*****************************************************************************/
#include "Motor.h"

int g_mmTargPulse = 0;
u32 g_mmActDFlg = 0;
u8 g_dirFlg = '+';

void MotorGPIOInit(void)
{
	//GPIO端口设置
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
  
	//TIM4_CH4 设置 ，对应主电机
	TIM_TimeBaseStructure.TIM_Period = MM_DEFAULT_PERIOD;       
	TIM_TimeBaseStructure.TIM_Prescaler =MM_DEFAULT_PRE; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	TIM_ITConfig(  TIM4, TIM_IT_Update,  ENABLE);//开启TIM4的中断源
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = MM_DEFAULT_SPD;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	
	
	//TIM3_CH3 设置 ，对应卷纸电机
	TIM_TimeBaseStructure.TIM_Period = CM_DEFAULT_PERIOD;       
	TIM_TimeBaseStructure.TIM_Prescaler =CM_DEFAULT_PRE; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	//TIM_ITConfig(  TIM3, TIM_IT_Update,  ENABLE);//开启TIM3的中断源
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CM_DEFAULT_SPD;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);	

	//配置定时器中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1，优先级次高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  
	
}
void EncoderInit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure; 

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0 ); //指定外部中断输入是PA0

	//外部中断初始化
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		

	//中断参数设定
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
	TIM4->CCER |= TIM_CCER_CC4E;//使能Time2 通道4，开启电机
	g_mmActDFlg = 0;
}
void CMotStart(u8 spd)
{
	TIM_Cmd(TIM3,ENABLE);	
	TIM3->CCR3=spd;
	TIM3->CCER|=TIM_CCER_CC3E;//使能Time3 通道3，开启电机	
}
void CMotStop(void)
{
	TIM3->CCER &= ~TIM_CCER_CC3E;//关闭Time3 通道3，关闭电机		
}

void MMotSetSpd(u8 spd)
{
	TIM4->CCR4=spd;	
}

void MMotStop(void)
{
	TIM4->CCER &= ~TIM_CCER_CC4E;//关闭Time2 通道4，关闭电机	
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

void TIM4_IRQHandler(void)   //TIM4中断
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
	}//动作已经完成，不需要再计算
	else
	{;}
		
	s_sum += g_mmTargPulse; //积分
		
	//积分限幅
	if (s_sum > MM_MAX_INTEGRA )
	{
		s_sum = MM_MAX_INTEGRA;
	}
	else if (s_sum < -MM_MAX_INTEGRA )
	{
		s_sum = -MM_MAX_INTEGRA;
	}
	
	//计算转速
	spd = (1.0*g_mmTargPulse * MM_PID_P  + 1.0 * s_sum * MM_PID_I);

	//速度限幅及调向
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
