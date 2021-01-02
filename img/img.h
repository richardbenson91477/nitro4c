#ifndef NITRO_IMG_INCLUDED
#define NITRO_IMG_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#include "../nitro_defs.h"
#include "../arc/arc.h"

typedef struct {
    int32_t w;
    int32_t h;
    uint8_t *_b;
    uint32_t b_n;
    bool free_b_;
} img;

extern bool img_cons (img *_i);
extern bool img_decons (img *_i);
extern void img_free (void *_v);
extern bool img_load_raw (img *_i, arc *_arc, char const *_name_s,
        int32_t w, int32_t h);
extern bool img_load_rwh (img *_i, arc *_arc, char const *_name_s);
extern bool img_save_raw (img const *_i, char const *_name_s);
extern bool img_save_rwh (img const *_i, char const *_name_s);
extern bool img_swap_r_b (img *_i);
extern bool img_from_rgb24 (img *_i, int32_t w, int32_t h, uint8_t *_b);

#endif

