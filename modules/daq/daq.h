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
 * @defgroup    owntech_modules_daq OwnTech's daq module
 * @ingroup     owntech_modules
 * @brief       OwnTech Data AcQuisition layer
 *
 * @{
 * @file
 * @brief   Data acquisition layer interface definitions
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#ifndef DAQ_H
#define DAQ_H

#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "msg.h"

#include "periph_cpu.h"
//~ #include "periph/adc.h"

#if !defined(MODULE_PERIPH_ADC)
#include "periph/spi.h"
#endif /* !defined(MODULE_PERIPH_ADC) */

#ifdef MODULE_GNRC_UDP
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "mqttsn/mqttsn.h"
#endif /* MODULE_GNRC_UDP */

#include "correction.h"

#include "measures_struct.h"

#define DAQ_SPI_MODE            (1U)    /**< SPI mode for the MCP3208 ADC **/ 
#define DAQ_SPI_CLK             MHZ(8)  /**< SPI clock speed **/

/**
 * @brief   Type of measure enumeration
 * 
 * @note    If a type is added, leg_meas[] arrays in daq_conf.h must be
 *          reviewed.
 */
typedef enum {
    IH,     /**< High side current, (Iout on schematics) */
    IL,     /**< Low side current, (Iin on schematics) */
    VL,     /**< Low side voltage, (Vin on schematics) */
    VH,     /**< High side voltage, (Vout on schematics) */
    T       /**< Temperature */
} meas_type_t;

#if !defined(MODULE_PERIPH_ADC)
/**
 * @brief   Configuration bytes structure for the MCP3208 channels
 */
typedef struct {
    uint8_t b1;
    uint8_t b2;
} mcp3208_cha_t;

/**
 * @brief   Data acquisition leg measure configuration structure
 */
typedef struct {
    spi_t bus;      /**< SPI device */
    gpio_t cs;      /**< CS pin */
    uint8_t cha;    /**< MCP3208 channel number from 0 to 7 */
} daq_leg_meas_t;
#endif /* !defined(MODULE_PERIPH_ADC) */

/**
 * @brief   Initialize all available ADC lines
 */
void daq_init(void);

/**
 * @brief   Acquire a measure
 * 
 * @param[in]   leg     leg device from 1 to 10
 * @param[in]   mt      the type of measure to acquire
 * 
 * @return      12bit value of the measure acquired     
 */
int16_t daq_meas(uint8_t leg, meas_type_t mt);

/**
 * @brief   Thread handler that get 5 measures every 160µs
 *          (6,25kHz sample rate)
 */
void *measure_handler(void *arg);

/**
 * @brief   Thread handler that print 5 averaged measures over serial
 *          256 * 160µs = 40.96ms (24Hz)
 */
void *measure_transmit_handler(void *arg);

/**
 * @brief   Start the thread
 * 
 * @param[in]  priority     the thread priority
 */   
void measure_init(char priority);

/**
 * @brief   Set an ipv6 address for sock_udp_send and enable sock udp
 *          transmission
 * 
 * @param[in]  addr     the destination ipv6 address
 */   
void sock_udp_addr_set(char* addr);

#endif /* DAQ_H */
/** @} */
