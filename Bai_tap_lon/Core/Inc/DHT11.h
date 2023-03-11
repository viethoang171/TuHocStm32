#ifndef __DHT11_H
#define __DHT11_H
#include "main.h"
#define DHT11_STARTTIME 18000
#define DHT11 0X01
typedef struct
{
    TIM_HandleTypeDef *timer;
		uint16_t Pin;
		GPIO_TypeDef *Port;
		float temp;
		float hum;
}DHT_Name;
void DHT_Init(DHT_Name *DHT,TIM_HandleTypeDef *timer,GPIO_TypeDef *DH_Port,uint16_t DH_Pin);
uint8_t DHT_ReadTempHum(DHT_Name*DHT);
#endif
