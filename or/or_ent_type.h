#ifndef NITRO_OR_ENT_TYPE_INCLUDED
#define NITRO_OR_ENT_TYPE_INCLUDED

#include "or.h"

typedef struct {
    uint32_t id;
    void *_self;
    bool (* ext_tick_fn)(void *_self, void *_ent, float t);
    void (* ext_on_col_fn)(void *_self, void *_ent, void *_col);
    or_tiles *_tiles;
} or_ent_type;
 
extern bool or_ent_type_cons (or_ent_type *_t);
extern bool or_ent_type_decons (or_ent_type *_t);
extern void or_ent_type_free (void *_v);

#endif

