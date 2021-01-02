#include "../snd.h"
#include "../../timer/timer.h"

int main (int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 1;
    }

    if (! snd_init ())
        return 2;

    snd *_snd = malloc(sizeof(snd)); 
    snd_cons (_snd);
    if (! snd_load_raw (_snd, _, argv[1])) {
        snd_deinit ();
        return 3;
    }

    float _v[3] = {0.0, 0.0, 0.0};
    snd_pos3d (_snd, _v);

    if (! snd_play (_snd, -1)) {
        snd_deinit ();
        return 4;
    }

    uint32_t ticks_c = 0;
    uint32_t ticks_n = 10;
    const float step_t = 1.0;
    float t = 0.0;
    float a = 0.0;

    timer *_timer = malloc(sizeof(timer));
    timer_cons (_timer);

    while (ticks_c < ticks_n) {
        t += timer_tick (_timer);
        if (t < step_t) {
            usleep(step_t / 10.0);
        }
        else {
            t -= step_t;
            ticks_c ++;
            dlog_debug_u (_, "tick ", ticks_c);

            _v[0] = sin(a);
            _v[2] = cos(a);
            snd_pos3d (_snd, _v);

            a += 1.256;
        }
    }
    timer_decons (_timer);
    free (_timer);

    snd_decons (_snd);
    free (_snd);

    snd_deinit ();

    return 0;
}

