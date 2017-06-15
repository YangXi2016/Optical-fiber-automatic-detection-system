/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：Motor.h
*文件标识：
*摘    要：		 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/15
*****************************************************************************/
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"
#include "Motor.h"
#include "StepperMotor.h"

#define CM_SPEED		80
#define MM_ANGLE		25//12		//distance/diameter/pi*360=2.5/27.5/3.14*360=10.44
#define MM_SPEED		80
#define SM_ANGLE		(360*5)
#define SM_SPEED		1000

//#define WIPE_LEN		3 //一次擦拭宽度 mm
//#define CLOTH_LEN		(50*1000)
#define MAX_TIME		(30000) //仅横向移动次数
#define ROLL_TIME		(1000)	//ms

void Clean(void);
void Clean_Reset(void);
void Clean_Stop(void);
#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
