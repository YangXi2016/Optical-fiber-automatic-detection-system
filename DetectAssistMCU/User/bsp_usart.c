/**
 * @brief  USART1 GPIO配置，工作模式配置
 *         重定向printf和scanf到USART，方便通过串口打印来调试程序
 * @param  无
 * @retval 无
 */

#include "bsp_usart.h"

void USART1_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//定义初始化结构体
	USART_InitTypeDef USART_InitStructure;

	/* GPIOA和USART1时钟配置 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	 
	
	/* USART GPIO配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//USART1_TX配置推挽输出，50MHz翻转
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//USART1_RX配置浮空输入，50MHz翻转
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/* USART1工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率9600bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止1位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//奇偶校验无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制无
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//双线全双工
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);	
}

void USART1_ITConfigure()
{
	NVIC_USART1_Configure();
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	
}

/* 重定向C库函数printf到USART1 */
int fputc(int ch,FILE *f)
{
// 	USART_SendData(USART1,(uint8_t)ch);	//使用USART1发送一个字节
// 	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);	//等待发送完成 
// 	return(ch);
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}

/* 重定向C库函数scanf到USART1 */
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);		//等待串口1输入数据
		return (int)USART_ReceiveData(USART1);
}

/* NVIC设置USART1中断 */
static void NVIC_USART1_Configure()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//抢占1位，响应3位
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}
	

