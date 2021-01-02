#include "timer.h"
    
bool timer_cons (timer *_timer) {
    timer_reset (_timer);
    timer_tick (_timer);
    return true;
}

bool timer_decons (timer *_timer) {
    return true;
}

void timer_reset (timer *_timer) {
    _timer->init_sec = 0;
    _timer->prev_t = 0.0;
}

float timer_tick (timer *_timer) {   
    struct timeval tv;
    gettimeofday(&tv, _);

    if (_timer->init_sec == 0) {
        _timer->init_sec = tv.tv_sec;
        float now_t = (float)tv.tv_usec / 1000000.0 +
            (float)(tv.tv_sec - _timer->init_sec);
        _timer->prev_t = now_t;
        return 0.0;
    }

    float now_t = (float)tv.tv_usec / 1000000.0 +
        (float)(tv.tv_sec - _timer->init_sec);

    float _t = now_t - _timer->prev_t;
    _timer->prev_t = now_t;
    return _t;
}

