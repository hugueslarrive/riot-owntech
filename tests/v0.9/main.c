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
 * @brief   RIOT based firmware for the OwnTech v0.9 power converter board
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "shell.h"
#include "../../tests/test_pwm/test_pwm.h"
#include "../../tests/test_daq/test_daq.h"
#include "../../modules/ot_shell_commands/ot_shell_commands.h"

#include "xtimer.h"

//~ #include "daq_conf.h"
#include "../../modules/daq/daq.h"
#include "../../modules/sin/sin50hz.h"

int main(void)
{
    /** - Initializes the pwm management layer by inverter leg */
    leg_init();

    /** - Neutral - GND shunt */
    gpio_init(GPIO_PIN(PORT_A, 0), GPIO_OUT);
    gpio_set(GPIO_PIN(PORT_A, 0));

    /** - Initializes analog pins for negative inputs of differential
      * ADC's channels and enable differential mode for the
      * corresponding channels then initializes all available ADC lines.
      */
    daq_init();

    /** - Launch the measure thread */
    //~ measure_init(THREAD_PRIORITY_MAIN - 4);

    /* There is a periodic swithing noise with aprox 12 oscillations per
     * 200KHz period of 23040 so we can make it auto-attenuative if we
     * phase-shifting one leg by 180° of 23040 / 12 so 23040 / 12 / 2 = 960 */
    //~ sin50hz_set_shift(960);

    /** - Launch the sin50hz thread */
    sin50hz_init(THREAD_PRIORITY_MAIN - 6);
    
    /** - Power script **/
    sin50hz_add2p(1, 2);
    sin50hz_set_yscale(16128);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
