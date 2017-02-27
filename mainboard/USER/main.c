/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,º¼ÖİÎ¬¿±¿Æ¼¼ÓĞÏŞ¹«Ë¾
*All rights reserved
*
*ÎÄ¼şÃû³Æ£ºMain.c
*ÎÄ¼ş±êÊ¶£º
*Õª    Òª£º×Ô¶¯¼ì²âÏµÍ³ºËĞÄ°å
*µ±Ç°°æ±¾£º1.0
*×÷    Õß£ºyangxi
*Íê³ÉÈÕÆÚ£º2017/2/9
*****************************************************************************/

#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "control.h"
#include "sensor.h"
#include "variable.h"

void Init_All(void);
void get_period(u8 temp);
void station_work(u8 period);
void mutual_test(void);
u8 i,len,t;
//u8 MASTER_CMD=((u8)£(0B11110000));
//u8 master_temp;
u8 temp;
u8 detect_result;

extern u8 status_station;


int main(void)
{
	mutual_test();
	/********³õÊ¼»¯½×¶Î***********/
	Init_All();
	//printf("ready\r\n");
	period = ready;
	if (Check_Ready(3000) == 0) {				//¼ì²âËùÓĞ´Ó»úµÄÁ¬½Ó×´Ì¬£¬Èç¹ûÉè±¸Á¬½Ó´¦ÎÊÌâ£¬ÔòÖÆÍ£²¢Êä³öĞÅÏ¢
		Stop_All();
		printf("check_ready error");
	}
	while (1)											//µÈ´ıÉÏÎ»»úµÄ¿ªÊ¼ĞÅºÅ¡£
	{
		if (Check_DetectMCU_Start())
			break;
	}
	/********³õÊ¼»¯½×¶Î***********/


	while (1) {
		compress();		//¼Ğ¾ßÉÏÔØÇ°µç´ÅÌúÎüºÏ
		while (Check_Clip_Ready() == 0);			//µÈ´ı¼Ğ¾ßÉÏÔØ

		period = upload;

		Rail_Forward();

		while (status_station == 0);		//	µ¯¼Ğµ½´ïµÚÒ»¸ö¹¤Î»µÄÇ°·½
		status_station = 0;

		for (temp = 0; temp < NUM_TOTAL - 1 + DISTANCE1 + DISTANCE2; temp++) {
			get_period(temp);

			station_work(period);

		}
		period = back;
		g_num_clean = 0;
		g_num_detect = 0;
		g_num_hat = 0;
		Rail_Back();

		while (Check_Limit_L() == 0);
		uncompress();
		delay_ms(2000);
		while (Check_Clip_Unload() == 0);
		period = upload;

	}

}



void Init_All() {
	Sensor_gpio_init();
	Control_gpio_init();
	delay_init();	    	 //ÑÓÊ±º¯Êı³õÊ¼»¯	  
	uart_init(115200);	 	//´®¿Ú³õÊ¼»¯Îª115200
	SPI1_Init();		   //³õÊ¼»¯SPI,ÕâÀïÄ¬ÈÏÖ÷»ú
	SPI2_Init();
}


void get_period(u8 temp) {
	if (temp < DISTANCE1) {
		period = clean;
		g_num_clean++;
	}
	else if (temp < DISTANCE1 + DISTANCE2) {
		period = clean_detect;
		g_num_clean++;
		g_num_detect++;
	}
	else if (temp < NUM_TOTAL) {
		period = clean_detect_hat;
		g_num_clean++;
		g_num_detect++;
		g_num_hat++;
	}
	else if (temp < NUM_TOTAL + DISTANCE1 + DISTANCE2) {
		period = detect_hat;
		g_num_detect++;
		g_num_hat++;
	}
	else {
		period = hat;
		g_num_hat++;
	}

}


void station_work(u8 period) {
	while (1) {
		Fixture_Push();
		while (Check_PushMCU_Ready() == 0);
		clamp();
		Fixture_Open();
		while (Check_PushMCU_Ready() == 0);

		if (period == clean) {
			Clean();
			while (Check_CleanMCU_Ready() == 0);
		}
		else if (period == clean_detect) {
			Clean();
			Detect();
			while ((Check_CleanMCU_Ready() == 0) || Check_DetectMCU_Ready() == 0);
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect] = detect_result;
		}
		else if (period == clean_detect_hat) {
			Clean();
			Detect();
			if (g_status[g_num_hat] == 1) {
				Hat();
			}
			while ((Check_CleanMCU_Ready() == 0) || (Check_DetectMCU_Ready() == 0) || (Check_HatMCU_Ready() == 0));
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect - 1] = detect_result;
		}
		else if (period == detect_hat) {
			Detect();
			if (g_status[g_num_hat] == 1) {
				Hat();
			}
			while ((Check_DetectMCU_Ready() == 0) || (Check_HatMCU_Ready() == 0));
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect - 1] = detect_result;

		}
		else if (period == hat) {
			if (g_status[g_num_hat] == 1) {
				Hat();
			}
			while (Check_HatMCU_Ready() == 0);
		}

		loosen();

		if (g_status[g_num_hat] == 1) {
			while (Check_PushMCU_Ready() == 0);	//µÈ´ı»ØÍËÍê³É
			Fixture_Draw();
			Hat_Check();								//»ØÍËÊ±¼ì²âÊÇ·ñ´÷Ã±³É¹¦
			while (Check_PushMCU_Ready() == 0);	//µÈ´ı»ØÍËÍê³É

			while (Check_HatMCU_Ready()== 0);				//µÈ´ı´÷Ã±µÄ½á¹û

			if (Check_HatMCU_Result()== 1)	break;
		}
		else {
			Fixture_Draw();
			while (Check_PushMCU_Ready() == 0);	//µÈ´ı»ØÍËÍê³É
			break; 
		
		}

	}
}


void mutual_test(void){
	Init_All();
	printf("ready\r\n");           
	while(1){/*
		while(1){
			//CLEAN_ReadWriteByte(CMD_CleanStop);
			//HAT_ReadWriteByte(CMD_HatStop|CMD_RailStop);
			//SPI2_ReadWriteByte(0x22);
			temp = PUSH_ReadWriteByte(CMD_PushStop);//PUSH_ReadWriteByte(0x02);
			USART1->DR=temp;
			while((USART1->SR&0X40)==0);
			delay_ms(1800);
		}*/
		if(USART_RX_STA&0x8000){
			len=USART_RX_STA&0x3fff;
			//printf("\r\nÄú·¢ËÍµÄÏûÏ¢Îª:\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);
			}
			
			switch(USART_RX_BUF[0]){
				case 0x01:
					temp = DETECT_ReadWriteByte(USART_RX_BUF[1]);
					USART1->DR=temp;
					while((USART1->SR&0X40)==0);
					break;
				
				case 0x02:
					temp = CLEAN_ReadWriteByte(USART_RX_BUF[1]);
					USART1->DR=temp;
					while((USART1->SR&0X40)==0);
					break;
				
				case 0x03:
					temp = HAT_ReadWriteByte(USART_RX_BUF[1]);
					USART1->DR=temp;
					while((USART1->SR&0X40)==0);
					break;
				
				case 0x04:
					temp = PUSH_ReadWriteByte(USART_RX_BUF[1]);
					USART1->DR=temp;
					while((USART1->SR&0X40)==0);
					break;
				
				default:
					break;
			}
			
			USART_RX_STA=0;
				
		}

	}
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
