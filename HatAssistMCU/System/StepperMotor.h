/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�StepperMotor.h
*�ļ���ʶ��
*ժ	   Ҫ��		 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/9
*****************************************************************************/

#ifndef _STEPPERMOTOR_H_
#define _STEPPERMOTOR_H_

#include "stm32f10x.h"

//��ʱ������
#define e 2.71828 //��Ȼ����
#define DEFAULT_PERIOD   20 //��ʱ��Ĭ�ϼ�������
#define DEFAULT_PRESCALE 72  //��ʱ��Ĭ��Ԥ��Ƶ1000KhZ����Ƶ��

//��ñ������ɲ���
#define H_DEFAULT_PRE 10 //�����������������
#define H_MOTOR_DIV 400 //400����һ��
#define H_ACC_NUM 20 
#define HA_COEFF 2.0
#define HS_COEFF 5
#define H_MIN_SPD 100
#define H_MAX_SPD 1500

//���߼��ߵ�����ɲ���
#define MT_DEFAULT_PRE 10 //�����������������
#define MT_MOTOR_DIV 400 //400����һ��
#define MT_ACC_NUM 50 
#define MTA_COEFF 2.0
#define MTS_COEFF 5
#define MT_MIN_SPD 60
#define MT_MAX_SPD 1000


//����

//�����ⲿ���ú���
void StepMotorInit(void); //��ʼ���������ģ��

void MotorEN(u8 motor,u8 oper); //ʹ�ܻ�ʧ�ܲ������
void MotorDir(u8 motor,u8 oper);//����ת��


void HMotion(float angleDeg, u8 dir, u16 spd);
void MTMotion(float angleDeg, u8 dir, u16 spd);

u8 IsMotActDone(u8 motor);//����˶��Ƿ����

//ģ���ڲ�ʹ�ú���
void StepMotorGPIOInit(void);
void StepMotorTIMInit(void);

u16 HMotorDrive(void);
void HMClkGen(void);

u16 MTMotorDrive(void);
void MTMClkGen(void);
#endif

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

