/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�Main.c
*�ļ���ʶ��
*ժ    Ҫ���Ƽо�Э������������				 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/9
*****************************************************************************/
#include "stm32f10x.h"
#include "init.h"


void CntIOInit(void);

extern u8 SYS_STATE,MASTER_CMD;

u8 push_period = 4;	//0�����յ��������1�������������ʼ�У�2��ʾ��������н��ˣ�3����ʼ��ǰ�ƣ�4��ʾ��ǰ�Ƶ����ˡ�//��5�����м�����ʼ�ɿ���6��ʾ�����ȫ�ɿ��ˣ���������ɣ���
u8 draw_period = 6;	//0�����յ��������1�����м�����ʼ�У�2��ʾ�м����н��ˣ�3����ʼ�������4��ʾ����������ˣ�5�������������ʼ�ɿ���6��ʾ�����ȫ�ɿ��ˣ���������ɣ���
u8 close_period= 2;	//0�����յ��������1�����м�����ʼ�У�2��ʾ�м����н��ˡ�
u8 open_period = 2;	//0�����յ��������1�����м�����ʼ�ɣ�2��ʾ�м����ɿ��ˡ�
int main(void)
{
	InitAll();
	
	while(1)
	{
		if((push_period == 4) && (draw_period == 6) && (close_period == 2) && (open_period ==2)) READY_STATE;
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
					if(IsMotActDone('P')) push_period = 4;
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
					if(IsMotActDone('M') && IsMotActDone('S')) draw_period = 6;
				}
			}
			if(close_period!= 2){
				if(close_period == 0){
					MCMotion(CLAMP_ANGLE, '+', SPEED);
					close_period =1;
				}
				else if(close_period == 1){
					if(IsMotActDone('M'))	close_period = 2;
				}
			}
			
			if(open_period != 2){
				if(open_period == 0){
					MCMotion(CLAMP_ANGLE, '-', SPEED);
					open_period =1;					
				}
				else if(open_period == 1){
					if(IsMotActDone('M'))	open_period = 2;
				}
			}
			
		}
		
		if(MASTER_CMD!=DUMY){
			if(Is_Push(MASTER_CMD)){
				push_period = 0;
				WORK_STATE;
			}
			else if(Is_Draw(MASTER_CMD)){
				draw_period = 0;
				WORK_STATE;
			}
			else if(Is_Close(MASTER_CMD)){
				close_period = 0;
				WORK_STATE;
			}
			else if(Is_Open(MASTER_CMD)){
				open_period = 0;
				WORK_STATE;
			}
			else if(Is_Stop(MASTER_CMD)){
				Fixture_Stop();
			}
			//else if(MASTER_CMD == CHECK)	//��ѯ����
			//	;
			//else	SYS_STATE = ERROR;			//�����������
			printf("%c",MASTER_CMD);
			MASTER_CMD = DUMY;
		}
	}
}


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/


