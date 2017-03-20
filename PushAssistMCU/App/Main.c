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

enum status_period
{
	//push_period所需状态
	START,
	STICK_S,
	STICK_E,
	CAMP_S,
	CAMP_E,
	PUSH_S,
	END,
	
	//open_period所需状态
	//START,
	OPEN,
	//END,	
	
	//draw_period所需状态
	//START,
	OPEN_S,
	OPEN_E,
	DRAW_S,
	DRAW_E,
	DETACH
	//END,	
};

enum status_period push_period = END,draw_period = END,open_period = END;	

int main(void)
{
	InitAll();
	printf("Push Ready\n");
	SYS_STATE = READY_STATE;
	while(1)
	{
		if((push_period == END) && (draw_period == END) && (open_period ==END)) 
			SYS_STATE = READY_STATE;
		else{
			if(push_period != END){
				if(push_period == START){
					PushMotion(STICK_ANGLE, '-', SPEED);
					push_period = STICK_S;
				}
				else if(push_period == STICK_S){
					if(IsMotActDone('P'))	push_period = STICK_E;
				}
				else if(push_period == STICK_E){
					MCMotion(CLAMP_ANGLE, '+', SPEED);
					SCMotion(CLAMP_ANGLE, '+', SPEED);
					push_period = CAMP_S;
				}
				else if(push_period == CAMP_S){
					if(IsMotActDone('M') && IsMotActDone('S')) push_period = CAMP_E;
				}
				else if(push_period == CAMP_E){
					PushMotion(PROCESS_ANGLE, '+', SPEED);
					push_period = PUSH_S;
				}
				else if(push_period == PUSH_S){
					if(IsMotActDone('P')){
						push_period = END;
						printf("push done\n");
					}
				}
			}
			
			if(draw_period != END){			
					if(draw_period == START){
					SCMotion(CLAMP_ANGLE, '-', SPEED);
					draw_period =OPEN_S ;
				}
				else if(draw_period == OPEN_S){
					if(IsMotActDone('S')) draw_period = OPEN_E;
				}
				else if(draw_period == OPEN_E){
					PushMotion(PROCESS_ANGLE, '-', SPEED);
					draw_period = DRAW_S;
				}
				else if(draw_period == DRAW_S){
					if(IsMotActDone('P')) draw_period = DRAW_E;
				}
				else if(draw_period == DRAW_E){
					PushMotion(STICK_ANGLE, '+', SPEED);
					draw_period = DETACH;
				}
				else if(draw_period == DETACH){
					if(IsMotActDone('P')){
						draw_period = END;
						printf("draw done\n");
					}
				}
			}
			
			if(open_period != END){
				if(open_period == START){
					MCMotion(CLAMP_ANGLE, '-', SPEED);
					open_period =OPEN;					
				}
				else if(open_period == OPEN){
					if(IsMotActDone('M')){
						open_period = END;
						printf("open done\n");
					}
				}
			}
			
		}
		
		if(MASTER_CMD!=DUMY){
			if(Is_Push(MASTER_CMD)){
				push_period = START;
				SYS_STATE = WORK_STATE;
			}
			else if(Is_Draw(MASTER_CMD)){
				draw_period = START;
				SYS_STATE = WORK_STATE;
				//PushMotion(20*PROCESS_ANGLE, '+', SPEED);
			}
			else if(Is_Open(MASTER_CMD)){
				open_period = START;
				SYS_STATE = WORK_STATE;
			}
			else if(Is_Stop(MASTER_CMD)){
				Fixture_Stop();
				SYS_STATE = ACCIDENT_STATE;
			}

			printf("%c",MASTER_CMD);
			MASTER_CMD = DUMY;
			
		}
	}
}


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/


