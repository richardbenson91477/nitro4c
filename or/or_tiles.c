#include "or_tiles.h"

bool or_tiles_cons (or_tiles *_t) {
    dlog_debug (_, "or_tiles_cons");

    _t->w = 0;
    _t->h = 0;

    _t->_imgs = malloc(sizeof(blaze));
    if (! blaze_cons (_t->_imgs))
        return false;

    return true;
}

bool or_tiles_decons (or_tiles *_t) {
    dlog_debug (_, "or_tiles_decons");

    blaze_clear (_t->_imgs, img_free);
    free(_t->_imgs);
    _t->_imgs = _;

    return true;
}
 
img *or_tiles_get_img (or_tiles *_t, uint32_t i) {
    return blaze_get (_t->_imgs, i);
}

bool or_tiles_load_img (or_tiles *_t, char const *_name_s) {
    dlog_debug_s (_, "or_tiles_load_img", _name_s);

    img *_img = malloc(sizeof(img));
    if (! img_cons (_img))
        return false;

    if (! img_load_rwh (_img, _, _name_s)) {
        dlog_error_s (_, "or_tiles_load_img: img_load_rwh", _name_s);
        return false;
    }

    blaze_add (_t->_imgs, _img);

    _t->w = _img->w;
    _t->h = _img->h;
    return true;
}

bool or_tiles_load_set (or_tiles *_t, char const *_name_s, uint32_t n) {
    dlog_debug (_, "or_tiles_load_set");

    char _path_s [PATH_MAX];

    for (uint32_t c = 0; c < n; c ++) {
        sprintf (_path_s, "%s%d.rwh", _name_s, c + 1);
        if (! or_tiles_load_img (_t, _path_s)) {
            dlog_error_s (_, "or_tiles_load_set: or_tiles_load_img", _path_s);
            return false;
        }
    }

    return true;
}

