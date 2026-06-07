# STM32 WS2812 LED Ring

A clean, reusable WS2812 (NeoPixel) driver for STM32 using Timer PWM + DMA.

## Features

- Self-contained animation functions (brightness, send, and delay handled inside each function)
- Adjustable speed and brightness per animation
- Easy to copy into new projects as a library
- Built with STM32CubeMX + CMake + VS Code

## How to Install in a New Project

1. Copy ws2812.h to Core/Inc/
2. Copy ws2812.c to Core/Src/

3. Update CMakeLists.txt:
   Add Core/Src/ws2812.c to target_sources()
   Add Core/Inc to target_include_directories()

4. Update main.c:
   Add #include "ws2812.h" in USER CODE BEGIN Includes
   In USER CODE BEGIN PD:
   #define MAX_LED 16
   #define USE_BRIGHTNESS 1
   In USER CODE BEGIN 2:
   WS2812_Init();

## Main Loop Examples

while (1)
{
/_ USER CODE BEGIN 3 _/

    Set_Rotating_Rainbow(3, 20, 30);     // speed, brightness, delay
    // Set_Rainbow(25, 500);
    // Set_All_LEDs(255, 0, 0, 40, 500);
    // Test_Sequential_Colors(80, 45);

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

## Library Structure

The driver is split into ws2812.h + ws2812.c (in Core/ folder). A standalone copy is in the ws2812_driver/ folder for easy reuse.

## Hardware Configuration

Requires a timer configured for ~800kHz PWM with DMA support and DMA interrupt enabled.

**Happy Hacking!** 🚀
