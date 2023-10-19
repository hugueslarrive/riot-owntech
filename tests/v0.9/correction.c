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
        || m.vl1 > VL_BREAK  /*< 12v battery is not fully charged */
        || m.ih > IH_BREAK  /*< charge current is > 0, as the feeder
                             * sink current from the low side, it is
                             * better to test that the high side do not
                             * source current to the 24v battery */
        || m.il1 > IL_BREAK  /*< charge current is < 5.5A */
        ) {
        leg_stop(1);
        run = 0;
        soft_start = 1;
        dc = 0;
        if(m.vh < VH_BREAK) sprintf(break_msg, "# vh: %4u < %4u", m.vh, VH_BREAK);
        if(m.vl1 > VL_BREAK) sprintf(break_msg, "# vl1: %4u > %4u", m.vl1, VL_BREAK);
        if(m.ih > IH_BREAK) sprintf(break_msg, "# ih: %4u > %4u", m.ih, IH_BREAK);
        if(m.il1 > IL_BREAK) sprintf(break_msg, "# il1: %4u > %4u", m.il1, IL_BREAK);
        return 0;
    }
    else return 1;
    return 1;
}

void breaker_41ms(measures_ac_t ac)
{
    static int32_t t_break = (T_BREAK * 256);

    /* do not overheat */
    if (ac.t > t_break) {
        leg_stop(1);
        run = 0;
        soft_start = 1;
        dc = 0;
        printf("# t: %u > %u\n", (uint16_t)(ac.t / 256), (T_BREAK));
    }
}

void starter(measures_ac_t ac)
{
    static int32_t vh_start_min = (VH_START_MIN * 256),
                    vl_max = (VL_MAX * 256),
                    ih_break = (IH_BREAK * 256),
                    il_break = (IL_BREAK * 256),
                    t_start_max = (T_START_MAX * 256);

    if (!run) {
        puts(break_msg);
        /* (re)start */
        if (ac.vh >= vh_start_min    /*< 24v batteries are sufficiently
                                         * charged */
            && ac.vl1 <= vl_max          /*< 12v batteries is not fully
                                         * charged */
            && ac.ih < ih_break         /*< charge current is > 0, as the
                                         * feeder sink current from the low
                                         * side, it is better to test that
                                         * the high side do not source
                                         * current to the 24v battery */
            && ac.il1 < il_break         /*< charge current is < 5.5A */
            && ac.t <= t_start_max      /*< do not overheat */
            ) {
            /* Start with theorical duty-cycle */
            dc =    (uint16_t)(
                        ((float)(ac.vl1 / 256) * 0.016)
                        / ((float)(ac.vh / 256) * 0.031)
                        * LEG_RES
                    ); /* 18µs on 72MHz 334r8 */
            puts("# restart");
            run = 1;
        }
        else {
            if (ac.vh < vh_start_min) printf("# vh: %u < %u\n",
                                                (uint16_t)(ac.vh / 256),
                                                VH_START_MIN);
            if (ac.vl1 > vl_max) printf("# vl: %u > %u\n",
                                        (uint16_t)(ac.vl1 / 256),
                                        VL_MAX);
            if (ac.ih >= ih_break) printf("# ih: %u >= %u\n",
                                            (uint16_t)(ac.ih / 256),
                                            IH_BREAK);
            if (ac.il1 >= il_break) printf("# il: %u >= %u\n",
                                            (uint16_t)(ac.il1 / 256),
                                            IL_BREAK);
            if (ac.t > t_start_max) printf("# t: %u > %u\n",
                                            (uint16_t)(ac.t / 256),
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
            if (m.il1 < IL_MAX && m.vl1 < VL_MAX) dc++;
            else soft_start = 0;
        }

        /* track the maximum current */
        else {
            if      (m.il1 < IL_MAX && m.vl1 < VL_MAX) dc++;
            else if (m.il1 > IL_MAX || m.vl1 > VL_MAX) dc--;
        }

        /* boudary check */
        dc = dc < dc_min ? dc_min : dc;
        dc = dc > dc_max ? dc_max : dc;

        /* set the new duty cycle */
        if (run) {
            //~ if (soft_start) buck_set(1, dc);
            //~ else
            leg_set(1, dc, 0);
        }
    }
    return dc;
}
