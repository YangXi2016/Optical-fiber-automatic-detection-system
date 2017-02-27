
/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�control.c
*�ļ���ʶ��
*ժ    Ҫ���Զ����ϵͳ������ļ���ƻ���	 
*��ǰ�汾��0.1
*��    �ߣ�yangxi
*������ڣ�2017/2/10
*****************************************************************************/
#include "delay.h"
#include "spi.h"
#include "control.h"



void Control_gpio_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);//ʹ��PORTA,PORTDʱ��

	//���������ؼ�ϵͳ״ָ̬ʾ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 |GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//��ñ�����⼰������������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SAFE_MONITOR_Init();
}

/********************��Ƭ��Ӳ������******************************************/
/*�Ƶ�Э������
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
	EXTI_InitTypeDef EXTI_InitStructure; //�ⲿ�ж����ý����
	NVIC_InitTypeDef NVIC_InitStructure; //�ж�
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD ,GPIO_PinSource9); 
	
	//�ⲿ�������ã�����
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
	
	//�жϲ����趨
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//���ȼ�ȫ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
}

void EXTI9_5_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line9) == SET){
		Stop_All();
	EXTI_ClearITPendingBit(EXTI_Line9);  
	}

	/*
	if(EXTI_GetITStatus(EXTI_Line11) == SET){
		u8 state = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11);
		if( state == 1){
			status_station2 = 1;
		}else{
			status_station2 = 0;
		}
	EXTI_ClearITPendingBit(EXTI_Line11);  //���EXTI4��·����		
	}
	*/
}

/********************��Ƭ��Ӳ������******************************************/


/********************ͨ�Ŵӻ�����******************************************/
u8 MASTER_CMD = 0x00;

//����ǵĿ���
u8 Detect(void)
{
	u8 status;
	status = DETECT_ReadWriteByte(CMD_Detect);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}


//����Э���Ŀ���
u8 Clean(void)
{
	u8 status;
	status = CLEAN_ReadWriteByte(CMD_Clean);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}



//��ñ��ƽ��̨�ƶ�Э���Ŀ���
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

//��ñ��ƽ��̨�ƶ�Э���Ŀ���


//�����о��Ƽе�Э���Ŀ���
u8 Fixture_Push(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_Push);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}

u8 Fixture_Draw(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_Draw);
	if(status == CMD_COMFIRM)	return 1;
	else return 0;
}

u8 Fixture_Open(void)
{
	u8 status;
	status = HAT_ReadWriteByte(CMD_Open);
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





/********************ͨ�Ŵӻ�����******************************************/


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
