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
 * @brief   Workaround for complementary pwm output initialization
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */
 
#include "ot_pwm.h"

uint32_t ot_pwm_init(unsigned dev, pwm_mode_t mode, uint32_t freq, uint16_t res)
{
    uint32_t pwm_freq;

    switch (mode) {
        case PWM_RIGHT:
            /* Dans ce mode les registres CCMR configure les sorties en
             * mode 2 (inversées) ce qui n'est pas la bonne solution pour
             * produire l'effet escompté (il faudrait re-inverser les
             * signaux en hardware...
             * Pour obtenir des signaux alignés à droite il suffit
             * d'inverser la direction du compteur avec le bit DIR du
             * registre CR1. */

            /* initialisation en mode PWM_CENTER qui nous laissera corriger
             * les registres avant d'activer les sorties */
            pwm_freq = pwm_init(PWM_DEV(dev), PWM_CENTER, freq, res);
            
            /* correction des registres CCMR */
            pwm_config[dev].dev->CCMR1 = CCMR_NORM;
            pwm_config[dev].dev->CCMR2 = CCMR_NORM;
            
            /* inversion de la direction du compteur */
            pwm_config[dev].dev->CR1 = 0;
            pwm_config[dev].dev->CR1 |= TIM_CR1_DIR;
            pwm_config[dev].dev->CR1 |= TIM_CR1_CEN;
            break;
        case PWM_CENTER:
            /* Dans ce mode les registres CCMR sont configurés à 0 dans
             * pwm_init() ce qui rend les comparaisons entre TIMx_CCRn et
             * TIMx_CNT sans effet sur les sorties.
             * J'ai défini la constante CCMR_COMP qui configure le mode 1
             * pour le premier canal et le mode 2 inversé pour le second.
             * Avant de configurer les registres CCMR, ce qui aurait pour
             * effet de mettre les sorties 2 et 4 à l'état haut avec leur
             * registre CCR à 0, on va d'abord configurer ces registres à
             * la valeur de résolution pour que leur sorties restent à 0. */
            pwm_freq = pwm_init(PWM_DEV(dev), PWM_CENTER, freq * 2, res) / 2;
            /* prevent high output level on complementary outputs at
             * initialisation */
            for (unsigned i = 0; i < TIMER_CHANNEL_NUMOF; ++i) {
                if (pwm_config[dev].chan[i].pin != GPIO_UNDEF) {
                    switch (pwm_config[dev].chan[i].cc_chan) {
                        case 0 : pwm_config[dev].dev->CCR1 = 0; break;
                        case 1 : pwm_config[dev].dev->CCR2 = res; break;
                        case 2 : pwm_config[dev].dev->CCR3 = 0; break;
                        case 3 : pwm_config[dev].dev->CCR4 = res; break;
                    }
                }
            }
            
            pwm_config[dev].dev->CCMR1 = CCMR_COMP;
            pwm_config[dev].dev->CCMR2 = CCMR_COMP;
            break;
        default:
            /* PWM_LEFT : initialisation normale */
            pwm_freq = pwm_init(PWM_DEV(dev), mode, freq, res);
            break;
    }
    if (pwm_config[dev].dev == TIM8) {
        /* La sortie 3 du timer 8 peut être configurée soit sur PC8 qui
         * est le seul utilisable pour SDMMC1_D0, soit sur PI7 qui n'est
         * pas connecté sur le boîtier LQFP144.
         * La seule possibilité était donc d'utiliser la sortie
         * complémentaire qui d'ailleurs n'est pas inversée lorsque la
         * sortie principale n'est pas activée ce qui me simplifie la
         * tâche */
        pwm_config[dev].dev->CCER = (TIM_CCER_CC1E | TIM_CCER_CC2E |
                                    TIM_CCER_CC3NE | TIM_CCER_CC4E);
    }
    
    return pwm_freq;
}

