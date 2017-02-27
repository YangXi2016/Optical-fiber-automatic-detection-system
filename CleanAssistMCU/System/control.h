/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�Motor.h
*�ļ���ʶ��
*ժ    Ҫ��		 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/15
*****************************************************************************/
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"
#include "Motor.h"
#include "StepperMotor.h"

#define CM_SPEED		80
#define MM_ANGLE		8
#define MM_SPEED		80
#define SM_ANGLE		(360*5)
#define SM_SPEED		1000

//#define WIPE_LEN		3 //һ�β��ÿ�� mm
//#define CLOTH_LEN		(50*1000)
#define MAX_TIME		(30000) //�������ƶ�����

void Clean(void);
void Clean_Reset(void);
void Clean_Stop(void);
#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
