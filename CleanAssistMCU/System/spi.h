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

//spi comunication 
#define DUMY				0x00
#define HEAD				0x01
#define CMD_COMFIRM		0x02
#define FAULT					0xff
#define CHECK					0x02

//the received cmd
#define CMD_Clean											0x40
#define CMD_CleanSet								0x10
#define CMD_CleanStop									0xBF

//the sent state
#define READY_STATE							0x40
#define WORK_STATE							0x00
//#define DROPOUT_STATE						0x41		//	纸即将用光
#define DROPOUT_STATE						0x01

void SPI1_Init(void);			 //初始化SPI口
static void CSN_Init(void);
#endif

