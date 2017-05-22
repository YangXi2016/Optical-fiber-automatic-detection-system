/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：Main.c
*文件标识：
*摘    要：推夹具协处理器主程序				 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/14
*****************************************************************************/
#include "init.h"


extern u8 SYS_STATE,MASTER_CMD;
u8 temp;
int main(void)
{
	//u8 CCDRes[128] = {0};
	//u16 i = 0;
	u8 status;
	InitAll();
	printf("HAT ready\n");
	SYS_STATE = READY_STATE;
	while(1)
	{
		if(IsMotActDone('H') && IsMotActDone('T')){
			SYS_STATE = READY_STATE;
			MotorEN('H','D');
// 			MotorEN('T','D');		this motor's driver can't support
		}
		if(IsMotActDone('H'))
			MotorEN('H','D');
// 		if(IsMotActDone('T'))
// 			MotorEN('T','D');		
		if(MASTER_CMD != DUMY){
			USART1->DR=MASTER_CMD;
			while((USART1->SR&0X40)==0);
			
			if(MASTER_CMD == 0x01){
				HMotion(10, '+', HAT_SPEED);
			}
			else if(MASTER_CMD == 0x02){
				HMotion(10, '-', HAT_SPEED);
			}
			else if(MASTER_CMD == CMD_Hat){	//带帽前检测帽子是否还有
				SYS_STATE = WORK_STATE;
				status = IsHatExist();
				if(status == 0){
					SYS_STATE = HATNULL_STATE;
					printf("HAT NULL\n");
					//while(1);
				}
				else{
					Hat();
					printf("HAT exist\n");
				}
				while(1){
					while(IsMotActDone('H')==0);
					Hat();
				}
				Hat();
			}
			else if(MASTER_CMD == CMD_HatCheck){
				SYS_STATE = WORK_STATE;
				//Hat_Check();
				status = IsHatDone();		//注意，这里是堵塞式执行，没有得出结果时将一直停留在这里，不能执行新接收到的信号。
				if(status == 'Y') SYS_STATE = HATED_STATE;
				else SYS_STATE = UNHAT_STATE;
			}
			else if(MASTER_CMD == CMD_RailRunStation){
				SYS_STATE = WORK_STATE;
				Rail_RunStation();
			}
			else if(MASTER_CMD == CMD_RailRunToStation){
				printf("RunToStation\n");
				SYS_STATE = WORK_STATE;
				Rail_RunToStation();
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
			else if(MASTER_CMD == CMD_TuneBack){
				SYS_STATE = WORK_STATE;
				Tune_Back();
			}
			else if(MASTER_CMD == CMD_TuneForward){
				SYS_STATE = WORK_STATE;
				Tune_Forward();
			}
			else if(MASTER_CMD == 0x33){
				while(1){
				MTMotion(300, '+', 800);
				while(IsMotActDone('T')==0);
				MTMotion(300, '-', 800);
				while(IsMotActDone('T')==0);				
				}
			}
			MASTER_CMD = DUMY;
		}
		
	}
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
