#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

extern uint8_t OLED_DisplayBuf[8][128];

int main(void)
{
	OLED_Init();
	
	OLED_DisplayBuf[0][0] = 0xAA;
	
	OLED_Update();
	
	while(1)
	{
	
	}
}
