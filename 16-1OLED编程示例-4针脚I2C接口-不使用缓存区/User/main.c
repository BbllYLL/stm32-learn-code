#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

extern const uint8_t OLED_F8x16[][16];

int main(void)
{
	OLED_Init();
	OLED_Clear();
	
	OLED_ShowChinese(10, 1, "你好世界");
	
	while(1)
	{

	}
}
