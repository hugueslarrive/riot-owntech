/*
 * sin50hz.h
 * 
 * Created on: 8 jun. 2020
 * 
 * Copyright 2020 hugues <hugues.larrive@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
/**
 * @file
 * @brief   50Hz sine wave generator
 * @date    2020
 * @author  Hugues Larrive <hlarrive@laas.fr>
 * 
 */

#ifndef SIN50HZ_H
#define SIN50HZ_H

#include "thread.h"
#include "xtimer.h"
#include "leg.h"

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

void sin50hz_add2p(uint8_t leg1, uint8_t leg2);
void sin50hz_rm2p(uint8_t leg1, uint8_t leg2);
void sin50hz_add3p(uint8_t leg1, uint8_t leg2, uint8_t leg3);
void sin50hz_rm3p(uint8_t leg1, uint8_t leg2, uint8_t leg3);

#endif /* SIN50HZ_H */
