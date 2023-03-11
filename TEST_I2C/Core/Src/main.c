/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
#define DS3231_ADDRESS (0x68<<1)
#define DS3231_REG_TIME (0x00)
#define DS3231_REG_ALARM_1 (0x07)
#define DS3231_REG_ALARM_2 (0x0B)
#define DS3231_REG_CONTROL (0x0E)
#define DS3231_REG_STATUS (0x0F)
#define DS3231_REG_TEMPERATURE (0x11)
typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t I2C_Buffer[8];
}DS3231_t;
DS3231_t DS3231;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
uint8_t RTC_BCD2DEC(uint8_t c);
uint8_t RTC_DEC2BCD(uint8_t c);
void I2C_WriteBuffer(I2C_HandleTypeDef hi,uint8_t DEV_ADDR,uint8_t sizebuf);
void I2C_ReadBuffer(I2C_HandleTypeDef hi,uint8_t DEV_ADDR,uint8_t sizebuf);
void RTC_GetTime();
void RTC_SetTime(uint8_t hour,uint8_t min,uint16_t sec,uint8_t date,uint8_t month,uint8_t year)
{
	DS3231.I2C_Buffer[0]=0x00;
	DS3231.I2C_Buffer[1]=RTC_DEC2BCD(sec);
	DS3231.I2C_Buffer[2]=RTC_DEC2BCD(min);
	DS3231.I2C_Buffer[3]=RTC_DEC2BCD(hour);
	
	DS3231.I2C_Buffer[4]=RTC_DEC2BCD(date);
	DS3231.I2C_Buffer[5]=RTC_DEC2BCD(month);
	DS3231.I2C_Buffer[6]=RTC_DEC2BCD(year);
	
	I2C_WriteBuffer(hi2c1,(uint16_t)DS3231_ADDRESS,8);
	HAL_Delay(100);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t RTC_BCD2DEC(uint8_t c)
{
	return (c>>4)*10+(c&0x0f);
}
uint8_t RTC_DEC2BCD(uint8_t c)
{
	return (c/10)<<4|(c%10);
}
void I2C_WriteBuffer(I2C_HandleTypeDef hi,uint8_t DEV_ADDR,uint8_t sizebuf)
{
	while(HAL_I2C_Master_Transmit(&hi,(uint16_t) DEV_ADDR,(uint8_t *)&DS3231.I2C_Buffer,(uint16_t) sizebuf,(uint32_t) 1000))
	{
		if(HAL_I2C_GetError(&hi)!=HAL_I2C_ERROR_AF)
		{
			printf("Write Buffer error\r\n");
		}
	}
}
void I2C_ReadBuffer(I2C_HandleTypeDef hi,uint8_t DEV_ADDR,uint8_t sizebuf)
{
	while(HAL_I2C_Master_Receive(&hi,(uint16_t) DEV_ADDR,(uint8_t *)&DS3231.I2C_Buffer,(uint16_t) sizebuf,(uint32_t) 1000))
	{
		if(HAL_I2C_GetError(&hi)!=HAL_I2C_ERROR_AF)
		{
			printf("Read buffer error\r\n");
		}
	}
}
void RTC_GetTime()
{
	//Bat dau nhan qua trinh nhan du lieu tu thanh ghi 0x00
	DS3231.I2C_Buffer[0]=0x00;
	I2C_ReadBuffer(hi2c1,(uint16_t) DS3231_ADDRESS,1);
	while(HAL_I2C_GetState(&hi2c1)!=HAL_I2C_STATE_READY);
	I2C_ReadBuffer(hi2c1,(uint16_t) DS3231_ADDRESS,7);
	
	DS3231.day=RTC_BCD2DEC(DS3231.I2C_Buffer[3]);
	
	DS3231.date=RTC_BCD2DEC(DS3231.I2C_Buffer[4]);
	DS3231.month=RTC_BCD2DEC(DS3231.I2C_Buffer[5]);
	DS3231.year=RTC_BCD2DEC(DS3231.I2C_Buffer[6]);
	
	DS3231.sec=RTC_BCD2DEC(DS3231.I2C_Buffer[0]);
	DS3231.min=RTC_BCD2DEC(DS3231.I2C_Buffer[1]);
	DS3231.hour=RTC_BCD2DEC(DS3231.I2C_Buffer[2]);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	RTC_SetTime(15,49,14,2,12,22);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		RTC_GetTime();
		//printf("%d-%d-%d",DS3231.date,DS3231.month,DS3231.year);
		printf("%d:%d:%d\r\n",DS3231.hour,DS3231.min,DS3231.sec);
		HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__
/* with GCC/RAISONANCE, small printf( option LD Linker->Libraries->Small printf
set to 'Yes' call __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
	#endif /*__GNUC__*/
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	//place your implemantation of fputc here
	//write a character to the USART
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,100);
	return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
