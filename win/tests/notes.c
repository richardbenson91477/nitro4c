#include "../win.h" 
#include "../../colors/colors.h"

const uint32_t win_w = 400,
      win_h = 400;

uint32_t pen_color = COLOR_purple;
uint32_t bg_colors[] = {
    COLOR_navajo_white, COLOR_yellow, COLOR_green, COLOR_gray80
};
int32_t m_x1, m_y1, m_x3, m_y3;

void on_key (void *_v) {
    //win *_win = _v;
    pen_color = COLOR_black + rand () % 0x1000000;
}

void on_click (void *_v) {
    win *_win = _v;
    if (_win->m_b0_) {
        m_x1 = _win->m_x;
        m_y1 = _win->m_y;
    }
    else if (_win->m_b1_) {
        m_x3 = _win->m_x;
        m_y3 = _win->m_y;
    }
    else if (_win->m_b2_)
        draw_clear (_win->_draw, bg_colors[_win->user_id]);
}

void on_declick (void *_v) {
    win *_win = _v;
    if (_win->m_b1_)
        draw_rect (_win->_draw, m_x3, m_y3,
                _win->m_x - m_x3, _win->m_y - m_y3, pen_color);
}

void on_move (void *_v) {
    win *_win = _v;
    if (_win->m_b0_) {
        int32_t old_x = m_x1;
        int32_t old_y = m_y1;
        m_x1 = _win->m_x;
        m_y1 = _win->m_y;
        draw_line (_win->_draw, m_x1, m_y1, old_x, old_y, pen_color);
        draw_line (_win->_draw, m_x1 + 1, m_y1, old_x + 1, old_y, pen_color);
        draw_line (_win->_draw, m_x1, m_y1 + 1, old_x, old_y + 1, pen_color);
    }
}

void on_close (void *_v) {
    win *_win = _v;
    win_decons (_win);
}

int main (void) {
    if (! win_init ())
        return 1;

    win_conf *_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_win_conf))
        return 2;

    _win_conf->w = win_w;
    _win_conf->h = win_h;
    _win_conf->_title_s = "note";
    _win_conf->_ext_key_fn = on_key;
    _win_conf->_ext_click_fn = on_click;
    _win_conf->_ext_declick_fn = on_declick;
    _win_conf->_ext_move_fn = on_move;
    _win_conf->_ext_close_fn = on_close;

    const uint32_t win_n = 4;
    for (uint32_t c = 0; c < win_n; c ++) {
        win *_win = malloc(sizeof (win));
        if (! win_cons (_win, _win_conf))
            return c + 3;

        _win->free_ = 1;
        _win->user_id = c;
    
        if (! win_open (_win))
            return c + 3 + win_n;

        draw_clear (_win->_draw, bg_colors[c]);
    }

    while (win_tick ()) { 
        win_render_all ();
    }

    win_conf_decons (_win_conf);
    free (_win_conf);
    win_deinit ();
    return 0;
}

