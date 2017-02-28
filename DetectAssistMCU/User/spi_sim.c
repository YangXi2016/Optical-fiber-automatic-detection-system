/**
  ******************************************************************************
  * @file    bsp_spi_dac8830_a3992.c
  * @author  Gilbert
  * @version V3.5.0
  * @date    
  * @brief   SPI contrl the DAC8830,simulative SPI contrl the A3992
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "spi_sim.h"
#include "bsp_timer.h"

uint8_t Idiv8;

/* A3992 8细分表 */
static uint32_t Div8_Table[] = 
	{0x1E07E,0x1E67C,0x1EC74,0x1F1E8,0x1F658,0x1FA46,0x1FD30,0x1FF18,
	 0x1DF80,0x1DF18,0x1DD30,0x1DA46,0x1D658,0x1D1E8,0x1CC74,0x1C67C,
	 0x1807E,0x1867C,0x18C74,0x191E8,0x19658,0x19A46,0x19D30,0x19F18,
	 0x1BF80,0x1BF18,0x1BD30,0x1BA46,0x1B658,0x1B1E8,0x1AC74,0x1A67C};

/**
  * @brief  GPIO simulate SPI config for DAC8830
  * @param  None
  * @retval None
  */	
void GPIO_DAC8830_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Config the GPIO */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	/* Deselect the DAC8830 */
	DAC8830_CS_High();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	Write_DAC8830(0);
}

/**
  * @brief  Control the DAC
  * @param  da:digital code,0~65535
  * @retval None
  */	
void Write_DAC8830(uint16_t da)
{
	uint8_t i;
	
	DAC8830_CS_Low();
	for(i = 0;i < 16;i++)
	{
		if((da & 0x8000) == 0)
		DAC8830_SDI_Low();
		else if(da & 0x8000)
		DAC8830_SDI_High();
		da <<= 1;
		DAC8830_SCK_Low();
		Delayus(1);
    DAC8830_SCK_High();
		Delayus(1);
	}
	DAC8830_CS_High();
}

/**
  * @brief  GPIO simulate SPI config for two A3992s
  * @param  None
  * @retval None
  */	
void GPIO_A3992_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	/* 开启重映射时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	/* JTAG-DP失能，SW-DP使能，即此时PA15，PB3，PB4都能作为普通IO使用 */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/* Deselect both A3992 */
	A3992_CSX_High();
	A3992_CSY_High();
	
	Wake_MotorX();
	Wake_MotorY();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	Idiv8 = 0;
	Send_MotorWordX(A3992_Word1);	//传送配置字
	Delayms(1);
	Send_MotorWordX(Div8_Table[Idiv8]);	//进入第一个励磁

	/* Send first 19bit */
	Send_MotorWordY(A3992_Word1);
	Delayms(1);
	/* make motor at the first state of Div8_Table[] */
	Send_MotorWordY(Div8_Table[Idiv8]);
	
	Delayms(10);
	Sleep_MotorX();
	Sleep_MotorY();
}

/**
  * @brief  Move X line-motor
  * @param  dir:'+'or'-'
	* @param  pace:0~65536
  * @retval None
  */
void Move_LineMotorX(uint8_t dir,uint16_t pace)
{
	uint16_t i;

	Wake_MotorX();
	Delayms(5);
	Send_MotorWordX(A3992_Word1);
	Delayms(5);
	
	if(dir == '+')  
	{
		for(i = 0;i < pace;i++)
		{
			if(Idiv8 == 0)Idiv8 = 32;
			Send_MotorWordX(Div8_Table[Idiv8 - 1]);
			Idiv8--;
/* Change the delay time to change the frequency */
			Delayus(400);
//			GPIOB->ODR ^= (1<<15);	//PB15取反，输出脉冲频率
		}
	}
	else if(dir == '-')
	{
		for(i = 0;i < pace;i++)
		{
			Idiv8++;
			if(Idiv8 == 32)Idiv8 = 0;
			Send_MotorWordX(Div8_Table[Idiv8]);
			Delayus(400);
//			GPIOB->ODR ^= (1<<15);	//PB15取反，输出脉冲频率
		}
	}
	Delayms(5);
	Sleep_MotorX();
}

/**
  * @brief  Move Y line-motor
  * @param  dir:'+'or'-'
	* @param  pace:0~65536
  * @retval None
  */
void Move_LineMotorY(uint8_t dir,uint16_t pace)
{
	uint16_t i;
	
	Wake_MotorY();
	Delayms(5);
	Send_MotorWordY(A3992_Word1);
	Delayms(5);
	
	if(dir == '+')
	{
		for(i = 0;i < pace;i++)
		{
			if(Idiv8 == 0)Idiv8 = 32;
			Send_MotorWordY(Div8_Table[Idiv8 - 1]);
			Idiv8--;
			Delayus(400);
//			GPIOB->ODR ^= (1<<15);	//PB15取反，输出脉冲频率
		}
	}
	else if(dir == '-')
	{
		for(i = 0;i < pace;i++)
		{
			Idiv8++;
			if(Idiv8 == 32)Idiv8 = 0;
			Send_MotorWordY(Div8_Table[Idiv8]);
			Delayus(400);
//			GPIOB->ODR ^= (1<<15);	//PB7取反，输出脉冲频率
		}
	}
	Delayms(5);
	Sleep_MotorY();
}

/* A3992传送的word是19位，不是标准SPI器件，只能使用GPIO口来模拟 
 */
static void Send_MotorWordX(uint32_t motorword)
{
	uint8_t i;
	
	A3992_CSX_Low();
	Delayus(5);
	for(i = 0;i < 19;i++)
	{
		A3992_SCK_Low();
		if((motorword&0x00040000) == 0)
		A3992_SDI_Low();
		else if(motorword&0x00040000)
		A3992_SDI_High();
		motorword <<= 1;
		Delayus(1);
    A3992_SCK_High();
		Delayus(1);
	}
	/* Deselect the motor IC */
	A3992_CSX_High();	
}
static void Send_MotorWordY(uint32_t motorword)
{
	uint8_t i;
	
	A3992_CSY_Low();
	Delayus(5);
	for(i = 0;i < 19;i++)
	{
		A3992_SCK_Low();
		if((motorword&0x00040000) == 0)
		A3992_SDI_Low();
		else if(motorword&0x00040000)
		A3992_SDI_High();
		motorword <<= 1;
		Delayus(1);
    A3992_SCK_High();
		Delayus(1);
	}
	/* Deselect the motor IC */
	A3992_CSY_High();	
}

