/*
 * Copyright (c) 2020 LAAS-CNRS
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define SAMPLE_FREQ         20000
#define SIN_FREQ            50
#define TIMER_PERIOD        23040
#define TARGET_PEAK_TO_PEAK (TIMER_PERIOD * 8 / 10)
#define LOW_THRES           (TIMER_PERIOD * 1 / 10)

void main(void)
{
    uint16_t xs = SAMPLE_FREQ / SIN_FREQ;
    uint16_t ys = TARGET_PEAK_TO_PEAK;

    printf( "static uint16_t sample_numof = %d;\n"
            "static uint16_t sin_lookup[%d] = {\n", xs, xs);

    long double y;
    uint8_t val_by_line = 11;
    uint8_t val_of_line = 0;

    for (uint16_t i = 0; i < xs; i++) {
        /* compute the next point */
        y = (sin(M_PI * 2 * i / xs) + 1) * (ys / 2);
        printf(" %5d", (uint16_t)y);
        val_of_line++;

        /* print a comma */
        if ((i + 1 < xs)) {
            printf(",");
        }

        /* print a new line */
        if (val_of_line == val_by_line) {
            printf("\n");
            val_of_line = 0;
        }
    }
    printf(" };"
            "\nstatic uint16_t origin = %d;"
             "\nstatic uint16_t y_scale = %d;"
            "\nstatic uint16_t low_thres = %d;"
            "\nstatic uint16_t ys = %d;"
            "\n\n", (uint16_t)y, TARGET_PEAK_TO_PEAK, LOW_THRES,
            TARGET_PEAK_TO_PEAK);
}
