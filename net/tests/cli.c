#include "../net.h"

int main (void) {
    net net2;

    if (! net_connect_to (&net2, "localhost", 4404)) {
        dlog_debug (_, "cli: net_connect_to failed");
        return 1;
    }

    dlog_debug (_, "cli: sending REQUEST");
    net_send_str (&net2, "REQUEST");
    
    dlog_debug (_, "cli: calling net_get_line");
    char b_in [80] = {0};
    if (net_get_line (&net2, b_in, sizeof(b_in))) {
        dlog_debug_s (_, "cli: received: ", b_in);
    }

    net_decons (&net2);
    return 0;
} 

