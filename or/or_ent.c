#include "or_map.h"

bool or_ent_cons (or_ent *_e) {
    _e->_parent_map = _;

    _e->_ent_type = _;
    _e->ent_type_tile_cur = 0;

    _e->x = _e->y = 0;
    _e->w = _e->h = 0;
    _e->page_x = _e->page_y = 0;
    _e->x_f = _e->y_f = 0.0;
    
    _e->mov_x = _e->mov_y = 0.0;
    _e->jumping_ = false;
    _e->jump_r = 1.0;
    _e->falling_ = false;
    _e->fall_t = 0.0;
    _e->weight = 0.0;

    _e->state = 0;
    _e->state_t = 0.0;

    _e->visible_ = true;
    _e->collidable_ = true;
    _e->del_ = false;
    return true;
}

bool or_ent_decons (or_ent *_e) {
    return true;
}

void or_ent_free (void *_v) {
    dlog_debug (_, "or_ent_free");
    if (! _v) {
        dlog_error (_, "or_ent_free: missing _v");
        return;
    }

    if (! or_ent_decons ((or_ent *)_v))
        return;

    free(_v);
}

void or_ent_calc_size (or_ent *_e) {
    if (! _e->_ent_type)
        return; 

    if (! _e->_ent_type->_tiles)
        return;

    _e->w = _e->_ent_type->_tiles->w;
    _e->h = _e->_ent_type->_tiles->h;
}

void or_ent_calc_page (or_ent *_e) {
    _e->page_x = _e->x / _e->_parent_map->page_w;
    _e->page_y = _e->y / _e->_parent_map->page_h;
}

bool or_ent_tick (or_ent *_e, float t) {
    if ((_e->mov_x != 0.0) || (_e->mov_y != 0.0))
        if (! or_ent_move (_e, t))
            return false;

    if (_e->jumping_)
        or_ent_jump (_e, t);

    if (_e->weight > 0.0)
        if (! or_ent_fall (_e, t))
            return false;

    if (_e->_ent_type->ext_tick_fn)
        return _e->_ent_type->ext_tick_fn (_e->_ent_type->_self, _e, t);

    return true;
}

bool or_ent_render (or_ent const *_e, win *_win) {   
    if (! _e->visible_)
        return true;

    // cull out of range pages
    if ((_e->page_x > (_e->_parent_map->view_page_x + 1)) ||
            (_e->page_y > (_e->_parent_map->view_page_y + 1)) ||
            (_e->_parent_map->view_page_x > (_e->page_x + 1)) ||
            (_e->_parent_map->view_page_y > (_e->page_y + 1)))
        return true;

    // cull out of range ents
    if (((_e->x + _e->w) < _e->_parent_map->view_x) ||
            (_e->x >= (_e->_parent_map->view_x + _e->_parent_map->page_w)) ||
            ((_e->y + _e->h) < _e->_parent_map->view_y) ||
            (_e->y >= (_e->_parent_map->view_y + _e->_parent_map->page_h)))
        return true;
 
    img *_img = or_tiles_get_img (_e->_ent_type->_tiles, _e->ent_type_tile_cur);
    if (! _img)
        return false;

    int32_t tile_w = _e->_ent_type->_tiles->w;
    int32_t tile_h = _e->_ent_type->_tiles->h;
    int32_t tile_x = _e->x - ((tile_w - _e->w) / 2);
    int32_t tile_y = _e->y - ((tile_h - _e->h) / 2);

    int32_t src_x, src_y;
    int32_t dst_x, dst_y, dst_w, dst_h;

    if (tile_x < _e->_parent_map->view_x) {
        src_x = _e->_parent_map->view_x - tile_x;
        dst_w = tile_w - src_x;
        dst_x = 0;
    }
    else if ((tile_x + tile_w) >
            (_e->_parent_map->view_x + _e->_parent_map->page_w)) {
        src_x = 0;
        dst_w = (_e->_parent_map->view_x + _e->_parent_map->page_w) - tile_x;
        dst_x = tile_x - _e->_parent_map->view_x;
    }
    else {
        src_x = 0;
        dst_w = tile_w;
        dst_x = tile_x - _e->_parent_map->view_x;
    }

    if (tile_y < _e->_parent_map->view_y) {
        src_y = _e->_parent_map->view_y - tile_y;
        dst_h = tile_h - src_y;
        dst_y = 0;
    }
    else if ((tile_y + tile_h) >
            (_e->_parent_map->view_y + _e->_parent_map->page_h)) {
        src_y = 0;
        dst_h = (_e->_parent_map->view_y + _e->_parent_map->page_h) - tile_y;
        dst_y = tile_y - _e->_parent_map->view_y;
    }
    else {
        src_y = 0;
        dst_h = tile_h;
        dst_y = tile_y - _e->_parent_map->view_y;
    }

    draw_put_src_rect (_win->_draw, (uint32_t *)_img->_b, true,
            dst_x, dst_y, dst_w, dst_h, src_x, src_y, tile_w);

    return true;
}

