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

//主推线电机若干参数
#define P_DEFAULT_PRE 10 //电机输出脉冲计数周期
#define P_MOTOR_DIV 3200 //3200脉冲一周
#define PUSH_ACC_NUM 250 //推夹具步进电机加速脉冲数
#define PA_COEFF  2.0 //定义进入最大加速度的时间，参数越大，进入越快，2.0在居中的时间进入
#define PS_COEFF 5 //推夹具步进电机S加速曲线参数，最大加速区间曲线斜率大小参数
#define P_MIN_SPD 60 //推夹具电机加速初始转速r/min
#define P_MAX_SPD 250 //推夹具电机最大转速r/min

//擦拭工位推线电机若干参数
#define PC_DEFAULT_PRE 10 //电机输出脉冲计数周期
#define PC_MOTOR_DIV 3200 //3200脉冲一周
#define PC_ACC_NUM 250 //推夹具步进电机加速脉冲数
#define PCA_COEFF  2.0 //定义进入最大加速度的时间，参数越大，进入越快，2.0在居中的时间进入
#define PCS_COEFF 5 //推夹具步进电机S加速曲线参数，最大加速区间曲线斜率大小参数
#define PC_MIN_SPD 60 //推夹具电机加速初始转速r/min
#define PC_MAX_SPD 250 //推夹具电机最大转速r/min

//中间夹线电机若干参数
#define MC_DEFAULT_PRE 10 //电机输出脉冲计数周期
#define MC_MOTOR_DIV 400 //400脉冲一周
#define MC_ACC_NUM 100 
#define MCA_COEFF 2.0
#define MCS_COEFF 5
#define MC_MIN_SPD 60
#define MC_MAX_SPD 1000

//两边夹线电机若干参数
#define SC_DEFAULT_PRE 10 //电机输出脉冲计数周期
#define SC_MOTOR_DIV 400 //400脉冲一周
#define SC_ACC_NUM 30 
#define SCA_COEFF 2.0
#define SCS_COEFF 5
#define SC_MIN_SPD 60
#define SC_MAX_SPD 1000


//函数

//用于外部调用函数
void StepMotorInit(void); //初始化步进电机模块

void MotorEN(u8 motor,u8 oper); //使能或失能步进电机
void MotorDir(u8 motor,u8 oper);//控制转向

void PushMotion(float angleDeg, u8 dir, u16 spd);//两个个步进电机驱动接口
void PCMotion(float angleDeg, u8 dir, u16 spd);//擦拭工位步进电机驱动接口
void MCMotion(float angleDeg, u8 dir, u16 spd);
void SCMotion(float angleDeg, u8 dir, u16 spd);

u8 IsMotActDone(u8 motor);//检查运动是否完成

#define CLAMP_ANGLE			80		//用于夹紧的电机所需走的距离
#define CLAMP_SPEED			250
//#define	BACKWARD_ANGLE		70		//用于推拉的电机所需走的距离
#define FORWARD_ANGLE			100		//往回拉的拨片贴紧/移开的距离
#define PUSH_SPEED			100
#define PUSH_DIR				'-'
void Fixture_Stop(void);

u8 Check_Position_C(void);		//推向擦拭工位的电机位置检查
u8 Check_Position_M(void);		//中间的电机位置检查
//1到位，0未到位

//模块内部使用函数
void StepMotorGPIOInit(void);
void StepMotorTIMInit(void);

u16 PushMotorDrive(void);
void PMClkGen(void);

u16 PCMotorDrive(void);
void PCMClkGen(void);

u16 MCMotorDrive(void);
void MCMClkGen(void);

u16 SCMotorDrive(void);
void SCMClkGen(void);
#endif

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

