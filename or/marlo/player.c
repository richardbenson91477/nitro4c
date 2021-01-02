#include "player.h"

bool player_cons (player *_p, marlo_conf *_conf) {
    dlog_debug (_, "player_cons");

    if (! _conf)
        return false;
    _p->_conf = _conf;

    _p->_snd_jump = malloc(sizeof(snd));
    snd_cons (_p->_snd_jump);
    if (! snd_load_raw (_p->_snd_jump, _, "data/jump.raw"))
        return false;

    _p->_snd_fail = malloc(sizeof(snd));
    snd_cons (_p->_snd_fail);
    if (! snd_load_raw (_p->_snd_fail, _, "data/fail.raw"))
        return false;

    _p->_ent = malloc(sizeof(or_ent));
    if (! or_ent_cons (_p->_ent))
        return false;

    _p->_ent->_ent_type = malloc(sizeof(or_ent_type));
    if (! or_ent_type_cons (_p->_ent->_ent_type))
        return false;
    _p->_ent->_ent_type->id = _conf->player_ent_type_id;
    if (! or_tiles_load_set (_p->_ent->_ent_type->_tiles,
            "data/tiles/marl", 11)) {
        dlog_error (_, "player_cons: or_tiles_load_set");
        return false;
    }

    or_ent_calc_size (_p->_ent);
    _p->_ent->w = _p->_ent->w - 4;
    _p->_ent->weight = _conf->player_weight;

    _p->tries_c = _conf->player_tries_n;

    return true;
}

bool player_decons (player *_p) {
    dlog_debug (_, "player_decons");

    snd_decons (_p->_snd_jump);
    free(_p->_snd_jump);
    _p->_snd_jump = _;

    snd_decons (_p->_snd_fail);
    free(_p->_snd_fail);
    _p->_snd_fail = _;

    or_ent_type_decons (_p->_ent->_ent_type);
    free(_p->_ent->_ent_type);
    _p->_ent->_ent_type = _;

    or_ent_decons (_p->_ent);
    free(_p->_ent);
    _p->_ent = _;

    return true;
}

void player_reset (player *_p) {
    dlog_debug (_, "player_reset");

    _p->_ent->ent_type_tile_cur = 0;
 
    _p->_ent->x = _p->_ent->_parent_map->player_init_x;
    _p->_ent->y = _p->_ent->_parent_map->player_init_y; 
    _p->_ent->x_f = (float)_p->_ent->x;
    _p->_ent->y_f = (float)_p->_ent->y;
    or_ent_calc_page (_p->_ent);

    _p->_ent->mov_x = _p->_ent->mov_y = 0.0;
    _p->_ent->falling_ = false;
    _p->_ent->jumping_ = false;

    _p->_ent->state = 0;
    _p->_ent->collidable_ = true;

    player_calc_view (_p);
 
    _p->run_x = 0.0;
    _p->run_max = _p->_conf->run_max_walk;
    _p->_ent->jump_r = _p->_conf->jump_r_min;
}

void player_calc_view (player *_p) {
    or_map *_map = _p->_ent->_parent_map;

    int32_t view_x = _map->view_x;
    int32_t view_y = _map->view_y;

    int32_t rel_x = _p->_ent->x - view_x;
    int32_t page_w14 = (_map->page_w / 4) - (_p->_ent->w / 2);
    int32_t page_w34 = ((_map->page_w * 3) /4) - (_p->_ent->w / 2);
    if (rel_x < page_w14) {
        if (_p->_ent->x < page_w14)
            view_x = 0;
        else
            view_x = _p->_ent->x - page_w14;
    }
    else if (rel_x > page_w34) {
        view_x = _p->_ent->x - page_w34;
    }

    int32_t rel_y = _p->_ent->y - view_y;
    int32_t page_h14 = (_map->page_h / 4) - (_p->_ent->h / 2);
    int32_t page_h34 = ((_map->page_h * 3) /4) - (_p->_ent->h / 2);
    if (rel_y < page_h14) {
        if (_p->_ent->y < page_h14)
            view_y = 0;
        else
            view_y = _p->_ent->y - page_h14;
    }
    else if (rel_y > page_h34) {
        view_y = _p->_ent->y - page_h34;
    }
    
    if ((view_y + _map->page_h) > _map->player_limit_y) {
        view_y = _map->player_limit_y - _map->page_h;
        if (view_y < 0)
            view_y = 0;
    }

    or_map_set_view (_map, view_x, view_y);
}

bool player_render (player const *_p, win *_win) {
    if (! or_ent_render (_p->_ent, _win))
        return false;

    return true;
}

