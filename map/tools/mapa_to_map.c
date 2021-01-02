#include "../map.h"
#include "../../enums_map/enums_map.h"
#include "../map_enums_map.h"

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
        dlog_error_s (_, "fopen wb", argv[2]);
        return 3;
    }

    char _in_s [PATH_MAX];
    float in_f;
    while (true) {
        if (! fscanf (_f_in, "%s", _in_s))
            return -1;
        uint32_t cmd_id = enums_map_get_id (_map_enums_map, _in_s);
        if (! cmd_id) {
            dlog_error_s (_, "unknown cmd", _in_s);
            return 4;
        }
        fwrite(&cmd_id, 4, 1, _f_out);

        if (cmd_id == MAP_CMD_END) 
            break;
        else if (cmd_id == MAP_CMD_DEF_SYS) {
            for (uint32_t c = 0; c < 2; c ++) {
                if (! fscanf(_f_in, "%s", _in_s))
                    return -1;
                fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
            }
        }
        else if (cmd_id == MAP_CMD_ENT) { 
            for (uint32_t c = 0; c < 6; c ++) {
                if (! fscanf(_f_in, "%f", &in_f))
                    return -1;
                fwrite(&in_f, 4, 1, _f_out);
            }
            for (uint32_t c = 0; c < 2; c ++) {
                if (! fscanf(_f_in, "%s", _in_s))
                    return -1;
                fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
            }
        }
        else if (cmd_id == MAP_CMD_CAM) {
            for (uint32_t c = 0; c < 6; c ++) {
                if (! fscanf(_f_in, "%f", &in_f))
                    return -1;
                fwrite(&in_f, 4, 1, _f_out);
            }

            if (! fscanf(_f_in, "%s", _in_s))
                    return -1;
            fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
        }
    }

    return 0;
}

