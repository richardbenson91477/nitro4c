#include "draw.h"

bool draw_cons (draw *_draw, int32_t w, int32_t h) {
    dlog_debug (_, "draw_cons");

    _draw->_p = (uint32_t *)malloc(w * h * 4);
    if (! _draw->_p)
        return false;
    _draw->free_p_ = true;

    _draw->w = w;
    _draw->h = h;
    _draw->hw = w / 2;
    _draw->hh = h / 2;
    _draw->n = w * h;
    
    draw_init_alpha_table (_draw);
    return true;
}

bool draw_cons_from (draw *_draw, uint32_t *_p, int32_t w, int32_t h) {
    dlog_debug (_, "draw_cons_from");

    _draw->_p = _p;
    _draw->free_p_ = false;

    _draw->w = w;
    _draw->h = h;
    _draw->hw = w / 2;
    _draw->hh = h / 2;
    _draw->n = w * h;
    
    draw_init_alpha_table (_draw);
    return true;
}

bool draw_decons (draw *_draw) {
    dlog_debug (_, "draw_decons");

    if (_draw->free_p_) {
        if (! _draw->_p) {
            dlog_error (_, "draw_decons: free_p_ is set but _p is missing");
            return false;
        }
        free(_draw->_p);
        _draw->_p = _;
    }

    return true;
}

void draw_init_alpha_table (draw *_draw) {
    for (int a = 0; a < 256; a ++)
        for (int b = 0; b < 256; b ++)
            _draw->alpha_table[a][b] = ((uint8_t)((a * b) / 256));
}

void draw_clear (draw const *_draw, uint32_t col) {
    uint32_t *_p = _draw->_p;
    for (int32_t c = 0; c < _draw->n; c ++)
        *_p++ = col;
}

void draw_point (draw const *_draw, int32_t x, int32_t y, uint32_t col) {
    if ((x < 0) || (y < 0) || (x >= _draw->w) || (y >= _draw->h))
        return;

    _draw->_p[y * _draw->w + x] = col;
}

void draw_effect (draw const *_draw, uint32_t w, uint32_t d1, uint32_t d2) {
    uint32_t tmp_u32, tmp2_u32;
    uint8_t *_b = (uint8_t *)_draw->_p;

    // static: d1 = chance, d2 = brigtness range
    if (w == 0) {
        for (uint32_t c = 0; c < _draw->n; c ++) {
            tmp2_u32 = rand ();
            if (! (tmp2_u32 % d1)) {
                tmp2_u32 = rand ();
                tmp_u32 = _b[0] + ((tmp2_u32 & 255) % d2);
                if (tmp_u32 < 256)
                    _b[0] = tmp_u32;
                else
                    _b[0] = 255;

                tmp_u32 = _b[1] + (((tmp2_u32 >> 8) & 255) % d2);
                if (tmp_u32 < 256)
                    _b[1] = tmp_u32;
                else
                    _b[1] = 255;

                tmp_u32 = _b[2] + (((tmp2_u32 >> 16) & 255) % d2);
                if (tmp_u32 < 256)
                    _b[2] = tmp_u32;
                else
                    _b[2] = 255;
            }
            _b += 4;
        }
    }
    // ghosting: d1 = offset, d2 = depth
    else if (w == 1) {
        for (uint32_t line_c = 0; line_c < _draw->h; line_c ++) {
            for (uint32_t c = 0; c < _draw->w - d1; c ++) {
                tmp_u32 = _b[0] + _b[d1 * 4] / d2;
                if (tmp_u32 < 256)
                    _b[0] = tmp_u32;
                else
                    _b[0] = 255;

                tmp_u32 = _b[1] + _b[d1 * 4 + 1] / d2;
                if (tmp_u32 < 256)
                    _b[1] = tmp_u32;
                else
                    _b[1] = 255;

                tmp_u32 = _b[2] + _b[d1 * 4 + 2] / d2;
                if (tmp_u32 < 256)
                    _b[2] = tmp_u32;
                else
                    _b[2] = 255;

                _b += 4;
            }
            _b += d1 * 4;
        }
    }
    // scanlines: d1 = freq, d2 = pix depth
    else if (w == 2) {
        for (uint32_t line_c = 0; line_c < _draw->h; line_c ++) {
            if (! (line_c % d1)) {
                for (uint32_t c = 0; c < _draw->w; c ++) {
                    if (_b[0] >= d2)
                        _b[0] -= d2;
                    else
                        _b[0] = 0;

                    if (_b[1] >= d2)
                        _b[1] -= d2;
                    else
                        _b[1] = 0;

                    if (_b[2] >= d2)
                        _b[2] -= d2;
                    else
                        _b[2] = 0;
                        
                    _b += 4;
                }
            }
            else {
                _b += _draw->w * 4;
            }
        }
    }
}

void draw_line (draw const *_draw, int32_t x1, int32_t y1,
        int32_t x2, int32_t y2, uint32_t col) {
    if ((x1 < 0) || (y1 < 0) || (x2 < 0) || (y2 < 0) || 
            (x1 >= _draw->w) || (y1 >= _draw->h) || 
            (x2 >= _draw->w) || (y2 >= _draw->h))
        return;

    int32_t dx = x2 - x1;
    int32_t dy = y2 - y1;
    if ((! dx) && (! dy))
        return;

    float fdx = (float)dx;
    float fdy = (float)dy;

    float fdxm = fabs(fdx);
    float fdym = fabs(fdy);

    float t_step = 1.0 / (1.00001 * (fdxm > fdym ? fdxm : fdym));

    for (float t = 0.0; t <= 1.0; t += t_step) {
        uint16_t x = (uint16_t)(fdx * t + (float)x1);
        uint16_t y = (uint16_t)(fdy * t + (float)y1);
        if ((x < _draw->w) && (y < _draw->h))
            _draw->_p[y * _draw->w + x] = col;
    }
}

