#include "../comp.h"
#include "../../enums_map/enums_map.h"
#include "../comp_enums_map.h"

int main (int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input output\n", argv[0]);
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

    uint32_t elem_n;
    if (! fscanf (_f_in, "%u", &elem_n))
        return -1;
    fwrite (&elem_n, 4, 1, _f_out);
 
    char _in_s [PATH_MAX];
    for (uint32_t elem_c = 0; elem_c < elem_n; elem_c ++) {
        
        if (! fscanf(_f_in, "%s", _in_s))
            return -1;
        uint32_t combiner = enums_map_get_id (_comp_enums_map, _in_s);
        if (! combiner) {
            dlog_error_s (_, "unknown enum", _in_s);
            return 4;
        }
        fwrite(&combiner, 4, 1, _f_out);

        if (! fscanf(_f_in, "%s", _in_s))
            return -1;
        uint32_t type = enums_map_get_id (_comp_enums_map, _in_s);
        if (! type) {
            dlog_error_s (_, "unknown enum", _in_s);
            return 5;
        }
        fwrite(&type, 4, 1, _f_out);

        if (type == COMP_ELEM_TYPE_SPLINE) {
            if (! fscanf(_f_in, "%s", _in_s))
                return -1;
            fwrite(_in_s, 1, strlen(_in_s) + 1, _f_out);
        }
        else if (type == COMP_ELEM_TYPE_CONSTANTS) {
            uint32_t constants_dim_u;
            if (! fscanf(_f_in, "%u", &constants_dim_u))
                return -1;

            uint8_t constants_dim = (uint8_t)constants_dim_u;
            if (constants_dim > COMP_MAX_DIM) {
                dlog_error_u (_, "too many constants", constants_dim_u);
                return 6;
            }
            fwrite(&constants_dim, 1, 1, _f_out);

            float constant;
            for (uint8_t c = 0; c < constants_dim; c ++) {
                if (! fscanf(_f_in, "%f", &constant))
                    return -1;
                fwrite(&constant, 4, 1, _f_out);
            }
        }
        else {
            dlog_error (_, "comp_load: unsupported type");
            return 7;
        }
    }
    return 0;
}

