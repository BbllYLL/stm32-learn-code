#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern char Serial_RxString[100];

void Serial_TxPacket_Get(char *TxPacket, uint16_t Length);
void Serial_RxPacket_Get(char *RxPacket, uint16_t Length);

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint8_t Lenght);
void Serial_SendString(uint8_t *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);

uint8_t Serial_GetRxFlag(void);	

void Serial_SendPacket(void);

#endif
