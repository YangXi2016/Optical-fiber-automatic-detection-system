/******************** (C) COPYRIGHT 2017  **********************************
*Copyright(c)2016,����ά���Ƽ����޹�˾
*All rights reserved
*
*�ļ����ƣ�StepperMotor.c
*�ļ���ʶ��
*ժ	   Ҫ���Բ����������ɲ����Դ��ʼ�������ṩ��������		 
*��ǰ�汾��1.0
*��    �ߣ����
*������ڣ�2017/2/9
*****************************************************************************/
#include "StepperMotor.h"
#include "math.h"
#include "stm32f10x.h"
#include "usart.h"


static u16 gs_pAccPrescTab[PUSH_ACC_NUM] = {0};//�Ƽоߵ����/����Ԥ��Ƶ��
static u16 gs_mcAccPrescTab[MC_ACC_NUM] = {0};//�м���ߵ����/����Ԥ��Ƶ��
static u16 gs_scAccPrescTab[SC_ACC_NUM] = {0};//���߼��ߵ����/����Ԥ��Ƶ��

u32 g_pStps = 0;//�Ƽоߵ����Ҫ��ת�Ĳ���
u16 g_tpPresc = 0;//�Ƽоߵ��Ŀ���ٶȶ�Ӧ��Ԥ��Ƶ��
u8  g_npActFlg = 0; //�Ƽоߵ���¶�����־λ
u8  g_pActDFlg = 0; //�Ƽоߵ������������־λ
u8  g_pClkFlg  = 0; //�Ƽоߵ���������ʹ�ܱ�־λ

u32 g_mcStps = 0;//�м���ߵ����Ҫ��ת�Ĳ���
u16 g_tmcPresc = 0;//�м���ߵ��Ŀ���ٶȶ�Ӧ��Ԥ��Ƶ��
u8  g_nmcActFlg = 0; //�м���ߵ���¶�����־λ
u8  g_mcActDFlg = 0; //�м���ߵ������������־λ
u8  g_mcClkFlg  = 0; //�м���ߵ���������ʹ�ܱ�־λ

u32 g_scStps = 0;//���߼��ߵ����Ҫ��ת�Ĳ���
u16 g_tscPresc = 0;//���߼��ߵ��Ŀ���ٶȶ�Ӧ��Ԥ��Ƶ��
u8  g_nscActFlg = 0; //���߼��ߵ���¶�����־λ
u8  g_scActDFlg = 0; //���߼��ߵ������������־λ
u8  g_scClkFlg  = 0; //���߼��ߵ���������ʹ�ܱ�־λ



