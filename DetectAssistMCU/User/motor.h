/**
  ******************************************************************************
  * @file    bsp_motor.h
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   This file contains all the functions prototypes for the motor
	*          operation
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
	
#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

#include "stm32f10x.h"

#define OutCurrent_Off()			GPIO_ResetBits(GPIOA,GPIO_Pin_0)					//Cut off the out current
#define OutCurrent_On()  			GPIO_SetBits(GPIOA,GPIO_Pin_0)						//
//#define HoldCurrent_Full() 		GPIO_ResetBits(GPIOA,GPIO_Pin_0)					//full current when stop
//#define HoldCurrent_Half() 		GPIO_SetBits(GPIOA,GPIO_Pin_0)						//half current when stop
//#define FH_Half()							GPIO_ResetBits(GPIOA,GPIO_Pin_1)					//Half step mode
//#define FH_Full()							GPIO_SetBits(GPIOA,GPIO_Pin_1)						//Full step mode
//#define CW_PUL_SetOFF()				GPIO_SetBits(GPIOA,GPIO_Pin_6)						
//#define CW_PUL_SetON()				GPIO_ResetBits(GPIOA,GPIO_Pin_6)					
//#define CCW_PUL_SetOFF()			GPIO_SetBits(GPIOA,GPIO_Pin_3)						
//#define CCW_PUL_SetON()				GPIO_RetBits(GPIOA,GPIO_Pin_3)						
#define Buf_Out_On()					GPIO_ResetBits(GPIOB,GPIO_Pin_9)	
#define Buf_Out_Off()					GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define GetStatus_LSCCW()			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)	
#define GetStatus_LSORG()			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)	
#define GetStatus_LSCW()			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)		

#define Stage_Forward					'+'				//CCW，向正方向
#define Stage_Backward				'-'				//CW，向负方向
#define Stage_Range_Upper			8500
#define Stage_Range_Lower			-4000

void Motor_Init(void);
void Stage_Run(uint8_t,uint16_t,uint16_t);
void Back_ORG(uint8_t);
void Go_Limit(uint8_t);
void Move(uint8_t,uint16_t);
void MoveTo(int16_t);
void ACC_Move(uint8_t,uint16_t);
void Stage_Current_Set(void);

#endif
