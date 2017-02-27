/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�control.h
*�ļ���ʶ��
*ժ    Ҫ���Զ����ϵͳ������ļ���ƻ���	 
*��ǰ�汾��0.1
*��    �ߣ�yangxi
*������ڣ�2017/2/10
*****************************************************************************/

#ifndef __CONTROL_H
#define __CONTROL_H	
#include "sys.h"
#include "delay.h"
#include "spi.h"
 
/********************�򵥿���*********************************************/
//#define hat_check()

#define clamp()						GPIO_SetBits(GPIOA,GPIO_Pin_4);				//��������ϼн�
#define loosen()					GPIO_ResetBits(GPIOA,GPIO_Pin_4);			//����������ɳ�

#define compress()				GPIO_SetBits(GPIOD,GPIO_Pin_0);				//���������ǰ����
#define uncompress()			GPIO_ResetBits(GPIOD,GPIO_Pin_0);			//�����ж��ǰ����

/*
#define red_on()					GPIO_SetBits(GPIOD,GPIO_Pin_5);
#define red_off()					GPIO_ResetBits(GPIOD,GPIO_Pin_5);

#define blue_on()					GPIO_SetBits(GPIOD,GPIO_Pin_6);
#define blue_off()				GPIO_ResetBits(GPIOD,GPIO_Pin_6);

#define buzzer_on()				GPIO_SetBits(GPIOD,GPIO_Pin_7);
#define buzzer_off()			GPIO_ResetBits(GPIOD,GPIO_Pin_7);
*/

void Control_gpio_init(void);
//void HatResult_Check(void);	

//void sys_normal(void);
//void sys_warn(void);
//void sys_error(void);
static void SAFE_MONITOR_Init(void);
/********************�򵥿���*********************************************/


/********************ͨ�ſ���*******************************************/

//����� 			1����ʼ��⡣ 			2��һ�����м�����
//����װ��		1����ʼ/ֹͣ���á�
//��ñװ��		1����ʼ/ֹͣ��ñ�� 	2��ƽ��̨����ƶ�/�ƶ�	
//�Ƽ�װ��		1����ʼ/ֹͣ�ƼС�	2����ʼ/ֹͣ��ק

//ͨ�Ų�ѯ
//����� 			1������Ƿ������ 
//����װ��		1�������Ƿ������
//��ñװ��		1����ñ�Ƿ������ 	2��ƽ��̨����ƶ��Ƿ������	
//�Ƽ�װ��		1���Ƽ��Ƿ������		2����ק�Ƿ������

/*
#define CMD_Detect(CMD)							(CMD | 0x40)     //0100,0000
//#define CMD_UnDetect(CMD)					(CMD & 0xBF)

#define CMD_Clean(CMD)							(CMD | 0x40)     //0100,0000
#define CMD_CleanStop(CMD)					(CMD & 0xBF)

#define CMD_Hat(CMD)								(CMD | 0x40)     //0100,0000
#define CMD_HatStop(CMD)						(CMD & 0xBF)			//1011,1111
#define CMD_RailRun(CMD)						(CMD | 0x20)     	//0010,0000
#define CMD_RailBack(CMD)						(CMD | 0x10)     	//0001,0000
#define CMD_RailForward(CMD)				(CMD | 0x08)			//0000,1000
#define CMD_RailStop(CMD)						(CMD & 0xC7)			//1100,0111
			

#define CMD_Push(CMD)								(CMD | 0x40)
#define CMD_Draw(CMD)								(CMD | 0x20)
#define CMD_PushStop(CMD)						(CMD & 0x9F)
*/

#define CMD_Detect 							0x40	//(CMD | 0x40)     //0100,0000
//#define CMD_UnDetect 					(CMD & 0xBF)
#define CMD_Head								0x20
#define CMD_Tail								0x10

#define CMD_Clean 							0x40	//(CMD | 0x40)     //0100,0000
#define CMD_CleanStop 					0xBF	//(CMD & 0xBF)

#define CMD_Hat									0x40	//(CMD | 0x40)     //0100,0000
#define CMD_HatStop 						0xBF	//(CMD & 0xBF)			//1011,1111
#define CMD_RailRunStation 			0x20	//(CMD | 0x20)     	//0010,0000
#define CMD_RailBack 						0x10	//(CMD | 0x10)     	//0001,0000
#define CMD_RailForward 				0x08	//(CMD | 0x08)			//0000,1000
#define CMD_RailStop 						0xC7	//(CMD & 0xC7)			//1100,0111
#define CMD_HatCheck						0x04
#define CMD_AllStop							0x87
			
#define CMD_Push 								0x40	//(CMD | 0x40)
#define CMD_Draw 								0x20	//(CMD | 0x20)
#define CMD_Open								0x10	
//push:һֱ������Ǽн���һϵ�ж�����open:����Ǽ��ǰ�м����ɿ���Draw:�Ӽ�����һֱ�������Ƶĵ���ָ���̬��
#define CMD_PushStop 						0x87	//(CMD & 0x9F)


#define CMD_COMFIRM									0x02							//�����ȷ�ϻش���Ϣ

u8 Rail_Forward(void);
 
u8 Rail_RunStation(void);								//

u8 Rail_Back(void);

u8 Rail_Stop(void);

u8 Hat_Check(void);	

u8 Fixture_Push(void);

u8 Fixture_Draw(void);

u8 Fixture_Open(void);

u8 Fixture_Close(void);

u8 Hat(void);

u8 Detect(void);

u8 Clean(void);

u8 Stop_All(void);

//u8 wiping(void);



/********************ͨ�ſ���*******************************************/

#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

