/******************** (C) COPYRIGHT 2016  **********************************
*Copyright(c)2016,杭州鹰睿科技有限公司
*All rights reserved
*
*文件名称：delay.h
*文件标识：
*摘		 要：		 
*当前版本：1.0
*作    者：李华兵
*完成日期：2016/8/20
*****************************************************************************/

#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f10x.h"

void delay_clockint(void);
void delay_us(u32 time);
void delay_ms(u32 time);

#endif

/******************* (C) COPYRIGHT 2016  *****END OF FILE**********************/
