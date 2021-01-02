#ifndef NITRO_NET_INCLUDED
#define NITRO_NET_INCLUDED

#include <resolv.h>
#include <netdb.h>

#include "../nitro_defs.h"
#include "../dlog/dlog.h"

typedef struct {
    bool srv_on_;
    int32_t srv_sock;
    int32_t srv_port;

    bool cl_connected_;
    int32_t cl_sock;
} net;

extern bool net_cons (net *_n);
extern bool net_decons (net *_n);
extern bool net_connect_to (net *_n, char const *_name_s, int32_t port);
extern bool net_server (net *_n, int32_t port, void (*srv_fn)(void *));
extern bool net_close_cl (net *_n);
extern bool net_get_line (net const *_n, char *buf, int32_t len);
extern bool net_send_str (net const *_n, char const *s);

#endif

