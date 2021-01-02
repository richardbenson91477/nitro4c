#include "spline.h"

bool spline_cons (spline *_spline) {
    dlog_debug (_, "spline_cons");

    _spline->min_t = FLT_MAX;
    _spline->max_t = FLT_MIN;
    
    _spline->_segs = malloc(sizeof(blaze));
    if (! blaze_cons (_spline->_segs))
        return false;

    return true;
}

bool spline_decons (spline *_spline) {
    dlog_debug (_, "spline_decons");

    blaze_clear (_spline->_segs, free);
    free(_spline->_segs);
    _spline->_segs = _;
    return true;
}

bool spline_eval (spline const *_spline, float t, float *_v_out) {
    if (! _v_out) {
        dlog_error (_, "spline_eval: missing _v_out");
        return false;
    }

    if (! _spline->_segs->item_n) {
        dlog_error (_, "spline_eval: missing segs");
        return false;
    }

    spline_seg *_seg_cur = _;
    spline_seg *_seg_next = _;
    for (spline_seg *_seg = blaze_first (_spline->_segs); _seg;
            _seg = blaze_next (_spline->_segs)) {
        if (_seg->t > t) {
            _seg_next = _seg;
            break;
        }
        _seg_cur = _seg; 
    }

    if (_seg_cur && _seg_next) {
        float s = (t - _seg_cur->t) / (_seg_next->t - _seg_cur->t);

        if (_spline->type == SPLINE_TYPE_LINEAR) {
            for (uint8_t dim_c = 0; dim_c < _spline->dim; dim_c ++)
                _v_out[dim_c] = _seg_cur->_v[dim_c] +
                    s * (_seg_next->_v[dim_c] - _seg_cur->_v[dim_c]);
        }
        else if (_spline->type == SPLINE_TYPE_CUBIC_BEZIER) {
            // -1 3 -3 1
            float c1 = (-1 * (s * s * s)) + (3 * (s * s)) + (-3 * (s)) + 1;
            // 3 -6 3 0
            float c2 = (3 * (s * s * s)) + (-6 * (s * s)) + (3 * (s));
            // -3 3 0 0
            float c3 = (-3 * (s * s * s)) + (3 * (s * s));
            // 1 0 0 0
            float c4 = (s * s * s);

            for (uint8_t dim_c = 0; dim_c < _spline->dim; dim_c ++) 
                _v_out[dim_c] = 
                    c1 * _seg_cur->_v[dim_c] + 
                    c2 * _seg_cur->_v[dim_c + _spline->dim] +
                    c3 * _seg_next->_v[dim_c + _spline->dim + _spline->dim] +
                    c4 * _seg_next->_v[dim_c];
        }
        else {
            dlog_error_u (_, "spline_eval: unknown type", _spline->type);
            return false;
        }
    }
    else if (_seg_cur)
        for (int8_t dim_c = 0; dim_c < _spline->dim; dim_c ++) 
            _v_out[dim_c] = _seg_cur->_v[dim_c];
    else 
        for (int8_t dim_c = 0; dim_c < _spline->dim; dim_c ++) 
            _v_out[dim_c] = _seg_next->_v[dim_c];

    return true;
}

// NOTE: call in +t order
bool spline_add_seg (spline *_spline, float t, float *_v) {
    if (! _v) {
        dlog_error (_, "spline_add_seg: missing v");
        return false;
    }

    spline_seg *_seg = malloc(sizeof(spline_seg));
    _seg->t = t;

    uint32_t v_n;
    if (_spline->type == SPLINE_TYPE_LINEAR)
        v_n = _spline->dim;
    else if (_spline->type == SPLINE_TYPE_CUBIC_BEZIER)
        v_n = _spline->dim * 3;
    else {
        dlog_error_u (_, "spline_add_seg: unknown type", _spline->type);
        return false;
    }

    for (uint32_t c = 0; c < v_n; c ++) 
        _seg->_v[c] = _v[c];

    blaze_add (_spline->_segs, _seg);

    if (t < _spline->min_t) 
        _spline->min_t = t;

    if (t > _spline->max_t) 
        _spline->max_t = t;

    return true;
}

bool spline_load (spline *_spline, arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "spline_load", _name_s);

    uint8_t *_b;
    uint32_t b_n;

    if (! arc_or_file_get_b (_arc, _name_s, &_b, &b_n)) {
        dlog_error_s (_, "spline_load: arc_or_fs_get_b", _name_s);
        return false;
    }

    uint8_t *_m = _b;

    _spline->dim = *(uint8_t *)_m;
    _m += 1;

    _spline->type = *(uint32_t *)_m;
    _m += 4;

    uint32_t key_n = *(uint32_t *)_m;
    _m += 4;
    
    for (uint32_t key_c = 0; key_c < key_n; key_c ++) {
        float t = *(float *)_m;
        _m += 4;

        uint32_t v_n;
        if (_spline->type == SPLINE_TYPE_LINEAR)
            v_n = _spline->dim;
        else if (_spline->type == SPLINE_TYPE_CUBIC_BEZIER)
            v_n = _spline->dim * 3;
        else {
            dlog_error_u (_, "spline_load: unknown type", _spline->type);
            return false;
        }

        float v [v_n];
        for (uint32_t c = 0; c < v_n; c ++) {
            v[c] = *(float *)_m;
            _m += 4;
        }

        if (! spline_add_seg (_spline, t, v)) {
            dlog_error (_, "spline_load: spline_add_seg");
            return false;
        }
    }

    if (! _arc)
        free(_b);

    return true;
}

