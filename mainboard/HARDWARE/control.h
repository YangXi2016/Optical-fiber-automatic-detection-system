/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：control.h
*文件标识：
*摘    要：自动检测系统中所需的检控制汇总	 
*当前版本：0.1
*作    者：yangxi
*完成日期：2017/2/10
*****************************************************************************/

#ifndef __CONTROL_H
#define __CONTROL_H	
#include "sys.h"
#include "delay.h"
#include "spi.h"
 
/********************简单控制*********************************************/
//#define hat_check()

#define CLAMP()						GPIO_SetBits(GPIOA,GPIO_Pin_4)				//电磁铁吸合夹紧
#define LOOSEN()					GPIO_ResetBits(GPIOA,GPIO_Pin_4)			//电磁铁掉电松弛

#define COMPRESS()				GPIO_SetBits(GPIOD,GPIO_Pin_0)			//电磁铁上载前吸合
#define UNCOMPRESS()			GPIO_ResetBits(GPIOD,GPIO_Pin_0)			//电磁铁卸载前掉电

#define MOTION_ON()				GPIO_SetBits(GPIOD,GPIO_Pin_12)			//动力电源开启
#define MOTION_OFF()				GPIO_ResetBits(GPIOD,GPIO_Pin_12)			//动力电源关闭


void Control_gpio_init(void);
static void SAFE_MONITOR_Init(void);
/********************简单控制*********************************************/


/********************通信控制*******************************************/

//检测仪 			1、开始检测。 			2、一个弹夹检测结束
//擦拭装置		1、开始/停止擦拭。
//戴帽装置		1、开始/停止戴帽。 	2、平移台电机移动/制动	
//推夹装置		1、开始/停止推夹。	2、开始/停止夹拽

//通信查询
//检测仪 			1、检测是否结束。 
//擦拭装置		1、擦拭是否结束。
//戴帽装置		1、戴帽是否结束。 	2、平移台电机移动是否结束。	
//推夹装置		1、推夹是否结束。		2、夹拽是否结束。

//the cmd sent to DetectMCU
#define CMD_Detect 							0x40	//(CMD | 0x40)     //0100,0000
#define CMD_Head								0x20
#define CMD_Tail								0x10
#define CMD_SafeGateErr					0x80
#define CMD_HatNull							0x81
#define CMD_TissueNull					0xC1
#define CMD_ClearFlag						0x08
#define CMD_CLAMP								0x04

//the cmd sent to CleanAssistMCU
#define CMD_Clean 							0x40	//(CMD | 0x40)     //0100,0000
#define CMD_CleanSet						0x20
#define CMD_CleanStop 					0xBF	//(CMD & 0xBF)

//the cmd sent to HatAssistMCU
#define CMD_Hat									0x40	//(CMD | 0x40)     //0100,0000
#define CMD_HatStop 						0xBF	//(CMD & 0xBF)			//1011,1111
#define CMD_RailRunStation 			0x20	//(CMD | 0x20)     	//0010,0000
#define CMD_RailBack 						0x10	//(CMD | 0x10)     	//0001,0000
#define CMD_RailForward 				0x08	//(CMD | 0x08)			//0000,1000
#define CMD_RailStop 						0xC7	//(CMD & 0xC7)			//1100,0111
#define CMD_RailTuneBack				0x30
#define CMD_RailTuneForward			0x0C
#define CMD_HatCheck						0x04
#define CMD_RailRunToStation		0x80
#define CMD_AllStop							0x87
#define CMD_HatInit								0x88

//the cmd sent to PushAssistMCU
#define CMD_Push 								0x40	//(CMD | 0x40)
#define CMD_Draw 								0x20	//(CMD | 0x20)
#define CMD_Open								0x10	
//push:一直到检测仪夹紧的一系列动作；open:检测仪检测前中间电机松开；Draw:从检测结束一直到所控制的电机恢复初态。
#define CMD_PushStop 						0x87	//(CMD & 0x9F)
#define CMD_PushInit						0x88


#define CMD_COMFIRM									0x02							//命令的确认回传信息

u8 Rail_Forward(void);
 
u8 Rail_RunStation(void);								//

u8 Rail_RunTo_Station(void);

u8 Rail_Back(void);

u8 Rail_Stop(void);

u8 Rail_TuneBack(void);
u8 Rail_TuneForward(void);

u8 Hat_Check(void);	

u8 Fixture_Push(void);

u8 Fixture_Draw(void);

u8 Fixture_Open(void);

u8 Fixture_Close(void);

u8 Fixture_Init(void);

u8 Hat(void);
u8 Hat_Init(void);

u8 Clean(void);
u8 Clean_Set(void);

u8 Detect(void);
u8 Inform_Detect(u8 txdata);

u8 Stop_All(void);

//u8 wiping(void);



/********************通信控制*******************************************/

#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

