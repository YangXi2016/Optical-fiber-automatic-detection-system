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


#include "spi.h"
//#include "delay.h"
#include "StepperMotor.h"	  
/*SYS_STATE为全局变量，可代表当前的系统状态*/ 
u8 SYS_STATE = DUMY;//CHECK;
u8 MASTER_CMD = DUMY;
/*MASTER_CMD为接收到的指令，有命令和查询两种*/
//收发过程由中断完成


/*局部变量,接收中断中使用*/
static u8 head_flag=0,Slave_Temp = 0 ;
					  
//SPI口初始化
//这里针是对SPI1的初始化

SPI_InitTypeDef  SPI_InitStructure;

void SPI1_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;//定义中断结构体
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel=SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
  NVIC_Init(&NVIC_InitStructure);	 
	
	SPI_Cmd(SPI1, DISABLE); //使能SPI外设
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;		//设置SPI工作模式:设置为从SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 	
	CSN_Init();
	 
}   



void CSN_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; //外部中断配置结参数
	NVIC_InitTypeDef NVIC_InitStructure; //中断配置参数
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA ,GPIO_PinSource4); 
	
	//外部触发设置，边沿
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
	
	//中断参数设定
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//优先级全局最低
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
}
void EXTI4_IRQHandler(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(EXTI_GetITStatus(EXTI_Line4) == SET){
		u8 state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
		if( state == 0){
			
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA,&GPIO_InitStructure);
			SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);//开启中断
			SPI_Cmd(SPI1, ENABLE); //使能SPI外设
			SPI_I2S_ReceiveData(SPI1);
			
		}else{
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA,&GPIO_InitStructure);
			
			SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,DISABLE);//开启中断
			SPI_Cmd(SPI1, DISABLE); //使能SPI外设
			SPI_I2S_ReceiveData(SPI1);
			
		}
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI4线路挂起
	}
	
}



void SPI1_IRQHandler(void)
{	
	  if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==SET){
			Slave_Temp = SPI_I2S_ReceiveData(SPI1);
		if(head_flag == 1){
			if(Slave_Temp == DUMY){
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, DUMY);
				head_flag = 0;
			}else{
				MASTER_CMD = Slave_Temp;
				if(MASTER_CMD == CHECK){
					while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, SYS_STATE);
				}else{
					if(Is_Stop(MASTER_CMD))	Fixture_Stop();
					while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, CMD_COMFIRM);

				}
			}
		}else{
			if(Slave_Temp == HEAD){
				head_flag=1;
				SPI_I2S_SendData(SPI1, HEAD);
			}
		}
	
	}	
} 
