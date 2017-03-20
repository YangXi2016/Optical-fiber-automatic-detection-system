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
	back,
	unload,
} ;


//system operating status
enum system_status
{
	normal,			
	tissueNull,		//optical fiber
	hatNull,			//worker suddun stop system
	SafeGate_error,
};

//u8 rail_run = 0;


#endif
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