/*******************************************************************************
*�������ƣ�StepMotorGPIOInit
*����˵�������Ʋ��������GPIO���� 
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void StepMotorGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA
						  | RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PC�˿�ʱ��
	
	// �Ƽоߵ�� CLK - PA1  DIR - PA2   EN  - PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
		
	// ���Ҽгֵ�� CLK - PB0   DIR - PB3   EN - PB4  (ɲ����)BARKE - PB6   (ɲ����)BARKE - PB7
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// �мгֵ�� CLK - PA8   DIR - PA11   EN - PA12   (ɲ��)BARKE - PB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

/*******************************************************************************
*�������ƣ�StepMotorTIMInit
*����˵�������Ʋ�������Ķ�ʱ�����ж����� 
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void StepMotorTIMInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
 	//�Ƽоߵ��ʱ������ TIM2 - CH2 - PA1
	TIM_TimeBaseStructure.TIM_Period = DEFAULT_PERIOD - 1;	 //PWM����
	TIM_TimeBaseStructure.TIM_Prescaler = DEFAULT_PRESCALE - 1;  //����Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig( TIM2, TIM_IT_Update  |  TIM_IT_Trigger,  ENABLE);//����TIM2���ж�Դ�ʹ����ж�

	//���ö�ʱ���ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM1~3ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1�����ȼ��θ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 


}

/*******************************************************************************
*�������ƣ�StepMotorInit
*����˵�����������GPIO����ʱ����ʼ�������ٱ��ʼ��
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void StepMotorInit(void)
{
	u16 i = 0;
	double prescMin = 0;
	double prescMax = 0;
	double t_sx = 0; //
	double TIFreq = 0;
	
	
	StepMotorGPIOInit();//GPIO��ʼ��
	StepMotorTIMInit();//��ʱ�����жϳ�ʼ��
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);

	
	//���θ���Ԥ��Ƶ���е�ÿһ�� ���� f = fmin + (fmax - fmin)/(1 + e^(-a(i/num -1)))
	//���мӼ��٣��˴���fת������Ԥ��Ƶ�ϣ���˹�ʽ�б���
	TIFreq = 72000000.0 / DEFAULT_PERIOD /DEFAULT_PRESCALE;//ʱ���ж�Ƶ�ʼ���
	
	//�������Ƹ˵���Ӽ���Ԥ��Ƶ��
	prescMin =  TIFreq / (P_MAX_SPD * 1.0 / 60 * P_MOTOR_DIV);//�����趨�������СԤ��Ƶ
	prescMax = TIFreq /  (P_MIN_SPD * 1.0 / 60 * P_MOTOR_DIV);
	for (i=0; i<PUSH_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-PS_COEFF * (PA_COEFF * i / PUSH_ACC_NUM - 1)));
		gs_pAccPrescTab[i] = (u16)((1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin)) + 0.5);
		printf("pPrescTab[%d]:%d\n",i,gs_pAccPrescTab[i]);
	}
	
	//�����м���ߵ���Ӽ���Ԥ��Ƶ��
	prescMin =  TIFreq / (MC_MAX_SPD * 1.0 / 60 * MC_MOTOR_DIV);//�����趨�������СԤ��Ƶ
	prescMax = TIFreq /  (MC_MIN_SPD * 1.0 / 60 * MC_MOTOR_DIV);
	for (i=0; i<MC_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-MCS_COEFF * (MCA_COEFF * i / MC_ACC_NUM - 1)));
		gs_mcAccPrescTab[i] = (u16)(1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin) + 0.5);
		printf("mcPrescTab[%d]:%d\n",i,gs_mcAccPrescTab[i]);
	}	

	//�������߼��ߵ���Ӽ���Ԥ��Ƶ��
	prescMin =  TIFreq / (SC_MAX_SPD * 1.0 / 60 * SC_MOTOR_DIV);//�����趨�������СԤ��Ƶ
	prescMax = TIFreq /  (SC_MIN_SPD * 1.0 / 60 * SC_MOTOR_DIV);
	for (i=0; i<MC_ACC_NUM; i++)
	{
		t_sx = 1.0 + pow(e, (double)(-SCS_COEFF * (SCA_COEFF * i / SC_ACC_NUM - 1)));
		gs_scAccPrescTab[i] = (u16)(1.0*t_sx * prescMin * prescMax / (t_sx * prescMin + prescMax - prescMin) + 0.5);
		printf("scPrescTab[%d]:%d\n",i,gs_scAccPrescTab[i]);
	}
	
}

/*******************************************************************************
*�������ƣ�MotorEN
*����˵�����������ʹ��
*���������motor : P���ߵ����M�м���ߵ����S���߼��ߵ��
		   oper: E enable ; D disable
*�����������
*���ز������� 
*******************************************************************************/
void MotorEN(u8 motor,u8 oper)
{
	switch(motor)
	{
		case 'P':
		{
			if (oper == 'D')
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3);
			}
			else if (oper == 'E')
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
			}
			break;
		}//�Ƽоߵ��
		
		case 'M':
		{
			if (oper == 'D')
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_12);
			}
			else if (oper == 'E')
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);	
			}
			break;			
		}//�м�гֵ��
		
		case 'S':
		{
			if (oper == 'D')
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_4);
			}
			else if (oper == 'E')
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_4);	
			}
			break;			
		}//���߼гֵ��	
		default:break;
	}
}

/*******************************************************************************
*�������ƣ�MotorDir
*����˵�����������ת��
*���������motor : P���ߵ����M�м���ߵ����S���߼��ߵ��
		   oper: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
*�����������
*���ز������� 
*******************************************************************************/
void MotorDir(u8 motor,u8 oper)
{
	switch(motor)
	{
		case 'P':
		{
			if (oper == '+')
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_2);
			}
			else if (oper == '-')
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			}
			break;
		}//�Ƽоߵ��
		
		case 'M':
		{
			if (oper == '+')
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_11);
			}
			else if (oper == '-')
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_11);	
			}
			break;			
		}//�м�гֵ��
		
		case 'S':
		{
			if (oper == '+')
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_3);
			}
			else if (oper == '-')
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_3);	
			}
			break;			
		}//���߼гֵ��	
		default:break;
	}
}

