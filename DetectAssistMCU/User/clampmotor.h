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
  ******************************************************************************
  */
#ifndef __CLAMPMOTOR_H__
#define __CLAMPMOTOR_H__

#include "stm32f10x.h"


/*
PA12 + PB1 ----CN7

*/

void ClampMotor_Init(void);
void ClampMotor_Up(void);
void ClampMotor_Down(void);
void ClampMotor_Off(void);

#endif
