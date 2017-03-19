#ifndef __BSP_USART1_H
#define	__BSP_USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define CMD_Detect	"SDE\n"
#define CMD_Head		"SHE\n"
#define CMD_Tail		"STE\n"
#define CMD_ERROR		"SXE\n"

struct STATUS{
	u8 Union_Status;		//0代表仅检测仪，1代表自动检测系统
	u8 Detect_Status;		//开始检测信号
	u8 Clamp_Status;		//夹紧状态
	u8 Period_Status;		//0代表弹夹未进入工位，1代表在第一个工位，2代表在中间工位，3代表在最后一个工位，4代表离开工位
	u8 ERROR_Status;		//0代表正常运行，1代表安全光栅错误，2代表帽子用完，3代表纸巾用完
} ;

void USART1_Config(void);
void USART1_ITConfigure(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
static void NVIC_USART1_Configure(void);

#endif /* __USART1_H */
