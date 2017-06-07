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
 * @ingroup     boards_samr21-xpro_dw1000
 * @{
 *
 * @file
 * @brief       Board specific implementations for the Atmel SAM R21 Xplained
 *              Pro and Decawave DW1000 board
 *
 * @author      shreeharsha.i <shreeharsha.i@pathpartnertech.com>
 *
 * @}
 */

#include "dw1000_board.h"
#include "cpu.h"

/* RTT 32 bit variable defined by the platform */
int time32_incr;

#ifdef SYSTICK_ENABLE
void dw1000_systick_cb(void)
{
    time32_incr++;
}
#else
void dw1000_cb(void *arg)
{
    time32_incr++;
}
#endif

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

#ifdef SYSTICK_ENABLE
    /* initialise the systick module */
    dw1000_systick_init();
#else
    /* initialise the RTT module */
    dw1000_rtt_init();
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
void dw1000_rtt_init(void)
{
    /* initialise the RTT instance */
    rtt_init();

    /* set the RTT counter value */
    rtt_set_counter(RTT_INIT_COUNTER_VAL);

    /* set the alarm and also configure the ticks for the callback */
    rtt_set_alarm(RTT_TICKS_TO_WAIT, dw1000_cb, NULL);
}
#endif
