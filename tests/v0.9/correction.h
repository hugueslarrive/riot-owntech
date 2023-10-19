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
 * @brief   Correction
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#ifndef CORRECTION_H
#define CORRECTION_H

#include "../../modules/daq/measures_struct.h"
#include "../../modules/leg/leg.h"
#include "../../modules/sin/sin50hz.h"

#define VH_BREAK        (696)              /**< 21.6V / 0,031 */
#define VH_START_MIN    (774)              /**< 24V / 0,031 */
#define VL_MAX          (856)              /**< 13.7V / 0,016 */
#define VL_BREAK        (887)              /**< 14.2V / 0,016 */
#define IL_MIN          (2048)             /**< 0A */
#define IH_BREAK        (2068)             /**< 0A */
#define IL_MAX          (IL_MIN + 500)     /**< 5A / 0,01 */
#define IL_BREAK        (IL_MIN + 550)     /**< 5.5A / 0,01 */
#define T_BREAK         (500)              /**< 50°C / 0,1 */
#define T_START_MAX     (T_BREAK - 100)    /**< 40°C / 0,1 */     
/*
 * DC = VL / VH
 * DCmin = VLmin / VHmax = 10,8 / 28,2 = 0,383
 * DC_MIN = 0,383 * LEG_RES = 0,383 * 320 = 122
 * DCmax = VLmax / VHmax = 14,1 / 21,6 = 0,653
 * DC_MAX = 0,653 * LEG_RES = 0,653 * 320 = 208
 */
#define DC_MIN      (uint16_t)(LEG_RES * 0.383) /**< 320 * 10,8 / 28,2 */
#define DC_MAX      (uint16_t)(LEG_RES * 0.653) /**< 320 * 14,1 / 21,6 */

/**
 * @brief   Slow breaker, called by the measure_transmit thread every
 *          41ms and actually used for overheat protection.
 *
 * @param[in]  ac   the measure accumulators structure
 */
void breaker_41ms(measures_ac_t ac);

/**
 * @brief   Called by the measure_transmit thread every 41ms, (re)starts
 *          the converter when the safety conditions are met
 *
 * @param[in]  ac   the measure accumulators structure
 */
void starter(measures_ac_t ac);

/**
 * @brief   Called by the measure thread every 160µs, tracks the maximum
 *          charge current.
 *
 * @param[in]   m   the measure structure
 * @param[out]  dc  the new duty-cycle
 */
uint16_t correction(measures_t m);

#endif /* CORRECTION_H */
