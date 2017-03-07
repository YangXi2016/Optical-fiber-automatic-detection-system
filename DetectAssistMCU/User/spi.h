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
返回数据 STM_STATE CMD_COMFIRM
*/
//u8 STM_STATE = 0x00;
//u8 MASTER_CMD = 0x00;
/***/

#define CMD_COMFIRM		0x02
#define ERROR_STATE				0xff

#define CHECK					0x02

//判断从STM32接收到的命令
#define Is_Detect(CMD)			((CMD & 0x40) == 0x40)
#define Is_Clamp(CMD)				((CMD & 0x20) == 0x20)
#define Is_Loosen(CMD)			((CMD & 0x10) == 0x10)
#define Is_Head(CMD)				((CMD & 0x08) == 0x08)
#define Is_Tail(CMD)				((CMD & 0x04) == 0x04)


//准备发送给STM32的状态
#define START_STATE			0x80
#define TRUE_RESULT			0x60 //STM_STATE =
#define FALSE_RESULT		0x40 //
#define READY_STATE			0x40 //STM_STATE = (STM_STATE | 0x40)
#define WORK_STATE			0x00 //STM_STATE = (STM_STATE & 0xBF)
	

void SPI1_Init(uint16_t Mode);			 //初始化SPI口
#endif

