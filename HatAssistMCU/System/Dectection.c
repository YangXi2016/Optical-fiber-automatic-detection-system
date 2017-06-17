/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：Dectection.c
*文件标识：
*摘	   要：戴帽部分检测用接口配置，并提供接口		 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/14
*****************************************************************************/
#include "Dectection.h"

/*******************************************************************************
*函数名称：HatExistDectInit
*函数说明：保护帽断料检测初始化
*输入参数：无
*输出参数：无
*返回参数：无 
*******************************************************************************/
void HatExistDectInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // 使能PA端口时钟
	
	//检测接口 PA3
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //内部上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
*函数名称：IsHatExist
*函数说明：保护帽断料与否
*输入参数：无
*输出参数：未断料返回1，断料返回0
*返回参数：无 
*******************************************************************************/
u8 IsHatExist(void)
{
	u8 state;
	state =	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
	if(state == 0){
		delay_ms(10);
		state =	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
		if(state == 0) return 1;
	}
	return 0;
	//return 1;//for test.
}

/*******************************************************************************
*函数名称：ModeCheckInit
*******************************************************************************/
void ModeCheckInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // 使能PA端口时钟
	
	//检测接口 PA3
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //内部上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
*函数名称：ModeCheck
*函数说明：选择不同的光纤头检测模式
*输入参数：无
*输出参数：根据对应GPIO口电平选择；高电平返回1，低电平返回0。
*返回参数：无 
*******************************************************************************/
u8 ModeCheck(void)
{
	u8 state;
	state =	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
	if(state == 1){
		delay_ms(10);
		state =	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
		if(state == 1) return 1;
	}
	return 0;
	//return 1;//for test.
}
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
