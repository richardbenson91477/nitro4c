#include "turtle.h"

bool turtle_cons (turtle *_t, int32_t w, int32_t h) {
    if (! win_init ())
        return false;

    _t->bg_color = COLOR_gray80;
    _t->pen_color = COLOR_gray10;

    _t->down_ = true;
    _t->show_ = true;

    _t->x_init = (float)w / 2.0;
    _t->y_init = (float)h / 2.0;
    _t->a_init = 0.0;

    turtle_reset (_t);

    _t->_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_t->_win_conf))
        return false;

    _t->_win_conf->w = w;
    _t->_win_conf->h = h;
    _t->_win_conf->_title_s = "turtle";
    
    _t->_win = malloc(sizeof(win));
    if (! win_cons (_t->_win, _t->_win_conf))
        return false;

    if (! win_open (_t->_win))
        return false;

    _t->_stack = malloc(sizeof(blaze));
    if (! blaze_cons (_t->_stack))
        return false;

    return true;
}

bool turtle_decons (turtle *_t) {
    win_deinit ();
    free(_t->_win);
    _t->_win = _;

    if (! win_conf_decons (_t->_win_conf))
        return false;

    free(_t->_win_conf);
    _t->_win_conf = _;

    blaze_clear (_t->_stack, free);
    free(_t->_stack);
    _t->_stack = _;

    return true;
}

void turtle_goto (turtle *_t, float x, float y) {
    if (_t->down_)
        draw_line (_t->_win->_draw, (int)_t->x, (int)_t->y, (int)x, (int)y,
                _t->pen_color);
    _t->x = x;
    _t->y = y;
}

void turtle_forward (turtle *_t, float n) {
    float a_r = RADIANS(_t->a);
    turtle_goto (_t, (n * cosf(a_r)) + _t->x,
            (n * sinf(a_r)) + _t->y);
}

void turtle_back (turtle *_t, float n) {
    turtle_forward (_t, -n);
}

void turtle_push (turtle *_t) {
    float *save = malloc(4 * 3);
    save[0] = _t->x;
    save[1] = _t->y;
    save[2] = _t->a;
    blaze_add (_t->_stack, save);
}

void turtle_pop (turtle *_t) {
    float *saved = blaze_get (_t->_stack, _t->_stack->item_n - 1);
    _t->a = saved[2];
    turtle_goto (_t, saved[0], saved[1]);
    blaze_del (_t->_stack, _, _t->_stack->item_n - 1, free);
}

void turtle_home (turtle *_t) {
    turtle_goto (_t, _t->x_init, _t->y_init);
}

void turtle_left (turtle *_t, float a) {
    _t->a -= a;
    while (_t->a < 0.0)
        _t->a += 360.0;
}

void turtle_right (turtle *_t, float a) {
    _t->a += a;
    while (_t->a > 360.0)
        _t->a -= 360.0;
}

void turtle_reset (turtle *_t) {
    _t->x = _t->x_init;
    _t->y = _t->y_init;
    _t->a = _t->a_init;
}

bool turtle_tick (turtle const *_t) {
    return win_tick ();
}

void turtle_render (turtle const *_t) {
    win_render (_t->_win);
}

void turtle_clear (turtle const *_t) {
    draw_clear (_t->_win->_draw, _t->bg_color);
}

void turtle_bg_color (turtle *_t, uint32_t c) {
    _t->bg_color = c;
}

void turtle_pen_color (turtle *_t, uint32_t c) {
    _t->pen_color = c;
}

void turtle_up (turtle *_t) {
    _t->down_ = false;
}

void turtle_down (turtle *_t) {
    _t->down_ = true;
}

void turtle_show (turtle *_t) {
    _t->show_ = true;
}

void turtle_hide (turtle *_t) {
    _t->show_ = false;
}

