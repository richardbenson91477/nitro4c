#include <stdio.h>
#include "../../dlog/dlog.h"
#include "../timer.h"

int main (int argc, char **argv) {
    const uint32_t tick_n = 60;
    const float tick_t = 1.0 / 30.0;
    const float proj_t = tick_t * (float)tick_n;

    dlog_debug_u (_, "tick_n", tick_n);
    dlog_debug_f (_, "tick_t", tick_t);
    dlog_debug_f (_, "proj_t", proj_t);

    timer *_timer = malloc(sizeof(timer));
    timer_cons (_timer);
    dlog_debug (_, "timer_tick ()");
    timer_tick (_timer);
   
    uint32_t tick_c = 0;
    float t = 0.0;
    float tot_t = 0.0;
    while (tick_c < tick_n) {
        float t_lapse = timer_tick (_timer);
        t += t_lapse;
        tot_t += t_lapse;
        if (t < tick_t) {
            usleep(tick_t / 10.0);
        }
        else {
            t -= tick_t;
            tick_c ++;
            dlog_debug_u (_, "tick_c", tick_c);
        }
    }

    dlog_debug_f (_, "tot_t", tot_t);
    dlog_debug_f (_, "error", proj_t - tot_t);
    
    timer_decons (_timer);
    free(_timer);

    return 0;
}

