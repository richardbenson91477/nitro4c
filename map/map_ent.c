#include "map.h"

bool map_ent_cons (map_ent *_e) {
    dlog_debug (_, "map_ent_cons");

    _e->_parent_ents = _;

    _e->_sys = _;
    _e->_model = _;

    v_clear (_e->_inertia_v);
    
    _e->_path_comp = _;
    _e->path_ignore_ = true;
    _e->path_cur = 0;

    _e->lookat_ignore_ = true;
    
    _e->ph_ignore_ = false;
    _e->ph_poly_col_ = false;
    _e->ph_rad = 1.0f;
    _e->ph_grav = 0.0f;
    _e->ph_air_fric = 1.0f;
    _e->ph_bounciness = 1.0f;

    _e->render_ignore_ = false;
    _e->cam_ = false;
    _e->del_ = false;
    return true;
}

bool map_ent_decons (map_ent *_e) {
    dlog_debug (_, "map_ent_decons");

    return true;
}

void map_ent_free (void *_v) {
    dlog_debug (_, "map_ent_free");
    if (! _v) {
        dlog_error (_, "map_ent_free: missing _v");
        return;
    }

    map_ent_decons ((map_ent *)_v);
    free(_v);
}

bool map_ent_set_sys (map_ent *_e, sys *_sys) {
    dlog_debug (_, "map_ent_set_sys");

    _e->_sys = _sys;
    if (_sys) {
        _sys->_ext_process_msg_fn = map_ent_process_msg;
        _sys->_ext_process_msg_this = _e;
    }
    else {
        dlog_debug (_, "map_ent_set_sys: warning: missing _sys");
    }

    return true;
}

bool map_ent_set_path (map_ent *_e, uint32_t id) {
    dlog_debug_u (_, "map_ent_set_path", id);

    if (! _e->_sys) {
        dlog_error (_, "map_ent_set_path: missing sys");
        return false;
    }

    if (id == (uint32_t)-1) 
        _e->path_cur ++;
    else 
        _e->path_cur = id;

    if (_e->path_cur >= _e->_sys->_comps->item_n) {
        dlog_error (_, "map_ent_set_path: path_cur out of bounds");
        return false;
    }

    _e->_path_comp = blaze_get (_e->_sys->_comps, _e->path_cur);
    _e->path_len = _e->_path_comp->max_t;
    _e->path_t = 0.0f;
    _e->path_ignore_ = false;
    return true;
}

bool map_ent_set_init_v_r (map_ent *_e,
        float const *_init_v, float const *_init_r) {
    dlog_debug (_, "map_ent_set_init_v_r");

    v_copy (_e->_init_v, _init_v);
    v_copy (_e->_init_r, _init_r);
    v_clear (_e->_lookat_v);
    return map_ent_reset_v_r (_e);
}

bool map_ent_reset_v_r (map_ent *_e) {
    dlog_debug (_, "map_ent_reset_v_r");

    v_copy (_e->_v, _e->_init_v);
    mat_ident (_e->__r_mat);
    return map_ent_rotate (_e, _e->_init_r);
}

bool map_ent_rotate (map_ent *_e, float const *_r) {
    if (! _r) {
        dlog_error (_, "map_ent_rotate: missing _r");
        return false;
    }

    float __r_mat [4][4], __r_mat2 [4][4];
    mat_from_r (__r_mat, _r);
    mat_concat (__r_mat2, __r_mat, _e->__r_mat);
    mat_copy (_e->__r_mat, __r_mat2);
 
    if (! _e->lookat_ignore_) {
        // TODO
    }

    return true;
}

bool map_ent_rotate_3f (map_ent *_e, float r0, float r1, float r2) {
    float const r [3] = {r0, r1, r2};
    return map_ent_rotate (_e, r);
}

bool map_ent_model_load (map_ent *_e, view *_view,
        arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "map_ent_model_load", _name_s);

    model *_model = malloc(sizeof(model));
    if (! model_cons (_model))
        return false;

    if (! model_load (_model, _view, _arc, _name_s)) {
        dlog_error (_, "map_ent_model_load: model_load");
        return false;
    }

    _e->_model = _model;

    _e->ph_rad = _model->rad;
    _e->ph_poly_col_ = _model->poly_col_;

    return true;
}

