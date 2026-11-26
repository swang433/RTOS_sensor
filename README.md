# STM32 Sensor Node (FreeRTOS)

Firmware for the NUCLEO-C071RB (STM32C071RB, Arm Cortex-M0+) that samples a sensor on a fixed schedule and accepts text commands over a serial port. The node connects to a PC through the board's ST-Link USB virtual COM port, so no extra hardware is needed to use it.

The firmware is written at the register level, with no HAL or CubeMX, and uses its own startup code and linker script. FreeRTOS is the only third-party code.

> **Status:** in development. The commands, pin assignments and file layout described here are the target design and may change before the first release.

## Contents

1. [Features](#features)
2. [Hardware requirements](#hardware-requirements)
3. [Getting started](#getting-started)
4. [Command reference](#command-reference)
5. [Architecture](#architecture)
6. [Building from source](#building-from-source)
7. [Development roadmap](#development-roadmap)
8. [Design notes](#design-notes)
9. [Project layout](#project-layout)

## Features

- Periodic sensor sampling with an adjustable rate.
- A line-based text command protocol that you can use from any serial terminal.
- Continuous streaming of readings on request.
- Optional BME280 environmental sensor, OLED status display, buzzer and buttons.
- A small FreeRTOS design: separate tasks for receiving commands, sampling and transmitting, connected by queues and stream buffers.

## Hardware requirements

| Component | Purpose | Required |
| --- | --- | --- |
| NUCLEO-C071RB | Microcontroller board. The on-board ST-Link provides the USB virtual COM port. | Yes |
| USB cable | Power, programming and serial connection | Yes |
| Serial terminal (PuTTY, `screen`, minicom) | Sending commands to the node | Yes |
| BME280 breakout (I2C, address 0x76 or 0x77) | Second sensor on the I2C bus | Optional |
| SH1106 OLED display (I2C, address 0x3C) | Local status display | Optional |
| Passive piezo buzzer (TIM3 PWM) | Status and error tones | Optional |
| Push buttons | Local controls | Optional |
| Logic analyzer | Debugging I2C and UART timing | Optional |

The first sensor is the microcontroller's internal temperature sensor, read through the ADC, so it needs no wiring.

All components run at 3.3 V. The serial interface uses USART2 on PA2 (TX) and PA3 (RX), which the Nucleo board routes to the ST-Link virtual COM port. Confirm this against the NUCLEO-C071RB user manual for your board revision.

## Getting started

1. Build the firmware (see [Building from source](#building-from-source)) or use a prebuilt `firmware.bin`.
2. Connect the Nucleo board to your PC with a USB cable.
3. Flash `firmware.bin` to the board. You can drag it onto the board's USB mass-storage drive, or use STM32CubeProgrammer.
4. Open the board's virtual COM port in a serial terminal with these settings:

   | Setting | Value |
   | --- | --- |
   | Baud rate | 115200 |
   | Data bits | 8 |
   | Parity | None |
   | Stop bits | 1 |
   | Line ending | LF or CR+LF |

5. Type `STATUS` and press Enter. The node replies with a line starting with `OK`.

## Command reference

### Protocol rules

- Commands are ASCII text, one per line, ended with a line feed (`\n`). A carriage return (`\r`) before the line feed is ignored.
- Commands are case-insensitive.
- Every command receives exactly one reply line, starting with `OK` or `ERR`.
- Streamed readings are sent as separate lines and may appear between replies.

### Commands

| Command | Description |
| --- | --- |
| `GET TEMP` | Returns one temperature reading. |
| `SET RATE <ms>` | Sets the sampling period in milliseconds. |
| `STREAM ON` | Starts sending readings continuously at the sampling rate. |
| `STREAM OFF` | Stops continuous readings. |
| `STATUS` | Reports the sampling rate, streaming state and uptime. |
| `GET TIME` | Returns the real-time clock time. |
| `SET TIME hh:mm` | Sets the real-time clock time. |

### Errors

The node replies with `ERR` in these cases:

- unknown command
- missing or invalid argument
- line longer than the receive buffer
- receive buffer overflow

## Architecture

```
UART RX ISR ──bytes──> stream buffer ──> command task ──┬──> config (rate, stream on/off)
                                                        └──> reply ──┐
                                                                     ▼
sensor task (vTaskDelayUntil) ──readings──────────────────────> TX queue ──> TX task ──> UART
```

| Component | Role |
| --- | --- |
| UART RX interrupt | Pushes each received byte into a stream buffer using the `FromISR` API, and does nothing else. |
| Command task | Waits on the stream buffer, assembles complete lines, then parses and executes commands. |
| Sensor task | Wakes on a fixed period, reads the sensor, and queues the result when streaming is on or a reading was requested. |
| TX task | The only task that writes to the UART. It is fed by a queue, so replies and streamed readings never interleave. |
| Optional tasks | Buzzer (non-blocking tones from a queue), OLED status display, and button handling through a queue. |

## Building from source

### Requirements

- Arm GNU toolchain (`arm-none-eabi-gcc`)
- FreeRTOS kernel sources, placed in `FreeRTOS/`
- A shell that can run `build.sh` (Linux, macOS, or Git Bash on Windows)

### Build

```sh
./build.sh
```

The script compiles the application sources with the FreeRTOS kernel (`tasks.c`, `queue.c`, `stream_buffer.c`, `list.c`, `timers.c`, `heap_4.c` and the `ARM_CM0` port) and produces:

- `firmware.elf`, for debugging
- `firmware.bin`, for flashing

Key compiler options are `-mcpu=cortex-m0plus -mthumb -Os -ffreestanding`, with an `-I` path to the FreeRTOS headers.

## Development roadmap

The firmware is built up in stages. Each stage is tested before the next one starts.

| Stage | Milestone | Verifies |
| --- | --- | --- |
| 1 | FreeRTOS boots with a heartbeat task | Vector table, port files and tick timer |
| 2 | UART echo | Serial driver and RX interrupt |
| 3 | Command parser | Line assembly and command handling |
| 4 | Internal temperature sensor on a periodic task | ADC driver and periodic scheduling |
| 5 | Streaming and rate control | Task communication and runtime configuration |
| 6 | BME280 on the I2C bus | I2C driver, with a mutex protecting the shared bus |
| 7 | Extras: buzzer, OLED status, buttons, RTC timestamps | Optional peripherals |

## Design notes

- **Vector table:** the SVCall, PendSV and SysTick entries must point to `vPortSVCHandler`, `xPortPendSVHandler` and `xPortSysTickHandler`. Otherwise the first scheduler call causes a hard fault. The USART2 entry needs a real interrupt handler.
- **Clock:** the board runs at its default 12 MHz, so `configCPU_CLOCK_HZ` is `12000000`. A 1 kHz tick reloads SysTick every 12,000 cycles. At 115200 baud, the baud-rate register value is about 104, an error of roughly 0.16%.
- **Interrupt priorities:** the Cortex-M0+ has four priority levels and no `BASEPRI` register, so FreeRTOS critical sections mask all interrupts. Interrupt handlers must be kept short.
- **RAM:** the chip has 24 KB of RAM. Task stacks are sized deliberately, `configCHECK_FOR_STACK_OVERFLOW` is enabled, and `uxTaskGetStackHighWaterMark` is used to check actual stack usage.
- **C library:** the firmware is linked with `-nostdlib`, so it provides its own `memcpy`, `memset` and the string functions the parser uses (`strlen`, `strcmp`). It still links `libgcc`, because the Cortex-M0+ has no hardware divide instruction and division is done in software.

## Project layout

```
RTOS_sensor/
├── main.c              task creation and scheduler start
├── FreeRTOSConfig.h    FreeRTOS kernel configuration
├── config.h            register definitions
├── uart.c              USART2 driver and RX interrupt
├── cmd.c               line assembly and command parser
├── sensor.c            internal temperature sensor (ADC), later BME280
├── rtc.c               real-time clock (optional)
├── oled.c              SH1106 OLED display driver (optional)
├── memcpy.c            memcpy and memset
├── startup.s           vector table and reset handler
├── linker.ld           memory map and section placement
├── build.sh            build script
└── FreeRTOS/           FreeRTOS kernel sources
```
