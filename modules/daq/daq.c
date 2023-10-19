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
 * @brief   Data acquisition layer implementation
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "daq.h"
#include "daq_conf.h"

static char stack_measure[THREAD_STACKSIZE_DEFAULT];
#ifndef MODULE_GNRC_UDP
static char stack_measure_transmit[THREAD_STACKSIZE_DEFAULT];
#endif
static kernel_pid_t pid;

#if defined(MODULE_PERIPH_ADC) /* Use internal ADC */

static inline ADC_TypeDef *dev(adc_t line)
{
    return (ADC_TypeDef *)(ADC1_BASE + (adc_config[line].dev << 8));
}

/**
 * @brief   function from cpu/stm32/periph/adc_f3.c with mutex disabled
 */
static int32_t hack_adc_sample(adc_t line, adc_res_t res)
{
    int sample;

    /* Check if resolution is applicable */
    if (res & 0x3) {
        return -1;
    }

    /* Lock and power on the ADC device */
    //~ prep(line);

    /* Set resolution */
    dev(line)->CFGR &= ~ADC_CFGR_RES;
    dev(line)->CFGR |= res;

    /* Specify channel for regular conversion */
    dev(line)->SQR1 = adc_config[line].chan << ADC_SQR1_SQ1_Pos;

    /* Start conversion and wait for it to complete */
    dev(line)->CR |= ADC_CR_ADSTART;
    while (!(dev(line)->ISR & ADC_ISR_EOC)) {}

    /* Read the sample */
    sample = (int)dev(line)->DR;

    /* Power off and unlock device again */
    //~ done(line);

    return sample;
}

#else /* Use MCP3208 SPI ADC */

static uint8_t buf[3];

#ifdef MODULE_GNRC_UDP
static int sock_udp_en = 0;
static char sock_udp_addr[30];
#endif /* MODULE_GNRC_UDP */

/* From MCP3208 datasheet :
 * Because communication with the MCP3204/3208 devices may not need
 * multiples of eight clocks, it will be necessary to provide more
 * clocks than are required. This is usually done by sending ‘leading 
 * zeros’ before the start bit. [...] the first byte transmitted to
 * the A/D converter contains five leading zeros before the start bit.
 * Arranging the leading zeros this way allows the output 12 bits to
 * fall in positions easily manipulated by the MCU.
 * 
 * 
 * Chan|start singl D2 D1 D0 |  Byte1  |  Byte2  |Hex1|Hex2
 *  0  |  1	    1    0  0  0 |0000 0110|0000 0000|0x06|0x00
 *  1  |  1	    1    0  0  1 |0000 0110|0100 0000|0x06|0x40
 *  2  |  1	    1    0  1  0 |0000 0110|1000 0000|0x06|0x80
 *  3  |  1	    1    0  1  1 |0000 0110|1100 0000|0x06|0xc0
 *  4  |  1	    1    1  0  0 |0000 0111|0000 0000|0x07|0x00
 *  5  |  1	    1    1  0  1 |0000 0111|0100 0000|0x07|0x40
 *  6  |  1	    1    1  1  0 |0000 0111|1000 0000|0x07|0x80
 *  7  |  1	    1    1  1  1 |0000 0111|1100 0000|0x07|0xc0
 */
static mcp3208_cha_t mcp3208_cha[] = {
    { .b1 = 0x06, .b2 = 0x00 },
    { .b1 = 0x06, .b2 = 0x40 },
    { .b1 = 0x06, .b2 = 0x80 },
    { .b1 = 0x06, .b2 = 0xc0 },
    { .b1 = 0x07, .b2 = 0x00 },
    { .b1 = 0x07, .b2 = 0x40 },
    { .b1 = 0x07, .b2 = 0x80 },
    { .b1 = 0x07, .b2 = 0xc0 },
};

#endif /* defined(MODULE_PERIPH_ADC) */

