#include <png.h>

// 32bit per color RGBA only (for now)
bool load_png (img *_i, const char *file_s) {
    dlog_debug_s (_, "load_png", file_s);

    FILE *_f_in = fopen(file_s, "r");
    if (! _f_in) {
        dlog_error_s (_, "load_png", file_s);
        return false;
    }

    png_structp _png = png_create_read_struct (PNG_LIBPNG_VER_STRING, _, _, _);
    if (! _png) {
        dlog_error (_, "load_png: png_create_read_struct");
        return false;
    }

    png_infop _info = png_create_info_struct (_png);
    if (! _info) {
        dlog_error_s (_, "load_png", "png_create_info_struct");
        png_destroy_read_struct (&_png, (png_infopp) _, (png_infopp) _);
        return false;
    }

    png_init_io (_png, _f_in);
    png_read_info (_png, _info);
    png_set_filler (_png, 255, PNG_FILLER_AFTER);
    png_read_update_info (_png, _info);

    int32_t row_n = png_get_rowbytes (_png, _info);
    _i->w = png_get_image_width (_png, _info);
    _i->h = png_get_image_height (_png, _info);
    _i->b_n = _i->w * _i->h * 4;
    _i->_b = (uint8_t *)malloc(_i->b_n);
    if (! _i->_b) {
        dlog_error (_, "load_png: malloc");
        return false;
    }

    uint8_t **__rows = (uint8_t **)malloc(_i->h * sizeof(uint8_t *));
    for (int32_t c = 0; c < _i->h; c ++)
        __rows[c] = _i->_b + (c * row_n);

    png_read_image (_png, __rows);
    free(__rows);

    png_destroy_read_struct (&_png, &_info, _);
    fclose(_f_in);
    
    _i->free_b_ = 1;
    return true;
}

