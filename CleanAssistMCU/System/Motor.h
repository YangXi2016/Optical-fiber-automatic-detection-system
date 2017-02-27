/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�Motor.h
*�ļ���ʶ��
*ժ    Ҫ��		 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/15
*****************************************************************************/
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f10x.h"

#define MM_DEFAULT_PRE 720 //�����ֵ��Ĭ��Ԥ��Ƶ
#define MM_DEFAULT_PERIOD 100 //������Ĭ�����ڣ��ɶ���100��ռ�ձ�
#define MM_DEFAULT_SPD 50 //Ĭ���ٶ�50������ٶ�100
#define MM_REDUCT_RAT 75 //�����ֵ�����ٱ�
#define MM_PULSE_R 1 //�����ֱ�����ÿת���������
#define MM_DEAD_RANGE 3 //�����3����������
#define MM_MIN_SPD 0
#define MM_MAX_SPD 100
#define MM_MAX_INTEGRA 20000
#define MM_PID_P  1.2
#define MM_PID_I  0.001


#define CM_DEFAULT_PRE 144 //�����ֵ��Ĭ��Ԥ��Ƶ
#define CM_DEFAULT_PERIOD 100 //������Ĭ�����ڣ��ɶ���100���ٶ� 
#define CM_DEFAULT_SPD 50 //Ĭ���ٶ�50

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

