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
#define DISTANCE1 9
#define DISTANCE2 8
#define WAIT_TIME	2			//换弹夹时电磁铁掉电时间(单位s)
#define	CHECK_INTERVAL	50		//查询间隔时间（单位ms)

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
	safeGate_error,
	detect_error,
	communication_error,
};



#endif
/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/

