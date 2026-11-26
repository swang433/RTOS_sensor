# STM32 Sensor Node (FreeRTOS)

A small FreeRTOS firmware for the NUCLEO-C071RB (STM32C071RB, Cortex-M0+). It samples a sensor on a schedule and answers a line-based text command protocol over UART. The UART reaches a PC through the board's ST-Link USB virtual COM port.

Like [stm32_alarm](https://github.com/swang433/stm32_alarm), this is written at the register level: no HAL, no CubeMX, custom startup code and linker script. FreeRTOS is the only third-party code.

> **Status: planned.** Nothing here is implemented yet. This README describes the design and build order. Commands, pins and file names are proposals and may change.

## Goals

- Learn FreeRTOS fundamentals with a project small enough to finish: tasks, queues, stream buffers, mutexes, periodic timing and ISR-safe API calls.
- Keep the bare-metal, understand-every-layer approach from the alarm clock.
- End up with a node that is easy to exercise from a serial terminal, with no extra tooling.

## Hardware

| Part | Purpose | Required |
|---|---|---|
| NUCLEO-C071RB | MCU board; ST-Link provides the USB virtual COM port | yes |
| USB cable and serial terminal (`screen`, PuTTY, minicom) | talk to the node | yes |
| Internal temperature sensor (ADC) | first sensor, no wiring | yes |
| BME280 breakout (I2C, address 0x76 or 0x77) | second sensor, shares the I2C bus | optional |
| SH1106 OLED (I2C, address 0x3C) | local status display, reused from the alarm clock | optional |
| Passive piezo buzzer (TIM3 PWM) | status and error beeps, reused | optional |
| Tactile buttons | local controls, reused | optional |
| 8-channel logic analyzer | debugging I2C and UART timing | optional |

Everything runs at 3.3 V. The UART is expected to be USART2 on PA2/PA3 (verify against the Nucleo user manual). Those pins do not conflict with the alarm clock's pin map.

## Architecture (planned)

```
UART RX ISR ──bytes──> stream buffer ──> command task ──┬──> config (rate, stream on/off)
                                                        └──> reply ──┐
                                                                     ▼
sensor task (vTaskDelayUntil) ──readings──────────────────────> TX queue ──> TX task ──> UART
```

- **UART RX ISR:** pushes each received byte into a stream buffer with the `FromISR` API and does nothing else.
- **Command task:** blocks on the stream buffer, assembles lines, parses commands and acts on them.
- **Sensor task:** wakes on a fixed period, reads the sensor and queues the result when streaming is on or a reading was requested.
- **TX task:** the single owner of the UART transmitter, fed by a queue, so replies and streamed readings never interleave.
- **Optional tasks:** buzzer (melodies from a queue, replacing the blocking `delay()` version), OLED status, and buttons through a queue.

## Command protocol (proposed)

ASCII, one command per line, terminated by `\n` (a preceding `\r` is ignored). Case-insensitive. Every command gets exactly one reply line starting with `OK` or `ERR`.

| Command | Effect |
|---|---|
| `GET TEMP` | one temperature reading |
| `SET RATE <ms>` | set the sampling period |
| `STREAM ON` / `STREAM OFF` | start or stop continuous readings |
| `STATUS` | current rate, streaming state and uptime |
| `GET TIME` / `SET TIME hh:mm` | read or set the RTC (reused from the alarm clock) |

Streamed readings are sent as their own lines. Errors cover unknown commands, bad arguments, an overlong line and RX buffer overflow.

## Build order

1. Boot FreeRTOS with a heartbeat task. This validates the vector table, port files and tick.
2. UART echo.
3. Command parser.
4. Internal temperature sensor on a periodic task.
5. Streaming and rate control.
6. BME280 on the shared I2C bus, with a mutex around the bus.
7. Extras: buzzer task, OLED status, buttons, RTC timestamps.

## Reused from stm32_alarm

- **As-is:** `startup.s`, `linker.ld`, `build.sh`, `memcpy.c`, the register definitions style in `config.h`, the buzzer PWM routines, `rtc.c`, and the I2C init and write path.
- **With changes:** the button FSM pattern, `oled.c` (needs more font glyphs), and the melodies (must become non-blocking).
- **Replaced:** `delay()` busy-waits, the ISR debounce delays in `irq.c` and the global `button_event` variable. These become `vTaskDelay`, a debounce task and a queue.

## Things to get right

- **Vector table:** the SVCall, PendSV and SysTick entries must point at `vPortSVCHandler`, `xPortPendSVHandler` and `xPortSysTickHandler`. Otherwise the first SVC hard-faults. The USART IRQ slot also needs a real handler.
- **Clock:** the board runs at its default 12 MHz, so `configCPU_CLOCK_HZ` is 12000000 and a 1 kHz tick reloads SysTick every 12000 cycles. At 115200 baud the BRR is about 104, roughly 0.16% error.
- **Interrupt priorities:** the M0+ has only four priority levels and no BASEPRI, so critical sections mask all interrupts. Keep ISRs short.
- **RAM:** 24 KB total. Size task stacks deliberately, enable `configCHECK_FOR_STACK_OVERFLOW` and check `uxTaskGetStackHighWaterMark`.
- **No libc:** with `-nostdlib` you will need your own `memset` and the string helpers the parser uses (`strlen`, `strcmp`).
- **Build flags:** add `-I` for the FreeRTOS headers, `-Os` and probably `-ffreestanding`. The alarm clock's `build.sh` builds at `-O0`.

## Build and flash

Toolchain: `arm-none-eabi-gcc`. The alarm clock's `build.sh` will be adapted to also compile the FreeRTOS kernel sources (`tasks.c`, `queue.c`, `stream_buffer.c`, `list.c`, `timers.c`, `heap_4.c` and the `ARM_CM0` port) and produce `firmware.elf` and `firmware.bin`. Flash the `.bin` to the Nucleo as you do for the alarm clock. Then open the virtual COM port at 115200 8N1 and type `STATUS`.

## Planned layout

```
sensor_node/
├── main.c            task creation and scheduler start
├── FreeRTOSConfig.h
├── config.h          register definitions
├── uart.c            USART driver and RX ISR
├── cmd.c             line assembly and command parser
├── sensor.c          internal temp sensor (ADC), later BME280
├── startup.s  linker.ld  build.sh  memcpy.c
├── rtc.c  alarm.c  oled.c   (carried over, optional)
└── FreeRTOS/         kernel sources
```
