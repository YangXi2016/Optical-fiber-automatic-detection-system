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
//u32 tatal_angles,last_angles,angles,total_pulses,last_pulses,pulses;
double absolute_angles[38],relative_angles[38];
u32 pulses[38],relative_pulses[38],real_pulses[38];

void coordinate_conversion(double first_distance,double station_distance){
	absolute_angles[0]=0;
	absolute_angles[1]=(first_distance*360/75);
	pulses[0]=0;
	relative_angles[0]=0;
	relative_pulses[0]=0;
	real_pulses[0]=0;
	for(temp = 2;temp<38;temp++){
		absolute_angles[temp]=absolute_angles[temp-1]+(station_distance*360/75);
//		printf("absolute_angles[%d] = %.4f\n",temp,absolute_angles[temp]);
	}

	for(temp =1;temp<38;temp++){
		pulses[temp]=(u32)(absolute_angles[temp]/360*MT_MOTOR_DIV + 0.5);
//		printf("pulses[%d] = %d\n",temp,pulses[temp]);
	}

	for(temp =1;temp<38;temp++){
		relative_angles[temp]=(1.0*(pulses[temp]-pulses[temp-1]))/MT_MOTOR_DIV*360;
//		printf("relative_angles[%d] = %.4f\n",temp,relative_angles[temp]);
	}
	
// 	for(temp =1;temp<38;temp++){
// 		relative_pulses[temp]=(u32)(relative_angles[temp]/360*MT_MOTOR_DIV + 0.5);;
// 		printf("relative_pulses[%d] = %d\n",temp,relative_pulses[temp]);
// 	}
// 	for(temp =1;temp<38;temp++){
// 		real_pulses[temp]=real_pulses[temp-1]+ relative_pulses[temp];
// 		printf("real_pulses[%d] = %d\n",temp,real_pulses[temp]);
// 	}
// 	
	
}
int main(void)
{
	//u8 CCDRes[128] = {0};
	//u16 i = 0;
	u8 status;
	InitAll();
	if(ModeCheck()==1){
		printf("mode in 8 degree\n");
		coordinate_conversion(54.0,14.0);
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
			//MotorEN('H','D');
// 			MotorEN('T','D');		this motor's driver can't support
		}
		//if(IsMotActDone('H'))
			//MotorEN('H','D');
// 		if(IsMotActDone('T'))
// 			MotorEN('T','D');		
		if(MASTER_CMD != DUMY){
// 			USART1->DR=MASTER_CMD;
// 			while((USART1->SR&0X40)==0);
				printf("%02X\n",MASTER_CMD);
// 			if(MASTER_CMD == 0x01){
// 				HMotion(10, '+', HAT_SPEED);
// 			}
// 			else if(MASTER_CMD == 0x02){
// 				HMotion(10, '-', HAT_SPEED);
// 			}
			if(MASTER_CMD == CMD_Hat){	//带帽前检测帽子是否还有
				SYS_STATE = WORK_STATE;
				status = IsHatExist();
				if(status == 0){
					SYS_STATE |= HATNULL_STATE;
// 					printf("HAT NULL\n");
					while(IsHatExist()==0);
// 					printf("HAT exist again\n");
					Hat();
					delay_ms(1000);
					SYS_STATE &= (~HATNULL_STATE);
					SYS_STATE |= READY_STATE;
					//while(1);
				}
				else{
					SYS_STATE &= (~HATNULL_STATE);
					Hat();
// 					printf("HAT exist\n");
				}
// 				while(1){
// 					while(IsMotActDone('H')==0);
// 					Hat();
// 				}
// 				Hat();
			}
			else if(MASTER_CMD == CMD_HatInit){
				SYS_STATE = WORK_STATE;
				Hat_Init();
				SYS_STATE = READY_STATE;
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
				position++;
				MTMotion(relative_angles[position], '+', RAIL_STATION_SPEED);
				//Rail_RunStation();
			}
			else if(MASTER_CMD == CMD_RailRunToStation){
				printf("RunToStation\n");
				SYS_STATE = WORK_STATE;
				position = 1;
				MTMotion(relative_angles[position], '+', RAIL_STATION_SPEED);
				//Rail_RunToStation();
				
			}
			else if(MASTER_CMD == CMD_RailBack){
				printf("RailBack:\n");
				printf("position: %d \t absolute_angle: %.4f \t RAIL_STATION_SPEED: %d \n",position,absolute_angles[position],RAIL_STATION_SPEED);
				SYS_STATE = WORK_STATE;
				MTMotion(absolute_angles[position], '-', RAIL_FREE_SPEED);
				//Rail_Back();
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
				MTMotion(2700, '+', 800);
				while(IsMotActDone('T')==0);
				delay_ms(1000);
				MTMotion(2700, '-', 800);
				while(IsMotActDone('T')==0);	
				delay_ms(1000);					
				}
			}
			else if(MASTER_CMD == 0xe1)
				HMotion(HAT_ANGLE, '+', HAT_SPEED);
			else if(MASTER_CMD == 0xe2)
				HMotion(HAT_ANGLE, '-', HAT_SPEED);
			
			else if(MASTER_CMD == 0xee){
				position = 29;
				printf("absolute_angle:%.4f\n",absolute_angles[position]);
				printf("absolute_pulse:%d\n",pulses[position]);
				MTMotion(absolute_angles[position], '+', 300);
			}
			MASTER_CMD = DUMY;
		}
		
	}
}


	
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
