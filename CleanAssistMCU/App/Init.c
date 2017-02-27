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

extern u8 clean_position,wipe_time;	

void InitAll(void)
{
	u8 FLASH_DATA[3];
	
	delay_clockint();
	
	UsartInit(115200);	
	
	MotorInit();

	SPI1_Init();
	
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)FLASH_DATA,3);
	
	if(FLASH_DATA[0] != 1){
		//FLASH_DATA[0] = 1;
		//FLASH_DATA[1] = 0;
		//FLASH_DATA[2] = 0;
		clean_position = FLASH_DATA[1];
		wipe_time = FLASH_DATA[2];
		Clean_Reset();
	}else{
		clean_position = 0;
		wipe_time =0;
	}
	
}
/******************* (C) COPYRIGHT 2017  *****END OF FILE**********************/
