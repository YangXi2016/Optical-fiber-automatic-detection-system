/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "motor.h"
#include "instruction.h"
#include "bsp_timer.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

extern uint8_t  Ins_Flag;
extern uint8_t  Ins_Count;
extern uint8_t  Ins_Table[12];
extern int16_t  Stage_Coord;
extern uint8_t  Stage_Dir; 
extern uint16_t Stage_Pace_Count;

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
	
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
	
/**
  * @brief  This function handles USART1 Handler.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	uint8_t tag;

	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		tag = USART_ReceiveData(USART1);
		//USART_SendData(USART1,tag);
		if(tag == 'S')
		{
			Ins_Flag = Ins_Inputting;
		}
		
		if((Ins_Flag == Ins_Inputting)&&(tag == 'E'))
		{
			Ins_Flag = Ins_Enable;
			Ins_Table[Ins_Count] = tag;
			Ins_Table[Ins_Count+1] = '\0';
			Ins_Count = 0;
		}
		
		if(Ins_Flag == Ins_Inputting)
		{
			/* 连续记录指令字符，并且防止数组越界赋值 */
			if(Ins_Count < (sizeof(Ins_Table)/sizeof(uint8_t)-2))
			{
				Ins_Table[Ins_Count++] = tag; 	
			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

/**
  * @brief  This function handles TIM2 Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		if(Stage_Dir == Stage_Forward)Stage_Coord ++;
		else if(Stage_Dir == Stage_Backward)Stage_Coord--;

		Stage_Pace_Count--;
		/* 如果步数走完，在中断内停止脉冲输出 */
		if(!Stage_Pace_Count)
		{
			Pulse_Disable();
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
