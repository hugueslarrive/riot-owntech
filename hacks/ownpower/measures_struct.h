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
 *
 //~ * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 //~ *       _                                             *
 //~ *    _ | | _                _______        _          *
 //~ *   / || || \              |__   __|      | |         *
 //~ *  / /`|_| \ \_        ___ __ | | ___  ___| |__       *
 //~ * ( (`  ``  )\ \  /\  / | '_ \| |' _ `/ __| '_ `      *
 //~ *  \ \ ___ / /\ \/  \/ /| |`| | | '__( (__| |`| |     *
 //~ *   \_______/` \__/\__/`|_| |_|_ \___,\___|_| |_|     *
 //~ *    ````````   ``` ```  ``  ```` ```` ``````  ``     *
 //~ * D i g i t a l i z i n g  P o w e r  T o g e t h e r *
 //~ * ``````````````````````````````````````````````````` *
 //~ * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
 
/**
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
    uint16_t vh;
    uint16_t ih;
    uint16_t vl;
    uint16_t il;
    uint16_t t;
    uint16_t dc;
} measures_t;

/**
 * @brief   Measures acumulator data structure
 */
typedef struct {
    uint32_t vh;
    uint32_t ih;
    uint32_t vl;
    uint32_t il;
    uint32_t t;
    uint32_t dc;
    uint32_t time;
} measures_ac_t;

#endif /* MEASURES_STRUCT_H */
