# VMS_Data_logger
Embedded motorsport data logger using Teensy for tire temp, display output, and SD card data logging.
# VMS Motorsport Data Sensor

This project is an embedded motorsport data logger built for VMS Motorsport. It is designed to collect vehicle sensor data, display live readings on an LCD/TFT screen, and save the data to an SD card for later analysis.

## Goal

The goal of this project is to create a lightweight and practical sensor system that can:

- read sensor data in real time
- display values live for the driver or team
- log data to an SD card
- allow the SD card to be removed later for upload and review

## Current Focus

The current version is focused on:

- tire temperature sensing using IR temperature sensors aimed at the tires
- live display output on an LCD/TFT screen
- SD card logging
- embedded firmware running on a microcontroller without Linux

## Hardware Ideas

Possible boards for the project include:

- STM32 NUCLEO-F446RE
- Teensy 4.1
- ESP32 for future wireless support

Display options:

- I2C LCD
- SPI TFT display using ILI9341

Storage:

- SPI microSD card adapter (if we go with a board other than the Teensy)
- or built-in microSD on Teensy 4.1

## Software Design

The project currently uses a simple bare-metal style main loop instead of an RTOS.
Maybe RTOS

Main tasks include:

- reading sensor values
- updating the display
- writing logs to the SD card
- handling future warning logic

## Planned Features

- live tire temperature display
- SD card CSV logging
- multiple sensor channels
- warning indicators for abnormal readings
- possible CAN support in future versions
- possible steering-wheel display version later

## Project Structure

```bash
src/        # main source code
include/    # headers
lib/        # drivers or helper libraries
docs/       # notes, wiring diagrams, planning
