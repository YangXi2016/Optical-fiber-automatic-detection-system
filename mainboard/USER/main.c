/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：Main.c
*文件标识：
*摘    要：自动检测系统核心板
*当前版本：1.0
*作    者：yangxi
*完成日期：2017/2/9
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
void peripheral_test(void);
void rail_state_init(void);
void section_test(void);
u8 i,len,t;
//u8 MASTER_CMD=((u8)?0B11110000));
//u8 master_temp;
u8 temp;
u8 detect_result;

int g_status[20] = {0};	//20个夹具20个工位，但工位间距是19个。
int g_num_clean  = -1;		//计数统一从0开始，19结束
int g_num_detect = -1;
int g_num_hat  = -1;	

extern u8 status_station2;
enum running_status period;
enum system_status sys_error;
int main(void)
{
	section_test();
	//peripheral_test();
	//mutual_test();
	/********初始化阶段***********/
	Init_All();
	printf("Mainboard ready\r\n");
	period = ready;
	sys_error = normal;
	if (Check_Ready(3000) == 0) {				//检测所有从机的连接状态，如果设备连接处问题，则制停并输出信息
		Stop_All();
		while(1){
			printf("check_ready error");
			delay_ms(1000);
		}
	}
	
	while (1)											//等待上位机的开始信号。
	{
		if (Check_DetectMCU_Start())
			break;
		delay_ms(50);
	}
	/********初始化阶段***********/
	MOTION_ON();
	rail_state_init();
	Rail_RunTo_Station();
	while (1) {
		if(Check_DetectMCU_CleanSet()){		//周期开始前检测是否有纸巾校准信号
			Clean_Set();
			Inform_Detect(CMD_ClearFlag);
		}
		COMPRESS();		//夹具上载前电磁铁吸合
		while (Check_Clip_Upload() == 0)	;			//等待夹具上载

		period = upload;

		for (temp = 0; temp < NUM_TOTAL + DISTANCE1 + DISTANCE2; temp++) {
			get_period(temp);

			station_work(period);
			Rail_RunStation();

		}
		period = back;
		g_num_clean = -1;
		g_num_detect = -1;
		g_num_hat = -1;
		Rail_Back();

		while (status_station2 == 0);
		status_station2 = 0;
		Rail_Stop();
		
		UNCOMPRESS();
		for(temp=0;temp<WAIT_TIME;temp++)
			delay_ms(1000);
		while (Check_Clip_Unload() == 0);
		period = unload;

	}

}



void Init_All() {
	delay_init();	    	 //延时函数初始化	 
	delay_ms(1000);
	delay_ms(1000);
	//delay_ms(1000);
	Sensor_gpio_init();
	Control_gpio_init();
	 
	uart_init(115200);	 	//串口初始化为115200
	SPI1_Init();		   //初始化SPI,这里默认主机
	SPI2_Init();
}

void rail_state_init(void){
	//Rail_Back();
	//while (Check_Limit_L() == 0);
	//Rail_Stop();
	
	//Rail_Forward();
	//printf("Begin\n");
	//Rail_Back();
	int i;
	status_station2 = 0;
	
	i =0;
	while (status_station2 == 0){		//	弹夹到达第一个工位的前方
		Rail_TuneForward();
		while(Check_HatMCU_Ready()==0)
			delay_ms(33);
		i++;
	}
// 	while(status_station2 == 0);
  	status_station2 = 0;
//  	Rail_Stop();
 	printf("%d\n",i);
	//printf("stop\n");
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
	else if (temp < NUM_TOTAL + DISTANCE1) {
		period = detect_hat;
		g_num_detect++;
		g_num_hat++;
		g_num_clean = -1;
	}
	else if(temp < NUM_TOTAL + DISTANCE1 + DISTANCE2){
		period = hat;
		g_num_hat++;
		g_num_detect = -1;
	}
	else{
		printf("ERROR\n");
	}
	
	if(g_num_detect == 0)	
		while(Inform_Detect(CMD_Head)==0);			//弹夹检查开始发送CMD_Head指令
	if(g_num_detect == NUM_TOTAL - 1) 
		while(Inform_Detect(CMD_Tail)==0);	//弹夹检查结束发送CMD_Tail指令
}


