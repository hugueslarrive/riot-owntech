/*
 * sin50hz.c
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
 * 
 */

#include "sin50hz.h"

static char stack_sin50hz[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t pid;
static sin50hz_legs_t mask;


void *sin50hz_handler(void *arg)
{
    (void)arg;

    xtimer_ticks32_t last_wakeup = xtimer_now();

    uint16_t sin[60] = {    270,    293,    315,    337,    358,    378,
                            397,    415,    431,    445,    457,    467,
                            475,    481,    485,    486,    485,    481,
                            475,    467,    457,    445,    431,    415,
                            397,    378,    358,    337,    315,    293,
                            270,    247,    225,    203,    182,    162,
                            143,    125,    109,     95,     83,     73,
                             65,     59,     55,     54,     55,     59,
                             65,     73,     83,     95,    109,    125,
                            143,    162,    182,    203,    225,    247 };
                            
    
    uint8_t idx0 = 0, idx120 = 20, idx180= 30, idx240 = 40, corr = 0;
    
    while (1) {
        for (uint8_t i=1; i<=10; i++) {
            if ((mask.legs0   & (1 << i)) != 0) leg_set(i, sin[idx0]);
            if ((mask.legs120 & (1 << i)) != 0) leg_set(i, sin[idx120]);
            if ((mask.legs180 & (1 << i)) != 0) leg_set(i, sin[idx180]);
            if ((mask.legs240 & (1 << i)) != 0) leg_set(i, sin[idx240]);
        }

        idx0++; idx120++; idx180++; idx240++;
        if (idx0   == 60) idx0   = 0;
        if (idx120 == 60) idx120 = 0;
        if (idx180 == 60) idx180 = 0;
        if (idx240 == 60) idx240 = 0;

 
        xtimer_periodic_wakeup(&last_wakeup, 333);
 
        corr++;
        if (corr == 3) {
            corr = 0;
            last_wakeup.ticks32++;
        }
    }   

    return NULL;
}

void sin50hz_init(char priority)
{
    pid = thread_create( stack_sin50hz, sizeof(stack_sin50hz),
                         priority, THREAD_CREATE_STACKTEST,
                         sin50hz_handler, NULL, "sin50hz"    );
}

void sin50hz_add2p(uint8_t leg1, uint8_t leg2)
{
    mask.legs0 |= (1 << leg1);
    mask.legs180 |= (1 << leg2);
}


void sin50hz_rm2p(uint8_t leg1, uint8_t leg2)
{
    mask.legs0 &= ~(1 << leg1);
    mask.legs180 &= ~(1 << leg2);
    leg_stop(leg1);
    leg_stop(leg2);
}

void sin50hz_add3p(uint8_t leg1, uint8_t leg2, uint8_t leg3)
{
    mask.legs0 |= (1 << leg1);
    mask.legs120 |= (1 << leg2);
    mask.legs240 |= (1 << leg3);
}


void sin50hz_rm3p(uint8_t leg1, uint8_t leg2, uint8_t leg3)
{
    mask.legs0 &= ~(1 << leg1);
    mask.legs120 &= ~(1 << leg2);
    mask.legs240 &= ~(1 << leg3);
    leg_stop(leg1);
    leg_stop(leg2);
    leg_stop(leg3);
}

