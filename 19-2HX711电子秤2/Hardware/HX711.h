#ifndef __HX711_H
#define __HX711_H


#define MEDIAN_LEN       5  			              //中值滤波的滤波长度,一般取奇数
#define MEDIAN           3  			              //中值在滤波数组中的位置

extern uint32_t buffer[];
extern int medleng;
extern uint32_t num,weight;
extern uint32_t pi_weight;


void HX711_GPIO_Init(void);
void HX711_W_PD_SCK(uint8_t BitValue);
void HX711_W_DOUT(uint8_t BitValue);
void HX711_Reset(void);
uint8_t HX711_R_DOUT(void);
uint32_t HX711_ReadData(void);
uint32_t Get_Pi(void);
uint32_t Get_Weight(uint32_t pi_weight);
void Show_weight(void);

#endif
