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
 * @defgroup    owntech_modules_sin OwnTech's sin50hz module
 * @ingroup     owntech_modules
 * @brief       OwnTech 50Hz sine wave generator
 *
 * @{
 * @file
 * @brief   50Hz sine wave generator interface definitions
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 * 
 */

#ifndef SIN50HZ_H
#define SIN50HZ_H

#include "thread.h"
#include "xtimer.h"
#include "../leg/leg.h"

/**
 * @brief   A structure to make a bitmask of enabled legs for each
 *          possible phase shift.
 */
typedef struct {
    uint16_t legs0;
    uint16_t legs120;
    uint16_t legs180;
    uint16_t legs240;
} sin50hz_legs_t;

/**
 * @brief   Thread handler that change the duty cycle at 3kHz sample rate
 * to build 50Hz sine waves (60 samples/periode).
 */
void *sin50hz_handler(void *arg);

/**
 * @brief   Start the thread
 * 
 * @param[in]  priority     the thread priority
 */
void sin50hz_init(char priority);

/**
 * @brief   Add 2 legs to the sin50hz thread
 *
 * @param[in]   leg1    The first leg to add
 * @param[in]   leg2    The second leg to add
 */
void sin50hz_add2p(uint8_t leg1, uint8_t leg2);

/**
 * @brief   Remove 2 legs from the sin50hz thread
 *
 * @param[in]   leg1    The first leg to remove
 * @param[in]   leg2    The second leg to remove
 */
void sin50hz_rm2p(uint8_t leg1, uint8_t leg2);

/**
 * @brief   yscale getter
 * 
 * @return  the current yscale value
 */ 
uint16_t sin50hz_get_yscale(void);

/**
 * @brief   yscale setter
 * 
 * @param[in]   scale 
 */
void sin50hz_set_yscale(uint16_t scale);

/**
 * @brief   shift setter
 */
void sin50hz_set_shift(uint16_t shift);

//~ void sin50hz_add3p(uint8_t leg1, uint8_t leg2, uint8_t leg3);
//~ void sin50hz_rm3p(uint8_t leg1, uint8_t leg2, uint8_t leg3);

#endif /* SIN50HZ_H */
/** @} */
