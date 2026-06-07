# WS2812 Driver Library for STM32

A clean, reusable WS2812 (NeoPixel) driver for STM32 using Timer + DMA.

Author: Troy Hans
License: MIT

## Files

- ws2812.h
- ws2812.c

## How to Install in a New Project

1. Copy ws2812.h to Core/Inc/
2. Copy ws2812.c to Core/Src/

3. Update CMakeLists.txt:
   Add Core/Src/ws2812.c to target_sources()
   Add Core/Inc to target_include_directories()

4. Update main.c:
   Add #include "ws2812.h" in USER CODE BEGIN Includes
   In USER CODE BEGIN 2:
   WS2812_Init();

## Main Loop Examples

while (1)
{
/_ USER CODE BEGIN 3 _/

    // Set_Rotating_Rainbow(3, 20, 30);     // speed, brightness, delay
    // Set_Rainbow(25, 500);                 // brightness, delay
    // Set_All_LEDs(255, 0, 0, 40, 500);    // r,g,b, brightness, delay
    // Test_Sequential_Colors(80, 45);       // delay, brightness

    /* USER CODE END 3 */

}

## Function Reference

Set_Rotating_Rainbow(speed, brightness, delay_ms) - Rotating rainbow. Speed: 1=very slow, 4=normal, 8=fast
Set_Rainbow(brightness, delay_ms) - Static full rainbow
Set_All_LEDs(r, g, b, brightness, delay_ms) - Solid single color
Test_Sequential_Colors(delay_ms, brightness) - One LED at a time: Red → Green → Blue

## Parameter Guide

- speed: 1 (very slow) to 8+ (fast)
- brightness: 1 to 45 (15-30 recommended)
- delay_ms: 10 to 500 (30 is good for smooth animation)

## Important Configuration Note

MAX_LED and USE_BRIGHTNESS are defined in ws2812.h. Change them there when using in a new project.

## Required Hardware Configuration

The driver requires a timer configured for ~800kHz PWM output with DMA support and DMA interrupt enabled.
From the project configuration:

- Timer: TIM1 Channel 1 in PWM Generation mode
- DMA: DMA2 Stream 1, Memory To Peripheral, Normal mode, Memory Increment enabled, Half Word data width
- DMA Interrupt: DMA2 stream1 global interrupt enabled

This driver assumes the above Timer + DMA setup is configured in CubeMX.

To reuse: Copy the two files, update CMakeLists.txt and main.c as shown, and change MAX_LED / USE_BRIGHTNESS in ws2812.h as needed.
