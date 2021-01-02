#ifndef NITRO_MAP_INCLUDED
#define NITRO_MAP_INCLUDED

#include "../nitro_defs.h"
#include "../sys/sys.h"
#include "../model/model.h"

#include "map_ent.h"
 
enum { 
// enums_map_begin
    MAP_CMD_END = 1,
    MAP_CMD_DEF_SYS,
    MAP_CMD_ENT,
    MAP_CMD_CAM,
// enums_map_end
};

typedef struct {
    blaze *_ents;
    map_ent *_ent_cam;

    name_map *_name_map_sys;
    blaze *_syss;
} map;

extern bool map_cons (map *_map);
extern bool map_decons (map *_map);
extern bool map_tick (map *_map, float t);
extern void map_render (map *_map, view *_view);
extern bool map_load (map *_map, view *_view, arc *_arc, char const *_name_s);

#endif

