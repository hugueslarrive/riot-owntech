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

#include "test_mqttsn.h"

static char listener_stack[ASYMCUTE_LISTENER_STACKSIZE];
static asymcute_con_t _connection;
static asymcute_req_t _reqs[REQ_CTX_NUMOF];
static asymcute_sub_t _subscriptions[SUB_CTX_NUMOF];
static asymcute_topic_t _topics[TOPIC_BUF_NUMOF];

asymcute_req_t *_get_req_ctx(void)
{
    for (unsigned i = 0; i < REQ_CTX_NUMOF; i++) {
        if (!asymcute_req_in_use(&_reqs[i])) {
            return &_reqs[i];
        }
    }
    puts("error: no request context available\n");
    return NULL;
}

asymcute_sub_t *_get_sub_ctx(void)
{
    for (unsigned i = 0; i < SUB_CTX_NUMOF; i++) {
        if (!asymcute_sub_active(&_subscriptions[i])) {
            return &_subscriptions[i];
        }
    }
    return NULL;
}

asymcute_sub_t *_find_sub(const char *name)
{
    for (unsigned i = 0; i < SUB_CTX_NUMOF; i++) {
        if (asymcute_sub_active(&_subscriptions[i]) &&
            strcmp(_subscriptions[i].topic->name, name) == 0) {
            return &_subscriptions[i];
        }
    }
    return NULL;
}

uint16_t _topic_parse_pre(const char *name)
{
    if (strncmp(name, "pre_", 4) == 0) {
        return (uint16_t)atoi(&name[4]);
    }
    return 0;
}

int _topic_init(asymcute_topic_t *t, const char *name)
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

int _topic_find(asymcute_topic_t *t, const char *name)
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

void _topics_clear(void)
{
    memset(_topics, 0, sizeof(_topics));
}

asymcute_topic_t *_topic_get_free(void)
{
    for (unsigned i = 0; i < TOPIC_BUF_NUMOF; i++) {
        if (!asymcute_topic_is_init(&_topics[i])) {
            return &_topics[i];
        }
    }
    return NULL;
}

void _topic_print_help(void)
{
    puts("        topic can be\n"
         "        - short topic: 2 byte string\n"
         "        - predefined topic id: pre_XXXXX (e.g. pre_738)\n"
         "        - normal: any string\n");
}

int _qos_parse(int argc, char **argv, int pos, unsigned *flags)
{
    if (argc <= pos) {
        return 0;
    }
    /* parse QoS level */
    int qos = atoi(argv[pos]);
    switch (qos) {
        case 0: *flags = MQTTSN_QOS_0; break;
        case 1: *flags = MQTTSN_QOS_1; break;
        case 2: *flags = MQTTSN_QOS_2; break;
        default: return -1;
    }
    return qos;
}

void _on_con_evt(asymcute_req_t *req, unsigned evt_type)
{
    printf("Request %p: ", (void *)req);
    switch (evt_type) {
        case ASYMCUTE_TIMEOUT:
            puts("Timeout");
            break;
        case ASYMCUTE_REJECTED:
            puts("Rejected by gateway");
            break;
        case ASYMCUTE_CONNECTED:
            puts("Connection to gateway established");
            break;
        case ASYMCUTE_DISCONNECTED:
            puts("Connection to gateway closed");
            _topics_clear();
            break;
        case ASYMCUTE_REGISTERED:
            puts("Topic registered");
            break;
        case ASYMCUTE_PUBLISHED:
            puts("Data was published");
            break;
        case ASYMCUTE_SUBSCRIBED:
            puts("Subscribed topic");
            break;
        case ASYMCUTE_UNSUBSCRIBED:
            puts("Unsubscribed topic");
            break;
        case ASYMCUTE_CANCELED:
            puts("Canceled");
            break;
        default:
            puts("unknown event");
            break;
    }
}

void _on_pub_evt(const asymcute_sub_t *sub, unsigned evt_type,
                        const void *data, size_t len, void *arg)
{
    (void)arg;

    if (evt_type == ASYMCUTE_PUBLISHED) {
        char *in = (char *)data;

        printf("subscription to topic #%i [%s]: NEW DATA\n",
               (int)sub->topic->id, sub->topic->name);
        printf("         data -> ");
        for (size_t i = 0; i < len; i++) {
            printf("%c", in[i]);
        }
        puts("");
        printf("              -> %u bytes\n", (unsigned)len);
    }
    else if (evt_type == ASYMCUTE_CANCELED) {
        printf("subscription -> topic #%i [%s]: CANCELED\n",
               (int)sub->topic->id, sub->topic->name);
    }
}

int _ok(asymcute_req_t *req)
{
    printf("Request %p: issued\n", (void *)req);
    return 0;
}

int _cmd_connect(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage %s <cli id> <addr> [<will topic> <will msg>]\n",
               argv[0]);
        return 1;
    }

    /* get sock ep */
    sock_udp_ep_t ep;
    if (sock_udp_str2ep(&ep, argv[2]) != 0) {
        puts("error: unable to parse gateway address");
        return 1;
    }
    if (ep.port == 0) {
        ep.port = MQTTSN_DEFAULT_PORT;
    }

    /* get request context */
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        return 1;
    }

    if (asymcute_connect(&_connection, req, &ep, argv[1], true, NULL)
        != ASYMCUTE_OK) {
        puts("error: failed to issue CONNECT request");
        return 1;
    }
    return _ok(req);
}

int _cmd_disconnect(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* get request context */
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        return 1;
    }

    if (asymcute_disconnect(&_connection, req) != ASYMCUTE_OK) {
        puts("error: failed to issue DISCONNECT request");
        return 1;
    }
    return _ok(req);
}

