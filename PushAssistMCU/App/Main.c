/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：Main.c
*文件标识：
*摘    要：推夹具协处理器主程序				 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/9
*****************************************************************************/
#include "stm32f10x.h"
#include "init.h"


void CntIOInit(void);

extern u8 SYS_STATE,MASTER_CMD;

enum period
{
	//push_period所需状态
	PUSH_START,
	//BACKWARD_S,
	//BACKWARD_E,
	CLAMP_S,
	CLAMP_E,
	FORWARD_S,
	FORWARD_E,
	PUSH_END,
	//open_period所需状态
	//START,
	OPEN_START,
	OPEN_S,
	OPEN_E,
	OPEN_END,
	//END,	
	
	//draw_period所需状态
	//START,
	DRAW_START,
	DRAW_S,
	DRAW_E,
	DRAW_END,
	//END,	
};

enum period status_period = DRAW_END;	

u8 last_state_c=0;
u8 last_state_m=0;
extern u8 Position_Flag_C,Position_Flag_M;
void position_init(void);
int main(void)
{
	InitAll();
	position_init();
	printf("Push Ready\n");
	SYS_STATE = READY_STATE;
// 	while(1){
// 		if(Check_Position_C() != last_state_c){
// 			last_state_c = Check_Position_C();
// 			printf("Check clean\n");
// 		}
// 		if(Check_Position_M() != last_state_m){
// 			last_state_m = Check_Position_M();
// 			printf("Check mid\n");
// 		}
// 	}
	while(1)
	{		
		if((status_period == PUSH_END)||(status_period == OPEN_END)||(status_period == DRAW_END)){
			SYS_STATE = READY_STATE;
			//Fixture_Stop();
		}
		else{
			if(status_period == PUSH_START){
				MCMotion(CLAMP_ANGLE, PUSH_DIR, PUSH_SPEED);
				SCMotion(CLAMP_ANGLE, PUSH_DIR, PUSH_SPEED);
				status_period = CLAMP_S;
			}
// 			else if(status_period == BACKWARD_S){
// 				if(IsMotActDone('P')&&IsMotActDone('C'))	
// 					status_period = BACKWARD_E;
// 			}
// 			else if(status_period == BACKWARD_E){
// 				MCMotion(CLAMP_ANGLE, PUSH_DIR, PUSH_SPEED);
// 				SCMotion(CLAMP_ANGLE, PUSH_DIR, PUSH_SPEED);
// 				status_period = CLAMP_S;
// 			}
			else if(status_period == CLAMP_S){
				if(IsMotActDone('M') && IsMotActDone('S')) 
					status_period = CLAMP_E;
			}
			else if(status_period == CLAMP_E){
				PushMotion(FORWARD_ANGLE, PUSH_DIR, PUSH_SPEED);
				PCMotion(FORWARD_ANGLE, PUSH_DIR, PUSH_SPEED);
				status_period = FORWARD_S;
			}
			else if(status_period == FORWARD_S){
				if(IsMotActDone('P')&&IsMotActDone('C')){
					status_period = FORWARD_E;
					status_period = PUSH_END;
					printf("push done\n");
				}
			}
			
			else if(status_period == OPEN_START){
				MCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
				status_period =OPEN_S;					
			}
			else if(status_period == OPEN_S){
				if(IsMotActDone('M')){
					status_period = OPEN_E;
					status_period = OPEN_END;
					printf("loosen done\n");
				}
			}

			else if(status_period == DRAW_START){
				//SCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
				position_init();
				status_period = DRAW_E;
				status_period = DRAW_END;
				printf("draw done\n");
				
// 				status_period = DRAW_S;
// 				PushMotion(400, PUSH_DIR, PUSH_SPEED);
// 				PCMotion(400,'+',PUSH_SPEED);
// 				SCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
// 				Position_Flag_M = 0;
// 				Position_Flag_C = 0;
// 			}
// 			else if(status_period == DRAW_S){
// 				if(Position_Flag_M != 0){
// 					PushMotion(0, PUSH_DIR, PUSH_SPEED);
// 				}
// 				if(Position_Flag_C != 0){
// 					PCMotion(0, PUSH_DIR, PUSH_SPEED);	
// 				}
// 				if((Position_Flag_M != 0) && (Position_Flag_C != 0)){
// 					status_period = DRAW_E;
// 					status_period = DRAW_END;
// 					Position_Flag_M = 0;
// 					Position_Flag_C = 0;
// 					printf("draw done\n");
// 				}
			}
		}
			

		if(MASTER_CMD!=DUMY){
			if(Is_Push(MASTER_CMD)){
				status_period = PUSH_START;
				SYS_STATE = WORK_STATE;
			}
			else if(Is_Draw(MASTER_CMD)){
				status_period = DRAW_START;
				SYS_STATE = WORK_STATE;
				//PushMotion(20*PROCESS_ANGLE, '+', SPEED);
			}
			else if(Is_Open(MASTER_CMD)){
				status_period = OPEN_START;
				SYS_STATE = WORK_STATE;
			}
			else if(Is_Stop(MASTER_CMD)){
				Fixture_Stop();
				//SYS_STATE = ACCIDENT_STATE;
			}
			else if(MASTER_CMD == 0x80){	//test the motor and driver
				//GPIO_SetBits(GPIOA,GPIO_Pin_12);
				//while(1){
				PushMotion(180, '-', 100);
				PCMotion(180, '-', 100);
					while(IsMotActDone('P')==0);
				//MCMotion(CLAMP_ANGLE, '-', SPEED);
				printf("done1\n");
					//delay_ms(100);
				//}
			}
			else if(MASTER_CMD == 0x60){	//test the optical gate
				while(1){
					PushMotion(400, '-', 100);
					PCMotion(180, '-', 100);
					while(IsMotActDone('P')==0){
						if(Position_Flag_M == 1)
							break;
					}
					Position_Flag_M = 0;
					Fixture_Stop();
					printf("done0\n");
				}
			}
			else if(MASTER_CMD == 0x70){
				SCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
				while(IsMotActDone('S')==0);
				printf("done2\n");
			}
			else if(MASTER_CMD == 0x71){
				SCMotion(CLAMP_ANGLE, '+', CLAMP_SPEED);
				while(IsMotActDone('S')==0);
				printf("done2\n");
			}
			else if(MASTER_CMD == 0x72){
				while(1){
					SCMotion(CLAMP_ANGLE, '+', CLAMP_SPEED);
					while(IsMotActDone('S')==0);
					delay_ms(500);
					SCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
					while(IsMotActDone('S')==0);
					delay_ms(1500);
					MCMotion(CLAMP_ANGLE, '+', CLAMP_SPEED);
					while(IsMotActDone('M')==0);
					delay_ms(500);
					MCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
					while(IsMotActDone('M')==0);
				}
			}			
			printf("%c",MASTER_CMD);
			MASTER_CMD = DUMY;
			
		}
	}
}


void position_init()
{
	int s_flag_m=0,s_flag_c=0;
	PushMotion(400, PUSH_DIR, PUSH_SPEED);
	PCMotion(400,PUSH_DIR,PUSH_SPEED);
	Position_Flag_M = 0;
	Position_Flag_C = 0;
	while(IsMotActDone('P')==0 || IsMotActDone('C')==0){
		if(Position_Flag_M != 0){
			PushMotion(0, PUSH_DIR, PUSH_SPEED);
			Position_Flag_M = 0;
			s_flag_m = 1;
		}
		if(Position_Flag_C != 0){
			PCMotion(0, PUSH_DIR, PUSH_SPEED);		
			Position_Flag_C = 0;
			s_flag_c =1;
		}
		if((s_flag_m != 0) && (s_flag_c != 0)){
			break;
		}
	}
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/


