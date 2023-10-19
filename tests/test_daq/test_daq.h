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
 * @brief       Test for daq management by inveter leg
 * @date        2020
 * @author      Hugues Larrive <hugues.larrive@laas.fr>
 */

#ifndef TEST_DAQ_H
#define TEST_DAQ_H

#include <stdlib.h>
#include "xtimer.h"

#include "../../modules/leg/leg.h"
#include "../../modules/daq/daq.h"

/**
 * @brief       daq_meas shell command handler
 * @verbatim
 usage: daq_meas <dev> <type>
    dev: device by number between 1 and 2
    type: measurement type :
        0: IH
        1: IL
        2: VL
        3: VH
        4: T
 @endverbatim         
 */
int test_daq_meas(int argc, char** argv);

#endif /* TEST_DAQ_H */
