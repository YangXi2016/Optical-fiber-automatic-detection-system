/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：init.c
*文件标识：
*摘    要：系统初始化			 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/14
*****************************************************************************/
#include "init.h"

void InitAll(void)
{
	
	delay_clockint();
	delay_ms(1000);
	usart_init(115200);	
	
	StepMotorInit();
	ModeCheckInit();
	HatExistDectInit();
	
	SPI1_Init();


}
/******************* (C) COPYRIGHT 2017  *****END OF FILE**********************/
