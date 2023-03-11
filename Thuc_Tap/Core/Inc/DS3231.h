#ifndef __DS3231_H
#define __DS3231_H
#include "main.h"
#define DS3231_ADDRESS 0x68<<1
typedef struct
{
    I2C_HandleTypeDef *I2C;
    uint8_t Tx_Time_Buff[3];
    uint8_t Tx_Date_Buff[4];
    uint8_t Rx_Time_Buff[3];
	  uint8_t Rx_Date_Buff[4];
    uint8_t hours;
    uint8_t min;
    uint8_t sec;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
}DS3231_Name;
void DS3231_Init(DS3231_Name *DS3231,I2C_HandleTypeDef *I2C_In);
void DS3231_Set_Time(DS3231_Name *DS3231,uint8_t hours,uint8_t min,uint8_t sec);
void DS3231_Set_Date(DS3231_Name *DS3231,uint8_t day,uint8_t date,uint8_t month,uint8_t year);
void DS3231_Get_Time(DS3231_Name *DS3231);
void DS3231_Get_Date(DS3231_Name *DS3231);
#endif
