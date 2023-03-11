#include "DS3231.h"
static void I2C_Write_Time(DS3231_Name *DS3231)
{
    HAL_I2C_Mem_Write(DS3231->I2C,DS3231_ADDRESS,0,I2C_MEMADD_SIZE_8BIT,DS3231->Tx_Time_Buff,3,1000);
}
static void I2C_Write_Date(DS3231_Name *DS3231)
{
	HAL_I2C_Mem_Write(DS3231->I2C,DS3231_ADDRESS,3,I2C_MEMADD_SIZE_8BIT,DS3231->Tx_Date_Buff,4,1000);
}
static void I2C_Read_Time(DS3231_Name *DS3231)
{
    HAL_I2C_Mem_Read(DS3231->I2C,DS3231_ADDRESS,0,I2C_MEMADD_SIZE_8BIT,DS3231->Rx_Time_Buff,3,1000);
}
static void I2C_Read_Date(DS3231_Name *DS3231)
{
    HAL_I2C_Mem_Read(DS3231->I2C,DS3231_ADDRESS,3,I2C_MEMADD_SIZE_8BIT,DS3231->Rx_Date_Buff,4,1000);
}
static uint8_t BCD2DEC(uint8_t data)
{
    return (data>>4)*10+(data&0x0f);
}
static uint8_t DEC2BCD(uint8_t data)
{
    return (data/10)<<4|(data%10);
}
//high level
void DS3231_Init(DS3231_Name *DS3231,I2C_HandleTypeDef *I2C_In)
{
    DS3231->I2C=I2C_In;
}
void DS3231_Set_Time(DS3231_Name *DS3231,uint8_t hours,uint8_t min, uint8_t sec)
{
    DS3231->Tx_Time_Buff[0]=DEC2BCD(sec);
    DS3231->Tx_Time_Buff[1]=DEC2BCD(min);
    DS3231->Tx_Time_Buff[2]=DEC2BCD(hours);
    
    I2C_Write_Time(DS3231);
}
void DS3231_Set_Date(DS3231_Name *DS3231,uint8_t day,uint8_t date,uint8_t month,uint8_t year)
{
	  DS3231->Tx_Date_Buff[0]=DEC2BCD(day);
    DS3231->Tx_Date_Buff[1]=DEC2BCD(date);
    DS3231->Tx_Date_Buff[2]=DEC2BCD(month);
    DS3231->Tx_Date_Buff[3]=DEC2BCD(year);
	  I2C_Write_Date(DS3231);
}
void DS3231_Get_Time(DS3231_Name *DS3231)
{
    I2C_Read_Time(DS3231);
    DS3231->sec=BCD2DEC(DS3231->Rx_Time_Buff[0]);
    DS3231->min=BCD2DEC(DS3231->Rx_Time_Buff[1]);
    DS3231->hours=BCD2DEC(DS3231->Rx_Time_Buff[2]);
    
}
void DS3231_Get_Date(DS3231_Name *DS3231)
{
	  I2C_Read_Date(DS3231);
	  DS3231->day=BCD2DEC(DS3231->Rx_Date_Buff[0]);
    DS3231->date=BCD2DEC(DS3231->Rx_Date_Buff[1]);
    DS3231->month=BCD2DEC(DS3231->Rx_Date_Buff[2]);
    DS3231->year=BCD2DEC(DS3231->Rx_Date_Buff[3]);
}
