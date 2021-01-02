#ifndef NITRO_NAMEMAP_INCLUDED
#define NITRO_NAMEMAP_INCLUDED

#include "../nitro_defs.h"
#include "../dlog/dlog.h"
#include "../blaze/blaze.h"

typedef struct {
    void *_parent_name_map;

    char *_name_s;
    uint32_t id;

    void *_ext_data;
} name_map_item;

typedef struct {
    blaze *_items;

    uint32_t id_cur;

    void (* _ext_data_free_fn)(void *);
    bool ext_data_free_;
} name_map;

extern bool name_map_cons (name_map *_n);
extern bool name_map_decons (name_map *_n);
extern bool name_map_clear (name_map *_n);
extern void name_map_item_free (void *_v);
extern uint32_t name_map_add (name_map *_n, char const *_name_s);
extern uint32_t name_map_get (name_map const *_n, char const *_name_s);
extern name_map_item *name_map_get_item (name_map const *_n,
        char const *_name_s);
extern name_map_item *name_map_get_item_by_id (name_map const *_n, uint32_t id);

#endif

