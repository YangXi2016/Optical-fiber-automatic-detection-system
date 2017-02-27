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
#define P_MOTOR_DIV 400 //400����һ��
#define PUSH_ACC_NUM 50 //�Ƽо߲����������������
#define PA_COEFF  2.0 //������������ٶȵ�ʱ�䣬����Խ�󣬽���Խ�죬2.0�ھ��е�ʱ�����
#define PS_COEFF 5 //�Ƽо߲������S�������߲�������������������б�ʴ�С����
#define P_MIN_SPD 60 //�Ƽоߵ�����ٳ�ʼת��r/min
#define P_MAX_SPD 1000 //�Ƽоߵ�����ת��r/min

//�м���ߵ�����ɲ���
#define MC_DEFAULT_PRE 10 //�����������������
#define MC_MOTOR_DIV 400 //400����һ��
#define MC_ACC_NUM 50 
#define MCA_COEFF 2.0
#define MCS_COEFF 5
#define MC_MIN_SPD 60
#define MC_MAX_SPD 1000

//���߼��ߵ�����ɲ���
#define SC_DEFAULT_PRE 10 //�����������������
#define SC_MOTOR_DIV 400 //400����һ��
#define SC_ACC_NUM 50 
#define SCA_COEFF 2.0
#define SCS_COEFF 5
#define SC_MIN_SPD 60
#define SC_MAX_SPD 1000


//����

//�����ⲿ���ú���
void StepMotorInit(void); //��ʼ���������ģ��

void MotorEN(u8 motor,u8 oper); //ʹ�ܻ�ʧ�ܲ������
void MotorDir(u8 motor,u8 oper);//����ת��

void PushMotion(float angleDeg, u8 dir, u16 spd);//����������������ӿ�
void MCMotion(float angleDeg, u8 dir, u16 spd);
void SCMotion(float angleDeg, u8 dir, u16 spd);

u8 IsMotActDone(u8 motor);//����˶��Ƿ����

#define CLAMP_ANGLE			100		//���ڼн��ĵ�������ߵľ���
#define	PROCESS_ANGLE		360		//���������ĵ�������ߵľ���
#define SPEED						800
void Fixture_Stop(void);

//ģ���ڲ�ʹ�ú���
void StepMotorGPIOInit(void);
void StepMotorTIMInit(void);

u16 PushMotorDrive(void);
void PMClkGen(void);

u16 MCMotorDrive(void);
void MCMClkGen(void);

u16 SCMotorDrive(void);
void SCMClkGen(void);
#endif

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

