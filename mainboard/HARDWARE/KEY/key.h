#ifndef __KEY_H
#define __KEY_H	
#include "sys.h"
/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�IO.txt
*�ļ���ʶ��
*ժ    Ҫ��ժ¼��Э������ʹ�õ�IO��			 
*��ǰ�汾��1.0
*��    �ߣ�yangxi
*������ڣ�2017/2/10
*****************************************************************************/ 
 

#define HOLDER_PLACE_1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)//��ȡ΢������1
#define HOLDER_PLACE_2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)//��ȡ΢������2
//#define SYS_START				GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)//��ȡ��ʼ��ͣ


void KEY_Init(void);//IO��ʼ��
u8 Check_ready(void);  	//����ɨ�躯��					    
#endif
