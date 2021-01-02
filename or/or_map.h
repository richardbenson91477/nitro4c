#ifndef NITRO_OR_MAP_INCLUDED
#define NITRO_OR_MAP_INCLUDED

#include "or.h"
#include "or_tiles.h"
#include "or_bg.h"

typedef struct {
    int32_t page_w, page_h;
    int32_t view_x, view_y;
    int32_t view_page_x, view_page_y;
    int32_t block_w, block_h;
    int32_t player_init_x, player_init_y;
    int32_t player_limit_y;

    bool pause_;
 
    float bg_r;
    or_bg *_bg;
 
    snd *_snd_mus;

    blaze *_ent_types;
    blaze *_ents;
} or_map;

#include "or_ent_type.h"
#include "or_ent.h"

extern bool or_map_cons (or_map *_m, int32_t page_w, int32_t page_h);
extern bool or_map_decons (or_map *_m);
extern void or_map_set_view (or_map *_m, int32_t x, int32_t y);
extern bool or_map_tick (or_map *_m, float t);
extern bool or_map_render (or_map const *_m, win *_win);
extern bool or_map_add_ent (or_map *_m, or_ent *_e);
extern bool or_map_load (or_map *_m, char const *_name_s);

#endif

