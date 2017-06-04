/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：spi.c
*文件标识：
*摘	   要：检测仪上STM32f103C8T6做从机与主控STM32的通信	 
*当前版本：1.2
*作    者：yangxi
*完成日期：2017/2/15
*****************************************************************************/

#ifndef __SPI_H
#define __SPI_H
//#include "sys.h"
#include "stm32f10x.h"

#define DUMY				0x00
#define HEAD				0x01
#define CMD_COMFIRM		0x02
#define FAULT					0xff
#define CHECK					0x02

#define Is_Init(CMD)								(CMD ==0x88)
#define Is_Push(CMD)								(CMD ==0x40)
#define Is_Draw(CMD)								(CMD ==0x20)
#define Is_Open(CMD)								(CMD ==0x10)
#define Is_Stop(CMD)								(CMD ==0x87)

#define READY_STATE			0x40//(SYS_STATE = 0x40)// = (SYS_STATE | 0x40)
#define WORK_STATE			0x00//(SYS_STATE = 0x00)//(SYS_STATE & 0xBF)
#define ACCIDENT_STATE	0xff//(SYS_STATE = 0xff)


void SPI1_Init(void);			 //初始化SPI口
static void CSN_Init(void);
#endif

