#include <png.h>

bool save_ppm (img *_img, const char *file_s, bool alpha_) {
    dlog_debug_s (_, "save_ppm", file_s);

    FILE *_f_out = fopen(file_s, "w");
    if (! _f_out) {
        dlog_error_s (_, "save_ppm", file_s);
        return false;
    }

    if (! alpha_)
        fprintf(_f_out, "P6");
    else
        fprintf(_f_out, "P5");

    fprintf(_f_out, "\n%d %d 255\n", _img->w, _img->h);

    uint8_t *_m = _img->_b;
    if (! alpha_) {
        for (uint32_t c = 0; c < _img->b_n; c ++) {
            if ((c + 1) % 4)
                fputc(*_m, _f_out);
            _m ++;
        }
    }
    else {
        for (uint32_t c = 0; c < _img->b_n; c ++) {
            if (! ((c + 1) % 4))
                fputc(*_m, _f_out);
            _m ++;
        }
    }

    fclose(_f_out);
    return true;
}

