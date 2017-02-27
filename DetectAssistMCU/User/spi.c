/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�spi.c
*�ļ���ʶ��
*ժ	   Ҫ���������STM32f103C8T6���ӻ�������STM32��ͨ��	 
*��ǰ�汾��1.2
*��    �ߣ�yangxi
*������ڣ�2017/2/15
*****************************************************************************/


#include "spi.h"
//#include "delay.h"
 	  
/*STM_STATEΪȫ�ֱ������ɴ���ǰ��ϵͳ״̬*/ 
u8 STM_STATE = DUMY;
u8 MASTER_CMD = DUMY;
/*MASTER_CMDΪ���յ���ָ�������Ͳ�ѯ����*/
//�շ��������ж����

/*�ֲ�����,�����ж���ʹ��*/
static u8 head_flag=0,Slave_Temp = 0 ;

					  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��

SPI_InitTypeDef  SPI_InitStructure;

void SPI1_Init(uint16_t Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;//�����жϽṹ��
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel=SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_Init(&NVIC_InitStructure);	 
	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = Mode;// SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 	
	SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);//�����ж�	
	//SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,ENABLE);//�����ж�
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
	 
}   



void SPI1_IRQHandler(void)
{	
	  if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==SET){

			Slave_Temp = SPI_I2S_ReceiveData(SPI1);
			USART1->DR=Slave_Temp;
			while((USART1->SR&0X40)==0);//�ȴ����ͽ���
		if(head_flag == 1){
			if(Slave_Temp == DUMY){
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				SPI_I2S_SendData(SPI1, DUMY);
				head_flag = 0;
			}else{
				MASTER_CMD = Slave_Temp;
				if(MASTER_CMD == CHECK){
					while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, STM_STATE);
				}else{
					while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
					SPI_I2S_SendData(SPI1, CMD_COMFIRM);
					//printf("%c",STM_STATE);
					//USART1->DR=STM_STATE;
					//while((USART1->SR&0X40)==0);//�ȴ����ͽ���
				}
				USART1->DR=STM_STATE;
				while((USART1->SR&0X40)==0);//�ȴ����ͽ���
			}
		}else{
			if(Slave_Temp == HEAD) head_flag=1;
		}
	
	}	
} 
