#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED1  PAout(1)
#define LED2  PAout(2)


void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Turn(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED2_Turn(void);

#endif
