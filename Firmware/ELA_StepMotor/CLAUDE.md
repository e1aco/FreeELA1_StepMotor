# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ELACO stepper motor driver firmware for STM32F103RET6 (LQFP64, Cortex-M3). Drives a 4-phase stepper motor via dual TB67H450FNG H-bridge chips with MT6816 14-bit magnetic encoder feedback. Communicates over CAN bus and Modbus RTU/ASCII (RS-485).

## Build System

- **IDE**: Keil MDK-ARM (uVision 5) — open `MDK-ARM/ELA_StepMotor.uvprojx`
- **Code generation**: STM32CubeMX — open `ELA_StepMotor.ioc`, regenerate code when peripherals change. User code goes between `USER CODE BEGIN/END` guards.
- **Compiler**: ARM Compiler 5 or 6 (per Keil project settings)
- **No CLI build or Makefile** — build exclusively from Keil IDE

## Hardware Architecture

### MCU & Clock
- STM32F103RET6, currently running on **HSI (8MHz)** during development — PLL disabled, `FLASH_LATENCY_0`. Target production clock: 72MHz via HSE 12MHz ×6 PLL.
- Startup file: `startup_stm32f103xe.s`

### Pin Assignments (from `Core/Inc/main.h`)
| Signal | Pin | Label |
|--------|-----|-------|
| Coil A+ | PA1 | AP |
| Coil A- | PA2 | AM |
| Coil B+ | PC2 | BP |
| Coil B- | PC3 | BM |
| PWM A (TIM2_CH4) | PB11 | PWM_A |
| PWM B (TIM2_CH3) | PB10 | PWM_B |
| SPI1 CS (MT6816) | PA4 | SPI_CS |
| SPI1 SCK/MISO/MOSI | PA5/PA6/PA7 | — |
| RS-485 TX (USART1) | PA9 | RS485_TX |
| RS-485 RX (USART1) | PA10 | RS485_RX |
| CAN RX/TX | PA11/PA12 | — |
| SW1/SW2 | PB1/PB2 | Buttons |
| LED1/LED2 | PB12/PB13 | — |
| ADC CH0 (voltage) | PA0 | POW_MT6816 |

### Timer Allocation
| Timer | Purpose | Config |
|-------|---------|--------|
| TIM1 | 100Hz periodic interrupt | Internal clock |
| TIM2 | PWM generation CH3/CH4 | Stepper coil current control |
| TIM3 | General purpose | Interrupt enabled, used for app timing |
| TIM4 | 20kHz periodic interrupt | Internal clock |
| TIM7 | Modbus 3.5-char timeout | 50µs tick base (via htim3 alias in porttimer.c) |

### Peripheral Details
- **SPI1**: Mode 0 (CPOL=0, CPHA=0), 8-bit frames, 4 Mbps, software NSS. Used for MT6816 magnetic encoder. **Note**: `ela_mt6816.c` code comment says "Mode 3, 16-bit" which contradicts the CubeMX config — verify actual MT6816 requirements.
- **USART1**: RS-485 Modbus RTU/ASCII, half-duplex (no DE/RE pin handling yet in port code)
- **USART3**: DMA RX (circular) + DMA TX (normal) — available for debug/expansion
- **CAN1**: 166kbps, all 4 interrupt sources enabled (TX, RX0, RX1, SCE)
- **ADC1**: DMA1_CH1, circular mode, CH0 (PA0) voltage sampling

## Software Architecture

### Directory Layout
```
Core/           — CubeMX-generated HAL peripheral init (do not hand-edit)
ELA_LIB/        — Application library (ela_* prefix), user-developed modules
FreeModbus/     — FreeModbus protocol stack + STM32 port layer
ModTest/        — Test harness modules (#ifdef ModTest in elaco_main.c)
MDK-ARM/        — Keil uVision 5 project files, startup code, RTE
```

### Initialization Flow (`Core/Src/main.c`)
```
HAL_Init → SystemClock_Config → MX_GPIO_Init → MX_DMA_Init
→ MX_ADC1_Init → MX_CAN_Init → MX_SPI1_Init → MX_TIM1_Init
→ MX_TIM2_Init → MX_TIM4_Init → MX_USART1_UART_Init
→ MX_USART3_UART_Init → MX_TIM3_Init → elaco_main() → while(1)
```

### ELA_LIB Modules (in `ELA_LIB/`)
- **`elaco_main.c/h`** — Application entry point. `elaco_main()` is called before `while(1)` in main.c. Calls `Uart_PrintfInit()` at startup. Centralizes HAL interrupt callbacks in the `cac` section. Header guards: `_ELACO_MAIN_H_`.
- **`ela_uart.c/h`** — Serial communication module. Redirects `printf` to USART3 (PC10/PC11, debug port) via `fputc`/`__io_putchar`. USART1 RS-485 DMA is configured by CubeMX.
- **`ela_can_queue.c/h`** — Fixed-frame queue (48 × 8 bytes) for CAN command buffering. Circular queue with front/rear pointers. Each `Queue_Insert` copies an 8-byte CAN frame.
- **`ela_uart_queue.c/h`** — Byte-level ring buffer (256 bytes) for UART RX/TX streaming data. Supports single-byte `Put`/`Get` and bulk `PutBuf`/`GetBuf` for ISR-friendly frame transfers.
- **`ela_mt6816.c/h`** — MT6816 magnetic encoder driver. `MT6816_Init()` / `MT6816_GetAngle()` populate global struct `g_mt6816_st` (type `MT6816_ANGLE_T`) with 14-bit angle, magnet status, and parity-validated data. Retries up to 3 SPI reads until even parity (MT6816 protocol requirement).

Planned but not yet created (referenced in project notes): `ela_button`, `ela_tb67h450`.

