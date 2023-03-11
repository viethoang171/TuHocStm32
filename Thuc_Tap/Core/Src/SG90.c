#include "SG90.h"
#include "main.h"
void Servo_Init(Servo_Name *servo,TIM_HandleTypeDef *htim_in,uint8_t CHANNEL, uint8_t speed)
{
    servo->timer=htim_in;
    servo->CHANNEL=CHANNEL;
    servo->speed=speed;
}
void Servo_Start(Servo_Name *servo)
{
	HAL_TIM_PWM_Start(servo->timer,servo->CHANNEL);
}
void Spin_Open_servo(Servo_Name *servo)
{
    for(uint8_t duty=10;duty<=100;duty+=servo->speed)
    {
        __HAL_TIM_SET_COMPARE(servo->timer,servo->CHANNEL,duty);
        HAL_Delay(100);
    }
}
void Spin_Close_servo(Servo_Name *servo)
{
	for(uint8_t duty=100;duty>=10;duty-=servo->speed)
	{
		__HAL_TIM_SET_COMPARE(servo->timer,servo->CHANNEL,duty);
		HAL_Delay(100);
	}
}
