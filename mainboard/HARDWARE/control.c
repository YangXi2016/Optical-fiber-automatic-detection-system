
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
#include "variable.h"

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

extern enum system_status sys_error;
void EXTI9_5_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line9) == SET){
		MOTION_OFF();
		Stop_All();
		printf("Safe Gate Work\n");
		sys_error = safeGate_error;
		EXTI_ClearITPendingBit(EXTI_Line9); 
		while(Inform_Detect(CMD_SafeGateErr)==0) delay_ms(CHECK_INTERVAL);
		while(1);
	}

}

/********************简单片上硬件控制******************************************/


/********************通信从机控制******************************************/
u8 MASTER_CMD = 0x00;

//检测仪的控制
u8 Detect(void)
{
	while(DETECT_ReadWriteByte(CMD_Detect)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Inform_Detect(u8 txdata){
	while(DETECT_ReadWriteByte(txdata)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}	

//擦拭协机的控制
u8 Clean(void)
{
	while(CLEAN_ReadWriteByte(CMD_Clean)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Clean_Set(void){
	while(CLEAN_ReadWriteByte(CMD_CleanSet)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

//戴帽及平移台移动协机的控制
u8 Hat(void)
{
	while(HAT_ReadWriteByte(CMD_Hat)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Hat_Init(void)
{
	while(HAT_ReadWriteByte(CMD_HatInit)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Hat_Check(void)
{
	while(HAT_ReadWriteByte(CMD_HatCheck)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Rail_Forward(void)
{
	while(HAT_ReadWriteByte(CMD_RailForward)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}


 
u8 Rail_RunStation(void)
{
	while(HAT_ReadWriteByte(CMD_RailRunStation)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Rail_RunTo_Station(void){
	while(HAT_ReadWriteByte(CMD_RailRunToStation)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Rail_Back(void)
{
	while(HAT_ReadWriteByte(CMD_RailBack)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Rail_Stop(void)
{
	while(HAT_ReadWriteByte(CMD_RailStop)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Rail_TuneBack(void)
{
	while(HAT_ReadWriteByte(CMD_RailTuneBack)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;	
}

u8 Rail_TuneForward(void)
{	
	while(HAT_ReadWriteByte(CMD_RailTuneForward)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

//戴帽及平移台移动协机的控制


//辅助夹具推夹的协机的控制
u8 Fixture_Push(void)
{
	while(PUSH_ReadWriteByte(CMD_Push)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Fixture_Draw(void)
{
	while(PUSH_ReadWriteByte(CMD_Draw)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Fixture_Open(void)
{
	while(PUSH_ReadWriteByte(CMD_Open)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}
u8 Fixture_Init(void)
{
	while(PUSH_ReadWriteByte(CMD_PushInit)!=CMD_COMFIRM) delay_ms(CHECK_INTERVAL);
	return 1;
}

u8 Stop_All(void)
{
	while((CLEAN_ReadWriteByte(CMD_CleanStop)!=CMD_COMFIRM) || (HAT_ReadWriteByte(CMD_AllStop)!=CMD_COMFIRM) ||(PUSH_ReadWriteByte(CMD_PushStop)!=CMD_COMFIRM)) delay_ms(CHECK_INTERVAL);
	return 1;
}





/********************通信从机控制******************************************/


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