bool or_ent_move (or_ent *_e, float t) {
    float mov_xt = _e->mov_x * t;
    float mov_yt = _e->mov_y * t;

    int mov_xt_i;
    if (mov_xt < 0.0)
        mov_xt_i = (int32_t)-ceilf(-mov_xt);
    else
        mov_xt_i = (int32_t)ceilf(mov_xt);
 
    int mov_yt_i;
    if (mov_yt < 0.0)
        mov_yt_i = (int32_t)-ceilf(-mov_yt);
    else
        mov_yt_i = (int32_t)ceilf(mov_yt);

    or_ent_col col;
    if (or_ent_collide (_e, mov_xt_i, mov_yt_i,
                _e->_parent_map->_ents,
                &col, OR_ENT_COL_MOVE)) {
        if (or_ent_collide (_e, 0, mov_yt_i,
                    _e->_parent_map->_ents,
                    &col, OR_ENT_COL_IGNORE)) {
            if (_e->jumping_) {
                dlog_debug (_, "or_ent_move: jump ends by col");
                _e->jumping_ = false;
            }

            mov_yt = 0.0;
            _e->mov_y = 0.0;
            return true;
        }
 
        mov_xt = 0.0;
        _e->mov_x = 0.0;
    }
    
    _e->x_f += mov_xt;
    _e->y_f += mov_yt;
    _e->x = (int32_t)_e->x_f;
    _e->y = (int32_t)_e->y_f;
    or_ent_calc_page (_e);

    return true;
}

void or_ent_jump (or_ent *_e, float t) {
    if (! _e->jumping_) {    
        dlog_debug (_, "or_ent_jump: jump begins");
        _e->jumping_ = true; 
        _e->mov_y = -_e->jump_r * _e->weight;
    }

    _e->mov_y -= t;

    if (_e->mov_y >= 0.0) {
        dlog_debug (_, "or_ent_jump: jump ends (gravity)");
        _e->mov_y = 0.0;
        _e->jumping_ = false;
    }
}

bool or_ent_fall (or_ent *_e, float t) {
    or_ent_col col;

    if (! _e->falling_) {
        if (! or_ent_collide (_e, 0, 1,
                    _e->_parent_map->_ents,
                    &col, OR_ENT_COL_IGNORE)) {
            dlog_debug (_, "or_ent_fall: fall begins");
            _e->falling_ = true;
            _e->fall_t = 0.0;
        }
    }
    else if (_e->falling_) {
        _e->fall_t += t;
        if (_e->fall_t > 1.0)
            _e->fall_t = 1.0;

        float down = _e->fall_t * _e->weight;
        int32_t down_i = ceilf(down);

        if (or_ent_collide (_e, 0, down_i,
                    _e->_parent_map->_ents,
                    &col, OR_ENT_COL_FALL)) {
            dlog_debug (_, "or_ent_fall: fall ends");
            _e->falling_ = false;
            _e->jumping_ = false;
            _e->mov_y = 0.0;
            _e->y_f += down_i - (col.over_y + col._ent->h);
            _e->y = (int32_t)_e->y_f;
        }
        else {
            _e->y_f += down;
            _e->y = (int32_t)_e->y_f;
        }
 
        or_ent_calc_page (_e);
    }

    return true;
}

bool or_ent_collide (or_ent *_e, int32_t dir_x, int32_t dir_y,
        blaze *_ents, or_ent_col *_col, or_ent_col_type_e ent_col_type) {
    if (! _e->collidable_)
        return false;

    int32_t x2 = _e->x + dir_x;
    int32_t y2 = _e->y + dir_y;

    for (or_ent *_ent = blaze_first (_ents); _ent; _ent = blaze_next (_ents)) {
        if ((_ent == _e) || (! _ent->collidable_))
            continue;

        // cull out of range pages
        if ((_ent->page_x > (_e->page_x + 1)) ||
                (_ent->page_y > (_e->page_y + 1)) ||
                (_e->page_x > (_ent->page_x + 1)) ||
                (_e->page_y > (_ent->page_y + 1)))
            continue;

        int32_t x3 = x2 - _ent->x;
        if ((x3 >= _ent->w) || ((x3 + _e->w) <= 0))
            continue;

        int32_t y3 = y2 - _ent->y;
        if ((y3 >= _ent->h) || ((y3 + _e->h) <= 0))
            continue;

        _col->over_x = x3;
        _col->over_y = y3;
        _col->_ent = _ent;
        _col->ent_col_type = ent_col_type;

        if (ent_col_type != OR_ENT_COL_IGNORE) {
            if (_ent->_ent_type->ext_on_col_fn)
                _ent->_ent_type->ext_on_col_fn (_ent->_ent_type->_self,
                        _e, _col);
        }

        return true;
    }

    return false;
}

