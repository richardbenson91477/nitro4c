#include "map.h"

bool map_cons (map *_map) {
    dlog_debug (_, "map_cons");
    
    _map->_ents = malloc(sizeof(blaze));
    if (! blaze_cons (_map->_ents))
        return false;

    _map->_ent_cam = _;

    _map->_name_map_sys = malloc(sizeof(name_map));
    if (! name_map_cons (_map->_name_map_sys))
        return false;
    _map->_name_map_sys->ext_data_free_ = true;
    _map->_name_map_sys->_ext_data_free_fn = free;

    _map->_syss = malloc(sizeof(blaze));
    if (! blaze_cons (_map->_syss))
        return false;

    return true;
}

bool map_decons (map *_map) {
    dlog_debug (_, "map_decons");

    if (! blaze_clear (_map->_syss, sys_free))
        return false;
    if (! blaze_decons (_map->_syss))
        return false;
    free(_map->_syss);
    _map->_syss = _;

    if (! name_map_decons (_map->_name_map_sys))
        return false;
    free(_map->_name_map_sys);
    _map->_name_map_sys = _;

    if (! blaze_clear (_map->_ents, map_ent_free))
        return false;
    if (! blaze_decons (_map->_ents))
        return false;
    free(_map->_ents);
    _map->_ents = _;

    return true;
}

bool map_tick (map *_map, float t) {
    for (map_ent *_ent = blaze_first (_map->_ents); _ent; ) {
        if (_ent->del_) {
            blaze_del (_map->_ents, _ent, 0, map_ent_free);
            _ent = blaze_cur (_map->_ents);
        }
        else {
            _ent = blaze_next (_map->_ents);
        }
    }

    for (map_ent *_ent = blaze_first (_map->_ents); _ent;
            _ent = blaze_next (_map->_ents)) {
        uint32_t iter_c = _map->_ents->iter_c;
        void **__iter = _map->_ents->__iter;
        if (! map_ent_tick (_ent, t)) {
            dlog_error (_, "map_tick: map_ent_tick");
            return false;
        }
        _map->_ents->__iter = __iter;
        _map->_ents->iter_c = iter_c;
    }

    return true;
}

void map_render (map *_map, view *_view) {
    for (map_ent *_ent = blaze_first (_map->_ents); _ent;
            _ent = blaze_next (_map->_ents)) {
        map_ent_render (_ent, _view);
    }
}

