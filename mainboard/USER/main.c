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
void mutual_test(void);//used for communication test
void peripheral_test(void);//used for peripheral test
void rail_state_init(void);
void section_test(void);//used for process test
u8 i,len,t;
u8 temp;
u8 detect_result;

int g_status[20] = {0};	//20个夹具20个工位，但工位间距是19个。
int g_num_clean  = 0;		//计数统一从0开始，19结束
int g_num_detect = 0;
int g_num_hat  = 0;	

extern u8 status_station2;
enum running_status period;
enum system_status sys_error;
int main(void)
{
	/********通信初始化阶段***********/
	Init_All();
	printf("Mainboard ready\r\n");
	period = ready;
	sys_error = normal;
	if (Check_Ready(3000) == 0) {				//检测所有从机的连接状态，如果设备连接处问题，则制停并输出信息
		while(1){
			printf("check_ready error\n");
			delay_ms(1000);
		}
	}
	printf("check_ready done\n");
	while (1)											//等待上位机的开始信号。
	{
		if (Check_DetectMCU_Start())
			break;
		delay_ms(CHECK_INTERVAL);
	}
	/********通信初始化阶段***********/
	
	/********位置初始化阶段***********/
	while(Check_DetectMCU_Start()==0) delay_ms(CHECK_INTERVAL);	//before each action,Check DetectMCU start or stop
	Hat_Init();
	while(Check_DetectMCU_Start()==0) delay_ms(CHECK_INTERVAL);
	Fixture_Init();
	while ((Check_HatMCU_Ready() == 0) || Check_PushMCU_Ready() == 0);
	MOTION_ON();
	while(Check_DetectMCU_Start()==0) delay_ms(CHECK_INTERVAL);
	rail_state_init();
	/********位置初始化阶段***********/
	
	/********正常工作阶段（重复）***********/
	while (1) {
		if(Check_DetectMCU_CleanSet()){		//周期开始前检测是否有纸巾校准信号
			printf("calibrate issue\n");
			Clean_Set();
			Inform_Detect(CMD_ClearFlag);
		}
		
		while (Check_Clip_Upload() == 0)	;			//等待夹具上载
		printf("upload done\n");
		delay_ms(500);
		
		COMPRESS();		//夹具上载前电磁铁吸合
		for(temp=0;temp<WAIT_TIME;temp++)
			delay_ms(1000);
		
		period = upload;
		temp = 1;
		get_period(temp);
		while(Check_DetectMCU_Start()==0) delay_ms(CHECK_INTERVAL);
		Rail_RunTo_Station();	//运动到第一个工位
		delay_ms(CHECK_INTERVAL);
		while(Check_HatMCU_Ready()==0)
			delay_ms(CHECK_INTERVAL);
		
		while(Check_DetectMCU_Start()==0) delay_ms(CHECK_INTERVAL);
		station_work(period);	//完成在第一个工位上的动作
		printf("done the work in first station\n");
		
		for (temp = 2; temp <= NUM_TOTAL + DISTANCE1 + DISTANCE2; temp++) {
			
			while(Check_DetectMCU_Start()==0) delay_ms(CHECK_INTERVAL);
			Rail_RunStation();	//运动到第n个工位
			printf("arrive at station %d\n",temp);
			while(Check_HatMCU_Ready()==0)
				delay_ms(CHECK_INTERVAL);
			
			get_period(temp);	//根据工位位置得到当前的时期
			
			while(Check_DetectMCU_Start()==0) delay_ms(CHECK_INTERVAL);
			station_work(period);	//根据当前的时期执行当前工位的动作
			printf("done the work in station %d\n",temp);
			

		}
		
		/********返回到初始位置***********/
		period = back;
		g_num_clean = 0;
		g_num_detect = 0;
		g_num_hat = 0;
		
		Rail_Back();
		while(Check_HatMCU_Ready()==0){
			delay_ms(CHECK_INTERVAL);
		}
		
		if(Check_Locat() == 0){		//返回时出现了误差
			printf("RAIL BACK done\n");
			while(Check_Limit_L()==0){
				Rail_TuneBack();
			}
			printf("back to limit_L done\n");
			
			status_station2 = 0;
			while (status_station2 == 0){		//	弹夹到达第一个工位的前方
				Rail_TuneForward();
				while(Check_HatMCU_Ready()==0)
					delay_ms(CHECK_INTERVAL);
			}
			
		}
		/********返回到初始位置***********/
		
		printf("arrive at init position\n");
		UNCOMPRESS();
		while (Check_Clip_Unload() == 0);
		printf("unload done\n");
		for(temp=0;temp<WAIT_TIME;temp++)
			delay_ms(1000);
		while (Check_Clip_Unload() == 0);
		period = unload;

	}
	/********正常工作阶段（重复）***********/
}



