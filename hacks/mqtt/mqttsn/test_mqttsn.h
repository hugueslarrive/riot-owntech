/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
/*
 * This code was extracted from examples/asymcute_mqttsn/main.c
 */
/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Example application for demonstrating RIOT's MQTT-SN library
 *              Asymcute
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#ifndef TEST_MQTTSN_H
#define TEST_MQTTSN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "shell.h"
#include "mutex.h"
#include "thread.h"
#include "net/asymcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#ifndef REQ_CTX_NUMOF
#define REQ_CTX_NUMOF       (8U)
#endif

#ifndef SUB_CTX_NUMOF
#define SUB_CTX_NUMOF       (8U)
#endif

#ifndef TOPIC_BUF_NUMOF
#define TOPIC_BUF_NUMOF     (8U + SUB_CTX_NUMOF)
#endif

//~ #define LISTENER_PRIO       (THREAD_PRIORITY_MAIN - 1)

asymcute_req_t *_get_req_ctx(void);
asymcute_sub_t *_get_sub_ctx(void);
asymcute_sub_t *_find_sub(const char *name);
uint16_t _topic_parse_pre(const char *name);
int _topic_init(asymcute_topic_t *t, const char *name);
int _topic_find(asymcute_topic_t *t, const char *name);
void _topics_clear(void);
asymcute_topic_t *_topic_get_free(void);
void _topic_print_help(void);
int _qos_parse(int argc, char **argv, int pos, unsigned *flags);
void _on_con_evt(asymcute_req_t *req, unsigned evt_type);
void _on_pub_evt(const asymcute_sub_t *sub, unsigned evt_type,
                        const void *data, size_t len, void *arg);
int _ok(asymcute_req_t *req);

/* Shell command handlers */
int _cmd_connect(int argc, char **argv);
int _cmd_disconnect(int argc, char **argv);
int _cmd_reg(int argc, char **argv);
int _cmd_unreg(int argc, char **argv);
int _cmd_pub(int argc, char **argv);
int _cmd_sub(int argc, char **argv);
int _cmd_unsub(int argc, char **argv);
int _cmd_info(int argc, char **argv);

/**
 * @brief   Start the asymcute_main thread
 * 
 * @param[in]   priority    the symcute_main thread priority
 */
void test_mqttsn_init(char priority);

#endif /* TEST_MQTTSN_H */
