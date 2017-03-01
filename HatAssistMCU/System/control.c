
/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：control.c
*文件标识：
*摘    要：自动检测系统中所需的检控制汇总	 
*当前版本：0.1
*作    者：yangxi
*完成日期：2017/2/19
*****************************************************************************/
#include "delay.h"
#include "spi.h"
#include "control.h"

extern u8 SYS_STATE;
void Hat(void)
{
	HMotion(10*HAT_ANGLE, '+', HAT_SPEED);
	while(SYS_STATE == WORK_STATE){
		if(IsMotActDone('H')==0)	break;
	}
	HMotion(10*HAT_ANGLE, '-', HAT_SPEED);
}

void Hat_Stop(void)
{
	 MotorEN('H','D');
	SYS_STATE = ACCIDENT_STATE;
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
	SYS_STATE = ACCIDENT_STATE;
}

void All_Stop(void)
{
	MotorEN('H','D');
	MotorEN('T','D');
	SYS_STATE = ACCIDENT_STATE;
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