/*******************************************************************************
*�������ƣ�PushMotion
*����˵�����Ƽо߲�������������ýӿ�
*���������angleDeg : ���������Ҫת���Ķ��� ��λ��
		   dir: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
		   spd:�ٶȣ���λr/min
*�����������
*���ز������� 
*******************************************************************************/
void PushMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //��ʱ�������жϵ�Ƶ��
	float spdFreq = 0; //�ﵽ��Ӧ�ٶ���Ҫ������Ƶ��
	
	MotorEN('P','E');//ʹ���Ƽоߵ��
	MotorDir('P',dir);//�趨ת��
	
	//�ٶ�����
	if (spd > P_MAX_SPD)
	{
		spd = P_MAX_SPD;
	}
	else if (spd < P_MIN_SPD)
	{
		spd = P_MIN_SPD;
	}
	
	//����ȫ�ֱ���
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//��ʱ���ж�Ƶ��
	spdFreq = spd * 1.0 / 60 * P_MOTOR_DIV;//�ﵽָ���ٶ���Ҫ������Ƶ��	
	
	g_tpPresc = (u16)( TIFreq / spdFreq  +0.5);//��������ٶȶ�Ӧ��Ԥ��Ƶֵ		
	g_pStps = (u32)((angleDeg * 1.0 / 360 * P_MOTOR_DIV)+0.5);//���´��˶��Ĳ���
	g_npActFlg = 1;
	g_pActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
	//printf("psteps: %d\n",g_pStps);
 	//printf("pPrec: %d\n",g_tpPresc);

}

/*******************************************************************************
*�������ƣ�MCMotion
*����˵�����м���߲�������������ýӿ�
*���������angleDeg : ���������Ҫת���Ķ��� ��λ��
		   dir: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
		   spd:�ٶȣ���λr/min
*�����������
*���ز������� 
*******************************************************************************/
void MCMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //��ʱ�������жϵ�Ƶ��
	float spdFreq = 0; //�ﵽ��Ӧ�ٶ���Ҫ������Ƶ��	
	
	MotorEN('M','E');//ʹ���Ƽоߵ��
	MotorDir('M',dir);//�趨ת��
	
	//�ٶ�����
	if (spd > MC_MAX_SPD)
	{
		spd = MC_MAX_SPD;
	}
	else if (spd < MC_MIN_SPD)
	{
		spd = MC_MIN_SPD;
	}
	
	//����ȫ�ֱ���	
	//����ȫ�ֱ���
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//��ʱ���ж�Ƶ��
	spdFreq = spd * 1.0 / 60 * MC_MOTOR_DIV;//�ﵽָ���ٶ���Ҫ������Ƶ��	
	
	g_tmcPresc = (u16)( TIFreq / spdFreq  +0.5);//��������ٶȶ�Ӧ��Ԥ��Ƶֵ		
	g_mcStps = (u32)((angleDeg * 1.0 / 360 * MC_MOTOR_DIV)+0.5);//���´��˶��Ĳ���
	g_nmcActFlg = 1;
	g_mcActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
 	//printf("mcSteps: %d\n",g_mcStps);
 	//printf("mcPrec: %d\n",g_tmcPresc);
	
	
}

/*******************************************************************************
*�������ƣ�SCMotion
*����˵�������߼��߲�������������ýӿ�
*���������angleDeg : ���������Ҫת���Ķ��� ��λ��
		   dir: + ���� ; -���򣻹涨���������ᣬ˳ʱ��Ϊ��
		   spd:�ٶȣ���λr/min
*�����������
*���ز������� 
*******************************************************************************/
void SCMotion(float angleDeg, u8 dir, u16 spd)
{
	float TIFreq = 0;  //��ʱ�������жϵ�Ƶ��
	float spdFreq = 0; //�ﵽ��Ӧ�ٶ���Ҫ������Ƶ��	
	
	MotorEN('S','E');//ʹ���Ƽоߵ��
	MotorDir('S',dir);//�趨ת��
	
	//�ٶ�����
	if (spd > SC_MAX_SPD)
	{
		spd = SC_MAX_SPD;
	}
	else if (spd < SC_MIN_SPD)
	{
		spd = SC_MIN_SPD;
	}
	
	//����ȫ�ֱ���
	TIFreq = 72000000.0 / DEFAULT_PERIOD / DEFAULT_PRESCALE;//��ʱ���ж�Ƶ��
	spdFreq = spd * 1.0 / 60 * SC_MOTOR_DIV;//�ﵽָ���ٶ���Ҫ������Ƶ��	
	
	g_tscPresc = (u16)( TIFreq / spdFreq  +0.5);//��������ٶȶ�Ӧ��Ԥ��Ƶֵ		
	g_scStps = (u32)((angleDeg * 1.0 / 360 * SC_MOTOR_DIV)+0.5);//���´��˶��Ĳ���
	g_nscActFlg = 1;
	g_scActDFlg = 0;

	TIM_Cmd(TIM2,ENABLE);
	
 	//printf("scSteps: %d\n",g_scStps);
 	//printf("scPrec: %d\n",g_tscPresc);
}

