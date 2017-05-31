/*! ----------------------------------------------------------------------------
 * @file    port.c
 * @brief   HW specific definitions and functions for portability
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author DecaWave
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
