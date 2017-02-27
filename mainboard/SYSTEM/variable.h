/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�variable.h
*�ļ���ʶ��
*ժ    Ҫ��ժ¼��Э������ʹ�õ�IO��			 
*��ǰ�汾��1.0
*��    �ߣ�yangxi
*������ڣ�2017/2/15
*****************************************************************************/

#ifndef __VARIABLE_H
#define __VARIABLE_H

//system parameter
#define NUM_TOTAL 20
#define DISTANCE  4			//����ͨ�����ⷴ�䶨λʱ���о�����ù�λ�ľ���
#define DISTANCE1 8
#define DISTANCE2 6

u8 g_status[20] = {0};	//20���о�20����λ������λ�����19����
u8 g_num_clean  = 0;		//����ͳһ��0��ʼ��19����
u8 g_num_detect = 0;
u8 g_num_hat  = 0;			

//system running status;recording the status of one peried 
enum running_status
{
	ready,
	upload,
	already,
	clean,
	clean_detect,
	clean_detect_hat,
	detect_hat,
	hat,
	//finish,
	back,
	unload,
} period;


//system operating status
enum operating_status
{
	usual,			//perform well
	drop_out,		//optical fiber
	halt,			//worker suddun stop system
	wipe_error,		//the material used for wiping finish or break
	hat_error,		//the hat finish
};

//u8 rail_run = 0;


#endif
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