void station_work(u8 period) {
	while (1) {
		Fixture_Push();
		while (Check_PushMCU_Ready() == 0) delay_ms(50);
		CLAMP();
		Fixture_Open();
		while (Check_PushMCU_Ready() == 0) delay_ms(50);

		if (period == clean) {
			Clean();
			while (Check_CleanMCU_Ready() == 0) delay_ms(50);
		}
		else if (period == clean_detect) {
			Clean();
			Detect();
			while ((Check_CleanMCU_Ready() == 0) || Check_DetectMCU_Ready() == 0) delay_ms(50);
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect] = detect_result;
		}
		else if (period == clean_detect_hat) {
			Clean();
			Detect();
			if (g_status[g_num_hat] == 1) {
				Hat();
			}
			while ((Check_CleanMCU_Ready() == 0) || (Check_DetectMCU_Ready() == 0) || (Check_HatMCU_Ready() == 0)) delay_ms(50);
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect - 1] = detect_result;
		}
		else if (period == detect_hat) {
			Detect();
			if (g_status[g_num_hat] == 1) {
				Hat();
			}
			while ((Check_DetectMCU_Ready() == 0) || (Check_HatMCU_Ready() == 0)) delay_ms(50);
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect - 1] = detect_result;

		}
		else if (period == hat) {
			if (g_status[g_num_hat] == 1) {
				Hat();
			}
			while (Check_HatMCU_Ready() == 0) delay_ms(50);
		}

		LOOSEN();

		if ((g_num_hat >= 0 ) && (g_num_hat <= NUM_TOTAL ) && (g_status[g_num_hat] == 1)) {
			while (Check_PushMCU_Ready() == 0) delay_ms(50);	//等待回退完成
			Fixture_Draw();
			Hat_Check();								//回退时检测是否戴帽成功
			while (Check_PushMCU_Ready() == 0) delay_ms(50);	//等待回退完成

			while (Check_HatMCU_Ready()== 0) delay_ms(50);				//等待戴帽的结果

			if (Check_HatMCU_Result()== 1)	break;
		}
		else {
			Fixture_Draw();
			while (Check_PushMCU_Ready() == 0) delay_ms(50);	//等待回退完成
			break; 
		
		}

	}
}


void mutual_test(void){
	Init_All();
	printf("Mainboard ready\r\n");           
	while(1){
		if(USART_RX_STA&0x8000){
			len=USART_RX_STA&0x3fff;
			//printf("\r\n您发送的消息为:\r\n");
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
				
				case 0x03:
					temp = CLEAN_ReadWriteByte(USART_RX_BUF[1]);
					USART1->DR=temp;
					while((USART1->SR&0X40)==0);
					break;
				
				case 0x02:
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


void peripheral_test(void){
	Init_All();
	printf("Mainboard ready\r\n");           
	while(1){
		
		
		
		if(USART_RX_STA&0x8000){
			len=USART_RX_STA&0x3fff;
			//printf("\r\n您发送的消息为:\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);
			}
			
			if(Check_Limit_L()==1){
				printf("Limit_L arrived\n");
			}
			if(Check_Limit_R()==1){
				printf("Limit_R arrived\n");
			}
			if(Check_Clip_Upload()){
				printf("Clip Upload\n");
			}
			//if(status_station2 == 1){
			if(Check_Locat() == 1){
				printf("laser locat\n");
			}
			switch(USART_RX_BUF[0]){
				case '1':
					CLAMP();
					printf("CLAMP\n");
					break;
				
				case '2':
					LOOSEN();
					printf("LOOSEN\n");
					break;
				
				case '3':
					COMPRESS();
					printf("COMPRESS\n");
					break;
				
				case '4':
					UNCOMPRESS();
					printf("UNCOMPRESS\n");
					break;
				
				default:
					break;
			}
			
			USART_RX_STA=0;
				
		}

	}
}


void section_test(void){
	int i;
	Init_All();
	printf("Mainboard section_test ready\r\n");
	period = ready;
	sys_error = normal;
	MOTION_ON();
	COMPRESS();
	if(!Check_HatMCU_Ready()){
		printf("Check error\n");
		while(1);
	}
	rail_state_init();
	while(1){
		Rail_RunTo_Station();
		printf("step 1\n");
		for(i=2;i<=37;i++){
			delay_ms(1000);
			printf("step %d\n",i);
			Rail_RunStation();
			while(Check_HatMCU_Ready()==0)
				delay_ms(33);

		}
		delay_ms(1000);
		Rail_Back();
		for(i=0;i<3;i++)
			delay_ms(1000);
// 	}
// 	{
// 		status_station2 = 0;
// 		printf("back begin\n");
// 		while(status_station2!=2);
// 		status_station2 = 0;
		i = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11);
		if(i==0){
			printf("back suitable %d\n",i);
			delay_ms(1000);
			continue;
		}
		while(Check_Limit_L()==0){
			Rail_TuneBack();
		}
		printf("back end\n");
		status_station2 = 0;
		while (status_station2 == 0){		//	弹夹到达第一个工位的前方
			Rail_TuneForward();
			while(Check_HatMCU_Ready()==0)
				delay_ms(33);
		}
		delay_ms(1000);
	}
}
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
