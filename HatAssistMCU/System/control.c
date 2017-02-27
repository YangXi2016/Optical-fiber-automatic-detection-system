
/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�control.c
*�ļ���ʶ��
*ժ    Ҫ���Զ����ϵͳ������ļ���ƻ���	 
*��ǰ�汾��0.1
*��    �ߣ�yangxi
*������ڣ�2017/2/19
*****************************************************************************/
#include "delay.h"
//#include "spi.h"
#include "control.h"


void Hat(void)
{
	HMotion(HAT_ANGLE, '+', HAT_SPEED);
	while(IsMotActDone('H')==0);
	HMotion(HAT_ANGLE, '-', HAT_SPEED);
}

void Hat_Stop(void)
{
	 MotorEN('H','D');
}
void Rail_RunStation(void)
{
	MTMotion(RAIL_STATION_ANGLE, '+', RAIL_STATION_SPEED);
}

void Rail_Back(void)
{
	MTMotion(RAIL_FREE_ANGLE, '+', RAIL_FREE_SPEED);
}

void Rail_Forward(void)
{
	MTMotion(RAIL_FREE_ANGLE, '-', RAIL_FREE_SPEED);
}

void Rail_Stop(void)
{
	MotorEN('T','D');
}

void All_Stop(void)
{
	MotorEN('H','D');
	MotorEN('T','D');
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
