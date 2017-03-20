/**
  ******************************************************************************
  * @file    main.c 
  * @author  Gilbert
  * @version V1.0
  * @date    2015-03-15
  * @brief   Make the FZ0201IU-AMI Project
  ******************************************************************************
  * @attention
	*
  ******************************************************************************
  */  

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "instruction.h"
#include "led.h"
#include "motor.h"
#include "spi_sim.h"
#include "clampmotor.h"			/*added by yangxi in 2016/12/23*/

#include "spi.h"						/*added by yangxi in 2017/2/15*/

#define INTERACTION		1
#if INTERACTION	
	#define INFORM_COM(MSG)		printf(MSG)
#else
	#define INFORM_COM(MSG)		
#endif

extern uint8_t MASTER_CMD, STM_STATE;	//used in communicating with STM32
struct STATUS COM_STATUS = {0,0,0,0,0};							//used in communicating with COMPUTER

uint8_t 	Ins_Flag = 0;
uint8_t 	Ins_Count = 0;
uint8_t 	Ins_Table[12];

int16_t		Stage_Coord;
uint16_t  Stage_Pace_Count;
uint8_t   Stage_Dir;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint16_t DAC_Code;
	uint16_t Pace;
	int16_t  Stage_Tar_Coord;
	
	Delayms(1000);
	
	/*Clamp_Motor init*/
	/*added by yangxi in 2016/12/23*/
	ClampMotor_Init();
	

	
	/* light sources init */
	LED_Init();
	/* USART1 init */
	USART1_Config();
	/* USART1 recrive interrupt config */
	USART1_ITConfigure();
	/* Stage driver config */
	Motor_Init();
	/* Enable Buf Output */
	Buf_Out_On();
	/* spi_sim control the a3992 config */
	GPIO_A3992_Config();
	/* spi_sim control the dac8830 config */
	GPIO_DAC8830_Config();
	Delayms(100);
	
	/*added by yangxi in 2017/2/15*/
	SPI1_Init(SPI_Mode_Slave);		   //初始化SPI
	STM_STATE = READY_STATE;
	printf("detect ready\n");
	while(1)
	{
		/* 指令执行标志位有效，执行指令 */
		if(Ins_Flag == Ins_Enable)
		{
			/* 指令数据预处理 */
			if((Ins_Table[1]=='M')|(Ins_Table[1]=='X')|(Ins_Table[1]=='Y'))
			{
				Pace = InsStr2Num(Ins_Table+3);		
			}
			else if(Ins_Table[1]=='A')
			{
				if(Ins_Table[2] == '-')
					Stage_Tar_Coord = (-1)*(int16_t)InsStr2Num(Ins_Table+3);
				else if(Ins_Table[2] == '+')
					Stage_Tar_Coord = (int16_t)InsStr2Num(Ins_Table+3);
				else if((Ins_Table[2] >= '0')&&(Ins_Table[2] <= '9'))
					Stage_Tar_Coord = (int16_t)InsStr2Num(Ins_Table+2);
			}
			else if(Ins_Table[1]=='K')
			{
				DAC_Code = InsStr2Num(Ins_Table+2);
			}
			switch(Ins_Table[1])
			{
				case 'C': printf("CE\n");break;
				case 'M': Move(Ins_Table[2],Pace);printf("OKE\n");break;
				case 'A': MoveTo(Stage_Tar_Coord);printf("OKE\n");break;
				case 'X': Move_LineMotorX(Ins_Table[2],Pace);printf("OKE\n");break;
				case 'Y': Move_LineMotorY(Ins_Table[2],Pace);printf("OKE\n");break;
				case 'K': Write_DAC8830(DAC_Code);printf("OKE\n");break;
				case 'H': Go_Limit(Ins_Table[2]);printf("OKE\n");break; 
				case 'O': Back_ORG(Ins_Table[2]);printf("OKE\n");break; 				
				case 'Q': printf("%d\n",Stage_Coord);break;
				case 'L': LED_Control(Ins_Table[2]);printf("OKE\n");break;				
				case 'W': 																										//这里夹紧检测根据当前工作状态选择检测方式
					if(COM_STATUS.Union_Status == 0)	Clamping_Check();
					else{
						if((COM_STATUS.Clamp_Status == 1))	printf("OE\n");
						else	printf("FE\n");
					}
					break;				
				
				case 'Z':
					switch(Ins_Table[2])
					{
						case '1':ClampMotor_Up();break;
						case '0':ClampMotor_Down();Delayms(50);ClampMotor_Off();break;
						default:ClampMotor_Off();break;
					}
					printf("OKE\n");
					break;
					
					//Where to updata STM_STATE /*added by yangxi in 2017/2/15*/
				case 'P':
					if(Ins_Table[2] == '0')	COM_STATUS.Union_Status = 0;
					else	COM_STATUS.Union_Status = 1;
					printf("OKE\n");
					break;
					
				case 'R':
					if(Ins_Table[2] == '1')	STM_STATE = TRUE_RESULT;
					else	STM_STATE = FALSE_RESULT;
					COM_STATUS.Detect_Status = 0;
					COM_STATUS.Clamp_Status = 0;
					if(COM_STATUS.Period_Status == 1)	COM_STATUS.Period_Status = 2;
					if(COM_STATUS.Period_Status == 3)	COM_STATUS.Period_Status = 0;
					printf("OKE\n");
					break;
				
				case 'T':
					STM_STATE = START_STATE;
					printf("OKE\n");
					break;
				case 'V':
					STM_STATE = CLEANSET_STATE;
					printf("OKE\n");
					break;
				case 'I':
					printf("SI%d%d%d%d%dE/n",COM_STATUS.Union_Status,COM_STATUS.Detect_Status,COM_STATUS.Clamp_Status,COM_STATUS.Period_Status,COM_STATUS.ERROR_Status);
					break;
				
				default	: printf("PE\n");break;
			}	
			/* 指令执行结束，失能指令可执行标志位 */
			Ins_Flag = Ins_Disable;
				
		}	
		
		/*added by yangxi in 2017/2/15*/
		if(MASTER_CMD != DUMY){
				USART1->DR=MASTER_CMD;
				while((USART1->SR&0X40)==0);
				if(Is_Detect(MASTER_CMD))
				{
					//Detect();
					STM_STATE = WORK_STATE;
					COM_STATUS.Detect_Status = 1;
					COM_STATUS.Clamp_Status = 1;
					INFORM_COM(CMD_Detect);
				}
// 				else if(Is_Clamp(MASTER_CMD))
// 				{
// 					COM_STATUS.Clamp_Status = 1;
// 				}
// 				else if(Is_Loosen(MASTER_CMD))
// 				{
// 					COM_STATUS.Clamp_Status = 0;
// 				}
				else if(Is_Head(MASTER_CMD))
				{
					COM_STATUS.Period_Status = 1;
					INFORM_COM(CMD_Head);
				}
				else if(Is_Tail(MASTER_CMD))
				{			
					COM_STATUS.Period_Status = 3;
					INFORM_COM(CMD_Tail);
				}
				else if(Is_SafeGateErr(MASTER_CMD))
				{
					COM_STATUS.ERROR_Status = 1;
					INFORM_COM(CMD_ERROR);
				}
				else if(Is_HatNull(MASTER_CMD))
				{
					COM_STATUS.ERROR_Status = 2;
					INFORM_COM(CMD_ERROR);
				}
				else if(Is_TissueNull(MASTER_CMD))
				{
					COM_STATUS.Period_Status = 3;
					INFORM_COM(CMD_ERROR);
				}
				else if(Is_ClearFlag(MASTER_CMD))
				{
					STM_STATE = READY_STATE;
				}				

				MASTER_CMD=DUMY;
			}
	

	}		
		
}

/********************************END OF FILE***********************************/
