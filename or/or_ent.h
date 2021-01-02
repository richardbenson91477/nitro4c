#ifndef NITRO_OR_ENT_INCLUDED
#define NITRO_OR_ENT_INCLUDED

#include "or.h"

typedef struct {    
    or_map *_parent_map;

    or_ent_type *_ent_type;
    uint32_t ent_type_tile_cur;

    int32_t x, y;
    float x_f, y_f;
    int32_t w, h;
    int32_t page_x, page_y;
    
    float mov_x, mov_y;
    bool jumping_;
    float jump_r;
    bool falling_;
    float fall_t;
    float weight;

    uint32_t state;
    float state_t;

    bool visible_;
    bool collidable_;
    bool del_;
} or_ent;

typedef enum {
    OR_ENT_COL_IGNORE, OR_ENT_COL_MOVE, OR_ENT_COL_FALL
} or_ent_col_type_e;

typedef struct {
    or_ent *_ent;
    int32_t over_x;
    int32_t over_y;
    or_ent_col_type_e ent_col_type;
} or_ent_col;

extern bool or_ent_cons (or_ent *_e);
extern bool or_ent_decons (or_ent *_e);
extern void or_ent_free (void *_v);
extern void or_ent_calc_size (or_ent *_e);
extern void or_ent_calc_page (or_ent *_e);
extern bool or_ent_tick (or_ent *_e, float t);
extern bool or_ent_render (or_ent const *_e, win *_win);
extern bool or_ent_move (or_ent *_e, float t);
extern void or_ent_jump (or_ent *_e, float t);
extern bool or_ent_fall (or_ent *_e, float t);
extern bool or_ent_collide (or_ent *_e, int32_t dir_x, int32_t dir_y,
        blaze *_ents, or_ent_col *_col, or_ent_col_type_e ent_col_type);

#endif

