/*
 * Copyright (c) 2021 Centre National de la Recherche Scientifique
 *
 * CNRS, établissement public à caractère scientifique et technologique,
 * dont le siège est situé 3 rue Michel-Ange, 75794 Paris cedex 16.
 *
 *            Luiz Villa - Projet OwnTech <owntech@laas.fr>
 *        Laboratoire d'analyse et d'architecture des systèmes
 *               LAAS-CNRS - 7, avenue du Colonel Roche
 *                 BP 54200 - 31031 Toulouse cedex 4
 *
 * SPDX-License-Identifier: CECILL-C
 */

/**
 * @brief   Test for owntech's adc_g4 driver integration in riot
 *
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "shell.h"
#include "shell_commands.h"

#include "xtimer.h"

#include "../../modules/adc_g4/adc.h"

#include "ps.h"

int main(void)
{
    xtimer_sleep(1);

    adc_g4_init();
    puts("adc_g4_init(): adc initialised");

    while (1) {
        ps();
    }

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
