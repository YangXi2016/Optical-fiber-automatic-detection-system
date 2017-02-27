/**
  ******************************************************************************
  * @file    instruction.h
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   This file contains all the functions prototypes for ins operaction
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "stm32f10x.h"

#define Ins_Enable			2
#define Ins_Inputting		1
#define Ins_Disable   	0


uint16_t InsStr2Num(uint8_t *);

#endif