void daq_init(void)
{
#if defined(MODULE_PERIPH_ADC) /* Use internal ADC */

    /* Channels can be configured to be either single-ended input or
     * differential input by writing into bits DIFSEL[15:1] in the
     * ADCx_DIFSEL register. This configuration must be written while
     * the ADC is disabled (ADEN=0).
     */

    static const gpio_t neg_in_pins[] = {
        GPIO_PIN(PORT_A, 5), /* ADC2_IN2  V_1LowN */
        GPIO_PIN(PORT_A, 7), /* ADC2_IN4  V_2LowN */
        GPIO_PIN(PORT_B, 2), /* ADC2_IN12 V_HighN */
    };
    #define NEG_IN_PINS_NUMOF       ARRAY_SIZE(neg_in_pins)

    for (unsigned i = 0; i < NEG_IN_PINS_NUMOF; i++) {
        /* initializes analog pins for negative inputs of differential
         * channels */
        gpio_init_analog(neg_in_pins[i]);
        /* now enable differential mode for the corresponding channels
         * (differential channels must be defined first in adc_config[]
         */
        dev(ADC_LINE(i))->DIFSEL |= (1 << adc_config[i].chan);
     }
 
    /* initializes all available ADC lines */
    for (unsigned i = 0; i < ADC_NUMOF; i++) {
        if (adc_init(ADC_LINE(i)) < 0) {
            printf("Initialization of ADC_LINE(%u) failed\n", i);
            //~ return 1;
        } else {
            printf("Successfully initialized ADC_LINE(%u)\n", i);
        }
    }

#else /* Use MCP3208 SPI ADC */

    /* Initialize all CS pins to activate all isolators with two
     * recursive loops (quick and dirty but not necessarily less
     * efficient than filtering) */

    for (uint8_t i=0; i<LEG_MEAS_NUMOF; i++) {
        for (uint8_t j=0; j<5; j++) {
            spi_init_cs(leg_meas[i][j].bus, leg_meas[i][j].cs);
        }
    }

#endif /* defined(MODULE_PERIPH_ADC) */
}

int16_t daq_meas(uint8_t leg, meas_type_t mt)
{

#if defined(MODULE_PERIPH_ADC) /* Use internal ADC */

    /* Acquire a measure */
    int sample = 0;
    sample = hack_adc_sample(ADC_LINE(leg_meas[leg-1][mt]), ADC_RES_12BIT);
    return sample;

#else /* Use MCP3208 SPI ADC */

    /* Acquire a measure */
    daq_leg_meas_t dlm = leg_meas[leg-1][mt];
    mcp3208_cha_t cha = mcp3208_cha[dlm.cha];

    /* Set the chip select pin */
    spi_init_cs(dlm.bus, dlm.cs);
    
    /* get bus access */
    spi_acquire(dlm.bus, dlm.cs, DAQ_SPI_MODE, DAQ_SPI_CLK);

    /* transfer data */
    char str[3] = {cha.b1, cha.b2, 0 };
    memset(buf, 0, sizeof(buf));
    spi_transfer_bytes(dlm.bus, dlm.cs, false, str, buf, 3);

    /* release the bus */
    spi_release(dlm.bus);
    
    /* return the raw avlue */
    return (buf[1] << 8) | buf[2];

#endif /* defined(MODULE_PERIPH_ADC) */
}

