#include "name_map.h"

bool name_map_cons (name_map *_n) {
    dlog_debug (_, "name_map_cons");

    _n->id_cur = 1;
    _n->_ext_data_free_fn = _;
    _n->ext_data_free_ = false;
    _n->_items = malloc(sizeof(blaze));
    return blaze_cons (_n->_items);
}

bool name_map_decons (name_map *_n) {
    dlog_debug (_, "name_map_decons");

    name_map_clear (_n);
    if (! blaze_decons (_n->_items))
        return false;

    free(_n->_items);
    _n->_items = _;
    return true;
}

bool name_map_clear (name_map *_n) {
    dlog_debug (_, "name_map_clear");
    return blaze_clear (_n->_items, name_map_item_free);
}

void name_map_item_free (void *_v) {
    dlog_debug (_, "name_map_item_free");
    name_map_item *_item = (name_map_item *)_v;
    if (! _item)
        return;

    if (_item->_name_s)
        free(_item->_name_s);

    if (_item->_ext_data) {
        name_map *_parent_name_map = (name_map *)_item->_parent_name_map;
        if (_parent_name_map->ext_data_free_ &&
                _parent_name_map->_ext_data_free_fn) {
            dlog_debug (_, "name_map_item_free: freeing _ext_data");
            _parent_name_map->_ext_data_free_fn (_item->_ext_data);
        }
    }

    free(_v);
}

// NOTE: order is significant
uint32_t name_map_add (name_map *_n, char const *_name_s) {
    dlog_debug_s (_, "name_map_add", _name_s);

    if (! _name_s) 
        return 0;
    
    for (name_map_item *_item = blaze_first (_n->_items); _item;
            _item = blaze_next (_n->_items))
        if (! strcmp (_item->_name_s, _name_s))
            return _item->id;

    uint32_t id = _n->id_cur ++;

    name_map_item *_item = malloc(sizeof(name_map_item));
    _item->_parent_name_map = _n;
    _item->_name_s = strdup(_name_s);
    _item->id = id;
    _item->_ext_data = _;
    blaze_add (_n->_items, _item);

    return id;
}

uint32_t name_map_get (name_map const *_n, char const *_name_s) {
    dlog_debug_s (_, "name_map_get", _name_s);

    if (! _name_s) 
        return 0;

    for (name_map_item *_item = blaze_first (_n->_items); _item;
            _item = blaze_next (_n->_items))
        if (! strcmp (_item->_name_s, _name_s))
            return _item->id;

    dlog_debug_s (_, "name_map_get: can't find", _name_s);
    return 0;
}

name_map_item *name_map_get_item (name_map const *_n,
        char const *_name_s) {
    dlog_debug_s (_, "name_map_get_item", _name_s);

    if (! _name_s)
        return _;

    for (name_map_item *_item = blaze_first (_n->_items); _item;
            _item = blaze_next (_n->_items))
        if (! strcmp (_item->_name_s, _name_s))
            return _item;

    dlog_debug_s (_, "name_map_get_item: can't find", _name_s);
    return _;
}

name_map_item *name_map_get_item_by_id (name_map const *_n, uint32_t id) {
    dlog_debug_u (_, "name_map_get_item_by_id", id);

    if (! id) 
        return _;
 
    for (name_map_item *_item = blaze_first (_n->_items); _item;
            _item = blaze_next (_n->_items))
        if (_item->id == id) 
            return _item;

    dlog_debug_u (_, "name_map_get_item_by_id: can't find", id);
    return _;
}

