/**
  ******************************************************************************
  * @file    bsp_timer.c
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   config the timer
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
	
#include "bsp_timer.h"
#include "motor.h"

extern uint8_t  Stage_Dir;

/**
  * @brief  T2C2 for CCW
	*					T2C3 for CW	
  * @param  None
  * @retval None
  */
void CCW_CW_Pulse_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	/* TIM2����ʱ�Ӵ򿪣�����TIM2CLKΪ72MHz */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM2->CR1		&= ~(1<<4);			//���ϼ���
	TIM2->CR1		&= ~(1<<5);			
	TIM2->CR1		&= ~(1<<6);			//��Ե����ģʽ	
	TIM2->CR1		|=  (1<<7);			//ʹ��TIM2Ԥװ����װ�ؼĴ���		

	//T2C2 Config	
	TIM2->CCMR1 &= ~(1<<8);  
	TIM2->CCMR1 &= ~(1<<9);			//CC2����Ϊ���
	TIM2->CCMR1 |=  (1<<11);		//ʹ�ܱȽ�2Ԥװ�ؼĴ��� 
	TIM2->CCER  &= ~(1<<5);			//��Ч��ƽΪHigh		
	TIM2->CCMR1 |=  (1<<12); 		
	TIM2->CCMR1 &= ~(1<<13); 
	TIM2->CCMR1 |=  (1<<14);		//OC2M[2:0]=101,ǿ�����ΪHigh
	TIM2->CCER  |= (1<<4);			//ʹ��OC3���		

	
	//T2C3 Config	
	TIM2->CCMR2 &= ~(1<<0);  
	TIM2->CCMR2 &= ~(1<<1);		//CC3����Ϊ���
	TIM2->CCMR2 |=  (1<<3);		//ʹ�ܱȽ�3Ԥװ�ؼĴ��� 		
	TIM2->CCMR2 |=  (1<<4); 		
	TIM2->CCMR2 &= ~(1<<5); 
	TIM2->CCMR2 |=  (1<<6);		//OC3M[2:0]=101,ǿ�����ΪHigh
	TIM2->CCER  &= ~(1<<9);		//��Ч��ƽΪHigh
	TIM2->CCER  |= (1<<8);		//ʹ��OC3���		

	TIM2->EGR		|=  (1<<0);
	TIM2->PSC		 = 0x0002;			//Ԥ��Ƶ��Ϊ2+1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}

/**
  * @brief  ���ռ�ձ�Ϊ50%ָ��Ƶ�ʵ�PWM�źţ��ṩ��CCWͨ��
  * @param  period�������ڣ�ȷ��Ƶ��
  * @retval None
  */
void CCW_Pulse_Output(uint16_t period)
{
	/* ------------------------------------------------------------
	TIM2 Configuration: generate 1 PWM signal
	TIM2CLK = 72 MHz, Prescaler = 2 
	TIM2 Counter Clock = TIM2CLK/(Prescaler+1)=24 MHz
	TIM2 Frequency = TIM2 counter clock/(TIM2_ARR + 1)
	TIM2 Channel duty cycle = (TIM3_CCR1/(TIM2_ARR+1))*100% = 50%
	------------------------------------------------------------- */
	TIM2->CCMR1 &=  ~(1<<12); 		
	TIM2->CCMR1 |=  (1<<13);
	TIM2->CCMR1 |=  (1<<14);	//OC2M[2:0]=110,PWM1���
	
	TIM2->CCMR2 |=  (1<<4); 		
	TIM2->CCMR2 &= ~(1<<5); 
	TIM2->CCMR2 |=  (1<<6);		//OC3M[2:0]=101,ǿ�����ΪHigh
	
	TIM2->ARR		= period;				//TIM2_ARR��ֵ
	TIM2->CCR2	= (period+1)/2;
	TIM2->CR1	 |= (1<<0);				//ʹ�ܼ����� 
	
	Stage_Dir = Stage_Forward;
}

/**
  * @brief  ���ռ�ձ�Ϊ50%ָ��Ƶ�ʵ�PWM�źţ��ṩ��CWͨ��
  * @param  period�������ڣ�ȷ��Ƶ��
  * @retval None
  */
void CW_Pulse_Output(uint16_t period)
{
	/* ------------------------------------------------------------
	TIM2 Configuration: generate 1 PWM signal
	TIM2CLK = 72 MHz, Prescaler = 2 
	TIM2 Counter Clock = TIM2CLK/(Prescaler+1)=24 MHz
	TIM2 Frequency = TIM2 counter clock/(TIM2_ARR + 1)
	TIM2 Channel duty cycle = (TIM3_CCR1/ (TIM2_ARR+1))* 100 = 50%
	------------------------------------------------------------- */	
	TIM2->CCMR2 &= ~(1<<4); 		
	TIM2->CCMR2 |=  (1<<5); 
	TIM2->CCMR2 |=  (1<<6);		//OC3M[2:0]=110,����ģʽ��PWM1
	
	TIM2->CCMR1 |=  (1<<12); 		
	TIM2->CCMR1 &= ~(1<<13); 
	TIM2->CCMR1 |=  (1<<14);		//OC2M[2:0]=101,ǿ�����ΪHigh
	
	TIM2->ARR		= period;				//TIM2_ARR��ֵ
	TIM2->CCR3	= (period+1)/2;
	TIM2->CR1	 |= (1<<0);				//ʹ�ܼ����� 
	
	Stage_Dir = Stage_Backward;
}

void Pulse_Disable()
{
	TIM_Cmd(TIM2,DISABLE);
}

/* ����TIM2����ж� */
void TIM2_ITConfigure()
{
	NVIC_TIM2_Configure();
	TIM2->CR1 |= TIM_CR1_URS;		//ʹTIM2ֻ���ڼ������ʱ�Ų��������ж�
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}

/* NVIC����TIM2�ж� */
static void NVIC_TIM2_Configure()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);						//��ռ1λ����Ӧ3λ 
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void Delayus(uint16_t time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;
      while(i--) ;    
   }
}

void Delayms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;
      while(i--) ;    
   }
}

