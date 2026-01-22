#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "OLED.h"
#include "BEEP.h"
#include "stdio.h"

extern uint8_t humi_int, humi_deci, temp_int, temp_deci;
extern uint16_t BH_int;
extern uint8_t BH_deci;
extern uint8_t Beep_alarmFlag;

char oledBuf[20];

void TIM2_Int_Init(uint16_t arr, uint16_t psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

void TIM3_Int_Init(uint16_t arr, uint16_t psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
}



void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		sprintf(oledBuf, "Temp: %d.%d C", temp_int, temp_deci);
		OLED_ShowString(1, 1, oledBuf);
		
		sprintf(oledBuf, "Humi: %d.%d %%", humi_int, humi_deci);
		OLED_ShowString(2, 1, oledBuf);
		
		sprintf(oledBuf, "Light:%d.%d Lx", BH_int, BH_deci);
		OLED_ShowString(3, 1, oledBuf);
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		if(Beep_alarmFlag)
		{
			BEEP = ~BEEP;
			LED2_Turn();
		}
		else
		{
			BEEP = 1;
			LED2_OFF();
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