bool map_ent_tick (map_ent *_e, float t) {
    if (_e->_sys) {
        if (! sys_tick (_e->_sys, t)) {
            dlog_error (_, "map_ent_tick: sys_tick");
            return false;
        }
    }

    if (_e->_model) {
        if (! model_tick (_e->_model)) {
            dlog_error (_, "map_ent_tick: model_tick");
            return false;
        }
    }
 
    if (_e->_path_comp && (! _e->path_ignore_)) {
        if (! map_ent_tick_path (_e, t)) {
            dlog_error (_, "map_ent_tick: map_ent_tick_path");
            return false;
        }
    }

    if (! _e->ph_ignore_) {
        v_scalef (_e->_inertia_v, _e->ph_air_fric - t);

        _e->_inertia_v[1] -= _e->ph_grav * t * t;

        if (! v_is_clear (_e->_inertia_v)) {
            if (! map_ent_move (_e, t)) {
                dlog_error (_, "map_ent_tick: map_ent_move");
                return false;
            }
        }
    }

    return true;
}

bool map_ent_tick_path (map_ent *_e, float t) {
    if (! _e->_path_comp) {
        dlog_error (_, "map_ent_tick_path: missing path");
        return false;
    }

    _e->path_t += t;
    if (_e->path_t >= _e->path_len) {
        _e->path_t = _e->path_len;
        _e->path_ignore_ = true;
    }

    if (! comp_eval (_e->_path_comp, _e->path_t, _e->_v)) {
        dlog_error (_, "map_ent_tick_path: comp_eval");
        return false;
    }

    v_add (_e->_v, _e->_init_v);

    return true;
}

void map_ent_render (map_ent *_e, view *_view) {
    if (_e->render_ignore_ || _e->cam_)
        return;
 
    float d;
    if (view_clip (_view, _e->_v, &d))
        if (d < -_e->ph_rad)
            return;

    view_mv_push (_view);
    view_mv_trans (_view, _e->_v);

    if (_e->_model) 
        model_render (_e->_model, _view);

    view_mv_pop (_view);
}

bool map_ent_move (map_ent *_e, float t) {
    uint32_t const ent_hit_max = 8, poly_hit_max = 8;
    float _rel_v [3];

    float _new_v [3];
    v_add2 (_new_v, _e->_v, _e->_inertia_v);

    float mov_t = v_len (_e->_inertia_v);

    float _ray_v [3];
    v_divf2 (_ray_v, _e->_inertia_v, mov_t);

    // bounding sphere tests
    map_ent *__ent_hit [ent_hit_max];
    uint32_t ent_hit_n = 0;
    for (map_ent *_e2 = blaze_first (_e->_parent_ents); _e2;
            _e2 = blaze_next (_e->_parent_ents)) {
        if ((_e2 == _e) || _e2->ph_ignore_)
            continue;

        v_sub2 (_rel_v, _new_v, _e2->_v);

        // TODO: replace with sweeping sph_to_sph
        if (sph_in_sph (_rel_v, _e->ph_rad, _e2->ph_rad)){
            __ent_hit[ent_hit_n ++] = _e2;
            if (ent_hit_n == ent_hit_max)
                break;
        }
    }

    if (! ent_hit_n) {
        v_copy (_e->_v, _new_v);
        return true;
    }

    float _hit_norm [3] = {0.0, 0.0, 0.0};
    uint32_t hit_n = 0;
    for (uint32_t hit_c = 0; hit_c < ent_hit_n; hit_c ++) {
        map_ent *_e2 = __ent_hit[hit_c];

        if ((! _e2->ph_poly_col_) || (! _e2->_model)) {
            v_sub2 (_rel_v, _new_v, _e2->_v);
            v_norm (_rel_v);
            v_add (_hit_norm, _rel_v);
            hit_n ++;
            if (hit_n == poly_hit_max)
                break;
            continue;
        } 

        v_sub2 (_rel_v, _e->_v, _e2->_v);

        model *_model = _e2->_model;
        float *_v = _model->_verts;
        float *_pn = _model->_p_norms;
        for (uint32_t poly_c = 0; poly_c < _model->poly_n; poly_c ++) {
            // TODO: utilize t_out version
            if (sph_to_poly (_rel_v, _e->ph_rad, _ray_v, mov_t,
                    _v, _v + 3, _v + 6, _pn, _, _)) {
                v_add (_hit_norm, _pn);
                hit_n ++;
                if (hit_n == poly_hit_max)
                    break;
            }

            _v += 9;
            _pn += 4;
        }

        if (hit_n == poly_hit_max)
            break;
    }

    if (! hit_n) {
        v_copy (_e->_v, _new_v);
        return true;
    }

    if (hit_n > 1)
        v_norm (_hit_norm);

    float _refl_v [3];
    v_reflect (_refl_v, _e->_inertia_v, _hit_norm);
    v_scalef2 (_e->_inertia_v, _refl_v, _e->ph_bounciness - t);
    return true;
}

