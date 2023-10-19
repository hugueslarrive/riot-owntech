/*
 * Copyright (c) 2021 Centre National de la Recherche Scientifique
 *
 * CNRS, établissement public à caractère scientifique et technologique,
 * dont le siège est situé 3 rue Michel-Ange, 75794 Paris cedex 16.
 *
 *            Luiz Villa - Projet OwnTech <owntech@laas.fr>
 *        Laboratoire d'analyse et d'architecture des systèmes
 *               LAAS-CNRS - 7, avenue du Colonel Roche
 *                 BP 54200 - 31031 Toulouse cedex 4
 *
 * SPDX-License-Identifier: CECILL-C
 */

/**
 * @brief   ADC driver for stm32g474re
 *
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "adc.h"
#include "adc_conf.h"

static char stack_adc_trigger[THREAD_STACKSIZE_DEFAULT];

static inline void _adc_init(ADC_TypeDef *adc)
{
    uint8_t channel, rank = 1;
    uint8_t num_ch = (adc == ADC1) ? num_adc1_ch : num_adc2_ch;

    /* enable clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

    /* synchronous clock mode */
        /* Fadc_clk = 170 / 4 = 42.5 MHz (must be < 60 MHz) */
    ADC12_COMMON->CCR |= ADC_CCR_CKMODE_1 | ADC_CCR_CKMODE_0;

    /* disable deep power down */
    adc->CR &= ~ADC_CR_DEEPPWD;

    /* enable internal regulator */
    adc->CR |= ADC_CR_ADVREGEN;

    /* wait for ADC voltage regulator start-up time (20us for g474) */
    //~ k_busy_wait(200);
    //~ k_usleep(20);
    xtimer_usleep(30);

    /* single ended calibration */
    adc->CR &= ~ADC_CR_ADCALDIF;
    adc->CR |= ADC_CR_ADCAL;
    while (adc->CR & ADC_CR_ADCAL) {}

    /* if differential calibration is done immediately after single ended
     * one, the calibration auto-injection will not be done but 4us wait
     * before is sufficient to fix this bug */ 
    //~ k_busy_wait(4);

    /* differential ended calibration */
    adc->CR |= ADC_CR_ADCALDIF;
    adc->CR |= ADC_CR_ADCAL;
    while (adc->CR & ADC_CR_ADCAL) {}
//~ printf("CALFAC_D = %ld  CALFAC_S = %ld\n", adc->CALFACT >> 16, adc->CALFACT & 0xff);
    /* enable internal reference voltage and temperature */
    ADC12_COMMON->CCR |= ADC_CCR_VREFEN | ADC_CCR_VSENSESEL;

    /* set regular sequencer and channels sampling time */
    for (int i = 0; i < NUM_ADC_INPUTS; i++) {
        channel = adc_config[i].chan;
        /* regular sequencer */
        if (adc_config[i].dev == adc) {
            switch (rank) {
                case 1: adc->SQR1 |= channel << ADC_SQR1_SQ1_Pos; break;
                case 2: adc->SQR1 |= channel << ADC_SQR1_SQ2_Pos; break;
                case 3: adc->SQR1 |= channel << ADC_SQR1_SQ3_Pos; break;
                case 4: adc->SQR1 |= channel << ADC_SQR1_SQ4_Pos; break;
                case 5: adc->SQR2 |= channel << ADC_SQR2_SQ5_Pos; break;
                case 6: adc->SQR2 |= channel << ADC_SQR2_SQ6_Pos; break;
                case 7: adc->SQR2 |= channel << ADC_SQR2_SQ7_Pos; break;
                case 8: adc->SQR2 |= channel << ADC_SQR2_SQ8_Pos; break;
                case 9: adc->SQR2 |= channel << ADC_SQR2_SQ9_Pos; break;
                case 10: adc->SQR3 |= channel << ADC_SQR3_SQ10_Pos; break;
                case 11: adc->SQR3 |= channel << ADC_SQR3_SQ11_Pos; break;
                case 12: adc->SQR3 |= channel << ADC_SQR3_SQ12_Pos; break;
                case 13: adc->SQR3 |= channel << ADC_SQR3_SQ13_Pos; break;
                case 14: adc->SQR3 |= channel << ADC_SQR3_SQ14_Pos; break;
                case 15: adc->SQR4 |= channel << ADC_SQR4_SQ15_Pos; break;
                case 16: adc->SQR4 |= channel << ADC_SQR4_SQ16_Pos; break;
            }
            rank++;
            /* channels sampling time
             */
                /* 000: 2.5 ADC clock cycles
                 * 001: 6.5 ADC clock cycles
                 * 010: 12.5 ADC clock cycles
                 * 011: 24.5 ADC clock cycles
                 * 100: 47.5 ADC clock cycles
                 * 101: 92.5 ADC clock cycles
                 * 110: 247.5 ADC clock cycles
                 * 111: 640.5 ADC clock cycles
                 */
                /* Vrefint minimum sampling time : 4us
                 */
                /* Vts minimum sampling time : 5us
                 */
                /* Tadc_clk = 1 / 42.5 MHz = 23.5 ns
                 * Tsar = 12.5 * Tadc_clk = 293.75 ns
                 * Tsmpl = 247.5 * Tadc_clk = 5816.25 ns
                 * Tconv = Tsmpl + Tsar = 6.11 us
                 * -> Fconv up to 163.6 KSPS for 1 channel per ADC
                 * Fconv up to 27.2 KSPS with the 6 channels actally
                 * used on the ADC1 */
            if (channel < 10) adc->SMPR1 |= (0b110 << (3 * channel));
            else adc->SMPR2 |= (0b110 << (3 * (channel - 10)));
        }
    }

    /* set regular sequence lenght */
    adc->SQR1 |= (num_ch - 1);

    /* enable dma and circular mode */
    adc->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;

    /* enable adc and wait for it to be ready */
    adc->CR |= ADC_CR_ADEN;
    while ((adc->ISR & ADC_ISR_ADRDY) == 0) {}
}

