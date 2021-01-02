#include "../net.h"

void srv_fn (void *_v) {
    dlog_debug (_, "srv: srv_fn");
    net *_n = _v;

    char b_in[80] = {0};
    if (net_get_line (_n, b_in, sizeof(b_in))) {
        dlog_debug_s (_, "srv: srv_fn: received: ", b_in);
    }

    if (! strcmp (b_in, "REQUEST")) {
        dlog_debug (_, "srv: srv_fn: sending ACK");
        net_send_str (_n, "ACK");
    }

    return;
}

int main (void) {
    net net1;
    net_cons (&net1);

    dlog_debug (_, "srv: calling net_server");
    if (! net_server (&net1, 4404, srv_fn))
        return -1;

    net_decons (&net1);
    return 0;
} 

