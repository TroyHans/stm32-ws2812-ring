# STM32 WS2812 LED Ring

A simple yet clean WS2812 (NeoPixel) driver for a 16-LED ring using **STM32 + Timer PWM + DMA**.

## Features

- Smooth rotating rainbow animation
- Adjustable rotation speed
- Brightness control (static or breathing ready)
- Clean, well-organized code structure
- Built with STM32CubeMX + CMake

## Hardware

- STM32 (tested on common Nucleo/F4 boards)
- 16-LED WS2812 ring
- 5V power supply with proper decoupling
- Data pin connected to TIM1_CH1 (PA8 by default)

## Software Setup

1. Open project in VS Code
2. Configure CMake (Kit + build)
3. Build: `cmake --build build`
4. Flash using ST-Link / OpenOCD / STM32CubeProgrammer

## Code Structure

- `main.c` → Main logic and animation
- `Set_Rainbow()` → Static rainbow
- `Set_Rotating_Rainbow()` → Smooth rotating effect
- `Set_Brightness()` → Brightness scaling
- `WS2812_Send()` → DMA + Timer driver

## How to Customize

- Change rotation speed: Modify `offset += X` in `Set_Rotating_Rainbow()`
- Change brightness: Adjust value in `Set_Brightness(35)`
- Add new effects: Create new functions similar to `Set_Rotating_Rainbow()`

## Next Steps / Ideas

- Breathing effect (brightness pulse)
- Multiple animation modes (button selectable)
- Gamma correction for better colors
- Non-blocking timing with timers

---

**Happy Hacking!** 🚀
