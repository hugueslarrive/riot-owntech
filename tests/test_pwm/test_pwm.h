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
 * @brief       Test for pwm management by inveter leg
 * @date        2020
 * @author      Hugues Larrive <hugues.larrive@laas.fr>
 */

#ifndef TEST_PWM_H
#define TEST_PWM_H

#include <stdlib.h>
#include "xtimer.h"

#include "../../modules/leg/leg.h"
#include "../../modules/sin/sin50hz.h"

/**
 * @brief       legset shell command handler
 * 
 * Set the duty cycle for the given leg. A 0 value call the leg_stop()
 * function.
 * 
 * @verbatim
 usage: legset <dev> <val> <pha>
    dev: device by number between 1 and 10
    val: duty cycle
    pha: phase shift
 @endverbatim         
 */
int test_pwm_legset(int argc, char** argv);

/**
 * @brief       sinadd shell command handler
 * 
 * Add legs to sin50hz thread. Signals are phase shifted by 180° if two
 * legs are provided, 120° if three.
 * 
 * @verbatim
 usage: sinadd <leg> <leg> [leg]
 	dev: device by number between 1 and 10
 @endverbatim         
 */
int test_pwm_sinadd(int argc, char** argv);

/**
 * @brief       sinrm shell command handler
 * 
 * Remove legs from sin50hz thread.
 * 
 * @verbatim
 usage: sinrm <leg> <leg> [leg]
    dev: device by number between 1 and 10
 @endverbatim         
 */
int test_pwm_sinrm(int argc, char** argv);

int test_pwm_getysc(int argc, char** argv);
int test_pwm_setysc(int argc, char** argv);
int test_pwm_setshift(int argc, char** argv);

int test_pwm_sin_bench(int argc, char** argv);

#endif /* TEST_PWM_H */
