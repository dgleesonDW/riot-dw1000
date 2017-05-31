/*
 * Copyright (C) 2017 (c) DecaWave Ltd, Dublin, Ireland
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    boards_nrf52840_dw1000 Nordic NRF52840
 *              and Decawave DW1000 board
 * @ingroup     boards
 * @brief       Support for the Nordic NRF52840 and Decawave DW1000
 *              board
 * @{
 *
 * @file
 * @brief       Board specific definitions for the Nordic NRF52840
 *              board and Decawave DW1000 board
 *
 * @author      shreeharsha.i <shreeharsha.i@pathpartnertech.com>
 */

#ifndef DW1000_BOARD_H
#define DW1000_BOARD_H

#include <periph/spi.h>
#include <periph/rtt.h>
#include "dw1000_port.h"

#ifdef __cplusplus
extern "c" {
#endif

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
#define DECAIRQ                         (30U)

/**
 * @brief SPI Configuration for Mode, Clock, Chip Select and SPI Instance
 * @{
 */
#define DW1000_SPI_MODE                 SPI_MODE_0
#define DW1000_SPI_HIGH                 SPI_CLK_5MHZ
#define DW1000_SPI_LOW                  SPI_CLK_1MHZ
#define DW1000_SPI_CS                   (29U)
#define DW1000_SPI_DEV                  DW1000_SPI_CFG_INSTANCE
/** @} */

/**
 * @brief Configures RTT tick value for 1msec on NRF52840 Board
 */
#define RTT_TICKS_TO_WAIT               (50U)

/**
 * @brief Configures RTT Initial Counter Value
 */
#define RTT_INIT_COUNTER_VAL            (0x0U)

/**
 * @brief Configures External irq on NRF52840 Board
 */
#define EXT_IRQ                         GPIOTE_IRQn

/**
 * @brief DW1000 Interrupt handler
 */
void process_deca_irq(void);

/**
 * @brief Initialize decawave DW1000 board specific hardware for the peripherals
 *        GPIO, SPI and External Interrupt configuration
 *
 * @param  No arguments
 * @return Void
 */
void dw1000_board_init(void);

/**
 * @brief Initializes and Enables a GPIO pin for External Interrupt usage
 *
 * @param  No arguments
 * @return Void
 */
void dw1000_interrupt_init(void);

/**
 * @brief This function does basic SPI initialization and configures pin
 *        configuration for MISO, MOSI, and CLK pins
 *
 * @param  No arguments
 * @return Void
 */
void dw1000_spi_init(void);

/**
 * @brief Initializes and configures GPIO's for decawave DW1000 Board
 *
 * @param  No arguments
 * @return Void
 */
void dw1000_gpio_init(void);

/**
 * @brief Intialize RTC as counter
 *
 * @param  No arguments
 * @return Void
 */
void dw1000_rtt_init(void);

/**
 * @brief This function handles RTT Handler
 */
void dw1000_cb(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* DW1000_BOARD_H */
/** @} */