bool player_tick (player *_p, win *_win, float t) {
    if (_win->keys[SDL_SCANCODE_RETURN]) {
        _win->keys[SDL_SCANCODE_RETURN] = false;
        _p->_ent->_parent_map->pause_ = ! _p->_ent->_parent_map->pause_;
    }

    if (_p->_ent->_parent_map->pause_)
        return true;

    if (! (_p->_ent->falling_ || _p->_ent->jumping_)) {
        if (_win->keys[SDL_SCANCODE_K])
            _p->run_max = _p->_conf->run_max_run;
        else
            _p->run_max = _p->_conf->run_max_walk;
    }

    if (_p->run_x != 0.0) {
        if (! (_p->_ent->falling_ || _p->_ent->jumping_))
            _p->run_x *= _p->_conf->ground_friction;
        else
            _p->run_x *= _p->_conf->air_friction;

        if ((_p->run_x > -_p->_conf->rho_clamp) &&
                (_p->run_x < _p->_conf->rho_clamp))
            _p->run_x = 0.0;
    }

    if (_win->keys[SDL_SCANCODE_D] &&
            (! _win->keys[SDL_SCANCODE_A])) {
        if ((! _p->_ent->falling_) && (! _p->_ent->jumping_))
            _p->run_x += _p->_conf->run_inc;
        else 
            _p->run_x += (_p->_conf->run_inc * _p->_conf->run_air_r); 

        if (_p->run_x > _p->run_max)
            _p->run_x = _p->run_max;
    }
    else if (_win->keys[SDL_SCANCODE_A] && 
            (! _win->keys[SDL_SCANCODE_D])) {
        if ((! _p->_ent->falling_) && (! _p->_ent->jumping_))
            _p->run_x -= _p->_conf->run_inc;
        else
            _p->run_x -= (_p->_conf->run_inc * _p->_conf->run_air_r);

        if (_p->run_x < -_p->run_max)
            _p->run_x = -_p->run_max;
    }
    
    _p->_ent->mov_x = _p->run_x;
    if (! _p->_ent->jumping_)
        _p->_ent->jump_r = _p->_conf->jump_r_min +
            fabs (_p->run_x) * _p->_conf->jump_run_r;

    if (_win->keys[SDL_SCANCODE_J]) {
        if ((! _p->_ent->jumping_) && (! _p->_ent->falling_)) {
            or_ent_jump (_p->_ent, t);
            if (_p->_ent->jumping_) {
                snd_stop (_p->_snd_jump);
                snd_play (_p->_snd_jump, 0);
            }
        }
    }
 
    if (_win->keys[SDL_SCANCODE_ESCAPE]) {
        _win->keys[SDL_SCANCODE_ESCAPE] = false;
        return false;
    }

    // currently in jump states
    if ((_p->_ent->state == 4) || (_p->_ent->state == 9)) {
        if ((! _p->_ent->jumping_) && (! _p->_ent->falling_)) {
            // reset to standing state
            _p->_ent->state -= 4;     
            _p->_ent->ent_type_tile_cur = _p->_ent->state;
        }
    }
    
    // currently in standing or walk states
    if ((_p->_ent->state < 4) || ((_p->_ent->state > 4) &&
            (_p->_ent->state < 9)) ) {
        // set jump states
        if (_p->_ent->jumping_) {
            if (_p->_ent->state < 5)
                _p->_ent->state = 4;
            else
                _p->_ent->state = 9;
        }
        else if (_p->_ent->falling_) {
            if (_p->_ent->state < 5)
                _p->_ent->state = 3;
            else
                _p->_ent->state = 8;
        }
        else {
            // reset to standing states
            if (_p->run_x == 0.0) {
                if (_p->_ent->state >= 5)
                    _p->_ent->state = 5;
                else
                    _p->_ent->state = 0;
                _p->_ent->state_t = _p->_conf->player_state_len;
            }
            else {
                if ((_p->run_x < 0.0) && (_p->_ent->state < 5))
                    _p->_ent->state += 5;
                else if ((_p->run_x > 0.0) && (_p->_ent->state > 4))
                    _p->_ent->state -= 5;
                
                _p->_ent->state_t += fabs (_p->run_x) * t;
                if (_p->_ent->state_t > _p->_conf->player_state_len) {
                    _p->_ent->state_t -= _p->_conf->player_state_len;
                    _p->_ent->state ++;
                    if (_p->_ent->state == 3)
                        _p->_ent->state = 0;
                    else if (_p->_ent->state == 8)
                        _p->_ent->state = 5;
                }
            }
        }       
    }

    _p->_ent->ent_type_tile_cur = _p->_ent->state;

    // failure check
    if ((_p->_ent->state < 10) && 
            ((_p->_ent->y + _p->_ent->h) >
                _p->_ent->_parent_map->player_limit_y)) {
        _p->_ent->state = 10;
        _p->_ent->ent_type_tile_cur = 10;
        _p->_ent->collidable_ = false;
        _p->_ent->falling_ = false;
        _p->_ent->jumping_ = false;
        
        or_ent_jump (_p->_ent, t);
        
        snd_stop (_p->_snd_fail);
        snd_play (_p->_snd_fail, 0);
    }
    else if ((_p->_ent->state == 10) &&
            (_p->_ent->y > _p->_ent->_parent_map->player_limit_y)) {
        _p->tries_c --;
        if (_p->tries_c == 0)
            return false;

        snd_stop (_p->_snd_fail);
        
        player_reset (_p);
    }

    if (! or_ent_tick (_p->_ent, t))
        return false;

    if (_p->_ent->mov_x == 0.0)
        _p->run_x = 0.0;

    player_calc_view (_p);

    return true;
}
 
