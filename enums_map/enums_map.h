#ifndef NITRO_ENUMSMAP_INCLUDED
#define NITRO_ENUMSMAP_INCLUDED

#include <stdint.h>
#include <string.h>

#include "../nitro_defs.h"
#include "../dlog/dlog.h"

typedef struct {
    char const *_name_s;
    uint32_t id;
} enums_map;

extern uint32_t enums_map_get_id (enums_map const *_maps, char const *_name_s);
extern char const *enums_map_get_name (enums_map const *_maps, uint32_t id);

#endif

