/**
  ******************************************************************************
  * @file    clampmotor.h
  * @author  yangxi
  * @version V0.0.0
  * @date    2016/12/23
  * @brief   
  ******************************************************************************
  * @attention
  *
  *******************************************************************************/

#include "clampmotor.h"

/*
PA12 + PB1 ----CN7

*/

void ClampMotor_Init()  
{    
    GPIO_InitTypeDef GPIO_InitStructure;  

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);  
                                                                              
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	ClampMotor_Up();//默认进入不挡光状态
	
}


void ClampMotor_Up()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}

void ClampMotor_Down()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);

}

void ClampMotor_Off()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	
	//GPIO_SetBits(GPIOA,GPIO_Pin_12);
	//GPIO_SetBits(GPIOB,GPIO_Pin_1);
}