void *measure_handler(void *arg)
{
    (void)arg;

    xtimer_ticks32_t last_wakeup = xtimer_now();

#if defined(MODULE_PERIPH_ADC) /* Use internal ADC */

    measures_t m;           /*< measures storage */
    measures_ac_t ac;       /*< measure acumulators */
    uint16_t avg_ct = 0;    /*< counter for average values */
    msg_t msg;              /*< IPC msg structure */
    measures_ac_t tac;      /*< measure acumulators to transmit */

    /* launch the measure_transmit thread */
    kernel_pid_t pid = thread_create(stack_measure_transmit,
                            sizeof(stack_measure_transmit),
                            THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST,
                            measure_transmit_handler, NULL,
                            "measure_transmit");

    uint32_t start=0, end=0, time = 0; /*< for runtime measurement */

    /* Measurement loop */
    while (1) {

        start = xtimer_now_usec();

        /* take measures */
        m.vh = daq_meas(1, VH);
        m.ih = daq_meas(1, IH);
        m.vl1 = daq_meas(1, VL);
        m.vl2 = daq_meas(2, VL);
        m.il1 = daq_meas(1, IL);
        m.il2 = daq_meas(2, IL);
        m.t = 0;//daq_meas(1, T);

        /* do safety checks and duty-cycle correction */
        //~ m.dc = correction(m);

        /* accumulate measures */
        ac.vh += m.vh;
        ac.ih += m.ih;
        ac.vl1 += m.vl1;
        ac.il1 += m.il1;
        ac.vl2 += m.vl2;
        ac.il2 += m.il2;
        ac.t += m.t;
        //~ ac.dc += m.dc;

        /* send measures accumulators to transmitter */
        avg_ct++;
        if (avg_ct == 200) {
            avg_ct = 0;
            ac.time = time;
            tac = ac;
            msg.content.ptr = &tac;
            msg_send(&msg, pid);
            ac.vh = 0; ac.ih = 0; ac.vl1 = 0; ac.il1 = 0; ac.vl2 = 0;
            ac.il2 = 0; ac.t = 0; //ac.dc = 0;
            end = xtimer_now_usec();
            time = end - start;
        }
        xtimer_periodic_wakeup(&last_wakeup, 200);
    }

#else /* Use MCP3208 SPI ADC */

#ifdef MODULE_GNRC_UDP /* for hacks/mqtt */

    uint16_t vh, ih, vr, ir, vs, is, vt, it;
    uint32_t    avh = 0, aih = 0, avr = 0, air = 0, avs = 0, ais = 0,
                avt = 0, ait = 0; /*< average values sums */
    uint16_t avg_ct = 0; /*< counter for average values */

    uint8_t loop = 1;
    char strtmp[41];        /* 4x8 + 7 + \n + 1 */
    char avg_strtmp[82];   /* 4x8 + {} + 5x8 + 7, + 1 */
    sock_udp_ep_t remote = { .family = AF_INET6 };

    if (sock_udp_en) {
        /* configure the sock udp remote end point */
        ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, sock_udp_addr);
        if (ipv6_addr_is_link_local((ipv6_addr_t *)&remote.addr)) {
            /* choose first interface when address is link local */
            gnrc_netif_t *netif = gnrc_netif_iter(NULL);
            remote.netif = (uint16_t)netif->pid;
        }
        remote.port = 8888;
    }

    while (1) {
        /* Measurement loop */
        switch (loop) {
            /* switch between the 4 legs */
            case 1:
                /* take voltage measurement */
                vh = daq_meas(1, VH);
                avh += vh;
                /* take current measurement */
                ih = daq_meas(1, IH);
                aih += ih;
                break;
            case 2:
                vr = daq_meas(1, VL);
                avr += vr;
                ir = daq_meas(1, IL);
                air += ir;
                break;
            case 3:
                vs = daq_meas(2, VL);
                avs += vs;
                is = daq_meas(2, IL);
                ais += is;
                break;
            case 4:
                vt = daq_meas(3, VL);
                avt += vt;
                it = daq_meas(3, IL);
                ait += it;
                if (sock_udp_en) {
                    /* build a line of measures to send over udp sock */
                    sprintf(strtmp, "%4u %4u %4u %4u %4u %4u %4u %4u\n",
                            vh, ih, vr, ir, vs, is, vt, it);
                    /* then send it */
                    sock_udp_send(NULL, strtmp, 40, &remote);
                }
                loop = 0;
                /* average 1024 series and publish them over mqtt-sn */
                avg_ct++;
                if (avg_ct == 1024) {
                    /* average and publish */
                    avg_ct = 0;
                    sprintf(
                        avg_strtmp,
                        "{'VH':%4u,'iH':%4u,'VR':%4u,'iR':%4u,'VS':%4u,'iS':%4u,'VT':%4u,'iT':%4u}",
                        (uint16_t)(avh>>10), (uint16_t)(aih>>10),
                        (uint16_t)(avr>>10), (uint16_t)(air>>10),
                        (uint16_t)(avs>>10), (uint16_t)(ais>>10),
                        (uint16_t)(avt>>10), (uint16_t)(ait>>10));
                    avh = 0; aih = 0; avr = 0; air = 0;
                    avs = 0; ais = 0; avt = 0; ait = 0;
                    mqttsn_pub("OT_TEST_TOKEN", avg_strtmp);
                }
                break;
        }
        loop++;
        xtimer_periodic_wakeup(&last_wakeup, 244);
    }   

