#ifndef __OLED_H
#define __OLED_H

void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t *Data, uint8_t Count);
void OLED_Init(void);
void OLED_SetCursor(uint8_t Page, uint8_t X);
void OLED_Update(void);
void OLED_Clear(void);


#endif
