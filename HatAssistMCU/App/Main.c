/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�Main.c
*�ļ���ʶ��
*ժ    Ҫ���Ƽо�Э������������				 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/14
*****************************************************************************/
#include "init.h"


extern u8 SYS_STATE,MASTER_CMD;

int main(void)
{
	//u8 CCDRes[128] = {0};
	//u16 i = 0;
	u8 status;
	InitAll();
	SYS_STATE = READY_STATE;
	while(1)
	{
		if (IsHatExist() == 0){
			All_Stop();
			SYS_STATE = HATNULL_STATE;
			printf("HATNULL");
			while(1);
		}
		if(IsMotActDone('H') && IsMotActDone('T')) SYS_STATE = READY_STATE;
		if(MASTER_CMD != DUMY){
			USART1->DR=MASTER_CMD;
			while((USART1->SR&0X40)==0);
			
			if(MASTER_CMD == CMD_Hat){
				SYS_STATE = WORK_STATE;
				Hat();
			}
			else if(MASTER_CMD == CMD_HatCheck){
				SYS_STATE = WORK_STATE;
				//Hat_Check();
				status = IsHatDone();		//ע�⣬�����Ƕ���ʽִ�У�û�еó����ʱ��һֱͣ�����������ִ���½��յ����źš�
				if(status == 'Y') SYS_STATE = HATED_STATE;
				else SYS_STATE = UNHAT_STATE;
			}
			else if(MASTER_CMD == CMD_RailRunStation){
				SYS_STATE = WORK_STATE;
				Rail_RunStation();
			}
			else if(MASTER_CMD == CMD_RailBack){
				SYS_STATE = WORK_STATE;
				Rail_Back();
			}
			else if(MASTER_CMD == CMD_RailForward){
				SYS_STATE = WORK_STATE;
				Rail_Forward();
			}
			else if(MASTER_CMD == CMD_RailStop){
				Rail_Stop();
			}
			else if(MASTER_CMD == CMD_AllStop){
				All_Stop();
			}
			MASTER_CMD = DUMY;
		}
		
	}
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
