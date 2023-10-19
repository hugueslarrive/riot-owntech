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

#ifndef ADC_H
#define ADC_H

#ifdef __ZEPHYR__
#include <zephyr.h>
#else /* RIOT */
#include <periph_cpu.h>
#include <xtimer.h>
//~ #include <periph/gpio.h>
#endif

#include "adc_enum.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes ADC1 and ADC2 driver with DMA transfert to adc_readings[]
 *
 * @note    Setup differential ended channels
 * @note    Start an 1 Khz adc trigger timer
 */
void adc_g4_init(void);

/**
 * @brief   Get raw values from an ADC input
 *
 * @param[in]   input   ADC input (see adc_enum.h)
 * @return              The raw ADC input value
 */
uint16_t adc_get_value(dt_adc_input_t input);

#ifndef __ZEPHYR__
static inline void k_usleep(uint32_t microseconds)
{
    xtimer_usleep(microseconds);
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* ADC_H */
