#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "DHT11.h"

static void                           DHT11_GPIO_Config                       ( void );
static void                           DHT11_Mode_IPU                          ( void );
static void                           DHT11_Mode_Out_PP                       ( void );
static uint8_t                        DHT11_ReadByte                          ( void );

#define Delay_us() CPU_TS_Tmr_Delay_US(us)
#define Delay_ms()  CPU_TS_Tmr_Delay_MS(ms)

void DHT11_Init ( void )
{
	DHT11_GPIO_Config ();
	
	DHT11_Dout_1;               // 拉高GPIOB10
}


static void DHT11_GPIO_Config ( void )
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	DHT11_Dout_SCK_APBxClock_FUN ( DHT11_Dout_GPIO_CLK, ENABLE );	
  	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init ( DHT11_Dout_GPIO_PORT, &GPIO_InitStructure );		  
	
}


static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;

	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 

	  GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 
	
}


static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	

  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 	 
	
}


static uint8_t DHT11_ReadByte ( void )
{
	uint8_t i, temp=0;
	

	for(i=0;i<8;i++)    
	{	 
		while(DHT11_Dout_IN()==Bit_RESET);
		DHT11_DELAY_US(40); 	   	  

		if(DHT11_Dout_IN()==Bit_SET)/* x us后仍为高电平表示数据“1” */
		{
			while(DHT11_Dout_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	
	return temp;
	
}


uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{  
	DHT11_Mode_Out_PP();
	DHT11_Dout_0;
	DHT11_DELAY_MS(18);
	DHT11_Dout_1; 
	DHT11_DELAY_US(30);   
	DHT11_Mode_IPU();

	if(DHT11_Dout_IN()==Bit_RESET)     
	{
		while(DHT11_Dout_IN()==Bit_RESET);
		while(DHT11_Dout_IN()==Bit_SET);
  
		DHT11_Data->humi_int= DHT11_ReadByte();

		DHT11_Data->humi_deci= DHT11_ReadByte();

		DHT11_Data->temp_int= DHT11_ReadByte();

		DHT11_Data->temp_deci= DHT11_ReadByte();

		DHT11_Data->check_sum= DHT11_ReadByte();



		DHT11_Mode_Out_PP();

		DHT11_Dout_1;

		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return SUCCESS;
		else 
			return ERROR;
	}
	
	else
		return ERROR;
	
}
