/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�spi.c
*�ļ���ʶ��
*ժ	   Ҫ���������STM32f103C8T6���ӻ�������STM32��ͨ��	 
*��ǰ�汾��1.2
*��    �ߣ�yangxi
*������ڣ�2017/2/15
*****************************************************************************/

#ifndef __SPI_H
#define __SPI_H
//#include "sys.h"
#include "stm32f10x.h"

#define DUMY				0x00
#define HEAD				0x01

/**
SPIͨѶЭ�飺
���յ�[HEAD,MASTER_CMD,DUMY];
DUMY��Ϊ�����ṩʱ���źţ����������塣���յ�����������ΪMASTER_CMD;
MASTER_CMD | 0x0F ==0xFF��������ָ��(��λȫΪ1)
MASTER_CMD | 0x0F ==0x0F�����ѯָ��(��λȫΪ0)
�������� SYS_STATE CMD_COMFIRM
*/
//u8 SYS_STATE = 0x00;
//u8 MASTER_CMD = 0x00;
/***/

#define CMD_COMFIRM		0x02
#define ERROR					0xff

#define CHECK					0x02

//#define CMD_Clean(CMD)							(CMD | 0x40)     //0100,0000
//#define CMD_CleanStop(CMD)					(CMD & 0xBF)
#define CMD_Clean											0x40
#define CMD_CleanReset								0x20
#define CMD_CleanStop									0xBF

#define READY_STATE							0x40
#define WORK_STATE							0x00
#define DROPOUT_STATE						0x41		//	ֽ�����ù�

void SPI1_Init(void);			 //��ʼ��SPI��
static void CSN_Init(void);
#endif

