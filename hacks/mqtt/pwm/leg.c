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
 * @brief   PWM management layer by inverter leg
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "leg.h"
#include "leg_conf.h"

void leg_init(void)
{
    for (uint8_t i = 0; i < PWM_NUMOF; i++) {
        ot_pwm_init(PWM_DEV(i), PWM_CENTER, LEG_FREQ, LEG_RES);
    }    
}

void leg_set(uint8_t leg_dev, uint16_t duty_cycle)
{
    leg_dev--;
    if (duty_cycle > leg_config[leg_dev].duty_cycle) {
        /* set negative channel first */
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm), leg_config[leg_dev].neg_chan,
            duty_cycle + leg_config[leg_dev].dead_time / 2
        );
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm), leg_config[leg_dev].pos_chan,
            duty_cycle - leg_config[leg_dev].dead_time / 2
        );
    }
    else {
        /* set positive channel first */
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm), leg_config[leg_dev].pos_chan,
            duty_cycle - leg_config[leg_dev].dead_time / 2
        );
        pwm_set(
            PWM_DEV(leg_config[leg_dev].pwm), leg_config[leg_dev].neg_chan,
            duty_cycle + leg_config[leg_dev].dead_time / 2
        );
    }
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
