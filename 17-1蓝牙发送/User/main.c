#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Serial.h"
#include <string.h>

int main(void)
{
	OLED_Init();
	Serial_Init();
	LED_Init();
	
//	char RxPacket[4];

	
//	char TxPacket[4] = {0x01, 0x02, 0x03, 0x04};
//	Serial_TxPacket_Get(TxPacket, 4);
//	Serial_SendPacket();
	
//	OLED_ShowHexNum(1, 1, TxPacket[0], 2);
//	OLED_ShowHexNum(2, 1, TxPacket[1], 2);
//	OLED_ShowHexNum(3, 1, TxPacket[2], 2);
//	OLED_ShowHexNum(4, 1, TxPacket[3], 2);
	
	OLED_ShowString(1,1,"TxPacket");
	OLED_ShowString(3,1,"RxPacket");
	OLED_ShowString(4, 1, "LED1_OFF_OK");
	
	while(1)
	{

	}
}
