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

//���ߵ�����ɲ���
#define P_DEFAULT_PRE 10 //�����������������
#define P_MOTOR_DIV 50 //50����һ��
#define STEP_ACC_NUM 50 //�Ƽо߲����������������
#define PA_COEFF  2.0 //������������ٶȵ�ʱ�䣬����Խ�󣬽���Խ�죬2.0�ھ��е�ʱ�����
#define PS_COEFF 5 //�Ƽо߲������S�������߲�������������������б�ʴ�С����
#define P_MIN_SPD 200 //�Ƽоߵ�����ٳ�ʼת��r/min
#define P_MAX_SPD 1000 //�Ƽоߵ�����ת��r/min


//����

//�����ⲿ���ú���
void StepMotorInit(void); //��ʼ���������ģ��

void MotorEN(u8 motor,u8 oper); //ʹ�ܻ�ʧ�ܲ������
void MotorDir(u8 motor,u8 oper);//����ת��

void StepMotion(float angleDeg, u8 dir, u16 spd);//������������ӿ�


u8 IsStepMotActDone(void);//����˶��Ƿ����

//ģ���ڲ�ʹ�ú���
void StepMotorGPIOInit(void);
void StepMotorTIMInit(void);
u16 StepMotorDrive(void);
void PMClkGen(void);

#endif

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

