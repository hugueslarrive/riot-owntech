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
 * @brief   RIOT based firmware for the OwnWall power converter board
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "shell.h"
#include "../../modules/ot_shell_commands/ot_shell_commands.h"

#include "xtimer.h"

#include "../../modules/daq/daq.h"

int main(void)
{
    /** - Initializes the pwm management layer by inverter leg */
    leg_init();

    /** - Initializes the DAQ's SPI CS GPIO pins */
    daq_init();

    /** - Launch the measure thread */
    measure_init(THREAD_PRIORITY_MAIN - 2);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