int _cmd_reg(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: %s <topic name>\n", argv[0]);
        _topic_print_help();
        return 1;
    }

    if (_topic_find(NULL, argv[1]) == 0) {
        puts("success: topic already registered (or no registration needed)\n");
        return 0;
    }

    /* find unused slot */
    asymcute_topic_t *t = NULL;
    for (unsigned i = 0; i < TOPIC_BUF_NUMOF; i++) {
        if (!asymcute_topic_is_reg(&_topics[i])) {
            t = &_topics[i];
            break;
        }
    }
    if (t == NULL) {
        puts("error: no empty slot left for storing the topic\n");
        return 1;
    }

    /* send registration request */
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        return 1;
    }
    if (_topic_init(t, argv[1]) != 0) {
        puts("error: unable to initialize topic");
        return 1;
    }
    if (asymcute_register(&_connection, req, t) != ASYMCUTE_OK) {
        puts("error: unable to send REGISTER request\n");
        return 1;
    }
    return _ok(req);
}

int _cmd_unreg(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: %s <topic name>\n", argv[0]);
        return 1;
    }

    unsigned i = 0;
    for (; i < TOPIC_BUF_NUMOF; i++) {
        if (strcmp(argv[1], _topics[i].name) == 0) {
            for (unsigned s = 0; s < SUB_CTX_NUMOF; s++) {
                if (_subscriptions[i].topic == &_topics[i]) {
                    puts("error: topic used in active subscription");
                    return 1;
                }
            }
            memset(&_topics[i], 0, sizeof(asymcute_topic_t));
            puts("success: removed local entry for given topic");
            break;
        }
    }
    if (i == TOPIC_BUF_NUMOF) {
        puts("error: unable to find topic in list of registered topics");
    }

    return 0;
}

int _cmd_pub(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: %s <topic> <data> [QoS level]\n", argv[0]);
        _topic_print_help();
        return 1;
    }

    /* parse and register topic */
    asymcute_topic_t t;
    if (_topic_find(&t, argv[1]) != 0) {
        puts("error: given topic is not registered");
        return 1;
    }

    /* parse QoS level */
    unsigned flags = 0;
    int qos = _qos_parse(argc, argv, 3, &flags);
    if (qos < 0) {
        puts("error: unable to parse QoS level");
        return 1;
    }

    /* get request context */
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        return 1;
    }

    /* publish data */
    size_t len = strlen(argv[2]);
    if (asymcute_publish(&_connection, req, &t, argv[2], len, flags) !=
        ASYMCUTE_OK) {
        puts("error: unable to send PUBLISH message");
        return 1;
    }
    if (qos == 0) {
        printf("Request %p: issued (one way)\n", (void *)req);
        return 0;
    }
    return _ok(req);
}

int _cmd_sub(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: %s <topic> [QoS level]\n", argv[0]);
        _topic_print_help();
        return 1;
    }

    /* parse QoS level */
    unsigned flags = 0;
    int qos = _qos_parse(argc, argv, 2, &flags);
    if (qos < 0) {
        puts("error: unable to parse QoS level");
        return 1;
    }

    /* get request context */
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        return 1;
    }

    /* get subscription context */
    asymcute_sub_t *sub = _get_sub_ctx();
    if (sub == NULL) {
        puts("error: unable to allocate subscription context");
        return 1;
    }

    /* parse topic */
    asymcute_topic_t *t = _topic_get_free();
    if (t == NULL) {
        puts("error: no free topic memory");
        return 1;
    }
    if (_topic_init(t, argv[1]) != 0) {
        puts("error: unable to initialize topic");
        return 1;
    }

    printf("using req %p, sub %p\n", (void *)req, (void *)sub);

    if (asymcute_subscribe(&_connection, req, sub, t, _on_pub_evt, NULL, flags)
        != ASYMCUTE_OK) {
        asymcute_topic_reset(t);
        puts("error: unable to send SUBSCRIBE request");
        return 1;
    }

    return _ok(req);
}

int _cmd_unsub(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: %s <topic>\n", argv[0]);
        return 1;
    }

    /* find active subscription for given topic name */
    asymcute_sub_t *sub = _find_sub(argv[1]);
    if (sub == NULL) {
        puts("error: no subscription for given topic found");
        return 1;
    }

    /* get request context */
    asymcute_req_t *req = _get_req_ctx();
    if (req == NULL) {
        return 1;
    }

    /* issue unsubscribe request */
    if (asymcute_unsubscribe(&_connection, req, sub) != ASYMCUTE_OK) {
        puts("error: unable to send UNSUBSCRIBE request");
        return 1;
    }

    return _ok(req);
}

int _cmd_info(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    puts("--- Asymcute MQTT-SN client state ---");
    puts("Topics:");
    for (unsigned i = 0; i < TOPIC_BUF_NUMOF; i++) {
        printf("topic #%2u - ", i);
        if (asymcute_topic_is_reg(&_topics[i])) {
            printf("[registered] id: %u, name: %s\n",
                    (unsigned)_topics[i].id, _topics[i].name);
        }
        else {
            puts("[unused]");
        }
    }

    puts("Subscriptions:");
    for (unsigned i = 0; i < SUB_CTX_NUMOF; i++) {
        printf("sub   #%2u - ", i);
        if (asymcute_sub_active(&_subscriptions[i])) {
            printf("[subscribed] id: %u, name: %s\n",
                   (unsigned)_subscriptions[i].topic->id,
                   _subscriptions[i].topic->name);
        }
        else {
            puts("[unused]");
        }
    }

    return 0;
}

void test_mqttsn_init(char priority)
{
    /* setup the connection context */
    asymcute_listener_run(&_connection, listener_stack, sizeof(listener_stack),
                          priority, _on_con_evt);
}

