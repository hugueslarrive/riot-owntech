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
 * @brief   shell_commands[] array definition  
 * @date    2020
 * @author  Hugues Larrive <hugues.larrive@laas.fr>
 * 
 */


#ifndef OT_SHELL_COMMANDS_H
#define OT_SHELL_COMMANDS_H

static const shell_command_t shell_commands[] = {
#ifdef TEST_DAQ_H
    { "daq_meas", "acquire a measure from a leg sensor", test_daq_meas },
#endif
#ifdef MODULE_GNRC_UDP
    { "udp", "send udp packets", udp_send },
    { "udps", "start udp server", udp_server },
#endif
#ifdef TEST_PWM_H
    { "legset", "set inverter leg duty cycle", test_pwm_legset },
    { "sinadd", "add legs to sin50hz thread", test_pwm_sinadd },
    { "sinrm", "remove legs from sin50hz thread", test_pwm_sinrm },
    { "getysc", "call sin50hz_get_yscale()", test_pwm_getysc },
    { "setysc", "call sin50hz_set_yscale(uint16_t scale)", test_pwm_setysc },
    { "setshift", "call sin50hz_set_shift(uint16_t shift)", test_pwm_setshift },
    { "sin_bench", "start a sine wave with sine oscillating amplitude", test_pwm_sin_bench },
#endif
#ifdef TEST_MQTTSN_H
    { "connect", "connect to MQTT-SN gateway", _cmd_connect },
    { "disconnect", "disconnect from MQTT-SN gateway", _cmd_disconnect },
    { "reg", "register a given topic", _cmd_reg },
    { "unreg", "remove a topic registration [locally]", _cmd_unreg },
    { "pub", "publish data", _cmd_pub },
    { "sub", "subscribe to topic", _cmd_sub },
    { "unsub", "unsubscribe from topic", _cmd_unsub },
    { "info", "print state information", _cmd_info },
#endif
    { NULL, NULL, NULL }
};

#endif /* OT_SHELL_COMMANDS_H */
