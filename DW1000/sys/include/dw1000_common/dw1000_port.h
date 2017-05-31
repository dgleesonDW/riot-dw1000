/*! ----------------------------------------------------------------------------
 * @file    dw1000_port.h
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


#ifndef DW1000_PORT_H_
#define DW1000_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dw1000_compiler.h"
#include "dw1000_device_api.h"

/*****************************************************************************************************************//*
 * To enable LDC update (i.e. printing/outputting range result to LCD set this to 1
 * NOTE: it takes 3.1 ms to output/write to LCD so this is limiting when performing fast ranging
 * (e.g. 6.8 with short slot times - wake up could be 2.25 ms, ranging exchange 1.65 ms and the LCD update 3.1 ms)
 */
#define LCD_UPDATE_ON (1)


/*****************************************************************************************************************//*
 * To enable Direct Memory Access for SPI set this option to (1)
 * This option will increase speed of spi transactions but it will use an extra RAM memory buffer
 */
#define DMA_ENABLE  (0)

/*****************************************************************************************************************//*
**/
#if (DMA_ENABLE == 1)
 #define writetospi     writetospi_dma
 #define readfromspi    readfromspi_dma
 void dma_init(void);
#else

 extern int writetospi_serial( uint16_t headerLength,
                    const uint8_t *headerBuffer,
                    uint32_t bodylength,
                    const uint8_t *bodyBuffer
                  );

 extern int readfromspi_serial( uint16_t    headerLength,
                     const uint8_t *headerBuffer,
                     uint32_t readlength,
                     uint8_t *readBuffer );

 #define writetospi     writetospi_serial
 #define readfromspi    readfromspi_serial
#endif

typedef enum
{
    LED_PC6,
    LED_PC7,
    LED_PC8,
    LED_PC9,
    LED_ALL,
    LEDn
} led_t;

void __weak process_deca_irq(void);

void SPI_ChangeRate(uint16_t scalingfactor);

void SPI_ConfigFastRate(uint32_t scalingfactor);

unsigned long portGetTickCnt(void);

#define portGetTickCount()          portGetTickCnt()

void reset_DW1000(void);
void setup_DW1000RSTnIRQ(int enable);

/**
 * @brief This function clears chip select line
 */
void port_SPIx_clear_chip_select(void);

/**
 * @brief This function set chip select line
 */
void port_SPIx_set_chip_select(void);

/**
 * @brief This function gets IRQ status
 */
decaIrqStatus_t  port_GetEXT_IRQStatus(void);

/**
 * @brief This function enables external IRQ
 */
void port_EnableEXT_IRQ(void);

/**
 * @brief This function disables external IRQ
 */
void port_DisableEXT_IRQ(void);

/**
 * @brief This function checks external IRQ status
 */
int  port_CheckEXT_IRQ( void);

#ifdef __cplusplus
}
#endif

#endif /* DW1000_PORT_H_ */
