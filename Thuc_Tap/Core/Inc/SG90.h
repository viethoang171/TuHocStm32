#ifndef __SG90_H
#define __SG90_H
#include "main.h"
typedef struct
{
    TIM_HandleTypeDef *timer;
    uint8_t CHANNEL;
    uint8_t speed;
}Servo_Name;
void Servo_Init(Servo_Name *servo,TIM_HandleTypeDef *htim,uint8_t CHANNEL,uint8_t speed);
void Spin_Open_servo(Servo_Name *servo);
void Spin_Close_servo(Servo_Name *servo);
void Servo_Start(Servo_Name *servo);
#endif
