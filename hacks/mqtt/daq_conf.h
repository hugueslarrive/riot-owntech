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
 * @brief   Legs array definition
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 * 
 * @note    This must only be included in daq.c
 */

#ifndef DAQ_CONF_H
#define DAQ_CONF_H

static daq_leg_meas_t leg_meas[][5] = {
    {   // R
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 6 }, // IH
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 5 }, // IL
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 5), .cha = 0 }, // VL
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 7 }, // VH
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 0 }  // T
    },
    {   // S
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 6 }, // IH
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 4 }, // IL
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 5), .cha = 1 }, // VL
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 7 }, // VH
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 2 }  // T
    },
    {   // T
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 6 }, // IH
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 3 }, // IL
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 5), .cha = 2 }, // VL
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 7 }, // VH
        { .bus = 0, .cs = GPIO_PIN(PORT_D, 4), .cha = 1 }  // T
    },
};
#define LEG_MEAS_NUMOF          ARRAY_SIZE(leg_meas)    /**< The number of configured legs */

#endif /* DAQ_CONF_H */
