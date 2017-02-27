/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：sensor.c
*文件标识：
*摘    要：自动检测系统中所需的检测汇总				 
*当前版本：0.1
*作    者：yangxi
*完成日期：2017/2/10
*****************************************************************************/
#include "sensor.h"
#include "delay.h"
#include "spi.h"

void Sensor_gpio_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTA,PORTD时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_8 |GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 |GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	LOCAT_Init();

}

/*********************************弹夹上载检测********************************/

/*******************************************************************************
*函数名称：Check_Clip_Ready
*函数说明：用于检测弹夹是否已经上载
*输入参数：无
*输出参数：status:0代表未上载，1代表已上载
*返回参数：无 
*******************************************************************************/
u8 Check_Clip_Ready(void)
{ 
	if((HOLDER_PLACE_1==1)||(HOLDER_PLACE_2==1))
	{
		delay_ms(10);//去抖动 
		if(HOLDER_PLACE_1==1 && HOLDER_PLACE_2==1)
			return 1;
	}     
	return 0;// 无按键按下
}

u8 Check_Clip_Unload(void)
{
	if((HOLDER_PLACE_1==0) && (HOLDER_PLACE_2==0))
	{
		delay_ms(100);//去抖动 
		if((HOLDER_PLACE_1==0) && (HOLDER_PLACE_2==0))
			return 1;
	}     
	return 0;// 无按键按下

}
/*********************************弹夹上载检测********************************/


/*********************************红外反射定位********************************/

static void LOCAT_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; //外部中断配置结参数
	NVIC_InitTypeDef NVIC_InitStructure; //中断配置参数
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA ,GPIO_PinSource10 | GPIO_PinSource11); //指定外部中断输入是PC6
	
	//外部触发设置，边沿
	EXTI_InitStructure.EXTI_Line = EXTI_Line10 | EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
	
	//中断参数设定
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//优先级全局最低
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
}

//GPIOD10/GPIOD11中断处理函数
//u8 status_station1 = 0,status_station2 = 0;
u8 status_station = 0;
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) == SET){
		u8 state = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10);
		if( state == 1){
			status_station = 1;
		}else{
			status_station = 0;
		}
	EXTI_ClearITPendingBit(EXTI_Line10);  //清除EXTI4线路挂起
	}
	/*
	if(EXTI_GetITStatus(EXTI_Line11) == SET){
		u8 state = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11);
		if( state == 1){
			status_station2 = 1;
		}else{
			status_station2 = 0;
		}
	EXTI_ClearITPendingBit(EXTI_Line11);  //清除EXTI4线路挂起		
	}
	*/
}


/*********************************红外反射定位********************************/


/*********************************导轨限位检测********************************/

u8 Check_Limit_L(void)
{
	if(L_LIMIT==1)
	{
		delay_ms(10); 
		if(L_LIMIT==1)return 1;
	}     
	return 0;
}

u8 Check_Limit_R(void)
{
	if(R_LIMIT==1)
	{
		delay_ms(10); 
		if(R_LIMIT==1)return 1;
	}     
	return 0;
}

/*********************************导轨限位检测********************************/





/*********************************协处理器状态查询*******************************/
//#define CHECK		0x01
//#define Is_Ready(rx_data)	((rx_data | 0xBF) == 0xFF)	//1011,1111 头文件中定义

u8 Check_CleanMCU_Ready(void)
{
	u8 RxData;
	RxData = CLEAN_ReadWriteByte(CHECK);
	if(Is_Ready(RxData))
		return 1;
	else 
		return 0;
}


u8 Check_PushMCU_Ready(void)
{
	u8 RxData;
	RxData = PUSH_ReadWriteByte(CHECK);
	if(Is_Ready(RxData))
		return 1;
	else 
		return 0;
}

u8 Check_HatMCU_Ready(void)
{
	u8 RxData;
	RxData = HAT_ReadWriteByte(CHECK);
	if(Is_Ready(RxData))
		return 1;
	else 
		return 0;
}

u8 Check_DetectMCU_Ready(void)
{
	u8 RxData;
	RxData = DETECT_ReadWriteByte(CHECK);
	if(Is_Ready(RxData))
		return 1;
	else 
		return 0;
}

u8 Check_HatMCU_Result(void)
{
	u8 RxData;
	RxData = DETECT_ReadWriteByte(CHECK);
	if(HatCheck_Result(RxData))
		return 1;
	else 
		return 0;
}

u8 Check_DetectMCU_Start()
{
	u8 RxData;
	RxData = DETECT_ReadWriteByte(CHECK);
	if(Is_Start(RxData))
		return 1;
	else 
		return 0;

}

u8 Check_DetectMCU_Result(void)
{
	u8 RxData;
	RxData = DETECT_ReadWriteByte(CHECK);
	if(Qualified(RxData))
		return 1;
	else// if(UnQualified(RxData))
		return 0;	

}

u8 Check_Ready(u16 try_times)
{
	u16 retry = 0;
	while(1)
	{
		retry += 1;
		if(retry > try_times)	return 0;
		if(Check_DetectMCU_Ready())
		{
			if(Check_CleanMCU_Ready())
			{
				if(Check_PushMCU_Ready())
				{
					if(Check_HatMCU_Ready())
						return 1;
				}
			}
		}
	}

}
/********************************协处理器状态查询********************************/




/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
