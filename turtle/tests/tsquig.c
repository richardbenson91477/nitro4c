#include "../turtle.h"

const int32_t win_w = 500,
        win_h = 500;

int main (void) {
    turtle tuck;
    if (! turtle_cons (&tuck, win_w, win_h))
        return 1;
   
    turtle_bg_color (&tuck, COLOR_black);
    turtle_clear (&tuck);

    uint32_t tick_c = 0;
    while (turtle_tick (&tuck)) {
        if (! (tick_c % 4))
            turtle_clear (&tuck);

        for (uint32_t c = 0; c < 10; c ++) {
            turtle_pen_color (&tuck,
                    ((0x80 + (random() % 0x80)) << 16) +
                    ((0x80 + (random() % 0x80)) << 8) +
                    (0x80 + (random() % 0x80)));

            turtle_up (&tuck);
            turtle_home (&tuck);
            turtle_down (&tuck);
            for (uint32_t c = 0; c < 300; c ++) {
                turtle_left (&tuck, (0.5 - ((float)random() / (float)RAND_MAX))
                    * 30.0);
                turtle_forward (&tuck, 1.1);
            }
        }

        tick_c += 1;
        turtle_render (&tuck);
    }

    turtle_decons (&tuck);
    return 0;
}

