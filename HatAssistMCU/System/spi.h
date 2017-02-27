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

/**
SPI通讯协议：
接收到[HEAD,MASTER_CMD,DUMY];
DUMY仅为主机提供时钟信号，数据无意义。接收到的意义数据为MASTER_CMD;
MASTER_CMD | 0x0F ==0xFF代表命令指令(高位全为1)
MASTER_CMD | 0x0F ==0x0F代表查询指令(高位全为0)
返回数据 SYS_STATE CMD_COMFIRM
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

void SPI1_Init(void);			 //初始化SPI口
static void CSN_Init(void);
#endif