### FreeModbus Integration
- **Port layer** in `FreeModbus/modbus/port*.c`:
  - `portserial.c` — USART1 RS-485, interrupt-driven TX/RX via `USART1_IRHandler()` (note: NOT the CubeMX-generated `USART1_IRQHandler` — manually defined)
  - `porttimer.c` — TIM7 for 3.5-character timeout (uses variable name `htim3` aliased to TIM7 via `htim3.Instance = TIM7`)
  - `portevent.c` — Simple queue-based event posting
  - `port.c` — Critical section stubs (empty — no RTOS)
- **Config**: `mbconfig.h` enables RTU + ASCII, all standard function codes. TCP disabled.
- **Warning**: `porttimer.c` declares `htim3.Instance = TIM7` — this is intentional aliasing, not a bug. The Modbus timer callback lives in `TIM7_IRQHandler()`.
- **Warning**: `portserial.c` provides its own `USART1_IRHandler()` (note spelling: no 'Q'). The CubeMX-generated `USART1_IRQHandler` in `stm32f1xx_it.c` calls `HAL_UART_IRQHandler` and may conflict. Ensure only one USART1 ISR is active.

### Test Harness (`ModTest/`)
- Conditionally compiled via `#define ModTest` in `elaco_main.h`
- `test_mt6816.c` — calls `MT6816_Init()` then loops `MT6816_GetAngle()`

## Naming & Code Style

> **Authoritative source**: `C:\Users\admin\Desktop\XM\ELACO_RULES\rules.md`

### Files
- Library files: `ela_` prefix (e.g., `ela_button.c`)
- Header guards: `_ELA_XXX_H_` (all caps)
- Main file: `elaco_main.c/h` (no `ela_` prefix)
- Every `.c` and `.h` file must end with **one blank line**, otherwise Keil emits a warning

### Functions
- Public functions: **PascalCase** — `Module_ActionDetails` (e.g., `MT6816_ReadAngle`, `Uart_SendString`)
- HAL callbacks keep ST's original names

### Variables
- Globals: `g_` prefix (e.g., `g_rx1_offset`)
- Struct globals: `g_` prefix + `_st` suffix (e.g., `g_mt6816_st`)
- FSM globals: `g_` prefix + `_fsm` suffix (e.g., `g_motor_mode_fsm`)
- Struct types: `_T` suffix, uppercase (e.g., `MT6816_ANGLE_T`)
- Locals and statics: lowercase, no special prefix

### Macros
- Constants: `UPPER_SNAKE_CASE` (e.g., `DMA_BUF_SIZE`)
- Masks: `_MASK` suffix (e.g., `MT6816_STATUS_MASK`)
- Status codes: module-prefixed uppercase (e.g., `MT6816_STATUS_NORMAL`)

### Documentation
Every function (including `static`) must have a doc comment before its definition. `@ 输入` and `@ 输出` are optional — omit for `void`:
```
/********
 * @ 输入: <params>    (optional, omit for void)
 * @ 输出: <returns>   (optional, omit for void)
 * @ 说明: <description>
 * @ 注意: <caveats>   (optional)
 ****/
```

### Formatting
- 4-space indentation, no tabs
- CRLF line endings, no trailing whitespace
- Egyptian-style braces (opening brace on same line)
- **84-character line width limit** — the standard separator line `//----------------------------------------------------------------------------------` (84 chars: 2 `/` + 82 `-`) is the reference

### Function Layering (hlp → drv → usr → cac)

Every `.c` file groups functions into layers in this order. Each layer is delimited by 84-char separators:

```
/* <module> hlp end */
//----------------------------------------------------------------------------------
/* <module> drv start */
...
/* <module> drv end */
//----------------------------------------------------------------------------------
/* <module> usr start */
...
/* <module> usr end */
//----------------------------------------------------------------------------------
/* <module> cac start */
...
/* <module> cac end */
```

| Layer | Purpose | Restrictions |
|-------|---------|-------------|
| **hlp** | Pure helper functions — calculations, data transforms | No HAL calls, no register access |
| **drv** | Hardware driver layer — init, register config, HAL calls | Lowest hardware encapsulation |
| **usr** | User-facing API functions — may use HAL | Ready-to-use interface functions |
| **cac** | Callback/ISR functions — only in `elaco_main.c` | Centralizes all interrupt callbacks |

- Omit layers that have no functions
- `<module>` is the lowercase module name (e.g., `can`, `eeprom`, `timer`)

### Conditions
- Yoda style: put constants on the left — `if (1 != g_relay_state)` not `if (g_relay_state != 1)`

### Include Order
1. `elaco_main.h` (central header)
2. ST HAL headers (`main.h`, `spi.h`, `usart.h`, etc.)
3. Standard library headers (`stdbool.h`, `string.h`, `stdint.h`)

## Key Constraints

- **CubeMX user code guards**: Never edit code between `USER CODE BEGIN/END` comments — it gets overwritten on code regeneration.
- **HSI clock**: Current code runs at 8MHz (HSI, PLL off). Production target is 72MHz HSE+PLL. This affects all timer prescaler calculations.
- **FreeModbus port ISR naming**: `portserial.c` defines `USART1_IRHandler()` (no 'Q'). The CubeMX-generated `USART1_IRQHandler` in `stm32f1xx_it.c` may need to be removed or disabled.
- **No RTOS**: Bare metal HAL with interrupt-driven concurrency. Critical sections are stubbed out (no preemption protection yet).

## Reference

- **Coding rules (authoritative)**: `C:\Users\admin\Desktop\XM\ELACO_RULES\rules.md`
- Reference project: `../../Reference/StepMotor/` (XM-003-08, contains schematics, PCB Gerber, firmware)
- Hardware notes: `../../Reference/StepMotor/CLAUDE.md`
