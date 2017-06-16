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
#include "periph/timer.h"
#include "cpu.h"

/* RTT 32 bit variable defined by the platform */
int time32_incr;

#ifdef RIOT_MULTI_THREADED_APP
void dw1000_timer_isr(void *arg, int ch)
{
    /* Wake-up the timer_thread Thread */
    sem_post(&sem);

    /* set the timer channel and also configures the ticks for the callback */
    timer_set(TIMER_CFG_INSTANCE,
              TIMER_CFG_CHANNEL,
              TIMER_TICKS_TO_WAIT);
}
#endif

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

#ifdef RIOT_MULTI_THREADED_APP
    /*initialise the timer for software interrupt */
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

void dw1000_rtt_init(void)
{
    /* initialise the RTT instance */
    rtt_init();

    /* set the alarm and also configure the ticks for the callback */
    rtt_set_alarm(RTT_TICKS_TO_WAIT, dw1000_cb, NULL);
}

#ifdef RIOT_MULTI_THREADED_APP
void dw1000_timer_init(void)
{
    /* initialise the timer instance */
    timer_init(TIMER_CFG_INSTANCE,
               TIMER_FREQ,
               dw1000_timer_isr,
               NULL);

    /* stop the timer */
    timer_stop(TIMER_CFG_INSTANCE);

    /* set the timer channel and also configures the ticks for the callback */
    timer_set(TIMER_CFG_INSTANCE,
              TIMER_CFG_CHANNEL,
              TIMER_TICKS_TO_WAIT);

    /* start the timer */
    timer_start(TIMER_CFG_INSTANCE);
}
#endif
