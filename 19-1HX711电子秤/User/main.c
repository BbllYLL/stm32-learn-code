#include "stm32f10x.h"           
#include <string.h>
#include <stdio.h>
#include "Delay.h"
#include "OLED.h"
#include "HX711.h"
#include "Servo.h"
#include "Timer.h"
#include "Serial.h"
#include "esp8266.h"
#include "onenet.h"

float Angle;
uint32_t My_Weight;

char PUB_BUF[256];

int main(void)
{
	const char *devSubTopics[] = {"/liwenhao/sub"};
	const char devPubTopics[] = "/liwenhao/pub";

	unsigned char *dataPtr = NULL;
	
	OLED_Init();
	Servo_Init();
	HX711_GPIO_Init();
	TIM2_Int_Init(3000, 7200);
	Usart1_Init(115200);
	Usart2_Init(115200);
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
	ESP8266_Init();
	while(OneNet_DevLink()) Delay_ms(500);			//接入OneNET
	
	OneNet_Subscribe(devSubTopics, 1);
	
	OLED_ShowString(1, 1, "Wight:    g");
	
	pi_weight=Get_Pi(); //获取一次皮重
	
	Servo_SetAngle(0);
	
	while(1)
	{
		My_Weight = Show_weight();
		
		if(My_Weight < 100)
		{
			OLED_Clear();
			setFEED();
			sprintf(PUB_BUF,"{\"Weight\":%d}", My_Weight);
			OneNet_Publish(devPubTopics, PUB_BUF);
		}
		else
		{
			sprintf(PUB_BUF,"{\"Weight\":%d}", My_Weight);
			
			OneNet_Publish(devPubTopics, PUB_BUF);
			
			ESP8266_Clear();
			

		}
		
		dataPtr = ESP8266_GetIPD(4);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		Delay_ms(10);
	}
}
