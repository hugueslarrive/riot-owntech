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
 * @brief       Test for pwm management by inveter leg implementation
 * @date        2020
 * @author      Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "test_pwm.h"
#include "leg_conf.h"

static char stack_sin_bench[THREAD_STACKSIZE_SMALL];
static kernel_pid_t pid;

static uint16_t sin_bench_min, sin_bench_max;
void *sin_bench_handler(void *arg)
{
    (void)arg;

    uint8_t i = 0;
    //~ int8_t s = 1;
    uint16_t min = sin_bench_min;
    uint16_t max = sin_bench_max;
    uint8_t sin_lookup[50] = {
        25, 28, 31, 34, 37, 39, 42, 44, 46, 47, 48, 49, 49, 49, 49, 48,
        47, 46, 44, 42, 39, 37, 34, 31, 28, 25, 21, 18, 15, 12, 10,  7,
         5,  3,  2,  1,  0,  0,  0,  0,  1,  2,  3,  5,  7, 10, 12, 15,
        18, 21 };

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while(1) {
        sin50hz_set_yscale(min + (max - min) * sin_lookup[i] / 50);
        //~ printf("%d %d\n", sin_lookup[i], min + (max - min) * sin_lookup[i] / 50);
        i = (i == 49) ? 0 : i + 1;
        xtimer_periodic_wakeup(&last_wakeup, 20000);
    }
}

static unsigned _get_dev(const char *dev_str)
{
    unsigned dev = atoi(dev_str);
    if (dev > leg_numof()) {
        printf("Error: device PWM_DEV(%u) is unknown\n", dev);
        return UINT_MAX;
    }
    return dev;
}

int test_pwm_legset(int argc, char** argv)
{
    if (argc != 4) {
        printf("usage: %s <dev> <val> <pha>\n", argv[0]);
        printf("\tdev: device by number between 1 and %d\n", leg_numof());
        puts("\tval: duty cycle");
        puts("\tpha: phase shift\n");
        return 1;
    }

    unsigned dev = _get_dev(argv[1]);
    uint16_t val = (uint16_t)atoi(argv[2]);

    if (val > 0) {
        leg_set(dev, val, (uint16_t)atoi(argv[3]));
    }
    else {
        leg_stop(dev);
    }
    return 0;
}

int test_pwm_sinadd(int argc, char** argv)
{
    if (argc < 3) {
        printf("usage: %s <leg> <leg> [leg]\n", argv[0]);
        printf("\tdev: device by number between 1 and %d\n", leg_numof());
        return 1;
    }
    if (argc == 3) sin50hz_add2p((uint8_t)atoi(argv[1]), (uint8_t)atoi(argv[2]));
    //~ if (argc == 4) sin50hz_add3p((uint8_t)atoi(argv[1]), (uint8_t)atoi(argv[2]), (uint8_t)atoi(argv[3]));
    return 0;
}

int test_pwm_sinrm(int argc, char** argv)
{
    if (argc < 3) {
        printf("usage: %s <leg> <leg> [leg]\n", argv[0]);
        printf("\tdev: device by number between 1 and %d\n", leg_numof());
        return 1;
    }
    if (argc == 3) sin50hz_rm2p((uint8_t)atoi(argv[1]), (uint8_t)atoi(argv[2]));
    //~ if (argc == 4) sin50hz_rm3p((uint8_t)atoi(argv[1]), (uint8_t)atoi(argv[2]), (uint8_t)atoi(argv[3]));
    return 0;    
}

int test_pwm_getysc(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    printf("%d\n", sin50hz_get_yscale());
    return 0;    
}

int test_pwm_setysc(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    uint16_t yscale = atoi(argv[1]);

    sin50hz_set_yscale(yscale);

    return 0;    
}

int test_pwm_setshift(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    uint16_t shift = atoi(argv[1]);

    sin50hz_set_shift(shift);

    return 0;    
}

int test_pwm_sin_bench(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: %s <min> <max>\n", argv[0]);
        puts("\tmin: minimum scale value");
        puts("\tmax: maximum scale value\n");
        return 1;
    }

    sin_bench_min = atoi(argv[1]);
    sin_bench_max = atoi(argv[2]);

    pid = thread_create( stack_sin_bench, sizeof(stack_sin_bench),
                         THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                         sin_bench_handler, NULL, "sin_bench"    );

    return 0;
}
