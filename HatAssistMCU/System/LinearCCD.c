/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：LinearCCD.c
*文件标识：
*摘    要：线性ccd配置，调用接口等			 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/14
*****************************************************************************/

#include "LinearCCD.h"
#include "stm32f10x.h"
#include "delay.h"

void LinCCDGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // 使能PA端口时钟
	
	//ADC接口 PA0
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//CLK PA1  SI PA2接口
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
}
void LinCCDADCInit(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器 

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束		
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
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5 );	//ADC1,ADC通道,采样时间为13.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

void GetCCD(u8 *CCDRes)
{
	u8 i = 0;
	u16 tempRes = 0;
	
	SI_HIGH;//开始读取	
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
