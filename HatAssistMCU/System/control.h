/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：control.h
*文件标识：
*摘    要：自动检测系统中所需的检控制汇总	 
*当前版本：0.1
*作    者：yangxi
*完成日期：2017/2/19
*****************************************************************************/

#ifndef __CONTROL_H
#define __CONTROL_H	
#include "StepperMotor.h"
#include "LinearCCD.h"
//#include "spi.h"

//戴帽用电机参数
#define HAT_ANGLE		(1140)
#define HAT_SPEED		(500)

//电机移动一工位参数
#define RAIL_STATION_ANGLE	(14.0*360/75)
#define RAIL_STATION_SPEED	(800)

#define RAIL_FIRSTSTATION_ANGLE (52.5*360/75)

//电机自由移动参数
#define RAIL_FREE_ANGLE		2674.4//((36*(u32)(RAIL_STATION_ANGLE/360*MT_MOTOR_DIV + 0.5)+(u32)(RAIL_FIRSTSTATION_ANGLE/360*MT_MOTOR_DIV + 0.5))/MT_MOTOR_DIV*360)//直接逆向运算
//(36*RAIL_STATION_ANGLE +  RAIL_FIRSTSTATION_ANGLE + 10) //+10是为了弥补运算中的误差
#define RAIL_FREE_SPEED		(600)

//电机微调移动参数
#define TUNE_ANGLE		(2)
#define TUNE_SPEED		(800)

void Hat(void);
void Hat_Init(void);
void Hat_Stop(void);
void Rail_RunStation(void);
void Rail_RunToStation(void);
void Rail_Back(void); 
void Rail_Forward(void);
void Rail_Stop(void);
void All_Stop(void);
void Tune_Back(void);
void Tune_Forward(void);
#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

