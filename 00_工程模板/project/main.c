#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "OLED.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

char RxData;

int main (void)
{
//	uint8_t Array[] = {0x41, 0x42, 0x43, 0x44};
	
	Serial_Init();
	OLED_Init();
	
//	SendString("Hello World!\r\n");
	
//	Serial_SendNumber(12345, 5);
	
//	char chinese[100] = "你好";
	UsartPrintf(USART1, " NUM=%d\r\n", 678);
	
	while(1)
	{
		if (Get_Serial_RxFlag() == 1)
		{
			RxData = Get_Serial_RxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1, 8, RxData, 2);
		}
	}
}	

