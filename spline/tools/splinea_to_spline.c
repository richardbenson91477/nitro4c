#include "../spline.h"
#include "../../enums_map/enums_map.h"
#include "../spline_enums_map.h"

int main (int argc, char **argv) {
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
 
    uint32_t dim_u;
    if (! fscanf(_f_in, "%u", &dim_u))
        return -1;
    uint8_t dim = (uint8_t)dim_u;
    fwrite(&dim, 1, 1, _f_out);

    char _in_s [PATH_MAX];
    if (! fscanf(_f_in, "%s", _in_s))
        return -1;
    uint32_t type = enums_map_get_id (_spline_enums_map, _in_s);
    if (! type) {
        dlog_error_s (_, "unknown enum", _in_s);
        return 4;
    }
    fwrite(&type, 4, 1, _f_out);
 
    uint32_t v_n;
    if (type == SPLINE_TYPE_LINEAR)
        v_n = dim;
    else if (type == SPLINE_TYPE_CUBIC_BEZIER)
        v_n = dim * 3;
    else {
        dlog_error_u (_, "unknown spline type", type);
        return 5;
    }

    uint32_t key_n;
    if (! fscanf(_f_in, "%u", &key_n))
        return -1;
    fwrite(&key_n, 4, 1, _f_out);

    float t, v;
    for (uint32_t c = 0; c < key_n; c ++) {
        if (! fscanf(_f_in, "%f", &t))
            return -1;
        fwrite(&t, 4, 1, _f_out);

        for (uint32_t c = 0; c < v_n; c ++) {
            if (! fscanf(_f_in, "%f", &v))
                return -1;
            fwrite(&v, 4, 1, _f_out);
        }
    }

    return 0;
}