static inline void _dmamux_init(DMA_TypeDef *dma)
{
    /* DMAMUX channels 0 to 7 are connected to DMA1 channels 0 to 7
     * DMAMUX channels 8 to 15 are connected to DMA2 channels 0 to 7
     * DMAREQ_ID (refman table 91):
     *       5: ADC1
     *      36: ADC2 */

    /* enable clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMAMUX1EN;

    /* configure dma request IDs */
    switch ((uint32_t)dma) {
        case (uint32_t)DMA1: DMAMUX1_Channel0->CCR |= 5U; break;
        case (uint32_t)DMA2: DMAMUX1_Channel8->CCR |= 36U; break;
    }
}

static inline void _dma_init(DMA_Channel_TypeDef* dma_channel)
{
    if (dma_channel == DMA1_Channel1) {
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; // enable clock
        dma_channel->CPAR = (uint32_t)(&(ADC1->DR));  // peripheral address
        dma_channel->CMAR = (uint32_t)(&(adc1_readings[0])); // memory address
        dma_channel->CNDTR = num_adc1_ch; // number of data to transfer
    }
    else if (dma_channel == DMA2_Channel1) {
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
        dma_channel->CPAR = (uint32_t)(&(ADC2->DR));
        dma_channel->CMAR = (uint32_t)(&(adc2_readings[0]));
        dma_channel->CNDTR = num_adc2_ch;
    }

    dma_channel->CCR |=
        ( DMA_CCR_MSIZE_0   // memory size: 16 bits
        | DMA_CCR_PSIZE_0   // peripheral size: 16 bits
        | DMA_CCR_MINC      // memory increment mode enabled
        | DMA_CCR_CIRC      // circular mode enabled
        | DMA_CCR_TEIE      // transfer error interrupt enable
        | DMA_CCR_TCIE      // transfer complete interrupt enable
        | DMA_CCR_EN );     // channel enable
}

//~ static inline void _adc_trigger_conversion(struct k_timer *timer_id)
//~ {
    //~ ADC1->CR |= ADC_CR_ADSTART;
    //~ ADC2->CR |= ADC_CR_ADSTART;
//~ }

void *adc_trigger_handler(void *arg)
{
    (void)arg;

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        ADC1->CR |= ADC_CR_ADSTART;
        ADC2->CR |= ADC_CR_ADSTART;

        xtimer_periodic_wakeup(&last_wakeup, 50);
    }

    return NULL;
}

void adc_g4_init(void)
{
    //~ static struct k_timer adc_trigger_timer;
    uint32_t channel;

    /* differential channels setup */
    for (int i = 0; i < NUM_ADC_INPUTS; i++) {
        channel = adc_config[i].chan;
        if (adc_config[i].diff) {
            if (adc_config[i].dev == ADC1) {
                 ADC1->DIFSEL |= (1 << channel);
            }
            if (adc_config[i].dev == ADC2) {
                 ADC2->DIFSEL |= (1 << channel);
            }
        }
    }

    _adc_init(ADC1);
    _adc_init(ADC2);
    _dmamux_init(DMA1);
    _dma_init(DMA1_Channel1);
    _dmamux_init(DMA2);
    _dma_init(DMA2_Channel1);

    //~ k_timer_init(&adc_trigger_timer, _adc_trigger_conversion, NULL);
    //~ k_timer_start(&adc_trigger_timer, K_USEC(50), K_USEC(50));        // 20 kHz

    thread_create(stack_adc_trigger,
                    sizeof(stack_adc_trigger),
                    THREAD_PRIORITY_MAIN - 1,
                    THREAD_CREATE_STACKTEST,
                    adc_trigger_handler, NULL,
                    "adc_trigger");

    k_usleep(1000);      // wait for ADC to collect some measurement values
}

uint16_t adc_get_value(dt_adc_input_t input)
{
    return (adc_config[input].dev == ADC1)
        ? adc1_readings[adc_config[input].idx]
        : adc2_readings[adc_config[input].idx];
}

