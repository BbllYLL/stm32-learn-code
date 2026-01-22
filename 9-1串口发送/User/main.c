#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SERIAL.h"

uint8_t KeyNum;

int main(void)
{
	OLED_Init();
	
	Serial_Init();
//	Serial_SendByte(0x41);
	
//	uint8_t MyArray[]={'B',0x43,0x44};
//	Serial_SendArray(MyArray,3);
//	Serial_SendString("HellowWorld!\r\n");
//	Serial_SendNumber(12345,5);
//	printf("Num=%d\r\n",666);
	
//	char String[100];
//	sprintf(String,"Num=%d\r\n",666);
//	Serial_SendString(String);
	
	Serial_Printf("你好");
	
	while(1)
	{

	}
}
