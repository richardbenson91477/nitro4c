#ifndef NITRO_DRAW_INCLUDED
#define NITRO_DRAW_INCLUDED

#include <math.h>

#include "../nitro_defs.h"
#include "../dlog/dlog.h"

typedef struct {
    uint32_t *_p;
    bool free_p_;
    int32_t w, h;
    int32_t hw, hh;
    int32_t n;

    uint8_t alpha_table [256][256];
} draw;

extern bool draw_cons (draw *_draw, int32_t w, int32_t h);
extern bool draw_cons_from (draw *_draw, uint32_t *_p, int32_t w, int32_t h);
extern bool draw_decons (draw *_draw);
extern void draw_init_alpha_table (draw *_draw);
extern void draw_clear (draw const *_draw, uint32_t col);
extern void draw_point (draw const *_draw, int32_t x, int32_t y, uint32_t col);
extern void draw_effect (draw const *_draw, uint32_t w, uint32_t d1,
        uint32_t d2);
extern void draw_line (draw const *_draw, int32_t x1, int32_t y1,
        int32_t x2, int32_t y2, uint32_t col);
extern void draw_rect (draw const *_draw, int32_t x, int32_t y,
        int32_t w, int32_t h, uint32_t col);
extern void draw_get_rect (draw const *_draw, uint32_t *_dst_p,
        int32_t x, int32_t y, int32_t w, int32_t h);
extern void draw_put_rect (draw const *_draw, uint32_t *_src_p,
        bool alpha_, int32_t x, int32_t y, int32_t w, int32_t h);
extern void draw_put_src_rect (draw const *_draw, uint32_t *_src_p,
        bool alpha_, int32_t x, int32_t y, int32_t w, int32_t h,
        int32_t src_x, int32_t src_y, int32_t src_w);

#endif

