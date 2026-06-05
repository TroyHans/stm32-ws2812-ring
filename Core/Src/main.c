/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_LED 16
#define USE_BRIGHTNESS 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/**********WS2812 Vaiables*********************** */

uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];  // for brightness
uint8_t datasentflag=0;
uint32_t pwmData[(24*MAX_LED)+50];
/**********END WS2812 Variables****************** */

/*****************Display Variables************************* */
int rainbow_offset = 0;   // ← Add this for rotation

/*******************END Display Variables********************** */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/********************WS2812 Function Prototypes*************************** */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void WS2812_Send (void);
void Set_LED (uint32_t LEDnum, uint8_t Red, uint8_t Green, uint8_t Blue);
void Set_Brightness (uint8_t brightness);
/********************END WS2812 Function Prototypes************************** */

/*********************Display Function Protypes******************************* */
void Set_Rainbow(void);
void Set_Rotating_Rainbow(void);
/********************ENS Display Function Prototypes*************************** */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
/* USER CODE BEGIN 2 */
  // Create a nice rainbow across all 16 LEDs
  Set_Rainbow();
  // Set a fixed brightness level once (0 to 45)
  //Set_Brightness(30);   // Change this number to adjust brightness (30-40 recommended)
/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    Set_Rotating_Rainbow();
    Set_Brightness(45);
    WS2812_Send();
    HAL_Delay(30);          // Fast refresh rate for smooth animation
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/*-----------------Display functions------------------------------------*/

/***************************Static Rainbow****************************** */
void Set_Rainbow(void)
{
    for(uint32_t i = 0; i < MAX_LED; i++)
    {
        uint8_t r, g, b;
        
        // Calculate hue (0 to 359)
        uint32_t hue = (i * 360u) / MAX_LED;

        uint8_t sector = hue / 60u;
        uint8_t remainder = hue % 60u;
        
        uint8_t q = (255u * (60u - remainder)) / 60u;
        uint8_t t = (255u * remainder) / 60u;

        switch(sector)
        {
            case 0:  r = 255; g = t;   b = 0;   break;
            case 1:  r = q;   g = 255; b = 0;   break;
            case 2:  r = 0;   g = 255; b = t;   break;
            case 3:  r = 0;   g = q;   b = 255; break;
            case 4:  r = t;   g = 0;   b = 255; break;
            default: r = 255; g = 0;   b = q;   break;
        }

        Set_LED(i, r, g, b);
    }
}

/************************END Static Rainbow******************************************** */

/****************************Rotating Rainbow***************************************** */
void Set_Rotating_Rainbow(void)
{
    static uint8_t offset = 0;

    for(uint32_t i = 0; i < MAX_LED; i++)
    {
        uint8_t pos = (i * 16 + offset) % 256;   // 256-step color wheel for smoothness
        uint8_t r, g, b;

        if(pos < 85)
        {
            r = pos * 3;
            g = 255 - pos * 3;
            b = 0;
        }
        else if(pos < 170)
        {
            pos -= 85;
            r = 255 - pos * 3;
            g = 0;
            b = pos * 3;
        }
        else
        {
            pos -= 170;
            r = 0;
            g = pos * 3;
            b = 255 - pos * 3;
        }

        Set_LED(i, r, g, b);
    }

    offset += 1;        // ← Change this number to control speed (2=slow, 8=fast)
}
/***************************END Rotating Rainbow****************************************** */
/*---------------------------END Display Functions------------------------------------------*/

/************************WS2812 Functions*************************************************** */
void Set_LED (uint32_t LEDnum, uint8_t Red, uint8_t Green, uint8_t Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}


void Set_Brightness (uint8_t brightness)  // brightness: 0 to 45
{
#if USE_BRIGHTNESS

    if (brightness > 45) brightness = 45;
    if (brightness < 0)  brightness = 0;

    float scale = brightness / 45.0f;   // 0.0 to 1.0

    for (uint32_t i = 0; i < MAX_LED; i++)
    {
        LED_Mod[i][0] = LED_Data[i][0];           // LED number (unused)
        LED_Mod[i][1] = LED_Data[i][1] * scale;   // Green
        LED_Mod[i][2] = LED_Data[i][2] * scale;   // Red
        LED_Mod[i][3] = LED_Data[i][3] * scale;   // Blue
    }

#endif
}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
	datasentflag = 1;
}


void WS2812_Send (void)
{
	uint32_t indx=0;
	uint32_t color;


	for (uint32_t i= 0; i<MAX_LED; i++)
	{
#if USE_BRIGHTNESS
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | (LED_Mod[i][3]));
#else
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));
#endif

		for (uint8_t i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 60;  // 2/3 of 90
			}

			else pwmData[indx] = 30;  // 1/3 of 90

			indx++;
		}

	}

	for (uint8_t i=0; i<50; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);
	while (!datasentflag){};
	datasentflag = 0;
}
/*************************END WS2812 Functions*********************************************** */


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
#ifdef USE_FULL_ASSERT
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
