/**
  ******************************************************************************
  * @file    bsp_spi_dac8830_a3992.c
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   SPI contrl the DAC8830,simulative SPI contrl the A3992
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"

#define LED_White_ON() 			GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define LED_White_OFF() 		GPIO_ResetBits(GPIOA,GPIO_Pin_4)

#define LED_Red_ON() 				GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define LED_Red_OFF() 			GPIO_ResetBits(GPIOA,GPIO_Pin_3)

#define PSD_Check_Num				100
#define GetStatus_PSD()			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)

#define Fixture_Clamp				0
#define Fixture_Loose				1


void LED_Init(void);
void LED_Control(uint8_t);
void Clamping_Check(void);

#endif
