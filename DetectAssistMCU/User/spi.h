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
�������� STM_STATE CMD_COMFIRM
*/
//u8 STM_STATE = 0x00;
//u8 MASTER_CMD = 0x00;
/***/

#define CMD_COMFIRM		0x02
#define ERROR_STATE				0xff

#define CHECK					0x02

//�жϴ�STM32���յ�������
#define Is_Detect(CMD)			((CMD & 0x40) == 0x40)
#define Is_Clamp(CMD)				((CMD & 0x20) == 0x20)
#define Is_Loosen(CMD)			((CMD & 0x10) == 0x10)
#define Is_Head(CMD)				((CMD & 0x08) == 0x08)
#define Is_Tail(CMD)				((CMD & 0x04) == 0x04)


//׼�����͸�STM32��״̬
#define TRUE_RESULT			0x60 //STM_STATE =
#define FALSE_RESULT		0x40 //
#define READY_STATE			0x40 //STM_STATE = (STM_STATE | 0x40)
#define WORK_STATE			0x00 //STM_STATE = (STM_STATE & 0xBF)
	

void SPI1_Init(uint16_t Mode);			 //��ʼ��SPI��
#endif

