#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define KEY0  PBin(0)
#define KEY1  PBin(1)

#define KEY0_PRES  1
#define KEY1_PRES  2

void Key_Init(void);
uint8_t KEY_Scan(uint8_t mode);

#endif
