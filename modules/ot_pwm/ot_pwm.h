/*
 * Copyright (c) 2020 LAAS-CNRS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * @brief   Workaround for complementary pwm output initialization
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#ifndef OT_PWM_H
#define OT_PWM_H

#include "periph_cpu.h"
#include "periph/pwm.h"

#define CCMR_NORM           (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | \
                             TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2)
#define CCMR_COMP           (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | \
                             TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | \
                             TIM_CCMR1_OC2M_2)

/**
 * @brief   Initialize a PWM device
 *
 * The PWM module is based on virtual PWM devices, which can have one or more
 * channels. The PWM devices can be configured to run with a given frequency and
 * resolution, which are always identical for the complete device, hence for
 * every channel on a device.
 *
 * The desired frequency and resolution may not be possible on a given device
 * when chosen too large. In this case the PWM driver will always keep the
 * resolution and decrease the frequency if needed. To verify the correct
 * settings compare the returned value which is the actually set frequency.
 *
 * @param[in] dev           PWM device to initialize
 * @param[in] mode          PWM mode, left, right or center aligned
 * @param[in] freq          PWM frequency in Hz
 * @param[in] res           PWM resolution
 *
 * @return                  actual PWM frequency on success
 * @return                  0 on error
 * 
 * @note    Right aligned mode work as expected.
 * @note    In center aligned mode, the odd channels polarity is inverted.
 */
uint32_t ot_pwm_init(unsigned dev, pwm_mode_t mode, uint32_t freq, uint16_t res);

#endif /* OT_PWM_H */
