/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�spi.c
*�ļ���ʶ��
*ժ    Ҫ��ժ¼��Э������ʹ�õ�IO��			 
*��ǰ�汾��1.0
*��    �ߣ�yangxi
*������ڣ�2017/2/15
*****************************************************************************/

#ifndef __SPI_H
#define __SPI_H
#include "sys.h"  

#define DUMY	0x00
#define HEAD	0x01

void SPI1_Init(void);			 //��ʼ��SPI��
static u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�

u8 DETECT_ReadWriteByte(u8 TxData);//��ӻ�����ָ��õ���Ӧ�ķ���


#define CSN_PUSH_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define CSN_PUSH_H()		GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define CSN_CLEAN_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define CSN_CLEAN_H()		GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define CSN_HAT_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define CSN_HAT_H()		GPIO_SetBits(GPIOB,GPIO_Pin_12)

void SPI2_Init(void);		//��Э��������SPIͨ�ſ�(�̶�Ϊ����ģʽ)
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�

u8 PUSH_ReadWriteByte(u8 TxData);//��ӻ�����ָ��õ���Ӧ�ķ���
u8 CLEAN_ReadWriteByte(u8 TxData);//��ӻ�����ָ��õ���Ӧ�ķ���
u8 HAT_ReadWriteByte(u8 TxData);//��ӻ�����ָ��õ���Ӧ�ķ���

#endif
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

