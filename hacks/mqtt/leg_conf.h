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
 * @ingroup     owntech_modules_leg
 * @file
 *
 * @{
 * @brief   leg_config array definition
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 * 
 * @note    This must only be included in leg.c
 */

#ifndef LEG_CONF_H
#define LEG_CONF_H

/**
 * @brief   Inverter leg configuration
 */
static leg_conf_t leg_config[] = {
    {   .pwm = 0,       .pos_chan  = 0,             .neg_chan = 1,
        .dev = TIM1,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 0,       .pos_chan  = 2,             .neg_chan = 3,
        .dev = TIM1,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 1,       .pos_chan  = 0,             .neg_chan = 1,
        .dev = TIM2,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 2,       .pos_chan  = 0,             .neg_chan = 1,
        .dev = TIM3,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 2,       .pos_chan  = 2,             .neg_chan = 3,
        .dev = TIM3,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 3,       .pos_chan  = 0,             .neg_chan = 1, 
        .dev = TIM4,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 3,       .pos_chan  = 2,             .neg_chan = 3, 
        .dev = TIM4,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 4,       .pos_chan  = 0,             .neg_chan = 1, 
        .dev = TIM5,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 5,       .pos_chan  = 0,             .neg_chan = 1, 
        .dev = TIM8,    .dead_time = LEG_DEFAULT_DT                 },
    {   .pwm = 5,       .pos_chan  = 2,             .neg_chan = 3, 
        .dev = TIM8,    .dead_time = LEG_DEFAULT_DT                 }
};
#define LEG_NUMOF           ARRAY_SIZE(leg_config)

#endif /* LEG_CONF_H */
