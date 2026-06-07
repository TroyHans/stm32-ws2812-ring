#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>

#define MAX_LED        16      // ← Change this number only here when reusing library
#define USE_BRIGHTNESS 1

// These are defined in main.c
extern uint8_t LED_Data[][4];
extern uint8_t LED_Mod[][4];

void WS2812_Init(void);
void Set_LED(int LEDnum, int Red, int Green, int Blue);
void Set_Brightness(int brightness);
void WS2812_Send(void);

void Set_Rainbow(void);
void Set_Rotating_Rainbow(uint8_t speed);
void Set_All_LEDs(uint8_t red, uint8_t green, uint8_t blue);
void Test_Sequential_Colors(uint16_t delay_ms);

#endif