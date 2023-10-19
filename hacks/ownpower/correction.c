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
 * @brief   Correction implementation
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "correction.h"

static uint8_t run = 0, soft_start = 1;
static uint16_t dc;                             /**< duty-cycle */
static char break_msg[18] = "# Trying to start";

/**
 * @brief   Quick breaker, called by the correction function every
 *          160µs, check that 24v battery isn't too discharged, 12v
 *          battery isn't charged by another charger, for no reverse
 *          current, and charge current not too high.
 *
 * @param[in]  m        the measure structure
 * @param[out] boolean  1 or 0 if break
 */
static uint8_t breaker_160us(measures_t m)
{
    if (m.vh < VH_BREAK     /*< 24v battery are sufficiently charged */
        || m.vl > VL_BREAK  /*< 12v battery is not fully charged */
        || m.ih > IH_BREAK  /*< charge current is > 0, as the feeder
                             * sink current from the low side, it is
                             * better to test that the high side do not
                             * source current to the 24v battery */
        || m.il > IL_BREAK  /*< charge current is < 5.5A */
        ) {
        leg_stop(1);
        run = 0;
        soft_start = 1;
        dc = 0;
        if(m.vh < VH_BREAK) sprintf(break_msg, "# vh: %4u < %4u", m.vh, VH_BREAK);
        if(m.vl > VL_BREAK) sprintf(break_msg, "# vl: %4u > %4u", m.vl, VL_BREAK);
        if(m.ih > IH_BREAK) sprintf(break_msg, "# ih: %4u > %4u", m.ih, IH_BREAK);
        if(m.il > IL_BREAK) sprintf(break_msg, "# il: %4u > %4u", m.il, IL_BREAK);
        return 0;
    }
    else return 1;
}

void breaker_41ms(measures_ac_t ac)
{
    static uint32_t t_break = (T_BREAK << 8);

    /* do not overheat */
    if (ac.t  > t_break) {
        leg_stop(1);
        run = 0;
        soft_start = 1;
        dc = 0;
        printf("# t: %u > %u\n", (uint16_t)(ac.t >> 8), (T_BREAK));
    }
}

void starter(measures_ac_t ac)
{
    static uint32_t vh_start_min = (VH_START_MIN << 8),
                    vl_max = (VL_MAX << 8),
                    ih_break = (IH_BREAK << 8),
                    il_break = (IL_BREAK << 8),
                    t_start_max = (T_START_MAX << 8);

    if (!run) {
        puts(break_msg);
        /* (re)start */
        if (ac.vh >= vh_start_min    /*< 24v batteries are sufficiently
                                         * charged */
            && ac.vl <= vl_max          /*< 12v batteries is not fully
                                         * charged */
            && ac.ih < ih_break         /*< charge current is > 0, as the
                                         * feeder sink current from the low
                                         * side, it is better to test that
                                         * the high side do not source
                                         * current to the 24v battery */
            && ac.il < il_break         /*< charge current is < 5.5A */
            && ac.t <= t_start_max      /*< do not overheat */
            ) {
            /* Start with theorical duty-cycle */
            dc =    (uint16_t)(
                        ((float)(ac.vl >> 8) * 0.016)
                        / ((float)(ac.vh >> 8) * 0.031)
                        * LEG_RES
                    ); /* 18µs on 72MHz 334r8 */
            puts("# restart");
            run = 1;
        }
        else {
            if (ac.vh < vh_start_min) printf("# vh: %u < %u\n",
                                                (uint16_t)(ac.vh >> 8),
                                                VH_START_MIN);
            if (ac.vl > vl_max) printf("# vl: %u > %u\n",
                                        (uint16_t)(ac.vl >> 8),
                                        VL_MAX);
            if (ac.ih >= ih_break) printf("# ih: %u >= %u\n",
                                            (uint16_t)(ac.ih >> 8),
                                            IH_BREAK);
            if (ac.il >= il_break) printf("# il: %u >= %u\n",
                                            (uint16_t)(ac.il >> 8),
                                            IL_BREAK);
            if (ac.t > t_start_max) printf("# t: %u > %u\n",
                                            (uint16_t)(ac.t >> 8),
                                            T_START_MAX);
        }
    }
}

uint16_t correction(measures_t m)
{
    static uint16_t dc_min = DC_MIN, dc_max = DC_MAX;
    
    if(run && breaker_160us(m)) {
        /* Increase the duty cycle up to maximum current or VL_MAX */
        if (soft_start) {
            if (m.il < IL_MAX && m.vl < VL_MAX) dc++;
            else soft_start = 0;
        }

        /* track the maximum current */
        else {
            if      (m.il < IL_MAX && m.vl < VL_MAX) dc++;
            else if (m.il > IL_MAX || m.vl > VL_MAX) dc--;
        }

        /* boudary check */
        dc = dc < dc_min ? dc_min : dc;
        dc = dc > dc_max ? dc_max : dc;

        /* set the new duty cycle */
        if (run) {
            if (soft_start) buck_set(1, dc);
            else leg_set(1, dc);
        }
    }
    return dc;
}
