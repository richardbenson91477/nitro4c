#include "../win.h"
#include "../../colors/colors.h"

const uint32_t w = 500;

int main (void) {
    if (! win_init ())
        return 1;

    win_conf *_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_win_conf))
        return 2;

    _win_conf->w = w;
    _win_conf->h = w;
    _win_conf->_title_s = "color test";

    win *_win = malloc(sizeof(win));
    if (! win_cons (_win, _win_conf))
        return 3;

    _win->free_ = true;

    if (! win_open (_win))
        return 4;

    draw_clear (_win->_draw, COLOR_black);

    const float rs = 2.0 / ((float)w * (float)w * 2.0);
    for (uint32_t x = 0; x < w; x ++)
        for (uint32_t y = 0; y < w; y ++) {
            uint32_t xx = x * x;
            uint32_t yy = y * y;
            uint32_t r = (uint32_t)(((xx + xx) * rs) * 255.0);
            uint32_t g = (uint32_t)(((yy + xx) * rs) * 255.0);
            uint32_t b = (uint32_t)(((yy + yy) * rs) * 255.0);
            draw_point (_win->_draw, x, y,
                    COLOR_black + (r << 16) + (g << 8) + b);
        }

    while (win_tick ())
        win_render (_win);

    win_deinit ();
    win_conf_decons (_win_conf);
    free (_win_conf);

    return 0;
}

