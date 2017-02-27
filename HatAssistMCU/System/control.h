/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�control.h
*�ļ���ʶ��
*ժ    Ҫ���Զ����ϵͳ������ļ���ƻ���	 
*��ǰ�汾��0.1
*��    �ߣ�yangxi
*������ڣ�2017/2/19
*****************************************************************************/

#ifndef __CONTROL_H
#define __CONTROL_H	
#include "StepperMotor.h"
#include "LinearCCD.h"

//��ñ�õ������
#define HAT_ANGLE		(360*3)
#define HAT_SPEED		(1000)

//����ƶ�һ��λ����
#define RAIL_STATION_ANGLE	(360*1)
#define RAIL_STATION_SPEED	(1000)

//��������ƶ�����
#define RAIL_FREE_ANGLE		(360*100)
#define RAIL_FREE_SPEED		(1000)

void Hat(void);
//void Hat_Check(void);
void Hat_Stop(void);
void Rail_RunStation(void);
void Rail_Back(void); 
void Rail_Forward(void);
void Rail_Stop(void);
void All_Stop(void);
#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

