/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�Dectection.c
*�ļ���ʶ��
*ժ	   Ҫ����ñ���ּ���ýӿ����ã����ṩ�ӿ�		 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/14
*****************************************************************************/
#include "Dectection.h"

/*******************************************************************************
*�������ƣ�HatExistDectInit
*����˵��������ñ���ϼ���ʼ��
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void HatExistDectInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // ʹ��PA�˿�ʱ��
	
	//���ӿ� PA3
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //�ڲ�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
*�������ƣ�IsHatExist
*����˵��������ñ�������
*�����������
*���������δ���Ϸ���1�����Ϸ���0
*���ز������� 
*******************************************************************************/
u8 IsHatExist(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
}
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
