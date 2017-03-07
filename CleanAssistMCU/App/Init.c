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

extern u8 clean_position,wipe_time;	

void InitAll(void)
{
	u8 FLASH_DATA[3]={0,0,0};
	delay_ms(1000);
	delay_clockint();
	
	uart_init(115200);	
	
	MotorInit();
	
	StepMotorInit();
	
	SPI1_Init();
	//printf("CLEAN ready\n");
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)FLASH_DATA,3);
	
	if(FLASH_DATA[0] == 1){
		//FLASH_DATA[0] = 1;
		//FLASH_DATA[1] = 0;
		//FLASH_DATA[2] = 0;
		clean_position = FLASH_DATA[1];
		wipe_time = FLASH_DATA[2];
		
	}else{
		clean_position = 0;
		wipe_time =0;
		Clean_Reset();
	}
	
}
/******************* (C) COPYRIGHT 2017  *****END OF FILE**********************/
