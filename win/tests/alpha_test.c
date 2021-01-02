#include "../win.h"
#include "../../img/img.h"
#include "../../colors/colors.h"

const uint32_t img_w = 256,
      img_h = 256;

int main (void) {
    img *_img = malloc(sizeof(img));
    if (! img_cons (_img))
        return 1;

    if (! img_load_raw (_img, _, "data/alpha_test.bgra", img_w, img_h))
        return 2;

    if (! win_init ())
        return 3;
    
    win_conf *_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_win_conf))
        return 4;

    _win_conf->w = img_w;
    _win_conf->h = img_h;
    _win_conf->_title_s = "alpha test";

    win *_win = malloc(sizeof(win));
    if (! win_cons (_win, _win_conf))
        return 5;

    _win->free_ = true;

    if (! win_open (_win))
        return 6;
    
    if (! win_icon_load (_win, _, "data/icon1.rwh"))
        return 7;

    uint32_t t = 0;
    while (win_tick ()) {
        t ++;
        if (t < 20)
            draw_clear (_win->_draw, COLOR_white);
        else if (t < 40)
            draw_clear (_win->_draw, COLOR_gray60);
        else {
            draw_clear (_win->_draw, COLOR_black);
            if (t == 60)
                t = 0;
        }
        draw_put_rect (_win->_draw, (uint32_t *)_img->_b, 1, 0, 0,
                _win->_conf->w, _win->_conf->h);
        win_render (_win);
    }
 
    win_deinit ();
    win_conf_decons (_win_conf);
    free (_win_conf);

    img_decons (_img);
    free (_img);

    return 0;
}

