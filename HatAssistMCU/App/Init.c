/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�init.c
*�ļ���ʶ��
*ժ    Ҫ��ϵͳ��ʼ��			 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/14
*****************************************************************************/
#include "init.h"

void InitAll(void)
{
	
	delay_clockint();
	
	UsartInit(115200);	
	
	StepMotorInit();
	
	HatExistDectInit();
	
	LinCCDInit();
	SPI1_Init();
	
	printf("HAT ready\n");


}
/******************* (C) COPYRIGHT 2017  *****END OF FILE**********************/
