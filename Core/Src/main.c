/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 Troy Hans
  * This project is licensed under the MIT License.
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
#include <stdlib.h>
#include "ws2812.h"
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

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
void Set_Random_Flash(uint8_t brightness, uint16_t delay_ms);
void Set_Cylon(uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue, uint16_t delay_ms);
void Run_Animations_With_Button(void);
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
  WS2812_Init();
/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    //Run_Animations_With_Button();

    Set_Random_Flash(10, 50);     // brightness, delay_ms
    //Set_Cylon(35, 255, 0, 0, 60);      // Classic Red Cylon

// === Available Self-Contained Functions ===

    //Set_Rainbow(20, 500);                    // Static rainbow, brightness + delay

    //Set_Rotating_Rainbow(2, 10, 10);         // Rotating rainbow (speed, brightness, delay)
                                             // speed: 1 = very slow, 4 = normal, 8 = fast

    //Set_All_LEDs(255, 0, 0, 40, 500);       // Solid color (R, G, B, brightness, delay)

    //Test_Sequential_Colors(30, 45);         // Test pattern: Red → Green → Blue laps
                                             // (delay per LED, brightness)
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
void Set_Random_Flash(uint8_t brightness, uint16_t delay_ms)
{
    // Clear the ring first
    Set_All_LEDs(0, 0, 0, 0, 0);

    // Create random vibrant flashes
    for(int i = 0; i < MAX_LED; i++)
    {
        if (rand() % 100 < 40)        // ~40% chance per LED to flash
        {
            uint8_t intensity = 140 + (rand() % 116);   // 140-255 (brighter flashes)

            uint8_t r, g, b;

            // Better random color distribution for rich primary/secondary colors
            switch(rand() % 8)
            {
                case 0:  r = intensity; g = 0;         b = 0;         break; // Red
                case 1:  r = 0;         g = intensity; b = 0;         break; // Green
                case 2:  r = 0;         g = 0;         b = intensity; break; // Blue
                case 3:  r = intensity; g = intensity; b = 0;         break; // Yellow
                case 4:  r = intensity; g = 0;         b = intensity; break; // Magenta
                case 5:  r = 0;         g = intensity; b = intensity; break; // Cyan
                case 6:  r = intensity; g = intensity/2; b = 0;       break; // Orange
                default: r = intensity; g = intensity; b = intensity; break; // White (rare)
            }

            Set_LED(i, r, g, b);
        }
    }

    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(delay_ms);
}

void Set_Cylon(uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue, uint16_t delay_ms)
{
    static int8_t position = 0;
    static int8_t direction = 1;

    // Clear everything
    Set_All_LEDs(0, 0, 0, 0, 0);

    // Draw the scanning eye with trail
    for(int i = -2; i <= 2; i++)
    {
        int pos = position + i;
        if (pos >= 0 && pos < MAX_LED)
        {
            uint8_t intensity = (i == 0) ? 255 : (90 - (abs(i) * 30));
            if (intensity > 0)
            {
                // Apply color with intensity falloff for the trail
                Set_LED(pos, 
                        (uint8_t)(red * intensity / 255),
                        (uint8_t)(green * intensity / 255),
                        (uint8_t)(blue * intensity / 255));
            }
        }
    }

    // Move the eye
    position += direction;

    // Bounce at the ends
    if (position >= MAX_LED - 1 || position <= 0)
    {
        direction = -direction;
    }

    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(delay_ms);
}

/*=====================Button switch+++++++++++++++++++++++++++++++++++++++++++*/
void Run_Animations_With_Button(void)
{
    static uint8_t current_mode = 0;
    static uint32_t last_press_time = 0;
    static uint8_t button_state = 1;

    uint32_t now = HAL_GetTick();

    uint8_t current_button = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

    // Stronger edge detection
    if (current_button == 0 && button_state == 1 && (now - last_press_time > 20))
    {
        last_press_time = now;
        button_state = 0;

        current_mode = (current_mode + 1) % 4;
        
        // Optional: Force clear LEDs when switching modes
        Set_All_LEDs(0, 0, 0, 0, 0);
    }
    else if (current_button == 1)
    {
        button_state = 1;
    }

    // === Animation Modes ===
    switch(current_mode)
    {
        case 0:
            Set_Rotating_Rainbow(2, 10, 10);
            break;

        case 1:
            Test_Sequential_Colors(30, 45);     // stays at your desired speed
            break;

        case 2:
            Set_Random_Flash(10, 60);
            break;

        case 3:
            Set_Cylon(35, 255, 0, 0, 40);
            break;

        default:
            current_mode = 0;
            Set_Rotating_Rainbow(2, 10, 10);
            break;
    }
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