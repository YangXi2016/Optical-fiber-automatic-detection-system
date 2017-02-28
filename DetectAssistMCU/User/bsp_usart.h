#ifndef __BSP_USART1_H
#define	__BSP_USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define CMD_Detect	"SDE\n"
#define CMD_Head		"SIE\n"
#define CMD_Tail		"SOE\n"


struct STATUS{
	u8 Union_Status;		//0代表仅检测仪，1代表自动检测系统
	u8 Detect_Status;		//开始检测信号
	u8 Clamp_Status;		//夹紧状态
	u8 Period_Status;		//0代表弹夹未进入工位，1代表在第一个工位，2代表在中间工位，3代表在最后一个工位，4代表离开工位
} ;

void USART1_Config(void);
void USART1_ITConfigure(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
static void NVIC_USART1_Configure(void);

#endif /* __USART1_H */
