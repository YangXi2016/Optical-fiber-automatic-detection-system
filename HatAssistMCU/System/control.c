
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
	HMotion(HAT_ANGLE2, '+', HAT_SPEED);
	while(IsMotActDone('H')==0);
	HMotion(HAT_ANGLE, '-', HAT_SPEED);
	while(IsMotActDone('H')==0);
	SYS_STATE |= READY_STATE;
	delay_ms(100);
	HMotion(HAT_ANGLE1, '+', HAT_INIT_SPEED);
	while(IsMotActDone('H')==0);
}

void Hat_Init(void)
{
	HMotion(HAT_ANGLE, '+', HAT_INIT_SPEED);
	while(IsMotActDone('H')==0);
	HMotion(HAT_ANGLE, '-', HAT_INIT_SPEED);
	while(IsMotActDone('H')==0);
	HMotion(HAT_ANGLE1, '+', HAT_INIT_SPEED);
	while(IsMotActDone('H')==0);
// 	HMotion(5, '+', HAT_SPEED);
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

void Rail_RunToStation(void)
{
	MTMotion(RAIL_FIRSTSTATION_ANGLE, '+', RAIL_STATION_SPEED);
}

void Rail_Back(void)
{
	MTMotion(RAIL_FREE_ANGLE, '-', RAIL_FREE_SPEED);
}

void Rail_Forward(void)
{
	MTMotion(RAIL_FREE_ANGLE, '+', RAIL_FREE_SPEED);
}

extern u8 g_hClkFlg;
void Rail_Stop(void)
{
	//以下三种停止方式制动效果依次提升
	//MotorEN('T','D');
	//g_hClkFlg = 1;
	TIM_Cmd(TIM2,DISABLE);
	MTMotion(0, '+', RAIL_FREE_SPEED);
	//printf("Stop\n");
	//SYS_STATE = ACCIDENT_STATE;
}

void All_Stop(void)
{
	MotorEN('H','D');
	MotorEN('T','D');
	//SYS_STATE = ACCIDENT_STATE;
}

void Tune_Back(void)
{
	MTMotion(TUNE_ANGLE, '-', TUNE_SPEED);
}

void Tune_Forward(void)
{
	MTMotion(TUNE_ANGLE, '+', TUNE_SPEED);
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
