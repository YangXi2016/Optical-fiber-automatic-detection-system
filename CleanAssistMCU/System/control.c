/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：Motor.c
*文件标识：
*摘    要：擦拭结构的直流电机配置等			 
*当前版本：1.0
*作    者：李华兵
*完成日期：2017/2/9
*****************************************************************************/
#include "control.h"
#include "spi.h"
#include "stmflash.h"
#include "delay.h"

u8 clean_position,wipe_time;	//0代表在最下方准备往上擦；1代表在中间准备往上擦；2代表在上方准备往下擦；3代表在中间准备往下擦；

extern u8 SYS_STATE;

u8 FLASH_DATA[3];//={1,0,0}; //{FLAG,clean_position,wipe_time}

void Clean(void)
{
	if(clean_position == 0){
		SYS_STATE = WORK_STATE;
		StepMotion(SM_ANGLE, '-', SM_SPEED);
		while(IsStepMotActDone()==0);
		SYS_STATE = READY_STATE;
		clean_position = 1;
		
	}
	else if(clean_position == 1){
		SYS_STATE = WORK_STATE;
		StepMotion(SM_ANGLE, '-', SM_SPEED);
		while(IsStepMotActDone() == 0);
		SYS_STATE = READY_STATE;
		delay_ms(500);
		CMotStart(CM_SPEED);
		MMotMotion(MM_ANGLE, '+', MM_SPEED);
		while(IsDCMotActDone() == 0 );
		CMotStop();
		clean_position = 2;
		
		wipe_time++;
	}
	else if(clean_position == 2){
		SYS_STATE = WORK_STATE;
		StepMotion(SM_ANGLE, '+', SM_SPEED);
		while(IsStepMotActDone() == 0);
		SYS_STATE = READY_STATE;
		clean_position = 3;
	}
	else if(clean_position == 3){
		SYS_STATE = WORK_STATE;
		StepMotion(SM_ANGLE*1.2, '+', SM_SPEED);
		while(IsStepMotActDone() == 0);
		SYS_STATE = READY_STATE;
		delay_ms(500);
		CMotStart(CM_SPEED);
		MMotMotion(MM_ANGLE*1.2, '+', MM_SPEED);//因为在低端和顶端，与拉纸电机空间距离发生了变化，因为额外的拉扯需要1.2来修正
		while(IsDCMotActDone() == 0 );
		CMotStop();
		clean_position = 0;
		wipe_time++;		
	}
	MotorEN('P','D');
	FLASH_DATA[0] = 1;
	FLASH_DATA[1] = clean_position;
	FLASH_DATA[2] = wipe_time;
	
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)FLASH_DATA,3);
	
	if(wipe_time>=MAX_TIME)	SYS_STATE = DROPOUT_STATE;
	printf("%d\n",clean_position);
}

void Clean_Reset(void)
{
	u8 temp = 0;
	StepMotion(2*SM_ANGLE*1.2, '+', SM_SPEED);
	while(1){
		temp = IsStepMotActDone();
		if(temp == 1) break;
	}
}

void Clean_Stop(void){
	MotorEN('P','D');
	MMotStop();
	CMotStop();
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
