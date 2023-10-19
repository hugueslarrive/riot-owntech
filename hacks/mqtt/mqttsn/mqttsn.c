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

#include "mqttsn.h"

static char listener_stack[ASYMCUTE_LISTENER_STACKSIZE];
static asymcute_con_t _connection;
static asymcute_req_t _reqs[REQ_CTX_NUMOF];
static asymcute_sub_t _subscriptions[SUB_CTX_NUMOF];
static asymcute_topic_t _topics[TOPIC_BUF_NUMOF];
static void _on_con_evt(asymcute_req_t *req, unsigned evt_type);
static asymcute_req_t *_get_req_ctx(void);
static asymcute_sub_t *_get_sub_ctx(void);
static void _on_pub_evt(const asymcute_sub_t *sub, unsigned evt_type,
                        const void *data, size_t len, void *arg);
static asymcute_topic_t *_topic_get_free(void);
static int _topic_init(asymcute_topic_t *t, const char *name);
static int _topic_find(asymcute_topic_t *t, const char *name);
static uint16_t _topic_parse_pre(const char *name);

void mqttsn_init(char priority)
{
    /* setup the connection context */
    asymcute_listener_run(&_connection, listener_stack, sizeof(listener_stack),
                          priority, _on_con_evt);
}

void mqttsn_con(char* token, char* address)
{
    /* get sock ep */
    sock_udp_ep_t ep;
    sock_udp_str2ep(&ep, address);
    /* get request context */
    asymcute_req_t *con_req = _get_req_ctx();
    /* connect */
    asymcute_connect(&_connection, con_req, &ep, token, true, NULL);
}    

void mqttsn_sub(char* topic)
{
    /* get request context */
    asymcute_req_t *req = _get_req_ctx();
    /* get subscription context */
    asymcute_sub_t *sub = _get_sub_ctx();
    /* parse topic */
    asymcute_topic_t *t = _topic_get_free();
    /* initialize topic */
    _topic_init(t, topic);
    /* send SUBSCRIBE request */
    unsigned flags = 0;
    asymcute_subscribe(&_connection, req, sub, t, _on_pub_evt, NULL, flags);
}

void mqttsn_pub(char* topic, char* data)
{
    /* parse and register topic */
    asymcute_topic_t t;
    _topic_find(&t, topic);

    /* get request context */
    asymcute_req_t *req = _get_req_ctx();

    /* publish data */
    size_t len = strlen(data);
    unsigned flags = 0;
    asymcute_publish(&_connection, req, &t, data, len, flags);
}

/* Dummy function to please asymcute_listener_run() */
static void _on_con_evt(asymcute_req_t *req, unsigned evt_type)
{
    (void)req;
    (void)evt_type;
    return;
}

/* Muted version of examples/asymcute_mqttsn _get_req_ctx() */
static asymcute_req_t *_get_req_ctx(void)
{
    for (unsigned i = 0; i < REQ_CTX_NUMOF; i++) {
        if (!asymcute_req_in_use(&_reqs[i])) {
            return &_reqs[i];
        }
    }
    //~ puts("error: no request context available\n");
    return NULL;
}

/* Function from examples/asymcute_mqttsn/main.c used in mqttsn_sub() */
static asymcute_sub_t *_get_sub_ctx(void)
{
    for (unsigned i = 0; i < SUB_CTX_NUMOF; i++) {
        if (!asymcute_sub_active(&_subscriptions[i])) {
            return &_subscriptions[i];
        }
    }
    return NULL;
}

/* Dummy function to please asymcute_subscribe() */
static void _on_pub_evt(const asymcute_sub_t *sub, unsigned evt_type,
                        const void *data, size_t len, void *arg)
{
    (void)sub;
    (void)evt_type;
    (void)data;
    (void)len;
    (void)arg;
    return;
}

/* Function from examples/asymcute_mqttsn/main.c used in mqttsn_sub() */
static asymcute_topic_t *_topic_get_free(void)
{
    for (unsigned i = 0; i < TOPIC_BUF_NUMOF; i++) {
        if (!asymcute_topic_is_init(&_topics[i])) {
            return &_topics[i];
        }
    }
    return NULL;
}

/* Function from examples/asymcute_mqttsn/main.c used in mqttsn_sub() */
static int _topic_init(asymcute_topic_t *t, const char *name)
{
    uint16_t id = _topic_parse_pre(name);

    if (id != 0) {
        name = NULL;
    }

    if (asymcute_topic_init(t, name, id) != ASYMCUTE_OK) {
        return 1;
    }
    return 0;
}

/* Function from examples/asymcute_mqttsn/main.c used in mqttsn_pub() */
static int _topic_find(asymcute_topic_t *t, const char *name)
{
    size_t len = strlen(name);
    uint16_t id = _topic_parse_pre(name);

    if ((id != 0) || (len == 2)) {
        if (t) {
            return _topic_init(t, name);
        }
        return 0;
    }

    /* need to find topic in list of registered ones */
    for (unsigned i = 0; i < TOPIC_BUF_NUMOF; i++) {
        if (asymcute_topic_is_reg(&_topics[i]) &&
            (strncmp(name, _topics[i].name, sizeof(_topics[i].name)) == 0)) {
            if (t) {
                *t = _topics[i];
            }
            return 0;
        }
    }
    return 1;
}

/* Function from examples/asymcute_mqttsn/main.c used in _topic_init() */
static uint16_t _topic_parse_pre(const char *name)
{
    if (strncmp(name, "pre_", 4) == 0) {
        return (uint16_t)atoi(&name[4]);
    }
    return 0;
}


