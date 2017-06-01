/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：StepperMotor.h
*文件标识：
*摘	   要：		 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/9
*****************************************************************************/

#ifndef _STEPPERMOTOR_H_
#define _STEPPERMOTOR_H_

#include "stm32f10x.h"

//定时器参数
#define e 2.71828 //自然对数
#define DEFAULT_PERIOD   20 //定时器默认计数周期
#define DEFAULT_PRESCALE 72  //定时器默认预分频1000KhZ计数频率

//戴帽电机若干参数
#define H_DEFAULT_PRE 10 //电机输出脉冲计数周期
#define H_MOTOR_DIV 800 //800脉冲一周
#define H_ACC_NUM 50 
#define HA_COEFF 2.0
#define HS_COEFF 5
#define H_MIN_SPD 100
#define H_MAX_SPD 1000

//平移台电机若干参数
#define MT_DEFAULT_PRE 10 //电机输出脉冲计数周期
#define MT_MOTOR_DIV 400 //400脉冲一周
#define MT_ACC_NUM 150 
#define MTA_COEFF 2.0
#define MTS_COEFF 2
#define MT_MIN_SPD 60
#define MT_MAX_SPD 1000


//函数

//用于外部调用函数
void StepMotorInit(void); //初始化步进电机模块

void MotorEN(u8 motor,u8 oper); //使能或失能步进电机
void MotorDir(u8 motor,u8 oper);//控制转向


void HMotion(float angleDeg, u8 dir, u16 spd);
void MTMotion(float angleDeg, u8 dir, u16 spd);

u8 IsMotActDone(u8 motor);//检查运动是否完成

//模块内部使用函数
void StepMotorGPIOInit(void);
void StepMotorTIMInit(void);

u16 HMotorDrive(void);
void HMClkGen(void);

u16 MTMotorDrive(void);
void MTMClkGen(void);
#endif

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

