#include "../turtle.h"
#include "time.h"

const int32_t win_w = 1440,
        win_h = 720;

const uint8_t flakes_init_n = 20;
const float perim_min = 30.0;
const float perim_max = 150.0;

typedef struct  {
    float x, y;
    float a;
    float perim;
    uint32_t draw_steps;
    uint32_t rec_depth;
    float rec_edge_scale;
    float rad;
    float edge_len;
    float twist;
    uint32_t color;
} snowflake;

bool snowflake_cons (snowflake *_sf, float x, float y, float a, float perim,
        uint32_t draw_steps, uint32_t rec_depth, float rec_edge_scale) {
    _sf->x = x;
    _sf->y = y;
    _sf->a = a;
    _sf->perim = perim;
    _sf->draw_steps = draw_steps;
    _sf->rec_depth = rec_depth;
    _sf->rec_edge_scale = rec_edge_scale;

    _sf->rad = perim / PI;
    _sf->edge_len = perim / (float)draw_steps / 2.0;
    _sf->twist = 360.0 / (float)_sf->draw_steps;
    _sf->color = ((0x80 + (random() % 0x80)) << 16) +
            ((0x80 + (random() % 0x80)) << 8) +
            (0x80 + (random() % 0x80));
    return true;
}

void snowflake_draw_rec (snowflake const *_sf, turtle *_t, float edge_len,
        uint32_t rec_depth) {
    for (uint32_t c = 0; c < _sf->draw_steps; c ++) {
        turtle_left (_t, _sf->twist);
        turtle_forward (_t, edge_len);
        if (rec_depth)
            snowflake_draw_rec (_sf, _t, edge_len * _sf->rec_edge_scale,
                    rec_depth - 1);
        turtle_right (_t, _sf->twist * 2.0);
        turtle_forward (_t, edge_len);
    }
}

void snowflake_draw (snowflake const *_sf, turtle *_t) {
    turtle_up (_t);
    turtle_goto (_t, _sf->x, _sf->y);
    turtle_down (_t);
    turtle_pen_color (_t, _sf->color);
    turtle_left (_t, _sf->a);

    snowflake_draw_rec (_sf, _t, _sf->edge_len, _sf->rec_depth);

    turtle_right (_t, _sf->a);
}

static inline float random_f (void) {
    return ((float)random() / (float)RAND_MAX);
}

void random_flakes (blaze *_f, uint32_t flakes_n) {
    for (uint32_t c = 0; c < flakes_n; c ++) {
        float perim = perim_min + (random_f () * (perim_max - perim_min));
        float r = perim / PI;
        float x = random_f () * ((float)win_w - r);
        float y = r * -2.0;
        float a = random_f () * 360.0;
        uint32_t draw_steps = 5 + (random() % 6);
        float rec_depth;
        float detail = (1.0 - ((perim - perim_min) / perim_max)) *
            (float)(draw_steps - 5) / 10.0;
        if (detail < 0.10)
            rec_depth = (random() % 3) + 1;
        else if (detail < 0.7)
            rec_depth = (random() % 2) + 1;
        else
            rec_depth = random() % 2;
        float rec_edge_scale = random_f ();

        snowflake *_sf = malloc (sizeof (snowflake));
        snowflake_cons (_sf, x, y, a, perim, draw_steps,
                rec_depth, rec_edge_scale);
        blaze_add (_f, _sf);
    }
}

int main (void) {
    srand(time(_));

    turtle tuck;
    if (! turtle_cons (&tuck, win_w, win_h))
        return true;
   
    turtle_bg_color (&tuck, COLOR_gray14);
    turtle_clear (&tuck);

    blaze flakes;
    blaze_cons (&flakes);
    random_flakes (&flakes, flakes_init_n);

    while (turtle_tick (&tuck)) {
        turtle_clear (&tuck);
        for (snowflake *_sf = blaze_first (&flakes); _sf;) {
            snowflake_draw (_sf, &tuck);
            
            _sf->y += _sf->rad * 0.3;
            if ((_sf->y - (_sf->rad * 2.0)) > win_h) {
                blaze_del (&flakes, _sf, 0, free);
                random_flakes (&flakes, 1);
                _sf = blaze_cur (&flakes);
                continue;
            }

            _sf->a += _sf->rad / 10.0;
            if (_sf->a > 360.0)
                _sf->a -= 360.0;
                
            _sf = blaze_next (&flakes);
        }

        turtle_render (&tuck);
    }

    blaze_clear (&flakes, free);
    blaze_decons (&flakes);

    turtle_decons (&tuck);

    return false;
}

