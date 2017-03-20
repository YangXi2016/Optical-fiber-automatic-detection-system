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
#define HAT_ANGLE		(360*3)
#define HAT_SPEED		(500)

//电机移动一工位参数
#define RAIL_STATION_ANGLE	(360*1)
#define RAIL_STATION_SPEED	(300)

#define RAIL_FIRSTSTATION_ANGLE (360*2.5)

//电机自由移动参数
#define RAIL_FREE_ANGLE		(360*100)
#define RAIL_FREE_SPEED		(300)

void Hat(void);
void Hat_Stop(void);
void Rail_RunStation(void);
void Rail_RunToStation(void);
void Rail_Back(void); 
void Rail_Forward(void);
void Rail_Stop(void);
void All_Stop(void);
#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

