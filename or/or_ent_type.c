#include "or_map.h"

bool or_ent_type_cons (or_ent_type *_t) {
    dlog_debug (_, "or_ent_type_cons");
    
    _t->id = 0;
    _t->_self = _;
    _t->ext_tick_fn = _;
    _t->ext_on_col_fn = _;
 
    _t->_tiles = malloc(sizeof(or_tiles));
    if (! or_tiles_cons (_t->_tiles))
        return false;
 
    return true;
}

bool or_ent_type_decons (or_ent_type *_t) {
    dlog_debug (_, "or_ent_type_decons");

    if (! or_tiles_decons (_t->_tiles))
        return false;

    free(_t->_tiles);
    _t->_tiles = _;

    return true;
}

void or_ent_type_free (void *_v) {
    dlog_debug (_, "or_ent_type_free");
    if (! _v) {
        dlog_error (_, "or_ent_type_free: missing _v");
        return;
    }

    if (! or_ent_type_decons ((or_ent_type *)_v))
        return;

    free(_v);
}

