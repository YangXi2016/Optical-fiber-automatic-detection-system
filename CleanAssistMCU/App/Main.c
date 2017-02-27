/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�Main.c
*�ļ���ʶ��
*ժ    Ҫ�����û���Э������������				 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/9
*****************************************************************************/

#include "Init.h"

u8 i;

extern u8 MASTER_CMD,SYS_STATE;

int main(void)
{
	InitAll();
	
	SYS_STATE = READY_STATE;
	printf("CLEAN ready\n");
	
	while(1)
	{
		if((IsStepMotActDone()) && (IsDCMotActDone()))	SYS_STATE = READY_STATE;
		if(MASTER_CMD != DUMY){
			if(MASTER_CMD == CMD_Clean){
				SYS_STATE = WORK_STATE;
				Clean();
			}
			else if(MASTER_CMD == CMD_CleanReset){
				SYS_STATE = WORK_STATE;
				Clean_Reset();				
			}
			else if(MASTER_CMD == CMD_CleanStop){
				SYS_STATE = WORK_STATE;
				Clean_Stop();
			}
		}
	}
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
