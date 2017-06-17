/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：Main.c
*文件标识：
*摘    要：擦拭机构协处理器主程序				 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/9
*****************************************************************************/

#include "Init.h"

u8 i;

extern u8 MASTER_CMD,SYS_STATE;
extern u8 clean_position,wipe_time;	
extern u8 FLASH_DATA[3];
int main(void)
{
	InitAll();
	SYS_STATE |= READY_STATE;
	printf("CLEAN ready\n");
	
	while(1)
	{
		if(wipe_time > MAX_TIME){
			SYS_STATE |= DROPOUT_STATE;
		}else{
			SYS_STATE &= (~DROPOUT_STATE);
		}
		if((IsStepMotActDone()) && (IsDCMotActDone()))	SYS_STATE |= READY_STATE;
		if(MASTER_CMD != DUMY){
			if(MASTER_CMD == CMD_Clean){
				SYS_STATE &= WORK_STATE;
				Clean();
			}
			else if(MASTER_CMD == CMD_CleanSet){//motor don't need reset,just data.
				clean_position = 0;
				wipe_time = 0;
				FLASH_DATA[0] = 1;
				FLASH_DATA[1] = clean_position;
				FLASH_DATA[2] = wipe_time;
				STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)FLASH_DATA,3);
			}
			else if(MASTER_CMD == CMD_CleanStop){
				SYS_STATE &= WORK_STATE;
				Clean_Stop();
			}
			MASTER_CMD = DUMY;
		}
	}
}


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
