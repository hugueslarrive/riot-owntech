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
 * @brief   stm32f334-discovery based 10v feeder for the OwnTech v0.9
 *          power converter
 *
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "../../hrtim/hrtim.h"

int main(void)
{
    uint32_t freq;
    uint16_t period;

    /** - Initialize the master timer, TIMA and TIMB at 250KHz frequency */
    freq = KHZ(250);
    hrtim_init_mstr(0, &freq);
    hrtim_init_tu(0, TIMA, &freq);
    period = hrtim_init_tu(0, TIMB, &freq);

    /** - Set 200ns dead-time for TIMB */
    hrtim_pwm_dt(0, TIMB, 200);

    /** - Enable counters */
    hrtim_cnt_en(0, (MCEN | TACEN | TBCEN));

    /** - Set TIMA output 1 high and output 2 low for boost operation */
    hrtim_set_cb_set(0, TIMA, OUT1, PER);
    hrtim_rst_cb_set(0, TIMA, OUT2, PER);
    hrtim_out_en(0, TIMA, (OUT1 | OUT2));

    /** - Set complementary outputs and 60% duty-cycle for TIMB */
    hrtim_cmpl_pwm_out(0, TIMB);
    hrtim_pwm_set(0, TIMB, (period * 0.6), 0);
    hrtim_out_en(0, TIMB, (OUT1 | OUT2));

    return 0;
}
