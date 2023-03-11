#include "DHT11.h"
#include "delay_timer.h"
static void DHT_DelayInit(DHT_Name *DHT)
{
	DELAY_TIM_Init(DHT->timer);
}
static void DHT_DelayUs(DHT_Name *DHT,uint16_t time)
{
	DELAY_TIM_Us(DHT->timer,time);
}
static void DHT_SetPinOut(DHT_Name *DHT)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin=DHT->Pin;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DHT->Port,&GPIO_InitStruct);
}
static void DHT_SetPinIn(DHT_Name *DHT)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	GPIO_InitStruct.Pin=DHT->Pin;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(DHT->Port,&GPIO_InitStruct);
}
static void DHT_WritePin(DHT_Name *DHT,uint8_t value)
{
	HAL_GPIO_WritePin(DHT->Port,DHT->Pin,value);
}
static uint8_t DHT_ReadPin(DHT_Name *DHT)
{
	uint8_t value;
	value =HAL_GPIO_ReadPin(DHT->Port,DHT->Pin);
	return value;
}
//middleware level layer
static uint8_t DHT_Start(DHT_Name *DHT)
{
	uint8_t response=0;
	DHT_SetPinOut(DHT);
	DHT_WritePin(DHT,0);
	DHT_DelayUs(DHT,DHT11_STARTTIME);
	DHT_SetPinIn(DHT);
	DHT_DelayUs(DHT,40);
	if(!DHT_ReadPin(DHT))
	{
		DHT_DelayUs(DHT,40);
		if(DHT_ReadPin(DHT))
			response=1;
		else
			response=0;
	}
	while(DHT_ReadPin(DHT)){};
		return response;
}
static uint8_t DHT_Read(DHT_Name *DHT)
{
	uint8_t value=0;
	DHT_SetPinIn(DHT);
	for(int i=0;i<8;i++)
	{
		while(!DHT_ReadPin(DHT)){};
		DHT_DelayUs(DHT,40);
		if(!DHT_ReadPin(DHT))
			value&=~(1<<(7-i));
		else value |=1<<(7-i);
		while(DHT_ReadPin(DHT)){};
	}
	return value;
}
// high level layer
void DHT_Init(DHT_Name *DHT,TIM_HandleTypeDef *timer,GPIO_TypeDef *DH_Port,uint16_t DH_Pin)
{
	DHT->Port=DH_Port;
	DHT->Pin=DH_Pin;
	DHT->timer=timer;
	DHT_DelayInit(DHT);
}
uint8_t DHT_ReadTempHum(DHT_Name*DHT)
{
	uint8_t temp1,temp2,rh1,rh2;
	uint16_t temp,humi;
	uint8_t sum=0;
	DHT_Start(DHT);
	rh1=DHT_Read(DHT);
	rh2=DHT_Read(DHT);
	temp1=DHT_Read(DHT);
	temp2=DHT_Read(DHT);
	sum=DHT_Read(DHT);
	temp=(temp1<<8)|temp2;
	humi=(rh1<<8)|rh2;
	DHT->temp=(float)(temp/10.0);
	DHT->hum=(float)(humi/10.0);
	return sum;
}
