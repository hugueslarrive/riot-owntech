/*
 * Copyright (C) 2017 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_nucleo-f746zg
 * @{
 *
 * @file
 * @brief       Peripheral MCU configuration for the nucleo-f746zg board
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

/* This board provides an LSE */
#ifndef CONFIG_BOARD_HAS_LSE
#define CONFIG_BOARD_HAS_LSE    1
#endif

/* This board provides an HSE */
#ifndef CONFIG_BOARD_HAS_HSE
#define CONFIG_BOARD_HAS_HSE    1
#endif

#include "periph_cpu.h"
#include "f2f4f7/cfg_clock_default_216.h"
#include "cfg_i2c1_pb8_pb9.h"
#include "cfg_rtt_default.h"
//~ #include "cfg_timer_tim2.h"     /* tim2 and tim5 are used for pwm     */
                                    /* but tim9 was configured for xtimer */
#include "cfg_usb_otg_fs.h"
#include "mii.h"

/**
 * @note    xtimer will use tim9 wich is only 16 bit timer
 */
#define XTIMER_WIDTH (16)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    DMA streams configuration
 * @{
 */
#ifdef MODULE_PERIPH_DMA
/* ref manual p. 226 */
static const dma_conf_t dma_config[] = {
    { .stream = 3 },   /* DMA1 Stream 3 - SPI2_RX(ch0)/USART3_TX(Ch4) */
    { .stream = 4 },   /* DMA1 Stream 4 - SPI2_TX(ch0)                */
    { .stream = 0 },   /* DMA1 Stream 0 - SPI3_RX(ch0)                */
    { .stream = 5 },   /* DMA1 Stream 5 - SPI3_TX(ch0)                */
    { .stream = 11 },  /* DMA2 Stream 3 - SPI4_RX(ch5)/SPI5_RX(ch2)   */
    { .stream = 12 },  /* DMA2 Stream 4 - SPI4_TX(ch5)/SPI5_TX(ch2)   */
    { .stream = 8 },   /* DMA2 Stream 0 - ETH_TX                      */
};

#define DMA_0_ISR  isr_dma1_stream3
#define DMA_1_ISR  isr_dma1_stream4
#define DMA_2_ISR  isr_dma1_stream0
#define DMA_3_ISR  isr_dma1_stream5
#define DMA_4_ISR  isr_dma2_stream3
#define DMA_5_ISR  isr_dma2_stream4
#define DMA_6_ISR  isr_dma2_stream0

#define DMA_NUMOF           ARRAY_SIZE(dma_config)
#endif
/** @} */

/**
 * @name    PWM configuration
 * @{
 */
static const pwm_conf_t pwm_config[] = {
    {
        .dev      = TIM1,
        .rcc_mask = RCC_APB2ENR_TIM1EN,
        .chan     = { { .pin = GPIO_PIN(PORT_E,  9) /* D6  CN10-4  */, .cc_chan = 0},
                      { .pin = GPIO_PIN(PORT_E, 11) /* D5  CN10-6  */, .cc_chan = 1},
                      { .pin = GPIO_PIN(PORT_E, 13) /* D3  CN10-10 */, .cc_chan = 2},
                      { .pin = GPIO_PIN(PORT_E, 14) /* D38 CN10-28 */, .cc_chan = 3} },
        .af       = GPIO_AF1,
        .bus      = APB2
    },
    {
        .dev      = TIM2,
        .rcc_mask = RCC_APB1ENR_TIM2EN,
        .chan     = { { .pin = GPIO_PIN(PORT_B, 10) /* D36 CN10-32 */, .cc_chan = 2},
                      { .pin = GPIO_PIN(PORT_B, 11) /* D35 CN10-34 */, .cc_chan = 3},
                      { .pin = GPIO_UNDEF,                             .cc_chan = 0},
                      { .pin = GPIO_UNDEF,                             .cc_chan = 0} },
        .af       = GPIO_AF1,
        .bus      = APB1
     },
    {
        .dev      = TIM3,
        .rcc_mask = RCC_APB1ENR_TIM3EN,
        .chan     = { { .pin = GPIO_PIN(PORT_A,  6) /* D12  CN7-12 */, .cc_chan = 0},
                      { .pin = GPIO_PIN(PORT_B,  5) /* D22  CN7-13 */, .cc_chan = 1},
                      { .pin = GPIO_PIN(PORT_B,  0) /* D33 CN10-31 */, .cc_chan = 2},
                      { .pin = GPIO_PIN(PORT_B,  1) /* A6  CN10-7  */, .cc_chan = 3} },
        .af       = GPIO_AF2,
        .bus      = APB1
     },
    {
        .dev      = TIM4,
        .rcc_mask = RCC_APB1ENR_TIM4EN,
        .chan     = { { .pin = GPIO_PIN(PORT_D, 12) /* D29 CN10-21 */, .cc_chan = 0},
                      { .pin = GPIO_PIN(PORT_D, 13) /* D28 CN10-19 */, .cc_chan = 1},
                      { .pin = GPIO_PIN(PORT_D, 14) /* D10  CN7-16 */, .cc_chan = 2},
                      { .pin = GPIO_PIN(PORT_D, 15) /* D9   CN7-18 */, .cc_chan = 3} },
        .af       = GPIO_AF2,
        .bus      = APB1
     },
    {
        .dev      = TIM5,
        .rcc_mask = RCC_APB1ENR_TIM5EN,
        .chan     = { { .pin = GPIO_PIN(PORT_A,  0) /* D32 CN10-29 */, .cc_chan = 0},
                      { .pin = GPIO_PIN(PORT_A,  3) /* A0   CN9-1  */, .cc_chan = 3},
                      { .pin = GPIO_UNDEF,                             .cc_chan = 0},
                      { .pin = GPIO_UNDEF,                             .cc_chan = 0} },
        .af       = GPIO_AF2,
        .bus      = APB1
     },
    {
        .dev      = TIM8,
        .rcc_mask = RCC_APB2ENR_TIM8EN,
        .chan     = { { .pin = GPIO_PIN(PORT_C,  6) /* D16  CN7-1  */, .cc_chan = 0},
                      { .pin = GPIO_PIN(PORT_C,  7) /* D21  CN7-11 */, .cc_chan = 1},
                      { .pin = GPIO_PIN(PORT_B, 15) /* D17  CN7-3  */, .cc_chan = 2},   /** Inversé */
                      { .pin = GPIO_PIN(PORT_C,  9) /* D44  CN8-4  */, .cc_chan = 3} },
        .af       = GPIO_AF3,
        .bus      = APB2
    },
};

#define PWM_NUMOF           ARRAY_SIZE(pwm_config)
/** @} */

/**
 * @name    Timer configuration
 * @{
 */
static const timer_conf_t timer_config[] = {
    {
        .dev      = TIM9,
        .max      = 0x0000ffff,
        .rcc_mask = RCC_APB2ENR_TIM9EN,
        .bus      = APB2,
        .irqn     = TIM1_BRK_TIM9_IRQn
    }
};

#define TIMER_0_ISR         isr_tim1_brk_tim9

#define TIMER_NUMOF         ARRAY_SIZE(timer_config)
/** @} */

/**
 * @name    UART configuration
 * @{
 */
static const uart_conf_t uart_config[] = {
    {
        .dev        = USART3,
        .rcc_mask   = RCC_APB1ENR_USART3EN,
        .rx_pin     = GPIO_PIN(PORT_D, 9),
        .tx_pin     = GPIO_PIN(PORT_D, 8),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB1,
        .irqn       = USART3_IRQn,
#ifdef MODULE_PERIPH_DMA
        .dma        = 0,
        .dma_chan   = 4
#endif
    }
};

#define UART_0_ISR          (isr_usart3)

#define UART_NUMOF          ARRAY_SIZE(uart_config)
/** @} */

/**
 * @name   SPI configuration
 * @{
 */
static const spi_conf_t spi_config[] = {
    {
        .dev      = SPI2,
        .mosi_pin = GPIO_PIN(PORT_C, 3),
        .miso_pin = GPIO_PIN(PORT_C, 2),
        .sclk_pin = GPIO_PIN(PORT_D, 3),
        .cs_pin   = GPIO_UNDEF,
        .mosi_af  = GPIO_AF5,
        .miso_af  = GPIO_AF5,
        .sclk_af  = GPIO_AF5,
        .cs_af    = GPIO_AF5,
        .rccmask  = RCC_APB1ENR_SPI2EN,
        .apbbus   = APB1,
#ifdef MODULE_PERIPH_DMA
        .tx_dma   = 1,
        .tx_dma_chan = 0,
        .rx_dma   = 0,
        .rx_dma_chan = 0,
#endif
    },
    {
        .dev      = SPI3,
        .mosi_pin = GPIO_PIN(PORT_D, 6),
        .miso_pin = GPIO_PIN(PORT_C, 11),
        .sclk_pin = GPIO_PIN(PORT_C, 10),
        .cs_pin   = GPIO_UNDEF,
        .mosi_af  = GPIO_AF5,
        .miso_af  = GPIO_AF6,
        .sclk_af  = GPIO_AF6,
        .cs_af    = GPIO_AF5,
        .rccmask  = RCC_APB1ENR_SPI3EN,
        .apbbus   = APB1,
#ifdef MODULE_PERIPH_DMA
        .tx_dma   = 3,
        .tx_dma_chan = 0,
        .rx_dma   = 2,
        .rx_dma_chan = 0,
#endif
    },
    {
        .dev      = SPI4,
        .mosi_pin = GPIO_PIN(PORT_E, 6),
        .miso_pin = GPIO_PIN(PORT_E, 5),
        .sclk_pin = GPIO_PIN(PORT_E, 12),
        .cs_pin   = GPIO_UNDEF,
        .mosi_af  = GPIO_AF5,
        .miso_af  = GPIO_AF5,
        .sclk_af  = GPIO_AF5,
        .cs_af    = GPIO_AF5,
        .rccmask  = RCC_APB2ENR_SPI4EN,
        .apbbus   = APB2,
#ifdef MODULE_PERIPH_DMA
        .tx_dma   = 5,
        .tx_dma_chan = 5,
        .rx_dma   = 4,
        .rx_dma_chan = 5,
#endif
    },
    {
        .dev      = SPI5,
        .mosi_pin = GPIO_PIN(PORT_F, 11),
        .miso_pin = GPIO_PIN(PORT_F, 8),
        .sclk_pin = GPIO_PIN(PORT_F, 7),
        .cs_pin   = GPIO_UNDEF,
        .mosi_af  = GPIO_AF5,
        .miso_af  = GPIO_AF5,
        .sclk_af  = GPIO_AF5,
        .cs_af    = GPIO_AF5,
        .rccmask  = RCC_APB2ENR_SPI5EN,
        .apbbus   = APB2,
#ifdef MODULE_PERIPH_DMA
        .tx_dma   = 5,
        .tx_dma_chan = 2,
        .rx_dma   = 4,
        .rx_dma_chan = 2,
#endif
    }
};

#define SPI_NUMOF           ARRAY_SIZE(spi_config)
/** @} */

/**
 * @name ETH configuration
 * @{
 */
static const eth_conf_t eth_config = {
    .mode = RMII,
    .addr = { 0 },
    .speed = MII_BMCR_SPEED_100 | MII_BMCR_FULL_DPLX,
    .dma = 6,
    .dma_chan = 8,
    .phy_addr = 0x00,
    .pins = {
        GPIO_PIN(PORT_G, 13),
        GPIO_PIN(PORT_B, 13),
        GPIO_PIN(PORT_G, 11),
        GPIO_PIN(PORT_C, 4),
        GPIO_PIN(PORT_C, 5),
        GPIO_PIN(PORT_A, 7),
        GPIO_PIN(PORT_C, 1),
        GPIO_PIN(PORT_A, 2),
        GPIO_PIN(PORT_A, 1),
    }
};

#define ETH_DMA_ISR        isr_dma2_stream0

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
/** @} */
