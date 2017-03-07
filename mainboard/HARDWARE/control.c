
/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：control.c
*文件标识：
*摘    要：自动检测系统中所需的检控制汇总	 
*当前版本：0.1
*作    者：yangxi
*完成日期：2017/2/10
*****************************************************************************/
#include "delay.h"
#include "spi.h"
#include "control.h"



void Control_gpio_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTA,PORTD时钟

	//电吸铁上载及系统状态指示用
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 |GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//戴帽结果检测及电吸铁吸合用
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	MOTION_OFF();
	SAFE_MONITOR_Init();
}

/********************简单片上硬件控制******************************************/
/*移到协处理器
void HatResult_Check(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	delay_ms(1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
*/
/*
void sys_normal(){
	red_off();
	blue_on();
	buzzer_off();
}

void sys_warn(){
	red_on();
	blue_off();
	buzzer_off();
}

void sys_error(){
	red_on();
	blue_off();
	buzzer_on();
}
*/

static void SAFE_MONITOR_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; //外部中断配置结参数
	NVIC_InitTypeDef NVIC_InitStructure; //中断
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD ,GPIO_PinSource9); 
	
	//外部触发设置，边沿
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
	
	//中断参数设定
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//优先级全局最低
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
}

void EXTI9_5_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line9) == SET){
		MOTION_OFF();
		Stop_All();
		printf("Safe Gate Work\n");
	EXTI_ClearITPendingBit(EXTI_Line9);  
	}

}

/********************简单片上硬件控制******************************************/


/********************通信从机控制******************************************/
u8 MASTER_CMD = 0x00;

//检测仪的控制
u8 Detect(void)
{
	u8 status;
	status = DETECT_ReadWriteByte(CMD_Detect);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}


//擦拭协机的控制
u8 Clean(void)
{
	u8 status;
	status = CLEAN_ReadWriteByte(CMD_Clean);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}



//戴帽及平移台移动协机的控制
u8 Hat(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_Hat);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}

u8 Hat_Check(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_HatCheck);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}

u8 Rail_Forward(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_RailForward);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}


 
u8 Rail_RunStation(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_RailRunStation);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}


u8 Rail_Back(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_RailBack);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}

u8 Rail_Stop(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_RailStop);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}

//戴帽及平移台移动协机的控制


//辅助夹具推夹的协机的控制
u8 Fixture_Push(void)
{
	u8 status;
	status = PUSH_ReadWriteByte(CMD_Push);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}

u8 Fixture_Draw(void)
{
	u8 status;
	status = PUSH_ReadWriteByte(CMD_Draw);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}

u8 Fixture_Open(void)
{
	u8 status;
	status = PUSH_ReadWriteByte(CMD_Open);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}
/*
u8 Fixture_Close(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_Close);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}
*/

u8 Stop_All(void)
{
	u8 status1,status2,status3;
	status1 = CLEAN_ReadWriteByte(CMD_CleanStop);
	status2 = HAT_ReadWriteByte(CMD_HatStop|CMD_RailStop);
	status3 = PUSH_ReadWriteByte(CMD_PushStop);
	if((status1==CMD_COMFIRM) && (status2==CMD_COMFIRM) && (status3==CMD_COMFIRM))
		return 1;
	else 
		return 0;
}





/********************通信从机控制******************************************/


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
