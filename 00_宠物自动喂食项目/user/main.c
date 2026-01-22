#include "stm32f10x.h"
#include "onenet.h"
#include "esp8266.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "Servo.h"
#include "HX711.h"
#include <string.h>
#include <stdio.h>

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

int32_t My_Weight;

char PUB_BUF[256];

int main(void)
{	
	unsigned short timeCount = 0;	//发送间隔变量
	unsigned char *dataPtr = NULL;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置
	Delay_Init();									//systick初始化
	Usart1_Init(115200);							//串口1，打印信息用
	Usart2_Init(115200);							//串口2，驱动ESP8266用
	Servo_Init();
	HX711_GPIO_Init();
	OLED_Init();
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
	pi_weight=Get_Pi(); //获取一次皮重
	Servo_SetAngle(0);
	
	OLED_ShowString(3,1,"Networking");
	ESP8266_Init();					//初始化ESP8266
	OLED_ShowString(1,1,"                ");
	OLED_ShowString(1,1," Connected to");
	OLED_ShowString(2,1,"ONENET");
	UsartPrintf(USART_DEBUG, "Connect MQTTs Server...\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
	UsartPrintf(USART_DEBUG, "NET_OK\r\n");
	
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);
	OLED_Clear();
	
	OLED_ShowString(2,8 ,"  g");

	
	OneNET_Subscribe();
	
	while(1)
	{
		
		if(++timeCount >= 50)									//发送间隔0.5s
		{
			My_Weight = Show_weight();	
			OLED_ShowNum(2,5,My_Weight,3);//显示重量	
			sprintf(PUB_BUF,"{\"Weight\":%d}",My_Weight);
			UsartPrintf(USART_DEBUG,PUB_BUF);
			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//发送数据
//			ESP8266_SendMYData(My_Weight);
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		delay_ms(10);
	
	}

}