bool map_load (map *_map, view *_view, arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "map_load", _name_s);

    uint8_t *_b;
    uint32_t b_n;
    
    if (! arc_or_file_get_b (_arc, _name_s, &_b, &b_n)) {
        dlog_error_s (_, "map_load: arc_or_fs_get_b", _name_s);
        return false;
    }

    uint8_t *_m = _b;
    while (true) {
        uint32_t cmd_id = *(uint32_t *)_m;
        _m += 4;

        if (cmd_id == MAP_CMD_END) {
            break;
        }
        else if (cmd_id == MAP_CMD_DEF_SYS) {
            char *_sys_name_s = (char *)_m;
            _m += strlen((char *)_m) + 1;

            char *_sys_file_name_s = (char *)_m;
            _m += strlen((char *)_m) + 1;

            uint32_t sys_id = name_map_add (_map->_name_map_sys, _sys_name_s);
            if (! sys_id) {
                dlog_error_s (_, "map_load: name_map_add", _sys_name_s);
                return false;
            }

            name_map_item *_sys_item = name_map_get_item_by_id (
                    _map->_name_map_sys, sys_id);
            if (! _sys_item) {
                dlog_error_u (_, "map_load: name_map_get_item_by_id", sys_id);
                return false;
            }

            _sys_item->_ext_data = (void *)strdup (_sys_file_name_s);
            if (! _sys_item->_ext_data) {
                dlog_error_s (_, "map_load: strdup", _sys_file_name_s);
                return false;
            }
        }
        else if (cmd_id == MAP_CMD_ENT) {
            float *_init_v = (float *)_m;
            _m += 4 * 3;

            float *_init_r = (float *)_m;
            _m += 4 * 3;

            char *_model_name_s = (char *)_m;
            _m += strlen((char *)_m) + 1;

            char *_sys_name_s = (char *)_m;
            _m += strlen((char *)_m) + 1;
            
            name_map_item *_sys_item = name_map_get_item (_map->_name_map_sys,
                    _sys_name_s);
            if (! _sys_item) {
                dlog_error_s (_, "map_load: name_map_get_item", _sys_name_s);
                return false;
            }
            char *_sys_file_name_s = (char *)_sys_item->_ext_data;

            sys *_sys = malloc(sizeof(sys));
            if (! sys_cons (_sys))
                return false;
            _sys->_parent_name_map = _map->_name_map_sys;
            _sys->id = _sys_item->id;

            if (! sys_load (_sys, _arc, _sys_file_name_s)) {
                dlog_error_s (_, "map_load: sys_load", _sys_file_name_s);
                return false;
            }
            blaze_add (_map->_syss, _sys);

            map_ent *_e = malloc(sizeof(map_ent));
            if (! map_ent_cons (_e))
                return false;
            _e->_parent_ents = _map->_ents;

            if (! map_ent_set_sys (_e, _sys)) {
                dlog_error (_, "map_load: map_ent_set_sys");
                return false;
            }
 
            if (! map_ent_set_init_v_r (_e, _init_v, _init_r)) {
                dlog_error (_, "map_load: map_ent_set_init_v_r");
                return false;
            }

            if (! map_ent_model_load (_e, _view, _arc, _model_name_s)) {
                dlog_error_s (_, "map_load: map_ent_model_load", _model_name_s);
                return false;
            }

            blaze_add (_map->_ents, _e);
        }
        else if (cmd_id == MAP_CMD_CAM) {
            float *_init_v = (float *)_m;
            _m += 4 * 3;

            float *_init_r = (float *)_m;
            _m += 4 * 3;

            char *_sys_name_s = (char *)_m;
            _m += strlen((char *)_m) + 1;
            
            name_map_item *_sys_item = name_map_get_item (_map->_name_map_sys,
                    _sys_name_s);
            if (! _sys_item) {
                dlog_error_s (_, "map_load: name_map_get_item", _sys_name_s);
                return false;
            }
            char *_sys_file_name_s = (char *)_sys_item->_ext_data;

            sys *_sys = malloc(sizeof(sys));
            if (! sys_cons (_sys))
                return false;
            _sys->_parent_name_map = _map->_name_map_sys;
            _sys->id = _sys_item->id;

            if (! sys_load (_sys, _arc, _sys_file_name_s)) {
                dlog_error_s (_, "map_load: sys_load", _sys_file_name_s);
                return false;
            }
            blaze_add (_map->_syss, _sys);

            map_ent *_e = malloc(sizeof(map_ent));
            if (! map_ent_cons (_e))
                return false;
            _e->_parent_ents = _map->_ents;
            _e->cam_ = true;

            if (! map_ent_set_sys (_e, _sys)) {
                dlog_error (_, "map_load: map_ent_set_sys");
                return false;
            }
 
            if (! map_ent_set_init_v_r (_e, _init_v, _init_r)) {
                dlog_error (_, "map_load: map_ent_set_init_v_r");
                return false;
            }

            _view->_ext_cam_this = _e;
            _view->_ext_cam_tick_fn = map_ent_cam_tick;
            _view->_ext_cam_apply_fn = map_ent_cam_apply;
            _view->_ext_cam_pre_clip_fn = map_ent_cam_pre_clip;

            blaze_add (_map->_ents, _e);
            _map->_ent_cam = _e;
        }
    }

    for (sys *_sys = blaze_first (_map->_syss); _sys;
            _sys = blaze_next (_map->_syss)) {
        _sys->state_t = 0.0;
        if (! sys_set_state (_sys, 0))
            return false;
    }

    if (! _arc)
        free(_b);

    return true;
}

