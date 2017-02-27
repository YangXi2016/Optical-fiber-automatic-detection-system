/**
  ******************************************************************************
  * @file    bsp_timer.h
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "stm32f10x.h"

void TIM2_ITConfigure(void);
void TIM3_ITConfigure(void);
void Delayus(uint16_t);
void Delayms(uint16_t);
static void NVIC_TIM2_Configure(void);
void CCW_CW_Pulse_Init(void);
void CCW_Pulse_Output(uint16_t);
void CW_Pulse_Output(uint16_t);
void Pulse_Disable(void);

#endif

