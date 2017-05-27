/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：spi.c
*文件标识：
*摘    要：摘录本协处理器使用的IO口			 
*当前版本：1.0
*作    者：yangxi
*完成日期：2017/2/15
*****************************************************************************/

#ifndef __SPI_H
#define __SPI_H
#include "sys.h"  

#define DUMY	0x00
#define HEAD	0x01
#define FAULT	0xff

void SPI1_Init(void);			 //初始化SPI口
static u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节

u8 DETECT_ReadWriteByte(u8 TxData);//向从机发送指令并得到相应的反馈


#define CSN_PUSH_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define CSN_PUSH_H()		GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define CSN_CLEAN_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define CSN_CLEAN_H()		GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define CSN_HAT_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define CSN_HAT_H()		GPIO_SetBits(GPIOB,GPIO_Pin_12)

void SPI2_Init(void);		//与协处理器的SPI通信口(固定为主机模式)
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节

u8 PUSH_ReadWriteByte(u8 TxData);//向从机发送指令并得到相应的反馈
u8 CLEAN_ReadWriteByte(u8 TxData);//向从机发送指令并得到相应的反馈
u8 HAT_ReadWriteByte(u8 TxData);//向从机发送指令并得到相应的反馈

#endif
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

