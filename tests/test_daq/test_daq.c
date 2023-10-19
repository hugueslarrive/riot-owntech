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
 * @brief       Test for daq management by inveter leg implementation
 * @date        2020
 * @author      Hugues Larrive <hugues.larrive@laas.fr>
 */


#include "test_daq.h"

int test_daq_meas(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: %s <dev> <type>\n", argv[0]);
        printf("\tdev: device by number between 1 and %d\n", leg_numof());
        puts("\ttype: measurement type :");
        puts("\t\t0: IH");
        puts("\t\t1: IL");
        puts("\t\t2: VL");
        puts("\t\t3: VH");
        puts("\t\t4: T\n");
        return 1;
    }
    
    unsigned dev = atoi(argv[1]);
    meas_type_t type = atoi(argv[2]);
    int16_t sample = 0;
    
    sample = daq_meas(dev, type);

    printf("%d\n", sample);

    return 0;    
}
