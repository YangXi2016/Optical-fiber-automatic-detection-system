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
#define ERROR					0xff
#define CHECK					0x02

//判断从STM32接收到的命令
#define Is_Detect(CMD)			((CMD & 0x40) == 0x40)
#define Is_Head(CMD)				((CMD & 0x20) == 0x20)
#define Is_Tail(CMD)				((CMD & 0x10) == 0x10)

#define Is_SafeGateErr(CMD)	((CMD & 0x80) == 0x80)
#define Is_HatNull(CMD)			((CMD & 0x90) == 0x90)
#define Is_TissueNull(CMD)	((CMD & 0xC1) == 0xC1)
#define Is_ClearFlag(CMD)		((CMD & 0x08)	== 0x08)

//准备发送给STM32的状态
#define START_STATE			0x80
#define TRUE_RESULT			0x20 
//#define FALSE_RESULT		0x40 //该状态即TRUE_SRATE取反，废除
#define READY_STATE			0x40 //STM_STATE = (STM_STATE | 0x40)
#define CLEANSET_STATE	0x10 //after change tissue reset the record.
//#define WORK_STATE			0x00 //STM_STATE = (STM_STATE & 0xBF)  该状态即READY_SRATE取反，废除
//除WORK_STATE使用&来清除标志位，其他状态都使用'|'来置标志位

void SPI1_Init(uint16_t Mode);			 //初始化SPI口
#endif

