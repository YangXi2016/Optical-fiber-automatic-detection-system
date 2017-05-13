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
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f10x.h"

#define MM_DEFAULT_PRE 720 //主动轮电机默认预分频
#define MM_DEFAULT_PERIOD 100 //主动轮默认周期，可定义100级占空比
#define MM_DEFAULT_SPD 50 //默认速度50，最大速度100
#define MM_REDUCT_RAT 298//75 //主动轮电机减速比
#define MM_PULSE_R 7//1 //主动轮编码器每转输出脉冲数
#define MM_DEAD_RANGE 5 //允许±3个脉冲的误差
#define MM_MIN_SPD 0
#define MM_MAX_SPD 100
#define MM_MAX_INTEGRA 20000
#define MM_PID_P  1.2
#define MM_PID_I  0.001


#define CM_DEFAULT_PRE 144 //卷纸电机默认预分频
#define CM_DEFAULT_PERIOD 100 //卷纸电机默认周期，可定义100级速度 
#define CM_DEFAULT_SPD 25 //默认速度25

#define MM_CW  {GPIO_SetBits(GPIOA,GPIO_Pin_1);GPIO_ResetBits(GPIOA,GPIO_Pin_2);}
#define MM_CCW  {GPIO_SetBits(GPIOA,GPIO_Pin_2);GPIO_ResetBits(GPIOA,GPIO_Pin_1);}


void MotorGPIOInit(void);
void MotorTIMInit(void);
void EncoderInit(void);
void MotorInit(void);

void MMotMotion(float angleDeg, u8 spd,u8 dir);
void MMotStop(void);
void MMotSetSpd(u8 spd);
void CMotStart(u8 spd);
void CMotStop(void);
u8 IsDCMotActDone(void);


#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