/*******************************************************************************
*�������ƣ�PMClkGen
*����˵��������ʱ���жϲ�����������������Ƽоߵ��
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void PMClkGen(void)
{
	static u16 s_clkCnt = 0;
	static u16 s_presc = P_DEFAULT_PRE;
	
	s_clkCnt ++;
	
	if (g_pClkFlg == 1)
	{
		if (s_clkCnt > (s_presc>>1))
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
		}
		else
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			return;
		}			
	}
	else
	{;}
	
	if (s_clkCnt == s_presc)
	{
		s_presc = PushMotorDrive();
		s_clkCnt = 0;
	}
}

/*******************************************************************************
*�������ƣ�MCMClkGen
*����˵��������ʱ���жϲ�����������������м���ߵ��
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void MCMClkGen(void)
{
	static u16 s_clkCnt = 0;
	static u16 s_presc = MC_DEFAULT_PRE;
	
	s_clkCnt ++;
	
	if (g_mcClkFlg == 1)
	{
		if (s_clkCnt > (s_presc>>1))
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			return;
		}			
	}
	else
	{;}
	
	if (s_clkCnt == s_presc)
	{
		s_presc = MCMotorDrive();
		s_clkCnt = 0;
	}
}

/*******************************************************************************
*�������ƣ�SCMClkGen
*����˵��������ʱ���жϲ�������������������߼��ߵ��
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void SCMClkGen(void)
{
	static u16 s_clkCnt = 0;
	static u16 s_presc = SC_DEFAULT_PRE;

	s_clkCnt ++;
	
	if (g_scClkFlg == 1)
	{
		if (s_clkCnt >(s_presc>>1))
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
		}
		else
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			return;
		}			
	}
	else
	{;}
	
	if (s_clkCnt == s_presc )
	{
		s_presc = SCMotorDrive();
		s_clkCnt = 0;
	}

}

/*******************************************************************************
*�������ƣ�PushMotorDrive
*����˵��������pmClkGen�������ɵ������������
*�����������
*������������ڵ�������Ƶ�ʵķ�Ƶ
*���ز������� 
*******************************************************************************/
u16 PushMotorDrive(void)
{
	static u16 s_currPresc = DEFAULT_PRESCALE ;
	static u8 s_state = 0;
	static u16 s_accCnt = 0;
	static u16 s_maxAccNum = 0;
		
	if (g_npActFlg == 1)
	{
		s_accCnt = 0;
		s_state = 0;
		g_npActFlg = 0;
	}
	else
	{;}//����������¶�������������е�״̬
	
	//printf("s : %d\n",s_state);
	switch (s_state)
	{
		case 0:
		{
			if (g_pStps > 2*PUSH_ACC_NUM)
			{
				s_maxAccNum = PUSH_ACC_NUM;
			}
			else 
			{
				s_maxAccNum = g_pStps / 2;
			}
			
			s_accCnt = 0;
			s_currPresc = gs_pAccPrescTab[0];
			s_state = 1;
			
			if (g_pStps !=0)
			{
				g_pClkFlg = 1;
			}				
			//break;
		}
		case 1:
		{
			
			if (g_tpPresc < s_currPresc && s_accCnt < s_maxAccNum)
			{
				s_currPresc = gs_pAccPrescTab[s_accCnt];
				g_pStps--;
				s_accCnt++;
				break;				
			}
			else
			{
				s_state = 2;
			}
		}
		case 2:
		{
			if (g_pStps > s_accCnt)
			{
				g_pStps --;
				break;				
			}
			else
			{
				s_state = 3;
			}		
		}
		case 3:
		{
			if (g_pStps != 0)
			{
				g_pStps --;
				s_accCnt--;
				s_currPresc = gs_pAccPrescTab[s_accCnt];
				break;
			}
			else
			{
				g_pActDFlg = 1;
				g_pClkFlg = 0;
			}
		}
	}
	return s_currPresc;
}

