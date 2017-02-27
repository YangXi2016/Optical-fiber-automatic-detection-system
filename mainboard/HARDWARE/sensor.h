#ifndef __SENSOR_H
#define __SENSOR_H	
#include "sys.h"
/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�sensor.h
*�ļ���ʶ��
*ժ    Ҫ���Զ����ϵͳ������ļ�����		 
*��ǰ�汾��1.0
*��    �ߣ�yangxi
*������ڣ�2017/2/10
*****************************************************************************/ 
 
 void Sensor_gpio_init(void);
 
/*********************************�������ؼ��********************************/
#define HOLDER_PLACE_1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)//��ȡ΢������1
#define HOLDER_PLACE_2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)//��ȡ΢������2
//#define SYS_START				GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)//��ȡ��ʼ��ͣ

//void KEY_Init(void);//IO��ʼ��
u8 Check_Clip_Ready(void);  	//
u8 Check_Clip_Unload(void);
/*********************************�������ؼ��********************************/


/********************************��ñ������*******************************/
//���Ƶ���ñMCU��һ����
//#define HAT_CHECK_FLAG  		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//����CCD����ñ���Ƿ��ѵó����
//#define HAT_CHECK_RESULT  	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//����CCD����ñ�õ��Ľ��


/********************************��ñ������******************************/





/*********************************������λ���********************************/

#define L_LIMIT  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)//��ȡ�����λ����
#define R_LIMIT  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)//��ȡ�Ҷ���λ����

u8 Check_Limit_L(void);
u8 Check_Limit_R(void);

/*********************************������λ���********************************/



/*********************************���ⷴ�䶨λ/��������********************************/

//��GPIOD10/GPIOD11�жϴ�����
static void LOCAT_Init(void);
static void SAFE_MONITOR_Init(void);

/*********************************���ⷴ�䶨λ********************************/



/*********************************Э������״̬��ѯ*******************************/
#define CHECK 0x01
//ͨ��״̬
#define Is_Ready(rx_data)			((rx_data & 0x40) == 0x40)	//0100,0000 ����ͬ���дӻ�ȷ���Ƿ�׼����
#define Is_Start(rx_data)			((rx_data & 0x20) == 0x20)	//0010,0000

//����Ǽ����
#define Qualified(rx_data)		((rx_data & 0x60) == 0x60)
#define UnQualified(rx_data)	((rx_data & 0x60) == 0x40)			

//��ñMCU״̬
//#define HatCheck_Done(rx_data)	((rx_data & 0x08) == 0x08)
#define HatCheck_Result(rx_data) ((rx_data & 0x0C) == 0x0C)	//1�ɹ���0ʧ��

u8 Check_HatMCU_Result(void);

 u8 Check_CleanMCU_Ready(void);
 u8 Check_PushMCU_Ready(void);
 u8 Check_HatMCU_Ready(void);

 u8 Check_DetectMCU_Ready(void);

 u8 Check_DetectMCU_Start(void);

u8 Check_DetectMCU_Result(void);

u8 Check_Ready(u16 try_times);
/********************************Э������״̬��ѯ********************************/



#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
