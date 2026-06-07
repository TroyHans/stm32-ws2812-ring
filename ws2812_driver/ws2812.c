/* ==================== ws2812.c ========================================= */
#include "ws2812.h"
#include "main.h"
#include "tim.h"
#include "dma.h"
#include <stdint.h>

// Variables defined here (simple and reliable)
uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];
uint8_t datasentflag = 0;
uint16_t pwmData[(24*MAX_LED)+50];

// ==================== WS2812 Driver Functions ====================

void Set_LED (int LEDnum, int Red, int Green, int Blue)
{
    if(LEDnum >= MAX_LED) return;
    
    LED_Data[LEDnum][0] = LEDnum;
    LED_Data[LEDnum][1] = Green;
    LED_Data[LEDnum][2] = Red;
    LED_Data[LEDnum][3] = Blue;
}

void Set_Brightness (int brightness)
{
#if USE_BRIGHTNESS
    if (brightness > 45) brightness = 45;
    if (brightness < 0)  brightness = 0;

    float scale = brightness / 45.0f;

    for (int i = 0; i < MAX_LED; i++)
    {
        LED_Mod[i][0] = LED_Data[i][0];
        LED_Mod[i][1] = LED_Data[i][1] * scale;
        LED_Mod[i][2] = LED_Data[i][2] * scale;
        LED_Mod[i][3] = LED_Data[i][3] * scale;
    }
#endif
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
    datasentflag = 1;
}

void WS2812_Send(void)
{
    uint32_t indx = 0;
    uint32_t color;

    for (uint32_t i = 0; i < MAX_LED; i++)
    {
#if USE_BRIGHTNESS
        color = ((LED_Mod[i][1] << 16) | (LED_Mod[i][2] << 8) | LED_Mod[i][3]);
#else
        color = ((LED_Data[i][1] << 16) | (LED_Data[i][2] << 8) | LED_Data[i][3]);
#endif

        for (int8_t bit = 23; bit >= 0; bit--)
        {
            if (color & (1u << bit))
                pwmData[indx] = 60;
            else
                pwmData[indx] = 30;

            indx++;
        }
    }

    for (uint8_t i = 0; i < 50; i++)
    {
        pwmData[indx++] = 0;
    }

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);

    while (!datasentflag) {}
    datasentflag = 0;
}

// ==================== Animation Functions ====================

void Set_Rainbow(uint8_t brightness, uint16_t delay_ms)
{
    for(int i = 0; i < MAX_LED; i++)
    {
        uint8_t r, g, b;
        int hue = (i * 360) / MAX_LED;

        int sector = hue / 60;
        int remainder = hue % 60;
        int q = (255 * (60 - remainder)) / 60;
        int t = (255 * remainder) / 60;

        switch(sector)
        {
            case 0:  r=255; g=t;   b=0;   break;
            case 1:  r=q;   g=255; b=0;   break;
            case 2:  r=0;   g=255; b=t;   break;
            case 3:  r=0;   g=q;   b=255; break;
            case 4:  r=t;   g=0;   b=255; break;
            default: r=255; g=0;   b=q;   break;
        }

        Set_LED(i, r, g, b);
    }

    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(delay_ms);
}

void Set_Rotating_Rainbow(uint8_t speed, uint8_t brightness, uint16_t delay_ms)
{
    static uint8_t offset = 0;

    for(int i = 0; i < MAX_LED; i++)
    {
        uint8_t pos = (i * 16 + offset) % 256;
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

    offset += speed;

    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(delay_ms);
}

void Set_All_LEDs(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, uint16_t delay_ms)
{
    for(uint32_t i = 0; i < MAX_LED; i++)
    {
        Set_LED(i, red, green, blue);
    }
    Set_Brightness(brightness);
    WS2812_Send();
    HAL_Delay(delay_ms);
}

void Test_Sequential_Colors(uint16_t delay_ms, uint8_t brightness)
{
    uint8_t colors[3][3] = {{255,0,0}, {0,255,0}, {0,0,255}};

    for(uint8_t c = 0; c < 3; c++)
    {
        for(uint32_t led = 0; led < MAX_LED; led++)
        {
            Set_All_LEDs(0, 0, 0, 0, 0);   // turn off
            Set_LED(led, colors[c][0], colors[c][1], colors[c][2]);
            Set_Brightness(brightness);
            WS2812_Send();
            HAL_Delay(delay_ms);
        }
    }
}

void WS2812_Init(void)
{
    Set_All_LEDs(0, 0, 0, 0, 10);   // Start with all LEDs off
}
/* ==================== End ws2812.c ===================================== */