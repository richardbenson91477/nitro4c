#include "brick.h"

bool brick_cons (brick *_b, marlo_conf *_conf) {
    if (! _conf)
        return false;

    _b->_conf = _conf;

    _b->_snd_break = malloc(sizeof(snd));
    snd_cons (_b->_snd_break);
    if (! snd_load_raw (_b->_snd_break, _, "data/break1.raw"))
        return false;

    return true;
}

bool brick_decons (brick *_b) {
    snd_decons (_b->_snd_break);
    free (_b->_snd_break);
    _b->_snd_break = _;

    _b->_conf = _;
    return true;
}

bool brick_tick (void *_self, void *_ent_v, float t) {
    brick *_b = _self;
    or_ent *_ent = _ent_v;

    if (_ent->state == 1) {
        _ent->del_ = true;
 
        or_ent *_e2 = malloc(sizeof(or_ent));
        if (! or_ent_cons (_e2))
            return false;

        _e2->_ent_type = _ent->_ent_type;
        _e2->ent_type_tile_cur = _ent->ent_type_tile_cur;
        _e2->collidable_ = false;
        _e2->x = _ent->x;
        _e2->y = _ent->y;
        _e2->x_f = (float)_e2->x;
        _e2->y_f = (float)_e2->y;
        or_ent_calc_size (_e2);

        if (! or_map_add_ent (_ent->_parent_map, _e2))
            return false;
 
        _e2->state = 2;
        _e2->weight = _b->_conf->brick_weight;
        _e2->jump_r = _b->_conf->brick_jump_r;
        or_ent_jump (_e2, t);
 
        snd_stop (_b->_snd_break);
        snd_play (_b->_snd_break, 0);
    }
    else if (_ent->state == 2) {
        _ent->state_t += t;
        if (_ent->state_t > _b->_conf->brick_del_t)
            _ent->del_ = true;
    }
 
    return true;
}

void brick_on_col (void *_self, void *_ent_v, void *_ent_col_v) {
    brick *_b = _self;
    or_ent *_ent = _ent_v;
    or_ent_col *_ent_col = _ent_col_v;

    if (_ent->_ent_type->id == _b->_conf->player_ent_type_id) {
        if (_ent_col->ent_col_type == OR_ENT_COL_MOVE) {
            if ((_ent_col->over_y >= (_ent->h / 2)) && 
                    ((_ent_col->over_x + _ent->w) > (_ent->w / 8)) &&
                    (_ent_col->over_x < (_ent->w - _ent->w / 8)))
                _ent_col->_ent->state = 1;
        }
    }
}

