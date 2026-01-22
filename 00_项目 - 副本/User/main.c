#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdio.h>
#include "sys.h"
#include "Delay.h"
#include "MYEXTI.h"
#include "LED.h"
#include "OLED.h"
#include "BSP_DHT11.h"
#include "BH1750.h"
#include "usart.h"
#include "Timer.h"
#include "Beep.h"
#include "Key.h"
#include "esp8266.h"
#include "onenet.h"
#include "CORE_DELAY.H"


uint8_t Beep_alarmFree = 10;	   
uint8_t Beep_alarmFlag = 0;        //蜂鸣器报警标志位
uint8_t LED_alarmFlag = 0;         
uint8_t humi_int, humi_deci, temp_int, temp_deci;
uint16_t BH_int;
uint8_t BH_deci;

char PUB_BUF[256];

int main(void)
{
	const char *devSubTopics[] = {"/liwenhao/sub"};
	const char devPubTopics[] = "/liwenhao/pub";
	
	unsigned short timeCount = 0;
	unsigned char *dataPtr = NULL;
	
	TIM2_Int_Init(5000, 7200);
	TIM3_Int_Init(3000, 7200);
	LED_Init();
	Beep_Init();
	OLED_Init();
	DHT11_Init();
	BH1750_Init();
	Key_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);
	MYEXTI_Init();
	CPU_TS_TmrInit();
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
	ESP8266_Init();
	while(OneNet_DevLink()) Delay_ms(500);			//接入OneNET
	
	BEEP = 0;
	Delay_ms(2500);
	BEEP = 1;
	
	OneNet_Subscribe(devSubTopics, 1);
	
	while(1) 
	{
		if(timeCount % 20 == 0)    //发送间隔1秒  
		{
			LED_alarmFlag = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);
			bh_read(&BH_int, &BH_deci);
			DHT11_Read_TempAndHumidityData(&humi_int, &humi_deci, &temp_int, &temp_deci);
			UsartPrintf(USART1, "温度：%d.%d  湿度：%d.%d  光照强度：%d.%d\r\n", temp_int,temp_deci,humi_int,humi_deci, BH_int, BH_deci);

			if(Beep_alarmFree == 10)
			{
				if(humi_int < 80 && temp_int < 35  && BH_int < 10000)
				{
					Beep_alarmFlag =0;
				}
				else
				{
					Beep_alarmFlag =1;
				}			
			}
			if(Beep_alarmFree < 10) Beep_alarmFree ++;
		//		UsartPrintf(USART1, "Beep_alarmFree = %d\r\n", Beep_alarmFree);
		//		UsartPrintf(USART1, "Beep_alarmFlag = %d\r\n", Beep_alarmFlag);
		}
		
		if(++timeCount >= 100)									//发送间隔5s
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			
			sprintf(PUB_BUF, "{\"Temp\":%d.%d,\"Humi\":%d.%d,\"Light\":%d.%d,\"LED\":%d,\"Beep\":%d}",
				temp_int,temp_deci,humi_int,humi_deci, BH_int, BH_deci,!LED_alarmFlag,Beep_alarmFlag );
			
			OneNet_Publish(devPubTopics, PUB_BUF);
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(4);
		if(dataPtr != NULL) OneNet_RevPro(dataPtr);
		
		CPU_TS_Tmr_Delay_MS(10);
	}
}