void draw_rect (draw const *_draw, int32_t x, int32_t y, int32_t w, int32_t h,
        uint32_t col) {
    if ((x < 0) || (y < 0) || (x >= _draw->w) || (y >= _draw->h) || !w || !h)
        return;

    if (w < 0) {
        x = x + w;
        w = -w;
    }
    if ((x + w) > _draw->w)
        w = (x + w) - _draw->w;

    if (h < 0) {
        y = y + h;
        h = -h;
    }
    if ((y + h) > _draw->h)
        h = (y + h) - _draw->h;

    uint32_t dst_line_d = _draw->w - w;
    uint32_t *_dst_p = _draw->_p + (y * _draw->w) + x;

    for (uint32_t cy = 0; cy < h; cy ++) {
        for (uint32_t cx = 0; cx < w; cx ++)
            *_dst_p++ = col;
        _dst_p += dst_line_d;
    }
}

void draw_get_rect (draw const *_draw, uint32_t *_dst_p, int32_t x, int32_t y,
        int32_t w, int32_t h) {
    uint32_t dst_line_d = _draw->w - w;
    uint32_t *_src_p = _draw->_p + (y * _draw->w) + x;

    for (uint32_t cy = 0; cy < h; cy ++) {
        for (uint32_t cx = 0; cx < w; cx ++) {
            *_dst_p = *_src_p;
            _dst_p ++;
            _src_p ++;
        }
        _src_p += dst_line_d;
    }
}

void draw_put_rect (draw const *_draw, uint32_t *_src_p, bool alpha_,
        int32_t x, int32_t y, int32_t w, int32_t h) {
    uint32_t dst_line_d = (_draw->w - w) * 4;
    uint32_t *_dst_p = _draw->_p + (y * _draw->w) + x;
    uint8_t *_dst_b = (uint8_t *)_dst_p;
    uint8_t *_src_b = (uint8_t *)_src_p;

    if (! alpha_) {
        for (uint32_t cy = 0; cy < h; cy ++) {
            for (uint32_t cx = 0; cx < w; cx ++) {
                _dst_b[1] = _src_b[3];
                _dst_b[2] = _src_b[2];
                _dst_b[3] = _src_b[1];
                _dst_b[4] = _src_b[4];
                _src_b += 4;
                _dst_b += 4;
            }
            _dst_b += dst_line_d;
        }
    }
    else {
        for (uint32_t cy = 0; cy < h; cy ++) {
            for (uint32_t cx = 0; cx < w; cx ++) {
                uint8_t a = _src_b[3];
                _dst_b[0] = _draw->alpha_table[255 - a][_dst_b[0]] +
                    _draw->alpha_table[a][_src_b[2]];

                _dst_b[1] = _draw->alpha_table[255 - a][_dst_b[1]] +
                    _draw->alpha_table[a][_src_b[1]];

                _dst_b[2] = _draw->alpha_table[255 - a][_dst_b[2]] +
                    _draw->alpha_table[a][_src_b[0]];

                _dst_b[3] = a;

                _src_b += 4;
                _dst_b += 4;
            }
            _dst_b += dst_line_d;
        }
    }
}

void draw_put_src_rect (draw const *_draw, uint32_t *_src_p, bool alpha_,
        int32_t x, int32_t y, int32_t w, int32_t h,
        int32_t src_x, int32_t src_y, int32_t src_w) {
    uint32_t dst_line_d = (_draw->w - w) * 4;
    uint32_t *_dst_p = _draw->_p + (y * _draw->w) + x;

    _src_p += src_y * src_w + src_x;
    uint32_t src_line_d = (src_w - w) * 4;

    uint8_t *_dst_b = (uint8_t *)_dst_p;
    uint8_t *_src_b = (uint8_t *)_src_p;

    if (! alpha_) {
        for (uint32_t cy = 0; cy < h; cy ++) {
            for (uint32_t cx = 0; cx < w; cx ++) {
                _dst_b[1] = _src_b[3];
                _dst_b[2] = _src_b[2];
                _dst_b[3] = _src_b[1];
                _dst_b[4] = _src_b[4];
                _src_b += 4;
                _dst_b += 4;
            }
            _dst_p += dst_line_d;
            _src_p += src_line_d;
        }
    }
    else {
        for (uint32_t cy = 0; cy < h; cy ++) {
            for (uint32_t cx = 0; cx < w; cx ++) {
                uint8_t a = _src_b[3];
                _dst_b[0] = _draw->alpha_table[255 - a][_dst_b[0]] +
                    _draw->alpha_table[a][_src_b[2]];

                _dst_b[1] = _draw->alpha_table[255 - a][_dst_b[1]] +
                    _draw->alpha_table[a][_src_b[1]];

                _dst_b[2] = _draw->alpha_table[255 - a][_dst_b[2]] +
                    _draw->alpha_table[a][_src_b[0]];

                _dst_b[3] = a;

                _src_b += 4;
                _dst_b += 4;
            }
            _dst_b += dst_line_d;
            _src_b += src_line_d;
        }
    }
}

