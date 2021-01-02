#include "sys.h"

bool sys_cons (sys *_sys) {
    _sys->_parent_name_map = _;
    _sys->id = 0;

    _sys->_msgs = malloc(sizeof(blaze));
    if (! blaze_cons (_sys->_msgs))
        return false;

    _sys->_ext_process_msg_fn = _;
    _sys->_ext_process_msg_this = _;

    _sys->_name_map_smt = malloc(sizeof(name_map));
    if (! name_map_cons (_sys->_name_map_smt))
        return false;
    _sys->_name_map_smt->ext_data_free_ = true;
    _sys->_name_map_smt->_ext_data_free_fn = free;

    _sys->_comps = malloc(sizeof(comp));
    if (! blaze_cons (_sys->_comps))
        return false;
 
    _sys->_name_map_comp = malloc(sizeof(name_map));
    if (! name_map_cons (_sys->_name_map_comp))
        return false;

    _sys->state_ignore_ = true;
    _sys->state_cur = 0;
    _sys->state_next = 0;
    _sys->state_t = 0.0;
    _sys->state_len = 1.0;

    _sys->_name_map_state = malloc(sizeof(name_map));
    if (! name_map_cons (_sys->_name_map_state))
        return false;

    return true;
}

bool sys_decons (sys *_sys) {
    if (! name_map_decons (_sys->_name_map_state))
        return false;
    free(_sys->_name_map_state);
    _sys->_name_map_state = _;
    
    if (! name_map_decons (_sys->_name_map_comp))
        return false;
    free(_sys->_name_map_comp);
    _sys->_name_map_comp = _;
 
    if (! blaze_clear (_sys->_comps, comp_free))
        return false;
    if (! blaze_decons (_sys->_comps))
        return false;
    free(_sys->_comps);
    _sys->_comps = _;
    
    if (! name_map_decons (_sys->_name_map_smt))
        return false;
    free(_sys->_name_map_smt);
    _sys->_name_map_smt = _;

    if (! blaze_clear (_sys->_msgs, sys_msg_free))
        return false;
    if (! blaze_decons (_sys->_msgs))
        return false;
    free(_sys->_msgs);
    _sys->_msgs = _;

    return true;
}

void sys_free (void *_v) {
    dlog_debug (_, "sys_free");

    if (! _v) {
        dlog_error (_, "sys_free: missing _v");
        return;
    }

    if (! sys_decons ((sys *)_v))
        return;

    free(_v);
}

void sys_msg_free (void *_v) {
    dlog_debug (_, "sys_msg_free");

    if (! _v) {
        dlog_error (_, "sys_msg_free: missing _v");
        return;
    }

    sys_msg *_msg = (sys_msg *)_v;
    if (_msg->_arg_s)
        free(_msg->_arg_s);

    free(_v);
}

bool sys_tick (sys *_sys, float t) {
    if (_sys->state_ignore_)
        return true;

    _sys->state_t += t;
    if (_sys->state_t >= _sys->state_len) {
        _sys->state_t -= _sys->state_len;
        if (! sys_set_state (_sys, _sys->state_next))
            return false;
    }

    return true;
}

bool sys_set_state (sys *_sys, uint32_t state_id) {
    dlog_debug_u (_, "sys_set_state", state_id);

    if (state_id == (uint32_t)-1) 
        _sys->state_cur ++;
    else 
        _sys->state_cur = state_id;

    _sys->state_next = (uint32_t)-1;
    _sys->state_len = 1.0;
    _sys->state_ignore_ = false;

    for (sys_msg *_msg = blaze_first (_sys->_msgs); _msg;
            _msg = blaze_next (_sys->_msgs)) {
        if (_msg->state_id != _sys->state_cur)
            continue;

        if (_msg->sys_id == (uint32_t)-1) {
            if (! sys_process_msg (_sys, _msg)) {
                dlog_error (_, "sys_set_state: sys_process_msg");
                return false;
            }
        }
        else {
            // TODO
            dlog_error_u (_, "TODO: sys_id is ", _msg->sys_id);
        }
    }
    return true;
}

