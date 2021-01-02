#include "model.h"

bool model_cons (model *_model) {
    _model->vert_n = 0;
    _model->poly_n = 0;
    _model->_verts = _;
    _model->_v_norms = _;
    _model->_t_coords = _;
    _model->_p_norms = _;
    _model->rad = 0.0;
    _model->poly_col_ = false;
    _model->_tex_name_s = _;
    _model->_view_tex = _;
    return true;
}

bool model_decons (model *_model, view *_view) {
    dlog_debug (_, "model_decons");

    if (_model->_tex_name_s)
        free(_model->_tex_name_s);

    if (_model->_view_tex) {
        view_tex_unref (_view, _model->_view_tex);
    }

    free(_model->_verts);
    free(_model->_v_norms);
    free(_model->_t_coords);
    free(_model->_p_norms);
    return true;
}

bool model_load (model *_model, view *_view, arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "model_load", _name_s);

    uint8_t *_b;
    uint32_t b_n;

    if (! arc_or_file_get_b (_arc, _name_s, &_b, &b_n)) {
        dlog_error_s (_, "model_load: arc_or_fs_get_b", _name_s);
        return false;
    }

    uint8_t *_m = _b;

    _model->vert_n = *(uint32_t *)_m;
    dlog_debug_u (_, "model_load: vert_n", _model->vert_n);

    _m += 4;
    _model->poly_n = _model->vert_n / 3;
    dlog_debug_u (_, "model_load: poly_n", _model->poly_n);
    
    _model->rad = *(float *)_m;
    dlog_debug_f (_, "model_load: rad", _model->rad);
    _m += 4;

    _model->poly_col_ = *(uint8_t *)_m;
    dlog_debug_u (_, "model_load: poly_col_", _model->poly_col_);
    _m += 1;

    if (*_m) {
        _model->_tex_name_s = strdup((char *)_m);
        dlog_debug_s (_, "model_load: _tex_name_s", _model->_tex_name_s);
        _m += strlen(_model->_tex_name_s) + 1;

        if (_view && _view->tex_) {
            _model->_view_tex = view_tex_load (_view, _arc, _model->_tex_name_s);
            if (! _model->_view_tex) {
                dlog_error_s (_, "model_load: view_tex_load",
                        _model->_tex_name_s);
                return false;
            }
        }
        else
            dlog_debug (_, "model_load: texture name but no _view_tex");
    }
    else {
        dlog_debug (_, "model_load: no texture");
        _model->_tex_name_s = _;
        _model->_view_tex = _;
    }

    _model->_verts = (float *)malloc(_model->vert_n * 3 * 4);
    _model->_v_norms = (float *)malloc(_model->vert_n * 3 * 4);
    _model->_t_coords = (float *)malloc(_model->vert_n * 2 * 4);
    if ((! _model->_verts) || (! _model->_v_norms) || (! _model->_t_coords)) {
        dlog_error (_, "model_load: malloc");
        return false;
    }

    float *_vert = _model->_verts;
    float *_v_norm = _model->_v_norms;
    float *_t_coord = _model->_t_coords;
    for (uint32_t c = 0; c < _model->vert_n; c ++) {
        *_vert++ = *(float *)_m;
        _m += 4;
        *_vert++ = *(float *)_m;
        _m += 4;
        *_vert++ = *(float *)_m;
        _m += 4;

        *_v_norm++ = *(float *)_m;
        _m += 4;
        *_v_norm++ = *(float *)_m;
        _m += 4;
        *_v_norm++ = *(float *)_m;
        _m += 4;

        *_t_coord++ = *(float *)_m;
        _m += 4;
        *_t_coord++ = *(float *)_m;
        _m += 4;
    }
    
    _model->_p_norms = (float *)malloc(_model->poly_n * 4 * 4);
    if (! _model->_p_norms) {
        dlog_error (_, "model_load: malloc (2)");
        return false;
    }
    memcpy(_model->_p_norms, _m, _model->poly_n * 4 * 4);

    if (_model->rad == 0.0) {
        _model->rad = v_list_farthest (_model->_verts, _model->vert_n);
        dlog_debug_f (_, "model_load: calculated rad", _model->rad);
    }

    if (! _arc)
        free (_b);

    return true;
}

bool model_tick (model *_model) {
    // TODO
    return true;
}

void model_render (model const *_model, view *_view) {
    if (! _view) {
        dlog_error (_, "model_render: missing view");
        return;
    }

    if (_model->_view_tex) {
        view_tex_bind (_view, _model->_view_tex);
        view_render_tris_tex (_view, _model->_verts, _model->_v_norms,
                _model->_t_coords, _model->vert_n);
    }
    else {
        view_render_tris (_view, _model->_verts, _model->_v_norms,
                _model->vert_n);
    }
}

