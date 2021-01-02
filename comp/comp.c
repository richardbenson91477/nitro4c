#include "comp.h"

bool comp_cons (comp *_comp) {
    dlog_debug (_, "comp_cons");

    _comp->_elems = malloc(sizeof(blaze));
    if (! blaze_cons (_comp->_elems))
        return false;

    _comp->dim = 0;
    _comp->min_t = FLT_MAX;
    _comp->max_t = FLT_MIN;
    return true;
}

bool comp_decons (comp *_comp) {
    dlog_debug (_, "comp_decons");

    blaze_clear (_comp->_elems, comp_elem_free);
    if (! blaze_decons (_comp->_elems))
        return false;
    free(_comp->_elems);
    _comp->_elems = _;

    return true;
}

void comp_free (void *_v) {
    dlog_debug (_, "comp_free");
    if (! _v) {
        dlog_error (_, "comp_free: missing _v");
        return;
    }

    if (! comp_decons ((comp *)_v))
        return;

    free(_v);
}

bool comp_eval (comp const *_comp, float t, float *_v_out) {
    for (uint8_t c = 0; c < _comp->dim; c ++)
        _v_out[c] = 0.0f;

    float _v [COMP_MAX_DIM];

    for (comp_elem *_elem = blaze_first (_comp->_elems); _elem; 
            _elem = blaze_next (_comp->_elems)) {
        if (_elem->type == COMP_ELEM_TYPE_SPLINE) {
            spline_eval (_elem->_spline, t, _v);
            if (_elem->combiner == COMP_ELEM_COMBINE_SET)
                for (uint8_t c = 0; c < _elem->_spline->dim; c ++)
                    _v_out[c] = _v[c];
            else if (_elem->combiner == COMP_ELEM_COMBINE_ADD)
                for (uint8_t c = 0; c < _elem->_spline->dim; c ++)
                    _v_out[c] += _v[c];
            else if (_elem->combiner == COMP_ELEM_COMBINE_MUL)
                for (uint8_t c = 0; c < _elem->_spline->dim; c ++)
                    _v_out[c] *= _v[c];
        }
        else if (_elem->type == COMP_ELEM_TYPE_CONSTANTS) {
            if (_elem->combiner == COMP_ELEM_COMBINE_SET)
                for (uint8_t c = 0; c < _elem->consts_dim; c ++)
                    _v_out[c] = _elem->_consts[c];
            else if (_elem->combiner == COMP_ELEM_COMBINE_ADD)
                for (uint8_t c = 0; c < _elem->consts_dim; c ++)
                    _v_out[c] += _elem->_consts[c];
            else if (_elem->combiner == COMP_ELEM_COMBINE_MUL)
                for (uint8_t c = 0; c < _elem->consts_dim; c ++)
                    _v_out[c] *= _elem->_consts[c];
        }
        else if (_elem->type == COMP_ELEM_TYPE_USER_FN) {
            _elem->_user_fn (_v, _v_out, t);
            if (_elem->combiner == COMP_ELEM_COMBINE_SET)
                for (uint8_t c = 0; c < _elem->user_fn_dim; c ++)
                    _v_out[c] = _v[c];
            else if (_elem->combiner == COMP_ELEM_COMBINE_ADD)
                for (uint8_t c = 0; c < _elem->user_fn_dim; c ++)
                    _v_out[c] += _v[c];
            else if (_elem->combiner == COMP_ELEM_COMBINE_MUL)
                for (uint8_t c = 0; c < _elem->user_fn_dim; c ++)
                    _v_out[c] *= _v[c];
        }
    }

    return true;
}

bool comp_add_spline (comp *_comp, uint32_t combiner,
        spline *_spline, bool spline_free_) {
    dlog_debug (_, "comp_add_spline");

    if (! _spline) {
        dlog_error (_, "comp_add_spline: missing _spline");
        return false;
    }

    comp_elem *_elem = malloc(sizeof(comp_elem));
    _elem->type = COMP_ELEM_TYPE_SPLINE;
    _elem->combiner = combiner;

    _elem->_spline = _spline;
    _elem->spline_free_ = spline_free_;

    blaze_add (_comp->_elems, _elem);

    if (_spline->dim > _comp->dim)
        _comp->dim = _spline->dim;

    if (_spline->min_t < _comp->min_t) 
        _comp->min_t = _spline->min_t;

    if (_spline->max_t > _comp->max_t) 
        _comp->max_t = _spline->max_t;

    return true;
}

