#include "../sys.h"
#include "../../enums_map/enums_map.h"
#include "../sys_enums_map.h"

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s input output\n", argv[0]);
        return 1;
    }

    FILE *_f_in = fopen(argv[1], "r");
    if (! _f_in) {
        dlog_error_s (_, "fopen r", argv[1]);
        return 2;
    }
 
    FILE *_f_out = fopen(argv[2], "w");
    if (! _f_out) {
        dlog_error_s (_, "fopen w", argv[2]);
        return 3;
    }

    name_map *_name_map_smt = malloc(sizeof(name_map));
    if (! name_map_cons (_name_map_smt))
        return 4;
    _name_map_smt->ext_data_free_ = true;
    _name_map_smt->_ext_data_free_fn = free;

    char _in_s [PATH_MAX];
    while (true) {
        if (! fscanf (_f_in, "%s", _in_s))
            return -1;
        uint32_t cmd_id = enums_map_get_id (_sys_enums_map, _in_s);
        if (! cmd_id) {
            dlog_error_s (_, "unknown cmd", _in_s);
            return 5;
        }
        fwrite(&cmd_id, 4, 1, _f_out);

        if (cmd_id == SYS_CMD_END)
            break;
        else if (cmd_id == SYS_CMD_DEF_STATES) {
            uint32_t state_n;
            if (! fscanf(_f_in, "%u", &state_n))
                return -1;
            fwrite(&state_n, 4, 1, _f_out);

            for (uint32_t state_c = 0; state_c < state_n; state_c ++) {
                if (! fscanf(_f_in, "%s", _in_s))
                    return -1;
                fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
            }
        }
        else if (cmd_id == SYS_CMD_DEF_COMP) {
            if (! fscanf(_f_in, "%s", _in_s))
                return -1;
            fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);

            if (! fscanf(_f_in, "%s", _in_s))
                return -1;
            fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
        }
        else if (cmd_id == SYS_CMD_STATE) {
            if (! fscanf(_f_in, "%s", _in_s))
                return -1;
            fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
        }
        else if (cmd_id == SYS_CMD_LOAD_MSG_TYPES) {
            if (! fscanf(_f_in, "%s", _in_s))
                return -1;
            fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);

            if (! sys_msg_type_load (_name_map_smt, _, _in_s))
                return 6;
        }
        else if (cmd_id == SYS_CMD_SEND_MSG) {
            if (! fscanf(_f_in, "%s", _in_s))
                return -1;
            fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
 
            if (! fscanf(_f_in, "%s", _in_s))
                return -1;
            name_map_item *_item = name_map_get_item (_name_map_smt, _in_s);
            if (! _item) {
                dlog_error_s (_, "unknown msg type", _in_s);
                return 7;
            }
            fwrite(&_item->id, 4, 1, _f_out);

            uint32_t arg_type = *(uint32_t *)_item->_ext_data;
            if ((arg_type == SYS_MSG_TYPE_ARG_TYPE_S) ||
                    (arg_type == SYS_MSG_TYPE_ARG_TYPE_STATE_NAME) ||
                    (arg_type == SYS_MSG_TYPE_ARG_TYPE_COMP_NAME)) {
                if (! fscanf(_f_in, "%s", _in_s))
                    return -1;
                fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
            }
            else if (arg_type == SYS_MSG_TYPE_ARG_TYPE_U) {
                uint32_t arg_i;
                if (! fscanf(_f_in, "%u", &arg_i))
                    return -1;
                fwrite(&arg_i, 4, 1, _f_out);
            }
            else if (arg_type == SYS_MSG_TYPE_ARG_TYPE_F) {
                float arg_f;
                if (! fscanf(_f_in, "%f", &arg_f))
                    return -1;
                fwrite(&arg_f, 4, 1, _f_out);
            }
            else if (arg_type == SYS_MSG_TYPE_ARG_TYPE_V) {
                float arg_f;
                for (uint32_t c = 0; c < 3; c ++) {
                    if (! fscanf(_f_in, "%f", &arg_f))
                        return -1;
                    fwrite(&arg_f, 4, 1, _f_out);
                }
            }
        }
    }
    
    if (! name_map_decons (_name_map_smt))
        return 8;
    free(_name_map_smt);

    return 0;
}

