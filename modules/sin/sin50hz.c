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
 * @brief   50Hz sine wave generator implementation
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 * 
 */

#include "sin50hz.h"

static char stack_sin50hz[THREAD_STACKSIZE_SMALL];
static kernel_pid_t pid;
static sin50hz_legs_t mask;
static uint16_t shift;

/* These variables are auto-generated with tools/singen_simple.c. If you
 * change the pwm frequency, you need to regenerate them and also modify
 * the LEG_RES and LEG_FREQ values ​​in leg/leg.h. You can use the shell
 * command "init <LEG_FREQ>" in tests/periph_hrtim to determine
 * LEG_RES / period value.
 */
static uint16_t sample_numof = 400;
static uint16_t sin_lookup[400] = {
  9216,  9360,  9505,  9650,  9794,  9939, 10083, 10227, 10371, 10514, 10657,
 10800, 10942, 11084, 11226, 11367, 11507, 11647, 11787, 11925, 12063, 12201,
 12337, 12473, 12608, 12742, 12876, 13008, 13139, 13270, 13399, 13528, 13655,
 13782, 13907, 14031, 14154, 14275, 14396, 14515, 14633, 14749, 14864, 14978,
 15090, 15201, 15310, 15418, 15524, 15629, 15732, 15834, 15934, 16032, 16129,
 16223, 16317, 16408, 16498, 16585, 16671, 16756, 16838, 16918, 16997, 17073,
 17148, 17221, 17292, 17360, 17427, 17492, 17554, 17615, 17674, 17730, 17784,
 17837, 17887, 17935, 17980, 18024, 18066, 18105, 18142, 18177, 18210, 18240,
 18268, 18294, 18318, 18340, 18359, 18376, 18391, 18403, 18413, 18421, 18427,
 18430, 18432, 18430, 18427, 18421, 18413, 18403, 18391, 18376, 18359, 18340,
 18318, 18294, 18268, 18240, 18210, 18177, 18142, 18105, 18066, 18024, 17980,
 17935, 17887, 17837, 17784, 17730, 17674, 17615, 17554, 17492, 17427, 17360,
 17292, 17221, 17148, 17073, 16997, 16918, 16838, 16756, 16671, 16585, 16498,
 16408, 16317, 16223, 16129, 16032, 15934, 15834, 15732, 15629, 15524, 15418,
 15310, 15201, 15090, 14978, 14864, 14749, 14633, 14515, 14396, 14275, 14154,
 14031, 13907, 13782, 13655, 13528, 13399, 13270, 13139, 13008, 12876, 12742,
 12608, 12473, 12337, 12201, 12063, 11925, 11787, 11647, 11507, 11367, 11226,
 11084, 10942, 10800, 10657, 10514, 10371, 10227, 10083,  9939,  9794,  9650,
  9505,  9360,  9216,  9071,  8926,  8781,  8637,  8492,  8348,  8204,  8060,
  7917,  7774,  7631,  7489,  7347,  7205,  7064,  6924,  6784,  6644,  6506,
  6368,  6230,  6094,  5958,  5823,  5689,  5555,  5423,  5292,  5161,  5032,
  4903,  4776,  4649,  4524,  4400,  4277,  4156,  4035,  3916,  3798,  3682,
  3567,  3453,  3341,  3230,  3121,  3013,  2907,  2802,  2699,  2597,  2497,
  2399,  2302,  2208,  2114,  2023,  1933,  1846,  1760,  1675,  1593,  1513,
  1434,  1358,  1283,  1210,  1139,  1071,  1004,   939,   877,   816,   757,
   701,   647,   594,   544,   496,   451,   407,   365,   326,   289,   254,
   221,   191,   163,   137,   113,    91,    72,    55,    40,    28,    18,
    10,     4,     1,     0,     1,     4,    10,    18,    28,    40,    55,
    72,    91,   113,   137,   163,   191,   221,   254,   289,   326,   365,
   407,   451,   496,   544,   594,   647,   701,   757,   816,   877,   939,
  1004,  1071,  1139,  1210,  1283,  1358,  1434,  1513,  1593,  1675,  1760,
  1846,  1933,  2023,  2114,  2208,  2302,  2399,  2497,  2597,  2699,  2802,
  2907,  3013,  3121,  3230,  3341,  3453,  3567,  3682,  3798,  3916,  4035,
  4156,  4277,  4400,  4524,  4649,  4776,  4903,  5032,  5161,  5292,  5423,
  5555,  5689,  5823,  5958,  6094,  6230,  6368,  6506,  6644,  6784,  6924,
  7064,  7205,  7347,  7489,  7631,  7774,  7917,  8060,  8204,  8348,  8492,
  8637,  8781,  8926,  9071 };
static uint16_t origin = 9071;
static uint16_t y_scale = 18432;
static uint16_t low_thres = 2304;
static uint16_t ys = 18432;
/* end of auto-generated variables */

void *sin50hz_handler(void *arg)
{
    (void)arg;

    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    //~ uint8_t xsc = 10; /* x-scale */
    uint16_t last_idx = sample_numof - 1;//sample_numof / xsc - 1; /* number of samples - 1 */

    uint16_t idx0 = 0;
    uint16_t idx180 = (last_idx + 1) / 2;

    while (1) {
        for (uint8_t i = 1; i <= 2; i++) {
            if ((mask.legs0 & (1 << i)) != 0) {
                leg_set(i, sin_lookup[idx0] * y_scale / ys - origin * y_scale / ys  + origin + low_thres, 0);
                idx0 = (idx0 == last_idx) ? 0 : idx0 + 1;
            }
            if ((mask.legs180 & (1 << i)) != 0) {
                leg_set(i, sin_lookup[idx180] * y_scale / ys - origin * y_scale / ys  + origin + low_thres, shift);
                idx180 = (idx180 == last_idx) ? 0 : idx180 + 1;
            }
        }

        xtimer_periodic_wakeup(&last_wakeup, 50);
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

uint16_t sin50hz_get_yscale(void)
{
    return y_scale;
}

void sin50hz_set_yscale(uint16_t scale)
{
    y_scale = scale;
}

void sin50hz_set_shift(uint16_t value)
{
    shift = value;
}
