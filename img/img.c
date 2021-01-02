#include "img.h"

bool img_cons (img *_i) {
    dlog_debug (_, "img_cons");

    _i->w = 0;
    _i->h = 0;
    _i->_b = _;
    _i->b_n = 0;
    _i->free_b_ = true;

    return true;
}

bool img_decons (img *_i) {
    dlog_debug (_, "img_decons");

    if (_i->free_b_ && _i->_b)
        free(_i->_b);

    _i->_b = _;
    return true;
}

void img_free (void *_v) {
    dlog_debug (_, "img_free");
    if (! _v) {
        dlog_error (_, "img_free: missing _v");
        return;
    }

    if (! img_decons ((img *)_v))
        return;

    free(_v);
}

bool img_load_raw (img *_i, arc *_arc, char const *_name_s,
        int32_t w, int32_t h) {
    dlog_debug_s (_, "img_load_raw", _name_s);

    uint8_t *_b;
    uint32_t b_n;
    
    if (! arc_or_file_get_b (_arc, _name_s, &_b, &b_n)) {
        dlog_error_s (_, "img_load_raw: arc_or_fs_get_b", _name_s);
        return false;
    }

    _i->w = w;
    _i->h = h;
    _i->_b = _b;
    _i->b_n = b_n;

    if (_arc) {
        _i->free_b_ = false;
    }

    return true;
}

bool img_load_rwh (img *_i, arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "img_load_rwh", _name_s);

    uint8_t *_b;
    uint32_t b_n;

    if (! arc_or_file_get_b (_arc, _name_s, &_b, &b_n)) {
        dlog_error_s (_, "img_load_raw: arc_or_fs_get_b", _name_s);
        return false;
    }

    uint8_t *_m = _b;
    _i->w = *(int32_t *)_m;
    _m += 4;
    _i->h = *(int32_t *)_m;
    _m += 4;
    _i->b_n = _i->w * _i->h * 4;
    
    if (_arc) {
        _i->_b = _m;
        _i->free_b_ = false;
    }
    else {
        _i->_b = (uint8_t *)malloc(_i->b_n);
        memcpy(_i->_b, _m, _i->b_n);
        free(_b);
    }

    return true;
}

bool img_save_raw (img const *_i, char const *_name_s) {
    dlog_debug_s (_, "img_save_raw", _name_s);

    file *_f = malloc(sizeof(file));
    file_cons_from_b (_f, _i->_b, _i->b_n);
    if (! file_export (_f, _name_s)) {
        dlog_error_s (_, "img_save_raw", _name_s);
        return false;
    }
    file_decons (_f);
    free(_f);

    return true;
}

bool img_save_rwh (img const *_i, char const *_name_s) {
    dlog_debug_s (_, "img_save_rwh", _name_s);

    uint32_t b_n = 4 + 4 + _i->b_n;
    uint8_t *_b = malloc(b_n);

    *(uint32_t *)_b = _i->w;
    *(uint32_t *)(_b + 4) = _i->h;
    memcpy(_b + 4 + 4, _i->_b, _i->b_n);
    
    file *_f = malloc(sizeof(file));
    file_cons_from_b (_f, _b, b_n);
    if (! file_export (_f, _name_s)) {
        dlog_error_s (_, "img_save_raw", _name_s);
        return false;
    }
    file_decons (_f);
    free(_f);

    return true;
}

bool img_swap_r_b (img *_i) {
    dlog_debug (_, "img_swap_r_b");
    
    if (! _i->_b) {
        dlog_error (_, "img_swap_r_b: missing _i->_b");
        return false;
    }

    uint8_t *_b = _i->_b;
    for (int32_t c = 0; c < (_i->w * _i->h); c ++) {
        _b[0] += _b[2];
        _b[2] = _b[0] - _b[2];
        _b[0] -= _b[2];
        _b += 4;
    }

    return true;
}

bool img_from_rgb24 (img *_i, int32_t w, int32_t h, uint8_t *_b) {
    dlog_debug (_, "img_from_rgb24");

    if (! _b) {
        dlog_error (_, "img_from_rgb24: missing _b");
        return false;
    }
    
    _i->w = w;
    _i->h = h;
    _i->b_n = w * h * 4;

    _i->_b = (uint8_t *)malloc(_i->b_n);
    if (! _i->_b) {
        dlog_error_u (_, "img_from_rgb24: malloc", _i->b_n);
        return false;
    }
    
    uint8_t *_m = _b;
    uint8_t *_n = _i->_b;
    for (int32_t c = 0; c < (w * h); c ++) {
        *_n = *_m++;
        _n ++;
        *_n = *_m++;
        _n ++;
        *_n = *_m++;
        _n ++;
        *_n = 255;
        _n ++;
    }

    return true;
}

