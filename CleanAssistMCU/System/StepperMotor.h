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

//步进电机若干参数
#define P_DEFAULT_PRE 10 //电机输出脉冲计数周期
#define P_MOTOR_DIV 48 //48脉冲一周
#define STEP_ACC_NUM 30 //推夹具步进电机加速脉冲数
#define PA_COEFF  2.0 //定义进入最大加速度的时间，参数越大，进入越快，2.0在居中的时间进入
#define PS_COEFF 5 //推夹具步进电机S加速曲线参数，最大加速区间曲线斜率大小参数
#define P_MIN_SPD 200 //推夹具电机加速初始转速r/min
#define P_MAX_SPD 1000 //推夹具电机最大转速r/min


//函数

//用于外部调用函数
void StepMotorInit(void); //初始化步进电机模块

void MotorEN(u8 motor,u8 oper); //使能或失能步进电机
void MotorDir(u8 motor,u8 oper);//控制转向

void StepMotion(float angleDeg, u8 dir, u16 spd);//步进电机驱动接口


u8 IsStepMotActDone(void);//检查运动是否完成

//模块内部使用函数
void StepMotorGPIOInit(void);
void StepMotorTIMInit(void);
u16 StepMotorDrive(void);
void PMClkGen(void);

#endif

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

