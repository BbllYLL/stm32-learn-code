#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "KEY.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up = 1;
	if(mode)key_up = 1;
	if(key_up && (KEY0 == 0 || KEY1 == 0))
	{
		Delay_ms(20);
		key_up = 0;
		if(KEY0 == 0)return KEY0_PRES;
		else if(KEY1 == 0)return KEY1_PRES;
	}else if(KEY0 == 1 && KEY1 == 1)key_up = 1;
	return 0;
}


