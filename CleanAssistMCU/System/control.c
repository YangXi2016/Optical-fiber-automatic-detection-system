/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�Motor.c
*�ļ���ʶ��
*ժ    Ҫ�����ýṹ��ֱ��������õ�			 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/9
*****************************************************************************/
#include "control.h"
#include "spi.h"
#include "stmflash.h"

u8 clean_position,wipe_time;	//0���������·�׼�����ϲ���1�������м�׼�����ϲ���2�������Ϸ�׼�����²���3�������м�׼�����²���

extern u8 SYS_STATE;

u8 FLASH_DATA[3];//={1,0,0}; //{FLAG,clean_position,wipe_time}

void Clean(void)
{
	//CMotStart(CM_SPEED);
	//MMotMotion(MM_ANGLE, MM_SPEED,'+');
	//while(IsMotActDone()==0);
	if(clean_position == 0){
		SYS_STATE = WORK_STATE;
		StepMotion(SM_ANGLE, '+', SM_SPEED);
		while(IsStepMotActDone()==0);
		SYS_STATE = READY_STATE;
		clean_position = 1;
		
	}
	else if(clean_position == 1){
		SYS_STATE = WORK_STATE;
		StepMotion(SM_ANGLE, '+', SM_SPEED);
		while(IsStepMotActDone() == 0);
		SYS_STATE = READY_STATE;
		
		CMotStart(CM_SPEED);
		MMotMotion(MM_ANGLE, '+', MM_SPEED);
		while(IsDCMotActDone() == 0 );
		CMotStop();
		clean_position = 2;
		
		wipe_time++;
	}
	else if(clean_position == 2){
		SYS_STATE = WORK_STATE;
		StepMotion(SM_ANGLE, '-', SM_SPEED);
		while(IsStepMotActDone() == 0);
		SYS_STATE = READY_STATE;
		clean_position = 3;
	}
	else if(clean_position == 3){
		SYS_STATE = WORK_STATE;
		StepMotion(SM_ANGLE, '-', SM_SPEED);
		while(IsStepMotActDone() == 0);
		SYS_STATE = READY_STATE;
		
		CMotStart(CM_SPEED);
		MMotMotion(MM_ANGLE, '+', MM_SPEED);
		while(IsDCMotActDone() == 0 );
		CMotStop();
		clean_position = 0;
		wipe_time++;		
	}
	
	FLASH_DATA[0] = 1;
	FLASH_DATA[1] = clean_position;
	FLASH_DATA[2] = wipe_time;
	
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)FLASH_DATA,3);
	
	if(wipe_time>=MAX_TIME)	SYS_STATE = DROPOUT_STATE;
	
}

void Clean_Reset(void)
{
	StepMotion(2*SM_ANGLE, '-', SM_SPEED);
	while(IsStepMotActDone() == 0);
}

void Clean_Stop(void){
	MotorEN('P','D');
	MMotStop();
	CMotStop();
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
