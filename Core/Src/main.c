/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  *
  * PD0 - LED0
  * PD1 - LED1
  * PD2 - LED2
  * PD3 - LED3
  *
  * PD4 - Col 0
  * PD5 - Col 1
  * PD6 - Col 2
  * PD7 - Col 3
  *
  * PD8 - ROW0
  * PD9 - ROW1
  * PDE - ROW2
  * PDF - ROW3
  *
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

// Inputs
#define ROW0 GPIO_PIN_8 // PD8
#define ROW1 GPIO_PIN_9 // PD9
#define ROW2 GPIO_PIN_14 // PDE
#define ROW3 GPIO_PIN_15 // PDF

// Outputs
#define COL0 GPIO_PIN_4 // PD4
#define COL1 GPIO_PIN_5 // PD5
#define COL2 GPIO_PIN_6 // PD6
#define COL3 GPIO_PIN_7 // PD7

// Outputs
#define LED0 GPIO_PIN_0 // PD0
#define LED1 GPIO_PIN_1 // PD1
#define LED2 GPIO_PIN_2 // PD2
#define LED3 GPIO_PIN_3 // PD3

// Debounce parameters
#define DEBOUNCE_ATTEMPTS 50
#define DEBOUNCE_COUNT 30
#define DEBOUNCE_INTERVAL 1000


//

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void InitGPIO(void);
int ReadKeypad(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  /* Configure the system clock */
  SystemClock_Config();

  InitGPIO();

  while (1)
  {
	  ReadKeypad();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void InitGPIO(void)
{
	// Initialize Port D GPIO
	RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIODEN);

	// Reset Port D pins
	GPIOD->MODER = 0;

	// Rows set as inputs

	// Set column pins and LEDs as outputs
	GPIOD->MODER |= (
			GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0 |
			GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0
	);

	// Clear output types for columns and LEDs (outputs)
	GPIOD->OTYPER  &= ~(
			GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3 |
			GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7
	);

	// Clear pull ups / downs for columns and LEDs (outputs)
	GPIOD->PUPDR   &= ~(
			GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3 |
			GPIO_PUPDR_PUPD4 | GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7
	);

	// Set pull downs for rows
	GPIOD->PUPDR |= (GPIO_PUPDR_PUPD8_1 | GPIO_PUPDR_PUPD9_1 | GPIO_PUPDR_PUPD14_1 | GPIO_PUPDR_PUPD15_1);

	// Set output speeds for columns and LEDs
	GPIOD->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED0_Pos) |
	                    (3 << GPIO_OSPEEDR_OSPEED1_Pos) |
						(3 << GPIO_OSPEEDR_OSPEED2_Pos) |
						(3 << GPIO_OSPEEDR_OSPEED3_Pos) |
						(3 << GPIO_OSPEEDR_OSPEED4_Pos) |
						(3 << GPIO_OSPEEDR_OSPEED5_Pos) |
						(3 << GPIO_OSPEEDR_OSPEED6_Pos) |
						(3 << GPIO_OSPEEDR_OSPEED7_Pos)
	);

	// Reset all outputs
	GPIOD->ODR = 0;
}

int ReadKeypad(void)
{
	int key = -1;
	// Loop through all rows
	for(uint8_t col = 0; col < 4; col++)
	{
		// Loop through all columns
		GPIOD->BSRR = (0b1 << col) << 4 | (~(0b1 << col) << 4) << 16;
		for(uint8_t row = 0; row < 4; row++)
		{
			// due to non-consecutive ports, we have to calculate the pin position from the given row
			int rowPos;
			if(row > 1)
			{
				rowPos = row % 2 + 14;
			}
			else
			{
				rowPos = row % 2 + 8;
			}

			uint16_t input = GPIOD->IDR & (0b1 << rowPos);
			// if this specific row and this specific column is pressed
			if (input)
			{
				// debounce it

				// Check for 30 highs in a row
				int highCount = 0;
				for(int i = 0; i < DEBOUNCE_ATTEMPTS; i++)
				{
					// Delay
					for(int j = 0; j < DEBOUNCE_INTERVAL; j++);
					input = GPIOD->IDR & (0b1 << rowPos);

					if(input)
						highCount++;
					else
						highCount = 0;

					// If we have 30 highs in a row
					if(highCount >= DEBOUNCE_COUNT)
						break;
				}

				// Map to number

			}
		}
	}
	return key;
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
