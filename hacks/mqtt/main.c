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
 * @brief   RIOT based firmware for the OwnTech power converter board
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 */

#include "shell.h"
#include "ot_banner.h"
//~ #include "pwm/test_pwm.h"
//~ #include "daq/test_spi.h"
//~ #include "mqttsn/test_mqttsn.h"
#include "../../modules/ot_shell_commands/ot_shell_commands.h"

#include "ps.h"
#include "xtimer.h"

#include "../../modules/daq/daq.h"
#include "pwm/sin50hz.h"
#include "mqttsn/mqttsn.h"

/* For UDP Client/Server */
/* needed for the `ping6` shell command */
#define MAIN_QUEUE_SIZE     (8)     /**< The main message queue size use
                                         by the `ping6` shell command */
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int main(void)
{
    /* For UDP Client/Server */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

#ifdef TEST_PWM_H
    /* all pwm devices will be initialized soon by leg_init(); */
    initiated = 0b1111111111;
#endif

    /** - Initializes the pwm management layer by inverter leg */
    leg_init();

    /** - Starts the 50Hz sine wave generator */
    sin50hz_init(THREAD_PRIORITY_MAIN - 1);
    
    /** - Initializes the DAQ's SPI CS GPIO pins */
    daq_init();

#ifdef TEST_SPI_H
    printf("There are %i SPI devices configured for your platform.\n",
           (int)SPI_NUMOF);
    /* reset local SPI configuration */
    spiconf.dev = SPI_UNDEF;
#endif

    /** - MQTT */
    /* add the "scope: site" address needed for mosquitto.rsmb
     * as the _netif_add() function in sys/shell/commands/sc_gnrc_netif.c file */
    netif_t *iface = netif_get_by_name("6");
    char *addr_str = "fec0:affe::99";
    ipv6_addr_t addr;
    uint16_t flags = GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID;
    uint8_t prefix_len;
    ipv6_addr_from_str(&addr, addr_str);
    prefix_len = 64U;
    flags |= (prefix_len << 8U);
    netif_set_opt(iface, NETOPT_IPV6_ADDR, flags, &addr, sizeof(addr));
    /* end of adding address */

    /* Launch the asymcute_listener thread */
    mqttsn_init(THREAD_PRIORITY_MAIN + 1);
    xtimer_sleep(2);
    mqttsn_con("DUMMY_TOKEN", "[fec0:affe::1]:1885");
    xtimer_sleep(3);
    mqttsn_sub("OT_TEST_TOKEN");
    xtimer_usleep(100000);
    //~ mqttsn_pub("OT_TEST_TOKEN","{'VH':19,'iH':1650,'VR':0,'iR':1650,'VS':0,'iS':1650,'VT':0,'iT':1650}");

    /* set the sock_udp target end point address end enable transmission */
    //~ sock_udp_addr_set("fe80::e3c4:67db:fc80:2b7d");

    /** - Launch the measure thread */
    measure_init(THREAD_PRIORITY_MAIN + 2);

    /* a small bench */
    //~ uint16_t meas[3][5];
    //~ uint32_t start, stop;
    
    //~ memset(meas, 0, sizeof(meas));
    //~ ps();
    //~ start = xtimer_now_usec();
    //~ for (uint8_t h=0; h<16; h++) {
        //~ for (uint8_t i=0; i<3; i++) {
            //~ for (uint8_t j=0; j<5; j++) {
                //~ meas[i][j] += daq_meas(i+1, j);
                //~ if (h == 15) meas[i][j] /= 16;
            //~ }
        //~ }
    //~ }
    //~ stop = xtimer_now_usec();
    //~ printf("Got 240 measures in %iµs\n", (int)(stop - start));
    //~ ps();
    //~ for (uint8_t i=0; i<3; i++) {
        //~ printf("Leg %u:\tIH=%u\tIL=%u\tVL=%u\tVH=%u\tT=%u\n", i+1, meas[i][IH], meas[i][IL], meas[i][VL], meas[i][VH], meas[i][T]);
    //~ }

#ifdef OT_BANNER_H
    /* Print the owntech banner */
    printf(OT_BANNER);
#endif
    
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
