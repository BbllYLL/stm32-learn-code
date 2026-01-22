#include "stm32f10x.h"                  // Device header
#include "HX711.h"
#include "Delay.h"
	
float	BiaoDing=384.7;       //标定系数,根据自己的传感器调节
uint32_t pi_weight,weight;
uint32_t buffer[MEDIAN_LEN];   	        //中值滤波的数据缓存
int medleng = 0;          	            
uint32_t   num;					                  //数据对比大小中间变量
uint8_t Pi_flag=0;

void HX711_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //SCK时钟线推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //DT数据线浮空输入     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void HX711_W_PD_SCK(uint8_t BitValue)                  //写SCK时钟线
{	
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
}

void HX711_W_DOUT(uint8_t BitValue)                    //写数据线，开始时，需要拉高DOUT电平
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
}

uint8_t HX711_R_DOUT(void)                             //读数据
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
}


void HX711_Reset(void)                                 // 传感器复位，去皮
{
	HX711_W_PD_SCK(1);                                   //拉高，延时60us
	Delay_us(60);
}

uint32_t HX711_ReadData(void)                          //读数据
{
	uint8_t i;
	uint32_t value = 0;    						//接收HX711采集的AD值
	
	HX711_W_DOUT(1); 									//初始DOUT为高电平
	HX711_W_PD_SCK(0); 							  //初始PD_SCK为低电平
	Delay_us(1);
	HX711_R_DOUT();                   //DOUT转换为输入
	
	while(HX711_R_DOUT());           //等待DOUT引脚变为高电平
	Delay_us(1);
	for(i=0;i<24;i++)                 //按照HX711时序接收数据
	{
		HX711_W_PD_SCK(1);              // PD_SCK置高，发送脉冲 
		value = value << 1;              //下降沿来时变量左移一位，右侧补零，
		Delay_us(1);
		HX711_W_PD_SCK(0);               // PD_SCK置低
		if(HX711_R_DOUT())               //读取一位数据，如果高电平，加一，低电平不变。
			value++;
	  Delay_us(1);
	}
	HX711_W_PD_SCK(1);               //最后一个脉冲，增益为128
	value = value^0x800000;          //第25个脉冲转换数据，24位AD转换，最高位为符号位，其余为有效位
	Delay_us(1);
	HX711_W_PD_SCK(0);
	Delay_us(1);
	return value;
}


uint32_t Get_Pi(void)                                  //皮重函数
{                                               //中值滤波参数，冒泡排序法
	uint32_t   buffer[MEDIAN_LEN];                //储存数组
	uint32_t HX711_dat,pi_weight,num;           				
	uint8_t i,medleng = 0;
	for(i=0;i<MEDIAN_LEN;i++)
	{
		HX711_dat=HX711_ReadData();	        	   
		if(medleng == 0)                           //第1个元素,直接放入,不需要排序
		{ 
			buffer[0] = HX711_dat;
			medleng = 1; 
		}
		else                            	           //中值滤波，取中间值，冒泡排序法
		{  
			for(i = 0; i < medleng; i ++)  
			{
				if( buffer[i] > HX711_dat) 	          
				{ 
					num = HX711_dat; HX711_dat = buffer[i]; buffer[i] = num;
				}
			}
			buffer[medleng] = HX711_dat;          	
			medleng++;
		}		
		if(medleng >= MEDIAN_LEN) 		           
		{
			HX711_dat = buffer[MEDIAN];	            //取排序好的中间值，
			medleng = 0; 
		}
	}
	pi_weight=HX711_dat;      
	return pi_weight;
}

uint32_t Get_Weight(uint32_t pi_weight)	               //称重函数
{
	uint32_t get_weight;
	uint32_t weight;
	
	get_weight=HX711_ReadData();		            //HX711数据采集函数
	if(get_weight>pi_weight)
	{
		get_weight=HX711_ReadData();			                  //重新采集HX711数据					 														   
		weight=(uint32_t)(((float)(get_weight-pi_weight))/BiaoDing);//修改标定系数，根据传感器不同
		if(weight==11164458)weight=0;     //干扰值校准为0
	}
	else 				
		weight=0;
	return weight;
}


uint32_t Show_weight(void)
{
	  weight=Get_Weight(pi_weight);
		if(medleng == 0)                         //中值滤波，取中间值，冒泡排序法
		{ 
			buffer[0] = weight; medleng = 1; 
		}
		else                            	       
		{  
			for(int i = 0; i < medleng; i ++)  
			{
				if( buffer[i] > weight) 	           
				{ 
					num = weight; weight = buffer[i]; buffer[i] = num;
				}
			}
			buffer[medleng] = weight; 	            
			medleng++;
		}		
		if(medleng >= MEDIAN_LEN) 		             
		{
			weight = buffer[MEDIAN];	                //取中间值
			medleng = 0; 
		}
		return weight;

}

