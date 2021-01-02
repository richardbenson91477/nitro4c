#ifndef NITRO_TIMER_INCLUDED
#define NITRO_TIMER_INCLUDED

#include <time.h>
#include <sys/time.h>

#include "../nitro_defs.h"

typedef struct {
    time_t init_sec;
    float prev_t;
} timer;

extern bool timer_cons (timer *_timer);
extern bool timer_decons (timer *_timer);
extern float timer_tick (timer *_timer);
extern void timer_reset (timer *_timer);

#endif

