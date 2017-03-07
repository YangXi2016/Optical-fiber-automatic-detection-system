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

u8 push_period = 4;	//0代表收到动作命令，1代表三个电机开始夹，2表示三个电机夹紧了，3代表开始向前推，4表示向前推到底了。//，5代表中间电机开始松开，6表示电机完全松开了（即动作完成）。
u8 draw_period = 6;	//0代表收到动作命令，1代表中间电机开始夹，2表示中间电机夹紧了，3代表开始向后拉，4表示向后拉到底了，5代表三个电机开始松开，6表示电机完全松开了（即动作完成）。
u8 close_period= 2;	//0代表收到动作命令，1代表中间电机开始夹，2表示中间电机夹紧了。
u8 open_period = 2;	//0代表收到动作命令，1代表中间电机开始松，2表示中间电机松开了。
int main(void)
{
	InitAll();
	printf("Push Ready\n");
	SYS_STATE = READY_STATE;
	while(1)
	{
		if((push_period == 4) && (draw_period == 6) && (close_period == 2) && (open_period ==2)) 
			SYS_STATE = READY_STATE;
		else{
			if(push_period != 4){
				if(push_period == 0){
					MCMotion(CLAMP_ANGLE, '+', SPEED);
					SCMotion(CLAMP_ANGLE, '+', SPEED);
					push_period = 1;
				}
				else if(push_period == 1){
					if(IsMotActDone('M') && IsMotActDone('S')) push_period = 2;
				}
				else if(push_period == 2){
					PushMotion(PROCESS_ANGLE, '+', SPEED);
					push_period = 3;
				}
				else if(push_period == 3){
					if(IsMotActDone('P')){
						push_period = 4;
						printf("push done\n");
					}
				}
			}
			
			if(draw_period != 6){			
					if(draw_period == 0){
					MCMotion(CLAMP_ANGLE, '+', SPEED);
					draw_period = 1;
				}
				else if(draw_period == 1){
					if(IsMotActDone('M')) draw_period = 2;
				}
				else if(draw_period == 2){
					PushMotion(PROCESS_ANGLE, '-', SPEED);
					draw_period = 3;
				}
				else if(draw_period == 3){
					if(IsMotActDone('P')) draw_period = 4;
				}
				else if(draw_period == 4){
					MCMotion(CLAMP_ANGLE, '-', SPEED);
					SCMotion(CLAMP_ANGLE, '-', SPEED);
					draw_period = 5;
				}
				else if(draw_period == 5){
					if(IsMotActDone('M') && IsMotActDone('S')){
						draw_period = 6;
						printf("draw done\n");
					}
				}
			}
			if(close_period!= 2){
				if(close_period == 0){
					MCMotion(CLAMP_ANGLE, '+', SPEED);
					close_period =1;
				}
				else if(close_period == 1){
					if(IsMotActDone('M')){
						close_period = 2;
						printf("close done\n");
					}
				}
			}
			
			if(open_period != 2){
				if(open_period == 0){
					MCMotion(CLAMP_ANGLE, '-', SPEED);
					open_period =1;					
				}
				else if(open_period == 1){
					if(IsMotActDone('M')){
						open_period = 2;
						printf("open done\n");
					}
				}
			}
			
		}
		
		if(MASTER_CMD!=DUMY){
			if(Is_Push(MASTER_CMD)){
				push_period = 0;
				SYS_STATE = WORK_STATE;
			}
			else if(Is_Draw(MASTER_CMD)){
				draw_period = 0;
				SYS_STATE = WORK_STATE;
				//PushMotion(20*PROCESS_ANGLE, '+', SPEED);
			}
			else if(Is_Close(MASTER_CMD)){
				close_period = 0;
				SYS_STATE = WORK_STATE;
			}
			else if(Is_Open(MASTER_CMD)){
				open_period = 0;
				SYS_STATE = WORK_STATE;
			}
			else if(Is_Stop(MASTER_CMD)){
				Fixture_Stop();
				SYS_STATE = ACCIDENT_STATE;
			}
			//else if(MASTER_CMD == CHECK)	//查询命令
			//	;
			//else	SYS_STATE = ERROR;			//接收命令出错
			printf("%c",MASTER_CMD);
			MASTER_CMD = DUMY;
			
		}
	}
}


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/


