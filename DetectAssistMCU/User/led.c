/**
  ******************************************************************************
  * @file    led.c
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   About diode device operations
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
	
#include "led.h"
#include "bsp_usart.h"

/**
	* IO involved： 
	* LED_Red   -- PB0
	* LED_White -- PB1
	*	Optical sensor	-- PB11
	*/

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	LED_White_ON();
	LED_Red_OFF();
}

void LED_Control(uint8_t led)
{
	if(led == 'R')
	{
		LED_White_OFF();
		LED_Red_ON();
	}
	else if(led == 'W')
	{
		LED_White_ON();
		LED_Red_OFF();
	}
}

void Clamping_Check(void)
{
	uint8_t temp;
	uint8_t i,j=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	temp = GetStatus_PSD();
	for(i = PSD_Check_Num;i > 0;i--)
	{
		if(GetStatus_PSD() == temp)j++;
	}
	if(j == PSD_Check_Num)
	{
		if(temp == Fixture_Clamp)
		{
			printf("OE\n");
		}
		else if(temp == Fixture_Loose)
		{
			printf("FE\n");
		}
	}
	else 
	{
		printf("FE\n");
	}
}

char Check_Fixture_pluse(void)
{
	u8 time=0;
	u8 tcount=0;//记录松开的次数
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
while(1)
{	
	if(Fixture_Clamp==GetStatus_PSD())//
		{
			time++;
			tcount=0;
		}	
		else if(Fixture_Loose==GetStatus_PSD())//
		{
			tcount++; 
			if(tcount>30)
			{		
				printf("FE\n");//
				return 0;
			}
		}
		 		
	 // for(i = 1000;i > 0;i--)
		if(time>=100)//按下超过3秒钟
		{
       printf("OE\n");
			return 1; 
		}		
	}		
}		
		








