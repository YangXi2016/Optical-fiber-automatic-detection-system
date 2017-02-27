#include "key.h"
#include "delay.h"

/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：readme.txt
*文件标识：
*摘    要：			 
*当前版本：0.1
*作    者：yangxx
*完成日期：2017/2/10
*****************************************************************************/

//按键初始化函数 
//PD1,PD2设置成输入
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 |GPIO_Pin_2;// |GPIO_Pin_8;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化3个开关
	

} 

//按键检测
//返回状态
u8 Check_ready()
{	 
	if(HOLDER_PLACE_1==0||HOLDER_PLACE_2==1)
	{
		delay_ms(10);//去抖动 
		if(HOLDER_PLACE_1==0 && HOLDER_PLACE_2==1)return 1;
	}     
	return 0;// 无按键按下
}