bool map_ent_process_msg (void *_this, sys_msg *_msg) {
    map_ent *_e = _this;
    sys *_sys = _e->_sys;

    // TODO: look up AOT
    if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_path")) {
        if (! map_ent_set_path (_e, _msg->arg_u)) {
            dlog_error (_, "map_ent_process_msg: map_ent_set_path");
            return false;
        }
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_reset_pos_rot")) {
        if (! map_ent_reset_v_r (_e)) {
            dlog_error (_, "map_ent_process_msg: map_ent_reset_v_r");
            return false;
        }
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_pos")) {
        v_copy (_e->_v, _msg->_arg_v);
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_pos_rel")) {
        v_add (_e->_v, _msg->_arg_v);
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_rot")) {
        mat_ident (_e->__r_mat);
        if (! map_ent_rotate (_e, _msg->_arg_v)) {
            dlog_error (_, "map_ent_process_msg: map_ent_rotate");
            return false;
        }
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_rot_rel")) {
        map_ent_rotate (_e, _msg->_arg_v);
        if (! map_ent_rotate (_e, _msg->_arg_v)) {
            dlog_error (_, "map_ent_process_msg: map_ent_rotate (2)");
            return false;
        }
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_lookat")) {
        v_copy (_e->_lookat_v, _msg->_arg_v);
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_grav")) {
        _e->ph_grav = _msg->arg_f;
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_ignore_phys")) {
        _e->ph_ignore_ = (bool)_msg->arg_u;
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_ignore_lookat")) {
        _e->lookat_ignore_ = (bool)_msg->arg_u;
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_ignore_render")) {
        _e->render_ignore_ = (bool)_msg->arg_u;
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "ent_set_ignore_path")) {
        _e->path_ignore_ = (bool)_msg->arg_u;
    }
    else {
        dlog_error_u (_, "map_ent_process_msg: unknown id",
                _msg->_type_item->id);
        return false;
    }

    return true;
}

void map_ent_cam_tick (void *_this, float t) {
    map_ent *_e = _this;

    if (! map_ent_tick (_e, t)) {
        dlog_error (_, "map_ent_cam_tick: map_ent_tick");
        return;
    }
}

void map_ent_cam_apply (void *_this, void *_v) {
    map_ent *_e = _this;
    view *_view = _v;

    if (! _v) {
        dlog_error (_, "map_ent_cam_apply: missing _v");
        return;
    }

    view_mv_reset (_view);
    view_mv_rot_m (_view, _e->__r_mat);
    view_mv_negtrans (_view, _e->_v);
}

void map_ent_cam_pre_clip (void *_this, float *_v) {
    map_ent *_e = _this;

    if (! _v) {
        dlog_error (_, "map_ent_cam_pre_clip: missing _v");
        return;
    }

    v_sub (_v, _e->_v);
    v_rotate_m (_v, _e->__r_mat);
}

