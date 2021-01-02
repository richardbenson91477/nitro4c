#ifndef NITRO_OR_BG_INCLUDED
#define NITRO_OR_BG_INCLUDED

#include "or.h"

typedef struct {
    img *_img;
} or_bg;

extern bool or_bg_cons (or_bg *_bg);
extern bool or_bg_decons (or_bg *_bg);
extern bool or_bg_load (or_bg *_bg, char const *_name_s);
extern bool or_bg_render (or_bg const *_bg, win *_win, int32_t ox, int32_t oy);

#endif

