/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�sensor.c
*�ļ���ʶ��
*ժ    Ҫ���Զ����ϵͳ������ļ�����				 
*��ǰ�汾��0.1
*��    �ߣ�yangxi
*������ڣ�2017/2/10
*****************************************************************************/
#include "sensor.h"
#include "delay.h"
#include "spi.h"

void Sensor_gpio_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);//ʹ��PORTA,PORTDʱ��

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_8 |GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 |GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	LOCAT_Init();

}

/*********************************�������ؼ��********************************/

/*******************************************************************************
*�������ƣ�Check_Clip_Ready
*����˵�������ڼ�ⵯ���Ƿ��Ѿ�����
*�����������
*���������status:0����δ���أ�1����������
*���ز������� 
*******************************************************************************/
u8 Check_Clip_Ready(void)
{ 
	if((HOLDER_PLACE_1==1)||(HOLDER_PLACE_2==1))
	{
		delay_ms(10);//ȥ���� 
		if(HOLDER_PLACE_1==1 && HOLDER_PLACE_2==1)
			return 1;
	}     
	return 0;// �ް�������
}

u8 Check_Clip_Unload(void)
{
	if((HOLDER_PLACE_1==0) && (HOLDER_PLACE_2==0))
	{
		delay_ms(100);//ȥ���� 
		if((HOLDER_PLACE_1==0) && (HOLDER_PLACE_2==0))
			return 1;
	}     
	return 0;// �ް�������

}
/*********************************�������ؼ��********************************/


/*********************************���ⷴ�䶨λ********************************/

static void LOCAT_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; //�ⲿ�ж����ý����
	NVIC_InitTypeDef NVIC_InitStructure; //�ж����ò���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA ,GPIO_PinSource10 | GPIO_PinSource11); //ָ���ⲿ�ж�������PC6
	
	//�ⲿ�������ã�����
	EXTI_InitStructure.EXTI_Line = EXTI_Line10 | EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
	
	//�жϲ����趨
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//���ȼ�ȫ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
}

//GPIOD10/GPIOD11�жϴ�����
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
	EXTI_ClearITPendingBit(EXTI_Line10);  //���EXTI4��·����
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


/*********************************���ⷴ�䶨λ********************************/


/*********************************������λ���********************************/

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

/*********************************������λ���********************************/





/*********************************Э������״̬��ѯ*******************************/
//#define CHECK		0x01
//#define Is_Ready(rx_data)	((rx_data | 0xBF) == 0xFF)	//1011,1111 ͷ�ļ��ж���

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
/********************************Э������״̬��ѯ********************************/




/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
