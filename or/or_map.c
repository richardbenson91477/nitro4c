#include "or_map.h"

bool or_map_cons (or_map *_m, int32_t page_w, int32_t page_h) {
    dlog_debug (_, "or_map_cons");

    _m->page_w = page_w;
    _m->page_h = page_h;
    _m->view_x = _m->view_y = 0;
    _m->view_page_x = _m->view_page_y = 0;
    _m->block_w = _m->block_h = 0;
    _m->player_init_x = _m->player_init_y = 0;
    _m->player_limit_y = 0;
    _m->bg_r = 0.5;
    _m->pause_ = false;

    _m->_bg = malloc(sizeof(or_bg));
    if (! or_bg_cons (_m->_bg))
        return false;

    _m->_snd_mus = malloc(sizeof(snd));
    if (! snd_cons (_m->_snd_mus))
        return false;
    
    _m->_ent_types = malloc(sizeof(blaze));
    if (! blaze_cons (_m->_ent_types))
        return false;

    _m->_ents = malloc(sizeof(blaze));
    if (! blaze_cons (_m->_ents))
        return false;

    return true;
}

bool or_map_decons (or_map *_m) {
    dlog_debug (_, "or_map_decons");

    blaze_clear (_m->_ents, or_ent_free);
    if (! blaze_decons (_m->_ents))
        return false;
    free(_m->_ents);
    _m->_ents = _;

    blaze_clear (_m->_ent_types, or_ent_type_free);
    if (! blaze_decons (_m->_ent_types))
        return false;
    free(_m->_ent_types);
    _m->_ent_types = _;

    if (_m->_bg) {
        if (! or_bg_decons (_m->_bg))
            return false;

        free(_m->_bg);
        _m->_bg = _;
    }

    if (_m->_snd_mus) {
        if (! snd_decons (_m->_snd_mus))
            return false;

        free(_m->_snd_mus);
        _m->_snd_mus = _;
    }

    return true;
}

void or_map_set_view (or_map *_m, int32_t x, int32_t y) {   
    _m->view_x = x;
    _m->view_y = y;

    if (_m->view_x < 0)
        _m->view_x = 0;

    if (_m->view_y < 0)
        _m->view_y = 0;
 
    _m->view_page_x = _m->view_x / _m->page_w;
    _m->view_page_y = _m->view_y / _m->page_h;
}

bool or_map_tick (or_map *_m, float t) {
    for (or_ent *_ent = blaze_first (_m->_ents); _ent; ) {
        if (_ent->del_) {
            blaze_del (_m->_ents, _ent, 0, or_ent_free);
            _ent = blaze_cur (_m->_ents);
        }
        else {
            _ent = blaze_next (_m->_ents);
        }
    }

    if (! _m->pause_) {
        for (or_ent *_ent = blaze_first (_m->_ents); _ent;
                _ent = blaze_next (_m->_ents))
            if (! or_ent_tick (_ent, t))
                return false;
    }

    return true;
}

bool or_map_render (or_map const *_m, win *_win) {
    int32_t x_off = (int32_t)((float)_m->view_x * _m->bg_r);
    int32_t y_off = (int32_t)((float)_m->view_y * _m->bg_r);
    if (! or_bg_render (_m->_bg, _win, x_off, y_off))
        return false;

    for (or_ent *_ent = blaze_first (_m->_ents); _ent;
            _ent = blaze_next (_m->_ents))
        if (! or_ent_render (_ent, _win))
            return false;

    return true;
}

bool or_map_add_ent (or_map *_m, or_ent *_e) {
    dlog_debug (_, "or_map_add_ent");

    if (! _e)
        return false;

    _e->_parent_map = _m;
    or_ent_calc_page (_e);

    blaze_add (_m->_ents, _e);
    return true;
}

