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

/*! ----------------------------------------------------------------------------
 * @file    port.c
 * @brief   HW specific definitions and functions for portability
 *
 */

#include "dw1000_compiler.h"
#include "dw1000_port.h"
#include "dw1000_board.h"
#include "periph/gpio.h"
#include "cpu.h"

uint32_t spiFreq;

/* RTT 32 bit variable defined by the platform */
extern __IO unsigned long time32_incr;

int No_Configuration(void)
{
    return -1;
}

unsigned long portGetTickCnt(void)
{
    return time32_incr;
}

void SPI_ConfigFastRate(uint32_t scalingfactor)
{
   spiFreq = scalingfactor;
}

void deca_reset(void *arg)
{
   NVIC_ClearPendingIRQ(EXT_IRQ);
}

void setup_DW1000RSTnIRQ(int enable)
{
    if(enable)
    {
        gpio_init_int(DW1000_RSTn,
                      GPIO_IN_PD,
                      GPIO_RISING,
                      deca_reset,
                      NULL);

        gpio_irq_enable(DW1000_RSTn);
    }
    else
    {
        gpio_init_int(DW1000_RSTn,
                      GPIO_IN,
                      GPIO_RISING,
                      deca_reset,
                      NULL);

        gpio_irq_disable(DW1000_RSTn);
    }
}

void port_DisableEXT_IRQ(void)
{
     NVIC_DisableIRQ(EXT_IRQ);
}

void port_EnableEXT_IRQ(void)
{
     NVIC_EnableIRQ(EXT_IRQ);
}

void port_SPIx_clear_chip_select(void)
{
    gpio_clear(DW1000_SPI_CS);
}

void port_SPIx_set_chip_select(void)
{
    gpio_set(DW1000_SPI_CS);
}

decaIrqStatus_t port_GetEXT_IRQStatus(void)
{
    return NVIC_GetPendingIRQ(EXT_IRQ);
}

int port_CheckEXT_IRQ( void)
{
   return gpio_read(DECAIRQ);
}
