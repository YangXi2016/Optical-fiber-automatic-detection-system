/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�LinearCCD.h
*�ļ���ʶ��
*ժ	   Ҫ��		 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/9
*****************************************************************************/

#ifndef _LINEARCCD_H_
#define _LINEARCCD_H_

#include "stm32f10x.h"

#define CLK_HIGH GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define CLK_LOW  GPIO_ResetBits(GPIOA,GPIO_Pin_1)

#define SI_HIGH GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define SI_LOW  GPIO_ResetBits(GPIOA,GPIO_Pin_2)

#define BIN_THERHOLD  2000 //��ֵ����ֵ
#define UNHAT_MAX_PIXEL 20 //δ��ñʱ��С���
#define HATED_MIN_PIXEL 40 //��ñ����С���ؿ��
#define INTEREST_REG_LEFT 20//������Ȥ������ 20-60������֮��
#define INTEREST_REG_RIGHT 60

void LinCCDGPIOInit(void);
void LinCCDADCInit(void);
void LinCCDInit(void);
u16 GetADC(void);
void GetCCD(u8 *CCDRes);
u8 IsHatDone(void);

#endif
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