#else /* for hacks/ownwall */

    measures_t m;           /*< measures storage */
    measures_ac_t ac;       /*< measure acumulators */
    uint16_t avg_ct = 0;    /*< counter for average values */
    msg_t msg;              /*< IPC msg structure */
    measures_ac_t tac;      /*< measure acumulators to transmit */

    /* launch the measure_transmit thread */
    kernel_pid_t pid = thread_create(stack_measure_transmit,
                            sizeof(stack_measure_transmit),
                            THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST,
                            measure_transmit_handler, NULL,
                            "measure_transmit");

    uint32_t start=0, end=0, time = 0; /*< for runtime measurement */

    /* Measurement loop */
    while (1) {

        start = xtimer_now_usec();

        /* take measures */
        m.vh = daq_meas(1, VH);
        m.ih = daq_meas(1, IH);
        m.vl = daq_meas(1, VL);
        m.il = daq_meas(1, IL);
        m.t = daq_meas(1, T);

        /* do safety checks and duty-cycle correction */
        m.dc = correction(m);

        /* accumulate measures */
        ac.vh += m.vh;
        ac.ih += m.ih;
        ac.vl += m.vl;
        ac.il += m.il;
        ac.t += m.t;
        ac.dc += m.dc;

        /* send measures accumulators to transmitter */
        avg_ct++;
        if (avg_ct == 256) {
            avg_ct = 0;
            ac.time = time;
            tac = ac;
            msg.content.ptr = &tac;
            msg_send(&msg, pid);
            ac.vh = 0; ac.ih = 0; ac.vl = 0; ac.il = 0; ac.t = 0; ac.dc = 0;
            end = xtimer_now_usec();
            time = end - start;
        }
        xtimer_periodic_wakeup(&last_wakeup, 160);
    }

#endif /* MODULE_GNRC_UDP */

#endif /* defined(MODULE_PERIPH_ADC) */
    return NULL;
}

void *measure_transmit_handler(void *arg)
{
    (void)arg;
    
    msg_t msg;
    measures_ac_t *ac;

    while (1) {
        /* receive mesure accumulators from measure thread every 41ms */
        msg_receive(&msg);
        ac = msg.content.ptr;

#if defined(MODULE_PERIPH_ADC) /* Use internal ADC */

        /* print averaged values on 115200 bauds serial console */
        printf("%5d %5d %5d %5d %5d %5d %5d %5d # %u µs\n",
                (int16_t)(ac->vh / 200), (int16_t)(ac->ih / 200),
                (int16_t)(ac->vl1 / 200), (int16_t)(ac->il1 / 200),
                (int16_t)(ac->vl2 / 200), (int16_t)(ac->il2 / 200),
                (int16_t)(ac->t / 200), (int16_t)(ac->dc1 / 200),
                (int16_t)(ac->time));

#else /* hacks/ownpower */

        /* thermal safety check */
        breaker_41ms(*ac);

        /* print averaged values on 115200 bauds serial console */
        printf("%4u %4u %4u %4u %4u %3u # %u µs\n",
                (uint16_t)(ac->vh>>8), (uint16_t)(ac->ih>>8),
                (uint16_t)(ac->vl>>8), (uint16_t)(ac->il>>8),
                (uint16_t)(ac->t>>8),  (uint16_t)(ac->dc>>8),
                (uint16_t)(ac->time));

        /* try to (re)start converter */
        starter(*ac);

#endif /* defined(MODULE_PERIPH_ADC) */

    }
    return NULL;
}

void measure_init(char priority)
{
    /* Launch the measure thread */
    pid = thread_create( stack_measure, sizeof(stack_measure),
                         priority, THREAD_CREATE_STACKTEST,
                         measure_handler, NULL, "measure"    );
}
