#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include <stdio.h>
#include "HX711.h"


int main(void)
{
	OLED_Init();
	Serial_Init();
	HX711_GPIO_Init();    //初始化HX711
	OLED_ShowChinese(1,1,4);
	OLED_ShowChinese(1,2,5);
  OLED_ShowString(1, 10, "g");
	OLED_ShowString(1, 5, ":");
	pi_weight=Get_Pi();  //获取一次皮重
	
	while (1)
	{
     Show_weight();    //获取并显示重量
		 
	}
}
