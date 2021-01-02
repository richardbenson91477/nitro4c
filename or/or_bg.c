#include "or_bg.h"

bool or_bg_cons (or_bg *_bg) {
    dlog_debug (_, "or_bg_cons");

    _bg->_img = malloc(sizeof(img));
    if (! img_cons (_bg->_img))
        return false;

    return true;
}

bool or_bg_decons (or_bg *_bg) {
    dlog_debug (_, "or_bg_decons");

    if (_bg->_img) {
        if (! img_decons (_bg->_img))
            return false;

        free(_bg->_img);
        _bg->_img = _;
    }

    return true;
}

bool or_bg_load (or_bg *_bg, char const *_name_s) {
    dlog_debug_s (_, "or_bg_load", _name_s);

    if (! img_load_rwh (_bg->_img, _, _name_s))
        return false;

    return true;
}

bool or_bg_render (or_bg const *_bg, win *_win, int32_t ox, int32_t oy) {
    img *_i = _bg->_img;
    if (! _i) {
        draw_clear (_win->_draw, 0xff505050);
        return true;
    }

    draw_clear (_win->_draw, 0xff505050);
// FIXME: scrolling background
    draw_put_rect (_win->_draw, (uint32_t *)_i->_b, true, 0, 0, _i->w, _i->h);
    
    return true;
}

