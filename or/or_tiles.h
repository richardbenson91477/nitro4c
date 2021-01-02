#ifndef NITRO_OR_TILES_INCLUDED
#define NITRO_OR_TILES_INCLUDED

#include "or.h"

typedef struct {
    int32_t w, h;
    blaze *_imgs;
} or_tiles;

extern bool or_tiles_cons (or_tiles *_t);
extern bool or_tiles_decons (or_tiles *_t);
extern img *or_tiles_get_img (or_tiles *_t, uint32_t i);
extern bool or_tiles_load_img (or_tiles *_t, char const *_name_s);
extern bool or_tiles_load_set (or_tiles *_t, char const *_name_s, uint32_t n);

#endif

