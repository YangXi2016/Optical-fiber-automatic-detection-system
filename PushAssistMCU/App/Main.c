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

extern u8 SYS_STATE,MASTER_CMD;


u8 last_state_c=0;
u8 last_state_m=0;
extern u8 Position_Flag_C,Position_Flag_M;
void position_init(void);
void clamp_init(void);
void push_forward(void);
void push_clamp(void);
void push_loosen(void);
void push_draw(void);

int main(void)
{
	InitAll();
	//position_init();
	//clamp_init();
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
		if(MASTER_CMD!=DUMY){
			if(Is_Push(MASTER_CMD)){
				SYS_STATE = WORK_STATE;
				
				push_clamp();
				delay_ms(500);
				push_forward();
				
				SYS_STATE = READY_STATE;
			}
			else if(Is_Draw(MASTER_CMD)){
				SYS_STATE = WORK_STATE;
				
				push_draw();
				
				SYS_STATE = READY_STATE;
			}
			else if(Is_Open(MASTER_CMD)){
				SYS_STATE = WORK_STATE;
				
				push_loosen();
				
				SYS_STATE = READY_STATE;
			}
			else if(Is_Stop(MASTER_CMD)){
				Fixture_Stop();
			}
			else if(Is_Init(MASTER_CMD)){
				SYS_STATE = WORK_STATE;
				clamp_init();
				position_init();
				SYS_STATE = READY_STATE;
			}
			else if(MASTER_CMD == 0x80){	//test the motor and driver
				//GPIO_SetBits(GPIOA,GPIO_Pin_12);
				//while(1){
				PushMotion(100, '-', 100);
				PCMotion(100, '-', 100);
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
					delay_ms(1500);
					SCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
					while(IsMotActDone('S')==0);
					delay_ms(1500);
					MCMotion(CLAMP_ANGLE, '+', CLAMP_SPEED);
					while(IsMotActDone('M')==0);
					delay_ms(1500);
					MCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
					while(IsMotActDone('M')==0);
					delay_ms(1500);
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
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)==1)		//已经处在初始位置时，不执行运动
		PushMotion(400, PUSH_DIR, PUSH_SPEED);
	else
		s_flag_m = 1;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)==1)
		PCMotion(400,PUSH_DIR,PUSH_SPEED);
	else
		s_flag_c = 1;
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

void clamp_init(void)
{
	MCMotion(CLAMP_ANGLE, '-', PUSH_SPEED);
	SCMotion(CLAMP_ANGLE, '-', PUSH_SPEED);
	while((IsMotActDone('P')==0) || (IsMotActDone('C')==0));
}
	
void push_clamp(void)
{
	MCMotion(CLAMP_ANGLE, '+', PUSH_SPEED);
	SCMotion(CLAMP_ANGLE, '+', PUSH_SPEED);
	while((IsMotActDone('M')==0) || (IsMotActDone('S')==0));
}

void push_forward(void)
{
	PushMotion(FORWARD_ANGLE, PUSH_DIR, PUSH_SPEED);
	PCMotion(FORWARD_ANGLE, PUSH_DIR, PUSH_SPEED);
	while((IsMotActDone('P')==0) || (IsMotActDone('C')==0)) ;
}

void push_loosen(void)
{
	MCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
	while((IsMotActDone('M')==0));
}

void push_draw(void)
{
	SCMotion(CLAMP_ANGLE, '-', CLAMP_SPEED);
	position_init();
	while((IsMotActDone('S')==0));
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/


