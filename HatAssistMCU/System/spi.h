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

#define CMD_Hat									0x40	//(CMD | 0x40)     //0100,0000
#define CMD_HatStop 						0xBF	//(CMD & 0xBF)			//1011,1111
#define CMD_RailRunStation 			0x20	//(CMD | 0x20)     	//0010,0000
#define CMD_RailBack 						0x10	//(CMD | 0x10)     	//0001,0000
#define CMD_RailForward 				0x08	//(CMD | 0x08)			//0000,1000
#define CMD_RailStop 						0xC7	//(CMD & 0xC7)			//1100,0111
#define CMD_HatCheck						0x04
#define CMD_AllStop							0x87


#define READY_STATE							0x40
#define WORK_STATE							0x00
#define HATED_STATE							0x60
#define UNHAT_STATE							0x40
#define HATNULL_STATE						0x10
#define HATEXIST_STATE					0x40

void SPI1_Init(void);			 //��ʼ��SPI��
static void CSN_Init(void);
#endif

