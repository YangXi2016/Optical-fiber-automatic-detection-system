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

#define CMD_Hat									0x40	//(CMD | 0x40)     //0100,0000
#define CMD_HatStop 						0xBF	//(CMD & 0xBF)			//1011,1111
#define CMD_RailRunStation 			0x20	//(CMD | 0x20)     	//0010,0000
#define CMD_RailRunToStation		0x80
#define CMD_RailBack 						0x10	//(CMD | 0x10)     	//0001,0000
#define CMD_TuneBack						0x30
#define CMD_TuneForward					0x0C	
#define CMD_RailForward 				0x08	//(CMD | 0x08)			//0000,1000
#define CMD_RailStop 						0xC7	//(CMD & 0xC7)			//1100,0111
#define CMD_HatCheck						0x04
#define CMD_AllStop							0x87
#define CMD_HatInit							0x88

#define READY_STATE							0x40
#define WORK_STATE							0x00
#define HATED_STATE							0x60
#define UNHAT_STATE							0x40
#define HATNULL_STATE						0x10
#define HATEXIST_STATE					0x40
#define ACCIDENT_STATE					0xff

void SPI1_Init(void);			 //初始化SPI口
static void CSN_Init(void);
#endif

