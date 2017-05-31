/*
 * Copyright (C) 2017 (c) DecaWave Ltd, Dublin, Ireland
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_nucleo-f401_dw1000
 * @{
 *
 * @file
 * @brief       Board specific implementations for the STM32F401RE Nucleo
 *              and Decawave DW1000 board
 *
 * @author      niharika.sonti <niharika.sonti@pathpartnertech.com>
 *
 * @}
 */

#include "dw1000_board.h"
#include "cpu.h"

/* Timer 32 bit variable defined by the platform */
int time32_incr;

#ifdef SYSTICK_ENABLE
void dw1000_systick_cb(void)
{
    time32_incr++;
}
#else
void dw1000_cb(void *arg, int channel)
{
    time32_incr++;

    /* set the timer channel and also configures the ticks for the callback */
    timer_set(TIMER_CFG_INSTANCE, TIMER_CFG_CHANNEL, TIMER_TICKS_TO_WAIT);
}
#endif

static void dw1000_isr (void* a)
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

#ifdef SYSTICK_ENABLE
    /* initialise the systick module */
    dw1000_systick_init();
#else
    /* initialise the timer module */
    dw1000_timer_init();
#endif
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

#ifdef SYSTICK_ENABLE
void dw1000_systick_init(void)
{
    /* configure the systick module */
    SysTick_Config(SYSTCK_FREQ);
}
#else
void dw1000_timer_init(void)
{
    /* initialise the timer instance */
    timer_init(TIMER_CFG_INSTANCE, TIMER_FREQ, dw1000_cb, NULL);

    /* stop the timer */
    timer_stop(TIMER_CFG_INSTANCE);

    /* set the timer channel and also configures the ticks for the callback */
    timer_set(TIMER_CFG_INSTANCE, TIMER_CFG_CHANNEL, TIMER_TICKS_TO_WAIT);

    /* start the timer */
    timer_start(TIMER_CFG_INSTANCE);
}
#endif
