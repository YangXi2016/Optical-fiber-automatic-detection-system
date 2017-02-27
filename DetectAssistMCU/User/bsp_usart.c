/**
 * @brief  USART1 GPIO���ã�����ģʽ����
 *         �ض���printf��scanf��USART������ͨ�����ڴ�ӡ�����Գ���
 * @param  ��
 * @retval ��
 */

#include "bsp_usart.h"

void USART1_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//�����ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;

	/* GPIOA��USART1ʱ������ */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	 
	
	/* USART GPIO���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//USART1_TX�������������50MHz��ת
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//USART1_RX���ø������룬50MHz��ת
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/* USART1����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200;	//������9600bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//��żУ����
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ����������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//˫��ȫ˫��
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);	
}

void USART1_ITConfigure()
{
	NVIC_USART1_Configure();
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	
}

/* �ض���C�⺯��printf��USART1 */
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(uint8_t)ch);	//ʹ��USART1����һ���ֽ�
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);	//�ȴ�������� 
	return(ch);
}

/* �ض���C�⺯��scanf��USART1 */
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);		//�ȴ�����1��������
		return (int)USART_ReceiveData(USART1);
}

/* NVIC����USART1�ж� */
static void NVIC_USART1_Configure()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);						//��ռ1λ����Ӧ3λ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}
	