bool sys_process_msg (sys *_sys, sys_msg *_msg) {
    // TODO: look-up name AOT
    if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "state_set")) {
        _sys->state_t = 0.0;
        if (! sys_set_state (_sys, _msg->arg_u)) {
            dlog_error_u (_, "sys_process_msg: sys_set_state", _msg->arg_u);
            return false;
        }
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "state_set_next")) {
        _sys->state_next = _msg->arg_u;
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "state_set_len")) {
        _sys->state_len = _msg->arg_f;
    }
    else if (_msg->_type_item->id ==
            name_map_get (_sys->_name_map_smt, "state_set_ignore")) {
        _sys->state_ignore_ = (bool)_msg->arg_u;
    }
    else {
        // TODO: multiple ext msg processors
        if (! _sys->_ext_process_msg_fn) {
            dlog_error (_, "sys_process_msg: missing _ext_process_msg_fn");
            return false;
        }
        return _sys->_ext_process_msg_fn (_sys->_ext_process_msg_this, _msg);
    }

    return true;
}

bool sys_load (sys *_sys, arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "sys_load", _name_s);

    uint8_t *_b;
    uint32_t b_n;
    
    if (! arc_or_file_get_b (_arc, _name_s, &_b, &b_n)) {
        dlog_error_s (_, "sys_load: arc_or_fs_get_b", _name_s);
        return false;
    }

    uint32_t state_cur = 0;
    uint8_t *_m = _b;
    while (true) {
        uint32_t cmd_id = *(uint32_t *)_m;
        _m += 4;

        if (cmd_id == SYS_CMD_END) {
            break;
        }
        else if (cmd_id == SYS_CMD_DEF_STATES) {
            uint32_t state_n = *(uint32_t *)_m;
            _m += 4;

            for (uint32_t state_c = 0; state_c < state_n; state_c ++) {
                if (! name_map_add (_sys->_name_map_state, (char *)_m)) {
                    dlog_error_s (_, "sys_load: name_map_add", (char *)_m);
                    return false;
                }
                _m += strlen((char *)_m) + 1;
            }
        }
        else if (cmd_id == SYS_CMD_DEF_COMP) {
            if (! name_map_add (_sys->_name_map_comp, (char *)_m)) {
                dlog_error_s (_, "sys_load: name_map_add (2)", (char *)_m);
                return false;
            }
            _m += strlen((char *)_m) + 1;

            comp *_comp = malloc(sizeof(comp));
            if (! comp_cons (_comp))
                return false;

            if (! comp_load (_comp, _arc, (char *)_m))
                return false;

            blaze_add (_sys->_comps, _comp);

            _m += strlen((char *)_m) + 1;
        }
        else if (cmd_id == SYS_CMD_STATE) {
            state_cur = name_map_get (_sys->_name_map_state, (char *)_m);
            if (! state_cur) {
                dlog_error_s (_, "sys_load: unknown state", (char *)_m);
                return false;
            }
            state_cur -= 1;

            _m += strlen((char *)_m) + 1;
        }
        else if (cmd_id == SYS_CMD_LOAD_MSG_TYPES) {
            if (! sys_msg_type_load (_sys->_name_map_smt, _arc, (char *)_m)) {
                dlog_error_s (_, "sys_load: sys_msg_type_load", (char *)_m);
                return false;
            }
            _m += strlen((char *)_m) + 1;
        }
        else if (cmd_id == SYS_CMD_SEND_MSG) {
            sys_msg *_msg = malloc(sizeof(sys_msg));
            _msg->_arg_s = _;
            _msg->state_id = state_cur;

            if (! strcmp((char *)_m, ".")) {
                _msg->sys_id = (uint32_t)-1;
            }
            else {
                _msg->sys_id = name_map_get (_sys->_parent_name_map, (char *)_m);
                if (! _msg->sys_id) {
                    dlog_error_s (_, "sys_load: unknown sys", (char *)_m);
                    return false;
                }
                _msg->sys_id -= 1;
            }
            _m += strlen((char *)_m) + 1;
 
            uint32_t smt_id = *(uint32_t *)_m;
            _m += 4;

            _msg->_type_item = name_map_get_item_by_id (_sys->_name_map_smt,
                    smt_id);
            if (! _msg->_type_item) {
                dlog_error_u (_, "sys_load: unknown smt", smt_id);
                return false;
            }

            uint32_t arg_type = *(uint32_t *)_msg->_type_item->_ext_data;
            if (arg_type == SYS_MSG_TYPE_ARG_TYPE_S) {
                _msg->_arg_s = strdup((char *)_m);
                _m += strlen((char *)_m) + 1;
            }
            else if (arg_type == SYS_MSG_TYPE_ARG_TYPE_U) {
                _msg->arg_u = *(uint32_t *)_m;
                _m += 4;
            }
            else if (arg_type == SYS_MSG_TYPE_ARG_TYPE_F) {
                _msg->arg_f = *(float *)_m;
                _m += 4;
            }
            else if (arg_type == SYS_MSG_TYPE_ARG_TYPE_V) {
                for (uint32_t c = 0; c < 3; c ++) {
                    _msg->_arg_v[c] = *(float *)_m;
                    _m += 4;
                }
            }
            else if (arg_type == SYS_MSG_TYPE_ARG_TYPE_COMP_NAME) {
                _msg->arg_u = name_map_get (_sys->_name_map_comp, (char *)_m);
                if (! _msg->arg_u) {
                    dlog_error_s (_, "sys_load: unknown comp", (char *)_m);
                    return false;
                }
                _msg->arg_u -= 1;
                _m += strlen((char *)_m) + 1;
            }
            else if (arg_type == SYS_MSG_TYPE_ARG_TYPE_STATE_NAME) {
                _msg->arg_u = name_map_get (_sys->_name_map_state, (char *)_m);
                if (! _msg->arg_u) {
                    dlog_error_s (_, "sys_load: unknown state (2)", (char *)_m);
                    return false;
                }
                _msg->arg_u -= 1;
                _m += strlen((char *)_m) + 1;
            }

            blaze_add (_sys->_msgs, _msg);
        }
    }
    
    if (! _arc)
        free(_b);

    return true;
}

