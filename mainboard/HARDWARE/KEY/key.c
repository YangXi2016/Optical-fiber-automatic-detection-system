#include "key.h"
#include "delay.h"

/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�readme.txt
*�ļ���ʶ��
*ժ    Ҫ��			 
*��ǰ�汾��0.1
*��    �ߣ�yangxx
*������ڣ�2017/2/10
*****************************************************************************/

//������ʼ������ 
//PD1,PD2���ó�����
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 |GPIO_Pin_2;// |GPIO_Pin_8;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��3������
	

} 

//�������
//����״̬
u8 Check_ready()
{	 
	if(HOLDER_PLACE_1==0||HOLDER_PLACE_2==1)
	{
		delay_ms(10);//ȥ���� 
		if(HOLDER_PLACE_1==0 && HOLDER_PLACE_2==1)return 1;
	}     
	return 0;// �ް�������
}