bool comp_add_consts (comp *_comp, uint32_t combiner,
        uint8_t consts_dim, float const *_consts) {
    dlog_debug (_, "comp_add_consts");
 
    comp_elem *_elem = malloc(sizeof(comp_elem));
    _elem->type = COMP_ELEM_TYPE_CONSTANTS;
    _elem->combiner = combiner;

    _elem->consts_dim = consts_dim;
    for (uint8_t c = 0; c < consts_dim; c ++)
        _elem->_consts[c] = _consts[c];

    blaze_add (_comp->_elems, _elem);
 
    if (consts_dim > _comp->dim)
        _comp->dim = consts_dim;

    return true;
}

bool comp_add_user_fn (comp *_comp, uint32_t combiner,
        uint8_t user_fn_dim, void (* _user_fn)(float *, float const *, float)) {
    dlog_debug (_, "comp_add_user_fn");

    if (! _user_fn) {
        dlog_error (_, "comp_add_user_fn: missing user_fn");
        return false;
    }

    comp_elem *_elem = malloc(sizeof(comp_elem));
    _elem->type = COMP_ELEM_TYPE_USER_FN;
    _elem->combiner = combiner;

    _elem->user_fn_dim = user_fn_dim;
    _elem->_user_fn = _user_fn;

    blaze_add (_comp->_elems, _elem);
    
    if (user_fn_dim > _comp->dim)
        _comp->dim = user_fn_dim;

    return true;
}

bool comp_load (comp *_comp, arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "comp_load", _name_s);
    
    uint8_t *_b;
    uint32_t b_n;
    
    if (! arc_or_file_get_b (_arc, _name_s, &_b, &b_n)) {
        dlog_error_s (_, "comp_load: arc_or_fs_get_b", _name_s);
        return false;
    }

    uint8_t *_m = _b;

    uint32_t elem_n = *(uint32_t *)_m;
    _m += 4;

    for (uint32_t elem_c = 0; elem_c < elem_n; elem_c ++) {
        uint32_t combiner = *(uint32_t *)_m;
        _m += 4;

        uint32_t type = *(uint32_t *)_m;
        _m += 4;

        if (type == COMP_ELEM_TYPE_SPLINE) {
            spline *_spline = malloc(sizeof(spline));
            if (! spline_cons (_spline))
                return false;

            if (! spline_load (_spline, _arc, (char *)_m)) {
                dlog_error (_, "comp_load: spline_load");
                return false;
            }
            _m += strlen((char *)_m) + 1;

            if (! comp_add_spline (_comp, combiner, _spline, 1)) {
                dlog_error (_, "comp_load: comp_add_spline");
                return false;
            }
        }
        else if (type == COMP_ELEM_TYPE_CONSTANTS) {
            uint8_t consts_dim = *(uint8_t *)_m;
            _m += 1;
            if (consts_dim > COMP_MAX_DIM) {
                dlog_error (_, "comp_load: too many constants");
                return false;
            }
            float _consts [COMP_MAX_DIM];
            for (uint8_t c = 0; c < consts_dim; c ++) {
                _consts[c] = *(float *)_m;
                _m += 4;
            }
 
            if (! comp_add_consts (_comp, combiner, consts_dim, _consts)) {
                dlog_error (_, "comp_load: comp_add_spline");
                return false;
            }
        }
        else {
            dlog_error (_, "comp_load: unsupported type");
            return false;
        }
    }

    if (! _arc)
        free(_b);

    return true;
}

void comp_elem_free (void *_v) {
    dlog_debug (_, "comp_elem_free");
    if (! _v)
        return;

    comp_elem *_elem = (comp_elem *)_v;
    if ((_elem->type == COMP_ELEM_TYPE_SPLINE) &&
            _elem->spline_free_) {
        if (! spline_decons (_elem->_spline))
            return;

        free(_elem->_spline);
    }
}

