/**
 * Copyright 2017 Decawave Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @defgroup    boards_nucleo-f401_dw1000 STM32F401RE Nucleo and
 *              Decawave DW1000 board
 * @ingroup     boards
 * @brief       Support for the STM32F401RE Nucleo and Decawave DW1000
 *              board
 * @{
 *
 * @file
 * @brief       Board specific definitions for the STM32F401RE Nucleo
 *              board and Decawave DW1000 board
 *
 * @author      niharika.sonti <niharika.sonti@pathpartnertech.com>
 */

#ifndef DW1000_BOARD_H
#define DW1000_BOARD_H

#include <periph/spi.h>
#include <periph/timer.h>
#include "dw1000_port.h"

#ifdef __cplusplus
extern "c" {
#endif

/**
 * @brief Flag to enable Delayed Transmission for DecaRange RTLS Application
 *
 * Note: If the macro is defined Delayed Transmission is enabled else Immediate Tx is enabled
 * for DecaRange RTLS Application
 */
#define DELAYED_TX_ENABLE

/**
 * @brief Flag to enable Systick for calculating the delay time for DecaRange RTLS Application
 *
 * Note: if the macro is defined, Systick is used for calculating the delay time else
 * Timer is used for calculating the delay time for the DecaRange RTLS Application
 */
#define SYSTICK_ENABLE

/**
 * @brief DW1000 SPI Instance Configuration
 */
#define DW1000_SPI_CFG_INSTANCE         (1U)

/**
 * @brief DW1000 Reset Configuration
 */
#define DW1000_RSTn                     GPIO_PIN(0, 2)

/**
 * @brief DW1000 Interrupt Configuration
 */
#define DECAIRQ                         GPIO_PIN(PORT_A, 6)

/**
 * @brief SPI Configuration for Mode, Clock, Chip Select and SPI Instance
 * @{
 */
#define DW1000_SPI_MODE                 SPI_MODE_0
#define DW1000_SPI_HIGH                 SPI_CLK_18MHZ
#define DW1000_SPI_LOW                  SPI_CLK_1MHZ
#define DW1000_SPI_CS                   GPIO_PIN(PORT_A, 4)
#define DW1000_SPI_DEV                  DW1000_SPI_CFG_INSTANCE
/** @} */

/**
 * @brief Configures TIMER tick value for 1msec on STM32F401RE Board
 */
#define TIMER_TICKS_TO_WAIT             (1000U)

/**
 * @brief Configures TIMER Frequency for 1usec on STM32F401RE Board
 */
#define TIMER_FREQ                      (1000000U)

/**
 * @brief Configures TIMER Channel Configuration
 */
#define TIMER_CFG_CHANNEL               (0U)

/**
 * @brief Configures TIMER Instance Configuration
 */
#define TIMER_CFG_INSTANCE              (0U)

/**
 * @brief Configures Systick Frequency for 1msec on STM32F401RE Board
 */
#define SYSTCK_FREQ                     (72000U)

/**
 * @brief Configures External irq on STM32F401RE Board
 */
#define EXT_IRQ                         EXTI9_5_IRQn

/**
 * @brief DW1000 Interrupt handler
 */
void process_deca_irq(void);

/**
 * @brief Initialize decawave DW1000 board specific hardware for the peripherals
 *        GPIO, SPI and External Interrupt configuration
 */
void dw1000_board_init(void);

/**
 * @brief Initializes and Enables a GPIO pin for External Interrupt usage
 */
void dw1000_interrupt_init(void);

/**
 * @brief This function does basic SPI initialization and configures pin
 *        configuration for MISO, MOSI, and CLK pins
 */
void dw1000_spi_init(void);

/**
 * @brief Initializes and configures GPIO's for decawave DW1000 Board
 */
void dw1000_gpio_init(void);

/**
 * @brief Intialize Timer as counter
 */
void dw1000_timer_init(void);

/**
 * @brief Intialize Systick module
 */
void dw1000_systick_init(void);

/**
 * @brief This function handles Timer Handler
 */
void dw1000_cb(void *arg, int channel);

/**
 * @brief This function handles SysTick Handler
 */
void dw1000_systick_cb(void);

#ifdef __cplusplus
}
#endif

#endif /* DW1000_BOARD_H */
/** @} */
