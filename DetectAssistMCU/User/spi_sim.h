/**
  ******************************************************************************
  * @file    bsp_spi_dac8830_a3992.h
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   This file contains all the functions prototypes for the spi
	*          devices operaction
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
	
#ifndef __BSP_SPI_DAC8830_A3992_H
#define __BSP_SPI_DAC8830_A3992_H

#include "stm32f10x.h"

/* Macros about A3992 and DAC8830 operations */
#define DAC8830_CS_High()  		GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define DAC8830_CS_Low()   		GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define DAC8830_SCK_High()  	GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define DAC8830_SCK_Low()   	GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define DAC8830_SDI_High()  	GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define DAC8830_SDI_Low()   	GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define A3992_CSX_High()			GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define A3992_CSX_Low() 			GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define A3992_CSY_High()			GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define A3992_CSY_Low() 			GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define A3992_SCK_High()			GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define A3992_SCK_Low() 			GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define A3992_SDI_High()			GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define A3992_SDI_Low()				GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define Sleep_MotorX()				GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define Wake_MotorX()					GPIO_SetBits(GPIOB,GPIO_Pin_3)

#define Sleep_MotorY()				GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define Wake_MotorY()					GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define A3992_Word1						0x400F9

void GPIO_DAC8830_Config(void);
void GPIO_A3992_Config(void);
void Move_LineMotorX(uint8_t,uint16_t);
void Move_LineMotorY(uint8_t,uint16_t);
static void Send_MotorWordX(uint32_t);
static void Send_MotorWordY(uint32_t);
void Write_DAC8830(uint16_t);

#endif

