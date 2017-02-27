#ifndef __BSP_USART1_H
#define	__BSP_USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define CMD_Detect	"SDE\n"
#define CMD_Head		"SIE\n"
#define CMD_Tail		"SOE\n"


struct STATUS{
	u8 Union_Status;		//0���������ǣ�1�����Զ����ϵͳ
	u8 Detect_Status;		//��ʼ����ź�
	u8 Clamp_Status;		//�н�״̬
	u8 Period_Status;		//0������δ���빤λ��1�����ڵ�һ����λ��2�������м乤λ��3���������һ����λ��4�����뿪��λ
} ;

void USART1_Config(void);
void USART1_ITConfigure(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
static void NVIC_USART1_Configure(void);

#endif /* __USART1_H */
