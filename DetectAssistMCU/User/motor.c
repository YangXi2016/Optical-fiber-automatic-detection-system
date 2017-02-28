/**
  ******************************************************************************
  * @file    motor.c
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   control the stage
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
	
#include "motor.h"
#include "bsp_timer.h"
#include "bsp_usart.h"

extern int16_t  Stage_Coord;
extern uint16_t Stage_Pace_Count;

uint16_t  Stage_Speed_Table[11] = {24000,16000,12000,9600,8000,6857,6000,5333,4800,4364,4000};
uint8_t   Stage_Pace_Table[11]  = {10,15,20,25,30,35,40,45,50,55,60};

/**
  * @brief  Init the stage motor driver IO
  * @param  None
  * @retval None
  */	
void Motor_Init(void)
{
	/* Config the GPIO */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/* Config the stage sensors IO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/* Config Buf_OE GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	/* Driver SD5107P3-A22 IO init */ 	
	OutCurrent_On(); 	
	
	/* TIM2 for CCW&CW Pulse Config */
	CCW_CW_Pulse_Init();
	TIM2_ITConfigure();		
}

/**
  * @brief  Go to any position of the two limit sensors
  * @param  stage_dir: select which position to go
  * @retval None
  */	
void Go_Limit(u8 stage_dir)
{
	uint8_t	i = 0;
	uint8_t icount = 0;

	if(!Stage_Pace_Count)
	{
		if(stage_dir == '+')
		{
			while(GetStatus_LSCCW() == RESET)   //pb10- io口读 
			{
				if(i < 11)
				{
					Stage_Pace_Count = 1;
					CCW_Pulse_Output(Stage_Speed_Table[i]-1);
					while(Stage_Pace_Count);
					icount = 1+icount;
					if(icount == Stage_Pace_Table[i])
					{
						i++;
						icount = 0;
					}
				}
				else if(i == 11)
				{
					Stage_Pace_Count = 1;
					CCW_Pulse_Output(Stage_Speed_Table[i]-1);
					while(Stage_Pace_Count);
				}                                          
			}
			while(i)
			{
				i--;
				Stage_Pace_Count = Stage_Pace_Table[i];
				CCW_Pulse_Output(Stage_Speed_Table[i]-1);
				while(Stage_Pace_Count);
			}
			Delayms(200);
			while(GetStatus_LSCCW() == SET)
			{
				Stage_Run(Stage_Backward,1000,1);
			}	
			Delayms(100);
			Stage_Run(Stage_Backward,1000,500);
		}
		else if(stage_dir == '-')
		{
//			while(GetStatus_LSCW() == RESET)
//			{
//				if(i < 11)
//				{
//					Stage_Pace_Count = 1;
//					CW_Pulse_Output(Stage_Speed_Table[i]-1);
//					while(Stage_Pace_Count);
//					icount = 1+icount;
//					if(icount == Stage_Pace_Table[i])
//					{
//						i++;
//						icount = 0;
//					}
//				}
//				else if(i == 11)
//				{
//					Stage_Pace_Count = 1;
//					CW_Pulse_Output(Stage_Speed_Table[i]-1);
//					while(Stage_Pace_Count);
//				}
//			}
//			while(i)
//			{
//				i--;
//				Stage_Pace_Count = Stage_Pace_Table[i];
//				CW_Pulse_Output(Stage_Speed_Table[i]-1);
//				while(Stage_Pace_Count);
//			}
//			Delayms(200);
//			while(GetStatus_LSCW() == SET)
//			{
//				Stage_Run(Stage_Forward,1000,1);
//			}	
//			Delayms(100);
//			Stage_Run(Stage_Forward,1000,100);		
		}
	}
	else printf("Stage is busy!\n");
}

/**
  * @brief  Go to the original sensor position
  * @param  dir: forward or backward the ORG
  * @retval None
  */	
