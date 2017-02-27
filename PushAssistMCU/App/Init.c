/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：init.c
*文件标识：
*摘    要：系统初始化			 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/9
*****************************************************************************/
#include "init.h"

void InitAll(void)
{
	
	delay_clockint();
	UsartInit(115200);	
	SPI1_Init();//这里做从机
	StepMotorInit();


}
/******************* (C) COPYRIGHT 2017  *****END OF FILE**********************/
