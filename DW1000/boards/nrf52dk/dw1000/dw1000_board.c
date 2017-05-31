/*
 * Copyright (C) 2017 (c) DecaWave Ltd, Dublin, Ireland
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_nrf52dk_dw1000
 * @{
 *
 * @file
 * @brief       Board specific implementations for the Nordic NRF52840
 *              and Decawave DW1000 board
 *
 * @author      shreeharsha.i <shreeharsha.i@pathpartnertech.com>
 *
 * @}
 */

#include "dw1000_board.h"
#include "cpu.h"

/* RTT 32 bit variable defined by the platform */
int time32_incr;

void dw1000_cb(void *arg)
{
    time32_incr++;

    /* clear the RTT counter */
    RTT_DEV->TASKS_CLEAR = 1;

    /* enable the RTT Compare0 interrupt */
    RTT_DEV->INTENSET = RTC_INTENSET_COMPARE0_Msk;
}

static void dw1000_isr(void *arg)
{
    /* interrupt handler which will process status events */
    process_deca_irq();

    /* clear the pending interrupt */
    NVIC_ClearPendingIRQ(EXT_IRQ);
}

void dw1000_board_init(void)
{
    /* initialise the spi module */
    dw1000_spi_init();

    /* initialise the interrupts */
    dw1000_interrupt_init();

    /* initialise the onboard LEDs for EVB1000 internal micro controller */
    dw1000_gpio_init();

    /* initialise the RTT module */
    dw1000_rtt_init();
}

void dw1000_gpio_init(void)
{
    /* TBD */
}
void dw1000_spi_init(void)
{
    /* initialise the spi instance */
    spi_init(DW1000_SPI_DEV);
    /* initialise the spi chip select */
    spi_init_cs(DW1000_SPI_DEV, DW1000_SPI_CS);
}

void dw1000_interrupt_init(void)
{
    /* initialise the external interrupt */
    gpio_init_int(DECAIRQ, GPIO_IN_PD, GPIO_RISING, dw1000_isr, NULL);

    /* enable the external interrupt */
    gpio_irq_enable(DECAIRQ);
}

void dw1000_rtt_init(void)
{
    /* initialise the RTT instance */
    rtt_init();

    /* set the alarm and also configure the ticks for the callback */
    rtt_set_alarm(RTT_TICKS_TO_WAIT, dw1000_cb, NULL);
}
