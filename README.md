# F767ZI-FanControl - Nucleo-based PWM fan controller

This project is prepared as Microprocessor Systems course on Poznan University of Technology.

## Features

 - Built for Nucleo-F767ZI (might be compatible with Nucleo-F746ZG with minimal tweaking)
 - Based on STM32Cube HAL with STM32CubeMX-generated code
 - STM32Cube-compatible project
 - hybrid open/closed-loop control system with a custom P-type controller
 - Windows Forms-based [desktop app](https://github.com/mattroot/MS-lab-2023-24-proj-desktop) with serial connection to the board

## Credits

The following components have been used during development of this project:
 - [BMP280 driver for STM32F0](https://github.com/ciastkolog/BMP280_STM32) (MiT licensed)
 - [I2C character display example for NUCLEO-F746ZG](https://github.com/adrianwojcikpp/NUCLEO-F746ZG-Examples-ver-2023-24/tree/main/EXTRAS/LCD_I2C_Example)
 - [drmrvr12](https://figdor32.pl)
 