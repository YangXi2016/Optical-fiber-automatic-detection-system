#ifndef __KEY_H
#define __KEY_H	
#include "sys.h"
/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：IO.txt
*文件标识：
*摘    要：摘录本协处理器使用的IO口			 
*当前版本：1.0
*作    者：yangxi
*完成日期：2017/2/10
*****************************************************************************/ 
 

#define HOLDER_PLACE_1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)//读取微动开关1
#define HOLDER_PLACE_2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)//读取微动开关2
//#define SYS_START				GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)//读取开始暂停


void KEY_Init(void);//IO初始化
u8 Check_ready(void);  	//按键扫描函数					    
#endif