void Init_All() {
	delay_init();	    	 //延时函数初始化	 
	delay_ms(1000);
	delay_ms(1000);
	Sensor_gpio_init();
	Control_gpio_init();
	 
	uart_init(115200);	 	//串口初始化为115200
	SPI1_Init();		   //初始化SPI,这里默认主机
	SPI2_Init();
}

void rail_state_init(void){
	int i=0;
	while(Check_Limit_L()==0){			//	弹夹到达左极限位置
		Rail_TuneBack();
	}
	printf("back to limit_L done\n");
	
	status_station2 = 0;
	while (status_station2 == 0){		//	弹夹到达第一个工位的前方
		Rail_TuneForward();
		while(Check_HatMCU_Ready()==0)
			delay_ms(CHECK_INTERVAL);
		i++;
	}
  	status_station2 = 0;
	printf("arrive at init position\n");
}

//根据工位位置得到当前的时期
void get_period(u8 temp) {
	if (temp <= DISTANCE1) {
		period = clean;
		g_num_clean++;
	}
	else if (temp <=DISTANCE1 + DISTANCE2) {
		period = clean_detect;
		g_num_clean++;
		g_num_detect++;
	}
	else if (temp <= NUM_TOTAL) {
		period = clean_detect_hat;
		g_num_clean++;
		g_num_detect++;
		g_num_hat++;
	}
	else if (temp <= NUM_TOTAL + DISTANCE1) {
		period = detect_hat;
		g_num_detect++;
		g_num_hat++;
		g_num_clean = 0;
	}
	else if(temp <= NUM_TOTAL + DISTANCE1 + DISTANCE2){
		period = hat;
		g_num_hat++;
		g_num_detect = 0;
	}
	else{
		printf("ERROR\n");
	}
	
	if(g_num_detect == 1){		
		while(Inform_Detect(CMD_Head)==0){
			printf("Inform_Detect(CMD_Head)\n");
			delay_ms(CHECK_INTERVAL);			//弹夹检查开始发送CMD_Head指令
		}
	}
	if(g_num_detect == NUM_TOTAL){ 
		while(Inform_Detect(CMD_Tail)==0){
			printf("Inform_Detect(CMD_Tail)\n");
			delay_ms(CHECK_INTERVAL);	//弹夹检查结束发送CMD_Tail指令
		}
	}
}

//根据当前的时期执行当前工位的动作
void station_work(u8 period) {
	while (1) {
		Fixture_Push();
		while (Check_PushMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);
		if((period > clean) && (period < hat)){
			Inform_Detect(CMD_CLAMP);
			while(Check_DetectMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);
		}
		Fixture_Open();
		while (Check_PushMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);

		if (period == clean) {
			Clean();
			while (Check_CleanMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);
		}
		else if (period == clean_detect) {
			Clean();
			Detect();
			while ((Check_CleanMCU_Ready() == 0) || Check_DetectMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect-1] = detect_result;
		}
		else if (period == clean_detect_hat) {
			Clean();
			Detect();
			if (g_status[g_num_hat-1] == 1) {
				Hat();
			}
			while ((Check_CleanMCU_Ready() == 0) || (Check_HatMCU_Ready() == 0) || (Check_DetectMCU_Ready() == 0)) delay_ms(CHECK_INTERVAL);
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect - 1] = detect_result;
		}
		else if (period == detect_hat) {
			Detect();
			if (g_status[g_num_hat-1] == 1) {
				Hat();
			}
			while ((Check_HatMCU_Ready() == 0) || (Check_DetectMCU_Ready() == 0)) delay_ms(CHECK_INTERVAL);
			detect_result = Check_DetectMCU_Result();
			g_status[g_num_detect - 1] = detect_result;

		}
		else if (period == hat) {
			if (g_status[g_num_hat-1] == 1) {
				Hat();
			}
			while (Check_HatMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);
		}

		LOOSEN();

		if ((g_num_hat >= 0 ) && (g_num_hat <= NUM_TOTAL ) && (g_status[g_num_hat] == 1)) {//当需要带帽时，Fixture_Draw的过程中需要检测戴帽是否成功
			while (Check_PushMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);	//等待回退完成
			Fixture_Draw();
//			Hat_Check();								//回退时检测是否戴帽成功
			while (Check_PushMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);	//等待回退完成
//			while (Check_HatMCU_Ready()== 0) delay_ms(CHECK_INTERVAL);				//等待戴帽的结果

// 			if (Check_HatMCU_Result()== 1)	break;
			break;		//	暂时不检测带帽
		}
		else {
			Fixture_Draw();
			while (Check_PushMCU_Ready() == 0) delay_ms(CHECK_INTERVAL);	//等待回退完成
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
				delay_ms(CHECK_INTERVAL);

		}
		delay_ms(1000);
		Rail_Back();
		for(i=0;i<3;i++)
			delay_ms(1000);

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
				delay_ms(CHECK_INTERVAL);
		}
		delay_ms(1000);
	}
}
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
