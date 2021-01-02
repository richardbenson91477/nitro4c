#include "../name_map.h"

int main () {
    uint32_t id;
    
    name_map *_name_map = malloc(sizeof(name_map));
    name_map_cons (_name_map);

    dlog_debug (_, "main: add \"first\"");
    if (! name_map_add (_name_map, "first"))
        return -1;

    dlog_debug (_, "main: add (\"second\")");
    if (! name_map_add (_name_map, "second"))
        return -2;

    dlog_debug (_, "main: add (\"first\")");
    if (! name_map_add (_name_map, "first"))
        return -3;

    dlog_debug (_, "main: add (\"second\")");
    if (! name_map_add (_name_map, "second"))
        return -4;

    dlog_debug (_, "main: get_map \"first\"");
    id = name_map_get (_name_map, "first");
    dlog_debug_u (_, "main: id", id);

    dlog_debug (_, "main: get_id \"second\"");
    id = name_map_get (_name_map, "second");
    dlog_debug_u (_, "main: id", id);
    
    _name_map->id_cur = 10;
    uint32_t third = name_map_add (_name_map, "third");
    dlog_debug (_, "main: get_id \"third\"");
    id = name_map_get (_name_map, "third");
    dlog_debug_u (_, "main: id", id);
    if ((id != third) || (id != 10)) {
        dlog_error (_, "main: incorrect ids");
        return -5;
    }
    else
        dlog_debug (_, "main: correct ids");

    dlog_debug (_, "main: name_map_get_item_by_id 1");
    name_map_item *_item = name_map_get_item_by_id (_name_map, 1);
    if (! _item)
        return -6;

    dlog_debug_s (_, "main: _item->_name_s", _item->_name_s);
 
    name_map_decons (_name_map);
    free(_name_map);

    return 0;
}

