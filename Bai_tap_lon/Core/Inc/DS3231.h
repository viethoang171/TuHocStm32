#ifndef __DS3231_H
#define __DS3231_H

#include "main.h"

#define DS3231_ADDRESS 0x68 << 1

typedef struct
{
    I2C_HandleTypeDef *I2C;
    uint8_t TxTimeBuff[3];
    uint8_t RxTimeBuff[3];
    uint8_t TxDateBuff[4];
    uint8_t RxDateBuff[4];
    uint8_t Hours;
    uint8_t Min;
    uint8_t Sec;
    uint8_t Date;
    uint8_t Day;
    uint8_t Month;
    uint8_t Year;
} DS3231_Name;

void DS3231_Init(DS3231_Name *DS3231, I2C_HandleTypeDef *I2C_In);
void DS3231_SetTime(DS3231_Name *DS3231, uint8_t Hour, uint8_t Min, uint8_t Sec);
void DS3231_GetTime(DS3231_Name *DS3231);
void DS3231_SetDate(DS3231_Name *DS3231, uint8_t Day, uint8_t Date, uint8_t Month, uint8_t Year);
void DS3231_GetDate(DS3231_Name *DS3231);
#endif
