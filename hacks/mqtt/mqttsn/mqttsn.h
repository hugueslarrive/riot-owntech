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
 * @brief   MQTT-SN communication layer
 * @date    2020
 * @author  Hugues Larrive <hlarrive@laas.fr>
 */

#ifndef MQTTSN_H
#define MQTTSN_H

#include "net/asymcute.h"

#define REQ_CTX_NUMOF       (8U)    /**< Number of request context available */
#define SUB_CTX_NUMOF       (8U)    /**< Number of subscribe context available */
#define TOPIC_BUF_NUMOF     (8U + SUB_CTX_NUMOF)    /**< Number of topic buffers */

/**
 * @brief   Start the asymcute_main thread
 * 
 * @param[in]   priority    the symcute_main thread priority
 */
void mqttsn_init(char priority);

/**
 * @brief   Connect to the MQTT-SN broker
 * 
 * @param[in]   token       access token or client id
 * @param[in]   address     ipv6 address tring of the broker 
 *                          (ex: "[fec0:affe::1]:1885")
 */
void mqttsn_con(char* token, char* address);

/**
 * @brief   Subscribe to a topic
 * 
 * @param[in]   topic       topic to which to subscibe
 */
void mqttsn_sub(char* topic);

/**
 * @brief   Publish data in a topic
 * 
 * @param[in]   topic       topic in which to publish
 * @param[in]   data        data string
 */
void mqttsn_pub(char* topic, char* data);

#endif /* MQTTSN_H */
