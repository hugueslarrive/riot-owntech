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
 *
 //~ * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 //~ *       _                                             *
 //~ *    _ | | _                _______        _          *
 //~ *   / || || \              |__   __|      | |         *
 //~ *  / /`|_| \ \_        ___ __ | | ___  ___| |__       *
 //~ * ( (`  ``  )\ \  /\  / | '_ \| |' _ `/ __| '_ `      *
 //~ *  \ \ ___ / /\ \/  \/ /| |`| | | '__( (__| |`| |     *
 //~ *   \_______/` \__/\__/`|_| |_|_ \___,\___|_| |_|     *
 //~ *    ````````   ``` ```  ``  ```` ```` ``````  ``     *
 //~ * D i g i t a l i z i n g  P o w e r  T o g e t h e r *
 //~ * ``````````````````````````````````````````````````` *
 //~ * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
 
 /**
 * @file
 * @brief   PWM management layer by inverter leg
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 * 
 */

#include "leg.h"
#include "leg_conf.h"

static inline TIM_TypeDef *dev(pwm_t pwm)
{
    return pwm_config[pwm].dev;
}

void leg_init(void)
{
    /* Do the same as pwm_init() except compemantary output
     * management. */

    pwm_t pwm = PWM_DEV(0);
    pwm_mode_t mode = PWM_CENTER;
    uint32_t freq = LEG_FREQ;
    uint16_t res = LEG_RES * 2;
    uint32_t timer_clk = periph_timer_clk(pwm_config[0].bus);

    /* power on the used timer */
    periph_clk_en(pwm_config[pwm].bus, pwm_config[pwm].rcc_mask);
    /* reset configuration and CC channels */
    dev(pwm)->CR1 = 0;
    dev(pwm)->CR2 = 0;
    for (unsigned i = 0; i < TIMER_CHANNEL_NUMOF; ++i) {
        TIM_CHAN(pwm, i) = 0;
    }

    /* configure the used pins */
    unsigned i = 0;
    while ((i < TIMER_CHANNEL_NUMOF) && (pwm_config[pwm].chan[i].pin != GPIO_UNDEF)) {
        gpio_init(pwm_config[pwm].chan[i].pin, GPIO_OUT);
        gpio_init_af(pwm_config[pwm].chan[i].pin, pwm_config[pwm].af);
        i++;
    }

    /* configure the PWM frequency and resolution by setting the auto-reload
     * and prescaler registers */
    dev(pwm)->PSC = (timer_clk / (res * freq)) - 1;
    dev(pwm)->ARR = (mode == PWM_CENTER) ? (res / 2) : res - 1;

    /* prevent high output level on complementary outputs at
     * initialisation */
    dev(pwm)->CCR3 = 0;
    dev(pwm)->CCR4 = res;

    /* set PWM mode */
    dev(pwm)->CCMR1 = CCMR_COMP;
    dev(pwm)->CCMR2 = CCMR_COMP;
    /* center-aligned mode 3 */
    dev(pwm)->CR1 |= (TIM_CR1_CMS_0 | TIM_CR1_CMS_1);

    /* enable PWM outputs and start PWM generation */
#ifdef TIM_BDTR_MOE
    dev(pwm)->BDTR = TIM_BDTR_MOE;
#endif
    dev(pwm)->CCER = (TIM_CCER_CC1E | TIM_CCER_CC2E |
                      TIM_CCER_CC3E | TIM_CCER_CC4E);
    dev(pwm)->CR1 |= TIM_CR1_CEN;
}

void leg_set(uint8_t leg_dev, uint16_t duty_cycle)
{
    leg_dev--;
    if (duty_cycle < MIN_DT) duty_cycle = MIN_DT;
    if (duty_cycle > MAX_DT) duty_cycle = MAX_DT;
    if (duty_cycle > leg_config[leg_dev].duty_cycle) {
        /* set negative channel first */
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm),
            leg_config[leg_dev].neg_chan,
            duty_cycle + leg_config[leg_dev].dead_time / 2
        );
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm),
            leg_config[leg_dev].pos_chan,
            duty_cycle - leg_config[leg_dev].dead_time / 2
        );
    }
    else {
        /* set positive channel first */
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm),
            leg_config[leg_dev].pos_chan,
            duty_cycle - leg_config[leg_dev].dead_time / 2
        );
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm),
            leg_config[leg_dev].neg_chan,
            duty_cycle + leg_config[leg_dev].dead_time / 2
        );
    }
    /* save the duty-cycle */
    leg_config[leg_dev].duty_cycle = duty_cycle;
}

void buck_set(uint8_t leg_dev, uint16_t duty_cycle)
{
    leg_dev--;
    if (duty_cycle < MIN_DT) duty_cycle = MIN_DT;
    if (duty_cycle > MAX_DT) duty_cycle = MAX_DT;
    /* set the negative channel to zero */
    pwm_set(
        PWM_DEV(leg_config[leg_dev].pwm),
        leg_config[leg_dev].neg_chan,
        LEG_RES
    );
    /* set the positive channel only */
    pwm_set(
        PWM_DEV(leg_config[leg_dev].pwm),
        leg_config[leg_dev].pos_chan,
        duty_cycle - leg_config[leg_dev].dead_time / 2
    );
    /* save the duty-cycle */
    leg_config[leg_dev].duty_cycle = duty_cycle;
}

void leg_stop(uint8_t leg_dev)
{
        leg_dev--;
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm),
            leg_config[leg_dev].pos_chan,
            0
        );
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm),
            leg_config[leg_dev].neg_chan,
            LEG_RES
        );
}

uint8_t leg_numof(void)
{
    return LEG_NUMOF;
}
