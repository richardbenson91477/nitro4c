#include "../sys.h"
#include "../../timer/timer.h"

int main (void) {
    dlog_debug (_, "main begin");

    name_map *_name_map_sys = malloc(sizeof(name_map));
    if (! name_map_cons (_name_map_sys))
        return 1;

    sys *_sys = malloc(sizeof(sys));
    if (! sys_cons (_sys))
        return 2;

    _sys->_parent_name_map = _name_map_sys;
    _sys->id = name_map_add (_name_map_sys, "sys_1");
    if (! _sys->id)
        return 3;

    if (! sys_load (_sys, _, "sys_1.sys"))
        return 4;

    if (! sys_set_state (_sys, 0))
        return 5;

    timer *_timer = malloc(sizeof(timer));
    if (! timer_cons (_timer))
        return 6;
 
    timer_tick (_timer);

    const float tick_t = 1.0 / 5.0;
    float t = 0.0;
    for (uint32_t tick_c = 0; tick_c < 10; ) {
        t += timer_tick (_timer);
        if (t < tick_t) {
            usleep (tick_t / 10.0);
        }
        else {
            t -= tick_t;
            tick_c ++;
            dlog_debug_f (_, "t: ", (float)tick_c * tick_t);
 
            if (! sys_tick (_sys, tick_t))
                return 7;
        }
    }

    if (! timer_decons (_timer))
        return 8;
    free(_timer);
 
    if (! sys_decons (_sys))
        return 9;
    free(_sys);

    if (! name_map_decons (_name_map_sys))
        return 10;
    free(_name_map_sys);

    dlog_debug (_, "main end");
    return 0;
}

