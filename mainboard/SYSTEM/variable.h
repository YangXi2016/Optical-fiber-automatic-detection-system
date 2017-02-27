/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2017,杭州维勘科技有限公司
*All rights reserved
*
*文件名称：variable.h
*文件标识：
*摘    要：摘录本协处理器使用的IO口			 
*当前版本：1.0
*作    者：yangxi
*完成日期：2017/2/15
*****************************************************************************/

#ifndef __VARIABLE_H
#define __VARIABLE_H

//system parameter
#define NUM_TOTAL 20
#define DISTANCE  4			//弹夹通过红外反射定位时弹夹距离擦拭工位的距离
#define DISTANCE1 8
#define DISTANCE2 6

u8 g_status[20] = {0};	//20个夹具20个工位，但工位间距是19个。
u8 g_num_clean  = 0;		//计数统一从0开始，19结束
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