bool or_map_load (or_map *_m, char const *_name_s) {
    dlog_debug_s (_, "or_map_load", _name_s);

    char b_in [PATH_MAX];

    sprintf(b_in, "%s.map", _name_s);    
    FILE *f_in = fopen(b_in, "r");
    if (! f_in) {
        dlog_error_s (_, "or_map_load", _name_s);
        return false;
    }

    if (! fgets(b_in, sizeof(b_in), f_in))
        return false;
    _m->block_w = atoi(b_in);

    if (! fgets(b_in, sizeof(b_in), f_in))
        return false;
    _m->block_h = atoi(b_in);
        
    if (! fgets(b_in, sizeof(b_in), f_in))
        return false;
    _m->player_init_x = atoi(b_in) * _m->block_w;

    if (! fgets(b_in, sizeof(b_in), f_in))
        return false;
    _m->player_init_y = atoi(b_in) * _m->block_h;

    if (! fgets(b_in, sizeof(b_in), f_in))
        return false;
    _m->player_limit_y = atoi(b_in) * _m->block_h;

    if (! fgets(b_in, sizeof(b_in), f_in))
        return false;
    b_in[strlen(b_in) - 1] = 0;
 
    if (! or_bg_load (_m->_bg, b_in))
        dlog_debug_s (_, "or_map_load: or_bg_load", b_in);

    if (! fgets(b_in, sizeof(b_in), f_in))
        return false;
    b_in[strlen(b_in) - 1] = 0;

    _m->_snd_mus = malloc(sizeof(snd));
    if (! snd_cons (_m->_snd_mus))
        return false;

    if (! snd_load_raw (_m->_snd_mus, _, b_in)) {
        if (! snd_decons (_m->_snd_mus))
            return false;

        free(_m->_snd_mus);
        _m->_snd_mus = _;
    }
    else {
        snd_play (_m->_snd_mus, -1);
    }

    if (! fgets(b_in, sizeof(b_in), f_in))
        return false;
    b_in[strlen(b_in) - 1] = 0;
    uint32_t ent_type_n = atoi (b_in);

    for (uint32_t ent_type_c = 0; ent_type_c < ent_type_n; ent_type_c++) {
        or_ent_type *_ent_type = malloc(sizeof(or_ent_type));
        if (! or_ent_type_cons (_ent_type))
            return false;

        if (! fgets(b_in, sizeof(b_in), f_in))
            return false;
        b_in[strlen(b_in) - 1] = 0;
        _ent_type->id = atoi (b_in); 

        if (! fgets(b_in, sizeof(b_in), f_in))
            return false;
        uint32_t tile_n = atoi (b_in);
        
        if (! fgets(b_in, sizeof(b_in), f_in))
            return false;
        b_in[strlen(b_in) - 1] = 0;
        
        if (! or_tiles_load_set (_ent_type->_tiles, b_in, tile_n))
            return false;
 
        blaze_add (_m->_ent_types, _ent_type);
    }

    fclose(f_in);

    sprintf(b_in, "%s.ent", _name_s);
    f_in = fopen(b_in, "r");
    if (! f_in) {
        dlog_error_s (_, "or_map_load", b_in);
        return false;
    } 

    while (fgets(b_in, sizeof(b_in), f_in)) {
        uint32_t subtype, tile_c, collidable_i, block_x, block_y;
        sscanf (b_in, "%u %u %u %u %u", &subtype, &tile_c, &collidable_i,
            &block_x, &block_y);

        or_ent_type *_ent_type_found = _;
        for (or_ent_type *_ent_type = blaze_first (_m->_ent_types); _ent_type;
                _ent_type = blaze_next (_m->_ent_types)) {
            if (_ent_type->id == subtype) {
                _ent_type_found = _ent_type;
                break;
            }
        }

        or_ent *_ent = malloc(sizeof(or_ent));
        if (! or_ent_cons (_ent))
            return false;

        _ent->_ent_type = _ent_type_found;
        _ent->ent_type_tile_cur = tile_c;
        _ent->collidable_ = (bool)collidable_i;
        _ent->x = block_x * _m->block_w;
        _ent->y = block_y * _m->block_h;
        _ent->x_f = (float)_ent->x;
        _ent->y_f = (float)_ent->y;
        or_ent_calc_size (_ent);

        if (! or_map_add_ent (_m, _ent))
            return false;
    }

    fclose(f_in);
    return true;
}

