#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "DHT11.h"

uint8_t humi_int, humi_deci, temp_int, temp_deci;

int main(void)
{
	OLED_Init();
	
	DHT11_Init();
	


	
	while(1)
	{
		DHT11_ReadHumiTempData(&humi_int, &humi_deci, &temp_int, &temp_deci);
		OLED_ShowNum(1,1,humi_int,2);
		OLED_ShowNum(1,4,humi_deci,2);
		OLED_ShowNum(2,1,temp_int,2);
		OLED_ShowNum(2,4,temp_deci,2);



		OLED_ShowString(3,1,"error");
			
	}
}
