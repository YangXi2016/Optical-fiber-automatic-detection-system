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
u8 position,temp;

double absolute_angles[38],relative_angles[38];
u32 pulses[38];

void coordinate_conversion(double first_distance,double station_distance){
	absolute_angles[0]=0;
	absolute_angles[1]=(first_distance*360/75);
	pulses[0]=0;
	relative_angles[0]=0;
	for(temp = 2;temp<38;temp++){
		absolute_angles[temp]=absolute_angles[temp-1]+(station_distance*360/75);
	}

	for(temp =1;temp<38;temp++){
		pulses[temp]=(u32)(absolute_angles[temp]/360*MT_MOTOR_DIV + 0.5);
	}

	for(temp =1;temp<38;temp++){
		relative_angles[temp]=(1.0*(pulses[temp]-pulses[temp-1]))/MT_MOTOR_DIV*360;
	}
	
}
int main(void)
{
	u8 status;
	InitAll();
	if(ModeCheck()==1){
		printf("mode in 8 degree\n");
		coordinate_conversion(54.0,14.0);	//单位为mm.转8度和正常光纤检测位置出现了偏差。
	}
	else{
		printf("mode in straight\n");
		coordinate_conversion(53.4,14.0);
	}
	printf("HAT ready\n");
	SYS_STATE = READY_STATE;
	while(1)
	{
		if(IsMotActDone('H') && IsMotActDone('T')){
			SYS_STATE |= READY_STATE;
		}	
		if(MASTER_CMD != DUMY){
			if(MASTER_CMD == CMD_Hat){	//带帽前检测帽子是否还有
				SYS_STATE = WORK_STATE;
				status = IsHatExist();
				if(status == 0){
					SYS_STATE |= HATNULL_STATE;
					while(IsHatExist()==0);
					Hat();
					delay_ms(1000);
					SYS_STATE &= (~HATNULL_STATE);
					SYS_STATE |= READY_STATE;
				}
				else{
					SYS_STATE &= (~HATNULL_STATE);
					Hat();
				}
			}
			else if(MASTER_CMD == CMD_HatInit){
				SYS_STATE = WORK_STATE;
				Hat_Init();
				SYS_STATE = READY_STATE;
			}
			else if(MASTER_CMD == CMD_RailRunStation){
				SYS_STATE = WORK_STATE;
				position++;
				MTMotion(relative_angles[position], '+', RAIL_STATION_SPEED);
			}
			else if(MASTER_CMD == CMD_RailRunToStation){
				SYS_STATE = WORK_STATE;
				position = 1;
				MTMotion(relative_angles[position], '+', RAIL_STATION_SPEED);
				
			}
			else if(MASTER_CMD == CMD_RailBack){
				SYS_STATE = WORK_STATE;
				MTMotion(absolute_angles[position], '-', RAIL_FREE_SPEED);
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
			MASTER_CMD = DUMY;
		}
		
	}
}


	
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
