@page boards Enhanced board configurations

# Enhanced board configurations


# nucleo-f334r8

Custom ADC config for tests_v0.9.


# nucleo-f746zg

- use TIM9 instead of TIM2 or TIM5 for xtimer
- 3 SPI DMA streams config
- 12 additional PWM channels using TIM2, TIM3, TIM5, and TIM8
- TIM9 config
- USART6 and USART2 config removed, USART3 dma_chan modified
- SPI1 config removed, SPI2, SPI3 and SPI5 configs added
- SPI4 sclk_pin, tx_dma and rx_dma modified
- ETH dma modified


# stm32f334-disco

A "work in progress" riot support for the stm32f334 discovery board.

The ADC feature must be added before PR.