/*******************************************************************************
*�������ƣ�MCMotorDrive
*����˵��������mcmClkGen�������ɵ������������
*�����������
*������������ڵ�������Ƶ�ʵķ�Ƶ
*���ز������� 
*******************************************************************************/
u16 MCMotorDrive(void)
{
	static u16 s_currPresc = MC_DEFAULT_PRE ;
	static u8 s_state = 0;
	static u16 s_accCnt = 0;
	static u16 s_maxAccNum = 0;

	
	if (g_nmcActFlg == 1)
	{
		s_accCnt = 0;
		s_state = 0;
		g_nmcActFlg = 0;
	}
	else
	{
		;
	}//����������¶�������������е�״̬
	
	//printf("s : %d\n",s_state);
	switch (s_state)
	{
		case 0:
		{
			if (g_mcStps > 2*MC_ACC_NUM)
			{
				s_maxAccNum = MC_ACC_NUM;
			}
			else 
			{
				s_maxAccNum = g_mcStps / 2;
			}
			
			s_accCnt = 0;
			s_currPresc = gs_mcAccPrescTab[0];
			s_state = 1;
			
			if (g_mcStps !=0)
			{
				g_mcClkFlg = 1;
			}				
			//break;
		}
		case 1:
		{
			
			if (g_tmcPresc < s_currPresc && s_accCnt < s_maxAccNum)
			{
				s_currPresc = gs_mcAccPrescTab[s_accCnt];
				g_mcStps--;
				s_accCnt++;
				break;				
			}
			else
			{
				s_state = 2;

			}
		}
		case 2:
		{
			if (g_mcStps > s_accCnt)
			{
				g_mcStps --;
				break;				
			}
			else
			{
				s_state = 3;
			}		
		}
		case 3:
		{
			if (g_mcStps != 0)
			{
				g_mcStps --;
				s_accCnt--;
				s_currPresc = gs_mcAccPrescTab[s_accCnt];
				break;
			}
			else
			{
				g_mcActDFlg = 1;
				g_mcClkFlg = 0;
			}
		}
	}
	return s_currPresc;	
}

/*******************************************************************************
*�������ƣ�SCMotorDrive
*����˵��������scmClkGen�������ɵ������������
*�����������
*������������ڵ�������Ƶ�ʵķ�Ƶ
*���ز������� 
*******************************************************************************/
u16 SCMotorDrive(void)
{
	static u16 s_currPresc = SC_DEFAULT_PRE;
	static u8 s_state = 0;
	static u16 s_accCnt = 0;
	static u16 s_maxAccNum = 0;
	
	
	if (g_nscActFlg == 1)
	{
		s_accCnt = 0;
		s_state = 0;
		g_nscActFlg = 0;
	}
	else
	{
		;
	}//����������¶�������������е�״̬
	
	//printf("s : %d\n",s_state);
	switch (s_state)
	{
		case 0:
		{
			if (g_scStps > 2*SC_ACC_NUM)
			{
				s_maxAccNum = SC_ACC_NUM;
			}
			else 
			{
				s_maxAccNum = g_scStps / 2;
			}
			
			s_accCnt = 0;
			s_currPresc = gs_scAccPrescTab[0];
			s_state = 1;
			
			if (g_scStps !=0)
			{
				g_scClkFlg = 1;
			}				
			//break;
		}
		case 1:
		{
			
			if (g_tscPresc < s_currPresc && s_accCnt < s_maxAccNum)
			{
				s_currPresc = gs_scAccPrescTab[s_accCnt];
				g_scStps--;
				s_accCnt++;
				break;				
			}
			else
			{
				s_state = 2;
			}
		}
		case 2:
		{
			if (g_scStps > s_accCnt)
			{
				g_scStps --;
				break;				
			}
			else
			{
				s_state = 3;
			}		
		}
		case 3:
		{
			if (g_scStps != 0)
			{
				g_scStps --;
				s_accCnt--;
				s_currPresc = gs_scAccPrescTab[s_accCnt];
				break;
			}
			else
			{
				g_scActDFlg = 1;
				g_scClkFlg = 0;
			}
		}
	}
	return s_currPresc;	
}

/*******************************************************************************
*�������ƣ�IsMotActDone
*����˵����������������Ƿ���ɱ�־λ
*���������motor P���Ƹ˵����M�м�гֵ����S���߼ӳֵ��
*�����������
*���ز��������״̬ 1 ��ɣ�0 δ��� 
*******************************************************************************/
u8 IsMotActDone(u8 motor)
{
	switch (motor)
	{
		case 'P':return g_pActDFlg;
		case 'M':return g_mcActDFlg;
		case 'S':return g_scActDFlg;
		default:return 0;		
	}
}

/*******************************************************************************
*�������ƣ�TIM2_IRQHandler
*����˵����TIM2�жϣ����ڲ���ʱ���ź������������
*�����������
*�����������
*���ز������� 
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
	else
	{
		return;
	}//ȷ����ָ���ж�Դ�����ж�
	
	PMClkGen(); //����������������������ź�
	MCMClkGen();
	SCMClkGen();
}




void Fixture_Stop(void){
	MotorEN('P','D');
	MotorEN('M','D');
	MotorEN('S','D');
}

/******************* (C) COPYRIGHT 2017 *****END OF FILE************************/