void Back_ORG(uint8_t dir)
{
	uint8_t	i = 0;
	uint8_t icount = 0;
	uint8_t ls_org;
	uint8_t ls_ccw;
//	uint8_t ls_cw;

	if(!Stage_Pace_Count)
	{
		/* forward the ORG */
		if(dir == '+')
		{
			while((GetStatus_LSORG() == RESET)&&(GetStatus_LSCCW() == RESET))
			{
				if(i < 11)
				{
					Stage_Pace_Count = 1;
					CCW_Pulse_Output(Stage_Speed_Table[i]-1);
					while(Stage_Pace_Count);
					icount = 1+icount;
					if(icount == Stage_Pace_Table[i])
					{
						i++;
						icount = 0;
					}
				}
				else if(i == 11)
				{
					Stage_Pace_Count = 1;
					CCW_Pulse_Output(Stage_Speed_Table[i]-1);
					while(Stage_Pace_Count);
				}
			}
			/* Record which the sensor position the stage arrives at */
			ls_ccw = GetStatus_LSCCW();
			ls_org = GetStatus_LSORG();
			while(i)
			{
				i--;
				Stage_Pace_Count = Stage_Pace_Table[i];
				CCW_Pulse_Output(Stage_Speed_Table[i]-1);
				while(Stage_Pace_Count);
			}
			Delayms(200);
			/* if arrive at ORG */
			if(ls_org == SET)
			{
				/* scan back slowly */
				while(GetStatus_LSORG() == SET)
				{
					Stage_Run(Stage_Backward,1000,1);
				}
				Delayms(100);
				Stage_Run(Stage_Backward,1000,100);
				while(Stage_Pace_Count);
				Stage_Coord = 0;
			}
			/* if arrive at LS_CCW,go back to the LS_ORG */
			else if(ls_ccw == SET)
			{
				while(GetStatus_LSORG() == RESET)
				{
					if(i < 11)
					{
						Stage_Pace_Count = 1;
						CW_Pulse_Output(Stage_Speed_Table[i]-1);
						while(Stage_Pace_Count);
						icount = 1+icount;
						if(icount == Stage_Pace_Table[i])
						{
							i++;
							icount = 0;
						}
					}
					else if(i == 11)
					{
						Stage_Pace_Count = 1;
						CW_Pulse_Output(Stage_Speed_Table[i]-1);
						while(Stage_Pace_Count);
					}
				}
				while(i)
				{
					i--;
					Stage_Pace_Count = Stage_Pace_Table[i];
					CW_Pulse_Output(Stage_Speed_Table[i]-1);
					while(Stage_Pace_Count);
				}
				Delayms(200);
				while(GetStatus_LSORG() == SET)
				{
					Stage_Run(Stage_Backward,1000,1);
				}
				Delayms(100);
				Stage_Run(Stage_Backward,1000,100);
				while(Stage_Pace_Count);
				Stage_Coord = 0;
			}
		}
		else if(dir == '-')
		{
		}	
	}
	else printf("Stage is busy!\n");
}

/**
  * @brief  Stage runs with any direction,any speed,and any pace
  * @param  dir: '+'or'-'
	*	@param	speed: any PPS
	* @Param	pace: any P
  * @retval None
  */	
void Stage_Run(uint8_t dir,uint16_t speed,uint16_t pace)
{
	uint16_t period;

	period = 24000000/speed -1;
	Stage_Pace_Count = pace;
	if(dir == Stage_Forward)
	{
		CCW_Pulse_Output(period);
	}
	else if(dir == Stage_Backward)
	{
		CW_Pulse_Output(period);
	}
}

/**
  * @brief  Move with a acc
  * @param  dir: 
	*	@param	pace:
  * @retval None
  */	
void ACC_Move(uint8_t dir,uint16_t pace)
{
	uint8_t  i;
	uint16_t pace_temp;
	
	pace_temp = pace - 770;
	
	if(dir == Stage_Forward)
	{
		for(i=0;i<11;i++)
		{
			Stage_Pace_Count = Stage_Pace_Table[i];
			CCW_Pulse_Output(Stage_Speed_Table[i]-1);
			while(Stage_Pace_Count);

		}
		Stage_Pace_Count = pace_temp;
		CCW_Pulse_Output(3692-1);
		while(Stage_Pace_Count);
		for(i = 11;i>0;i--)
		{
			Stage_Pace_Count = Stage_Pace_Table[i-1];
			CCW_Pulse_Output(Stage_Speed_Table[i-1]);
			while(Stage_Pace_Count);	
		}	
	}
	else if(dir == Stage_Backward)
	{
		for(i=0;i<11;i++)
		{
			Stage_Pace_Count = Stage_Pace_Table[i];
			CW_Pulse_Output(Stage_Speed_Table[i]-1);
			while(Stage_Pace_Count);
		}
		Stage_Pace_Count = pace_temp;
		CW_Pulse_Output(3692-1);
		while(Stage_Pace_Count);
		for(i = 11;i>0;i--)
		{
			Stage_Pace_Count = Stage_Pace_Table[i-1];
			CW_Pulse_Output(Stage_Speed_Table[i-1]);
			while(Stage_Pace_Count);	
		}	
	}
}

/**
  * @brief  Move stage
  * @param  dir: 
	*	@param	pace:
  * @retval None
  */	
void Move(uint8_t dir,uint16_t pace)
{
	int16_t stage_tar_coord; 
	
	if(!Stage_Pace_Count)
	{
		if(dir == '+')
		{
			stage_tar_coord = Stage_Coord + pace;
		}
		else if(dir == '-')
		{
			stage_tar_coord = Stage_Coord - pace;
		}
		if((stage_tar_coord >= Stage_Range_Lower)&&(stage_tar_coord <= Stage_Range_Upper))
		{
			if((pace > 0)&&(pace < 900)) //固定频率慢速转动 
			{
				Stage_Run(dir,1000,pace);
			}
			else if(pace>=900)        //做电机曲线频率变化的转动 
			{
				ACC_Move(dir,pace);    
			}
		}
		else printf("OORE\n");		
	}
	else printf("Stage is busy!\n");
}

/**
  * @brief  Move to a absolute coord
  * @param  stage_tar_coord: 
  * @retval None
  */	
void MoveTo(int16_t stage_tar_coord)
{
	uint16_t pace;
	
	if(!Stage_Pace_Count)
	{
		if(stage_tar_coord > Stage_Coord)
		{
			pace = stage_tar_coord - Stage_Coord;
			Move(Stage_Forward,pace);
		}
		else if(stage_tar_coord <Stage_Coord)
		{
			pace = Stage_Coord - stage_tar_coord;
			Move(Stage_Backward,pace);		
		}
	}
	else printf("Stage is busy!\n");
}
