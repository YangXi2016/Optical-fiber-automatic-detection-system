/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�LinearCCD.c
*�ļ���ʶ��
*ժ    Ҫ������ccd���ã����ýӿڵ�			 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/14
*****************************************************************************/

#include "LinearCCD.h"
#include "stm32f10x.h"
#include "delay.h"

void LinCCDGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // ʹ��PA�˿�ʱ��
	
	//ADC�ӿ� PA0
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//CLK PA1  SI PA2�ӿ�
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
}
void LinCCDADCInit(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ��� 

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����		
}
void LinCCDInit(void)
{
	LinCCDGPIOInit();
	LinCCDADCInit();
	CLK_LOW;
	SI_LOW;
}


u16 GetADC(void)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ13.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

void GetCCD(u8 *CCDRes)
{
	u8 i = 0;
	u16 tempRes = 0;
	
	SI_HIGH;//��ʼ��ȡ	
	delay_us(1);
	for (i=0; i<128; i++)
	{

		CLK_HIGH;
		delay_us(1);
		SI_LOW;		
		delay_us(1);
		CLK_LOW;		
		tempRes = GetADC();		
		if (tempRes >= BIN_THERHOLD)
		{
			CCDRes[i] = 1;
		}
		else
		{
			CCDRes[i] = 0;
		}
		CLK_HIGH;
	}
	CLK_LOW;

}

u8 IsHatDone(void)
{
	u8 i = 0;
	u8 CCDRes[128] = {0};
	u8 wCnt = 1;
	u8 minWidth = 1;
	
	GetCCD(CCDRes);


	while(wCnt != 0)
	{
		wCnt = 0;
		for (i = INTEREST_REG_LEFT; i<INTEREST_REG_LEFT;i++)
		{
			if (CCDRes[i] == 1)
			{
				wCnt ++;
			}		
		}
		if (wCnt != 0 && wCnt < minWidth)
		{
			minWidth = wCnt;
		}
	}
	
	if (minWidth <= UNHAT_MAX_PIXEL)
	{
		return 'N';
	}
	else if (minWidth >= HATED_MIN_PIXEL)
	{
		return 'Y';	
	}
	else 
	{
		return 'U';
	}
}
