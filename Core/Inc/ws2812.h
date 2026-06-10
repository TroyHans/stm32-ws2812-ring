#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>

#define MAX_LED        28      
#define USE_BRIGHTNESS 1

// These are defined in main.c
extern uint8_t LED_Data[][4];
extern uint8_t LED_Mod[][4];

// Core driver functions
void WS2812_Init(void);
void Set_LED(int LEDnum, int Red, int Green, int Blue);
void Set_Brightness(int brightness);
void WS2812_Send(void);

// Self-contained Animation Functions
void Set_Rainbow(uint8_t brightness, uint16_t delay_ms);
void Set_Rotating_Rainbow_V2(uint8_t speed, uint8_t brightness, uint16_t delay_ms);
void Set_Rotating_Rainbow_V1(uint8_t speed, uint8_t brightness, uint16_t delay_ms);
void Set_All_LEDs(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, uint16_t delay_ms);
void Test_Sequential_Colors(uint16_t delay_ms, uint8_t brightness);

#endif