#include "enums_map.h"

uint32_t enums_map_get_id (enums_map const *_maps, char const *_name_s) {
    dlog_debug_s (_, "enums_map_get_id", _name_s);

    if ((! _maps) || (! _name_s)) {
        dlog_error (_, "missing _maps->_name_s");
        return 0;
    }

    for (enums_map const *_map = _maps; _map->_name_s; _map ++)
        if (! strcmp(_name_s, _map->_name_s))
            return _map->id;

    return 0;
}

char const *enums_map_get_name (enums_map const *_maps, uint32_t id) {
    dlog_debug_u (_, "enums_map_get_s", id);

    if (! _maps)
        return _;

    for (enums_map const *_map = _maps; _map->_name_s; _map ++)
        if (_map->id == id)
            return _map->_name_s;

    return _;
}

