/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：LinearCCD.h
*文件标识：
*摘	   要：		 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/9
*****************************************************************************/

#ifndef _LINEARCCD_H_
#define _LINEARCCD_H_

#include "stm32f10x.h"

#define CLK_HIGH GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define CLK_LOW  GPIO_ResetBits(GPIOA,GPIO_Pin_1)

#define SI_HIGH GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define SI_LOW  GPIO_ResetBits(GPIOA,GPIO_Pin_2)

#define BIN_THERHOLD  2000 //二值化阈值
#define UNHAT_MAX_PIXEL 20 //未戴帽时最小宽度
#define HATED_MIN_PIXEL 40 //戴帽后最小像素宽度
#define INTEREST_REG_LEFT 20//检测感兴趣的区域 20-60个像素之间
#define INTEREST_REG_RIGHT 60

void LinCCDGPIOInit(void);
void LinCCDADCInit(void);
void LinCCDInit(void);
u16 GetADC(void);
void GetCCD(u8 *CCDRes);
u8 IsHatDone(void);

#endif
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

