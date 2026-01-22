#include "stm32f10x.h"                  // Device header
#include "OLED.h"

uint8_t OLED_DisplayBuf[8][128];

void I2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)BitValue);
}

void I2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)BitValue);
}

void OLED_GPIO_Init(void)
{
	uint32_t i, j;
	for (i = 0; i < 1000; i ++)
	{
		for (j = 0; j < 1000; j ++);
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_W_SCL(1);
	I2C_W_SDA(1);
}

void I2C_Start(void)
{
	I2C_W_SDA(1);	
	I2C_W_SCL(1);
	I2C_W_SDA(0);
	I2C_W_SCL(0);
}

void I2C_Stop(void)
{
	I2C_W_SDA(0);
	I2C_W_SCL(1);
	I2C_W_SDA(1);
}

void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0; i < 8; i ++)
	{
		I2C_W_SDA(!!(Byte & (0x80 >> i)));
		I2C_W_SCL(1);
		I2C_W_SCL(0);
	}
	I2C_W_SCL(1);
	I2C_W_SCL(0);
}   

void OLED_WriteCommand(uint8_t Command)
{
	I2C_Start();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_SendByte(0x00);    //D/C位为0
	OLED_I2C_SendByte(Command);
	I2C_Stop();
}

void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
	uint8_t i;
	
	I2C_Start();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_SendByte(0x40);    //D/C位为1
	for(i = 0; i < Count; i ++)
	{
		OLED_I2C_SendByte(Data[i]);
	}
	I2C_Stop();
}

void OLED_Init(void)
{
	OLED_GPIO_Init();
	
		/*写入一系列的命令，对OLED进行初始化配置*/
	OLED_WriteCommand(0xAE);	//设置显示开启/关闭，0xAE关闭，0xAF开启
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);	//0x00~0xFF
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);	//0x0E~0x3F
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);	//0x00~0x7F
	
	OLED_WriteCommand(0x40);	//设置显示开始行，0x40~0x7F
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常，0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常，0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度
	OLED_WriteCommand(0xCF);	//0x00~0xFF

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/反色显示，0xA6正常，0xA7反色

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
	
	OLED_Clear();				//清空显存数组
	OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
}

void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	OLED_WriteCommand(0x00 | (X & 0x0F));
	OLED_WriteCommand(0x10 | (X & 0xF0) >> 4);
	OLED_WriteCommand(0xB0 | Page);
}

void OLED_Update(void)
{
	uint8_t j;
	
	for(j = 0; j < 8; j ++)
	{
		OLED_SetCursor(j, 0);
		OLED_WriteData(OLED_DisplayBuf[j], 128);

	}
}

void OLED_Clear(void)
{
	uint8_t j, i;
	for(j = 0; j < 8; j ++)
	{
		for(i = 0; i < 128; i ++)
		{
			OLED_DisplayBuf[j][i] = 0x00;
		}
	}
}



