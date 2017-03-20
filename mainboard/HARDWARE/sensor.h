#ifndef __SENSOR_H
#define __SENSOR_H	
#include "sys.h"
/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：sensor.h
*文件标识：
*摘    要：自动检测系统中所需的检测汇总		 
*当前版本：1.0
*作    者：yangxi
*完成日期：2017/2/10
*****************************************************************************/ 
 
 void Sensor_gpio_init(void);
 
/*********************************弹夹上载检测********************************/
#define HOLDER_PLACE_1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)//读取微动开关1
#define HOLDER_PLACE_2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)//读取微动开关2
//#define SYS_START				GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)//读取开始暂停

//void KEY_Init(void);//IO初始化
u8 Check_Clip_Upload(void);  	//
u8 Check_Clip_Unload(void);
/*********************************弹夹上载检测********************************/


/********************************戴帽结果检测*******************************/
//已移到戴帽MCU中一起处理
//#define HAT_CHECK_FLAG  		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//线性CCD检测戴帽，是否已得出结果
//#define HAT_CHECK_RESULT  	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//线性CCD检测戴帽得到的结果


/********************************戴帽结果检测******************************/





/*********************************导轨限位检测********************************/

#define L_LIMIT  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)//读取左端限位开关
#define R_LIMIT  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)//读取右端限位开关

u8 Check_Limit_L(void);
u8 Check_Limit_R(void);

/*********************************导轨限位检测********************************/



/*********************************红外反射定位/防掉落检测********************************/

//见GPIOD10/GPIOD11中断处理函数;后面实际只使用了D10.
#define LOCAT		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
static void LOCAT_Init(void);
u8 Check_Locat(void);
/*********************************红外反射定位********************************/



/*********************************协处理器状态查询*******************************/
#define CHECK 0x02
//通用状态
#define Is_Ready(rx_data)			((rx_data & 0x40) == 0x40)	//0100,0000 用于同所有从机确认是否准备好


//检测仪检测结果
#define Is_Start(rx_data)			((rx_data & 0x80) == 0x80)	//0010,0000
#define Qualified(rx_data)		((rx_data & 0x60) == 0x60)
#define UnQualified(rx_data)	((rx_data & 0x60) == 0x40)
#define Is_CleanSet(rx_data)	((rx_data & 0x10)	== 0x10)


//戴帽MCU状态
//#define HatCheck_Done(rx_data)	((rx_data & 0x08) == 0x08)
#define HatCheck_Result(rx_data) ((rx_data & 0x60) == 0x60)	//1成功，0失败
#define Is_HatNull(rx_data)				((rx_data & 0x10) == 0x10)	//1没帽，


//纸巾用光状态
#define Is_TissueNull(rx_data)		((rx_data & 0x41) == 0x41)	//1没纸

u8 Check_HatMCU_Result(void);

u8 Check_CleanMCU_Ready(void);
u8 Check_PushMCU_Ready(void);
u8 Check_HatMCU_Ready(void);

u8 Check_DetectMCU_Ready(void);
u8 Check_DetectMCU_Start(void);
u8 Check_DetectMCU_CleanSet(void);
u8 Check_DetectMCU_Result(void);

u8 Check_Ready(u16 try_times);
/********************************协处理器状态查询********************************/



#endif


/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
