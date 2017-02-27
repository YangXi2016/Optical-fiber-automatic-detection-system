/**
  ******************************************************************************
  * @file    insturction.c
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "instruction.h"

/**
  * @brief  
  * @param  
  * @retval 
  */
	
uint16_t InsStr2Num(uint8_t * InsNum)
{
	long Num;
	u8 *pInsNum = InsNum;
	while(*pInsNum != 'E')
	{
		*pInsNum=*pInsNum - 48;
		pInsNum++;	
	}
	Num = (long)*InsNum;
	while(*(InsNum+1) != 'E')
	{
		Num = 10*Num+((long)*(InsNum+1)) ;
		InsNum ++;
	}
	return(Num);
}
