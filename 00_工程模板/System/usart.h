#ifndef __usart_H
#define __usart_H

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_POW(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number,uint8_t Length);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);
uint8_t Get_Serial_RxFlag(void);
uint8_t Get_Serial_RxData(void);


#endif