bool sys_msg_type_load (name_map *_name_map_smt, arc *_arc,
        char const *_name_s) {
    dlog_debug_s (_, "sys_msg_type_load", _name_s);

    uint8_t *_b;
    uint32_t b_n;
    
    if (! arc_or_file_get_b (_arc, _name_s, &_b, &b_n)) {
        dlog_error_s (_, "sys_load: arc_or_fs_get_b", _name_s);
        return false;
    }

    uint8_t *_m = _b;
    while (true) {
        uint32_t cmd_id = *(uint32_t *)_m;
        _m += 4;

        if (cmd_id == SYS_MSG_TYPE_END) {
            break;
        }
        else if (cmd_id == SYS_MSG_TYPE_ID_OFFSET) {
            _name_map_smt->id_cur = *(uint32_t *)_m;
            _m += 4;
        }
        else if (cmd_id == SYS_MSG_TYPE_DEF) {
            char *_smt_name_s = (char *)_m;
            _m += strlen((char *)_m) + 1;

            uint32_t arg_type = *(uint32_t *)_m;
            _m += 4;

            uint32_t smt_id = name_map_add (_name_map_smt, _smt_name_s);
            if (! smt_id) {
                dlog_error_s (_, "sys_msg_type_load: name_map_add", _smt_name_s);
                return false;
            }

            name_map_item *_type_item = name_map_get_item_by_id (
                    _name_map_smt, smt_id);
            if (! _type_item) {
                dlog_error_u (_, "sys_msg_type_load: unknown smt", smt_id);
                return false;
            }

            _type_item->_ext_data = malloc(4);
            *(uint32_t *)_type_item->_ext_data = arg_type;
        }
    }
    
    if (! _arc)
        free(_b);

    return true;
}

