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
 * @ingroup     owntech_modules_daq
 *
 * @{
 * @file
 * @brief   Measures data structures
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#ifndef MEASURES_STRUCT_H
#define MEASURES_STRUCT_H

#include <stdint.h>

/**
 * @brief   Measures data structure
 */
typedef struct {
    int16_t vh;
    int16_t ih;
    int16_t vl1;
    int16_t il1;
    int16_t vl2;
    int16_t il2;
    int16_t t;
    uint16_t dc1;
    uint16_t dc2;
} measures_t;

/**
 * @brief   Measures acumulator data structure
 */
typedef struct {
    int32_t vh;
    int32_t ih;
    int32_t vl1;
    int32_t il1;
    int32_t vl2;
    int32_t il2;
    int32_t t;
    uint32_t dc1;
    uint32_t dc2;
    uint32_t time;
} measures_ac_t;

#endif /* MEASURES_STRUCT_H */
/** @} */
