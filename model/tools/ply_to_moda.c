#include "../model.h"

// moda format: 
//   uint32_t vert_n, float rad, uint32_t poly_col_, char tex_fname[], 
//     [float v0 v1 v2 n0 n1 n2 t0 t1] * vert_n
// blender .ply export settings: normals, uvs, -z forward, y-up

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s input output\n", argv[0]);
        return 1;
    }

    dlog_debug_s (_, "opening for read", argv[1]);
    FILE *_f_in = fopen(argv[1], "r");
    if (! _f_in) {
        dlog_error_s (_, "error reading", argv[1]);
        return 2;
    }
    
    dlog_debug_s (_, "opening for write", argv[2]);
    FILE *_f_out = fopen(argv[2], "w");
    if (! _f_out) {
        dlog_error_s (_, "error writing", argv[2]);
        return 3;
    }

    uint32_t in_s_n = 160;
    char in_s [in_s_n];
    if ((! fgets(in_s, in_s_n, _f_in)) || (strcmp(in_s, "ply\n"))) {
        dlog_error (_, "invalid input header");
        return 4;
    }
    dlog_debug (_, "found ply header");

    uint32_t vert_n = 0;
    uint32_t poly_n = 0;
    bool found_end_header_ = false;
    while (fgets(in_s, in_s_n, _f_in)) {
        if (! strncmp(in_s, "element vertex ", 15))
            vert_n = atoi(in_s + 15);
        else if (! strncmp(in_s, "element face ", 13))
            poly_n = atoi(in_s + 13);
        else if (! strcmp(in_s, "end_header\n")) {
            found_end_header_ = true;
            dlog_debug (_, "found end_header");
            break;
        }
    }

    if ((! found_end_header_) || (vert_n < 3) || (poly_n < 1)) {
        dlog_error (_, "invalid input file");
        return 5;
    }

    dlog_debug_u (_, "vert_n", vert_n);
    dlog_debug_u (_, "poly_n", poly_n);

    float *_verts = (float *)malloc(vert_n * 8 * 4);
    uint32_t *_polys = (uint32_t *)malloc(poly_n * 4 * 4);
    if ((! _verts) || (!_polys)) {
        dlog_error (_, "malloc failed");
        return 6;
    }

    uint32_t c;
    for (c = 0; c < (vert_n * 8); c ++)
        if (! fscanf(_f_in, "%f", _verts + c))
            return -1;

    for (c = 0; c < (poly_n * 4); c ++)
        if (! fscanf(_f_in, "%u", _polys + c))
            return -1;
    
    fclose(_f_in);

    fprintf(_f_out, "%u\n0.0\n1\ndefault.rwh\n", poly_n * 3);

    uint32_t *_poly = _polys;
    for (c = 0; c < poly_n; c ++) {
        if (_poly[0] != 3) {
            dlog_error (_, "non 3-sided polygon detected");
            return 7;
        }

        float *_vert = _verts + _poly[1] * 8;
        fprintf(_f_out, "%f %f %f %f %f %f %f %f\n",
            _vert[0], _vert[1], _vert[2], _vert[3], _vert[4], _vert[5],
            _vert[6], _vert[7]);

        _vert = _verts + _poly[2] * 8;
        fprintf(_f_out, "%f %f %f %f %f %f %f %f\n", _vert[0], _vert[1],
            _vert[2], _vert[3], _vert[4], _vert[5], _vert[6], _vert[7]);
        
        _vert = _verts + _poly[3] * 8;
        fprintf(_f_out, "%f %f %f %f %f %f %f %f\n", _vert[0], _vert[1],
            _vert[2], _vert[3], _vert[4], _vert[5], _vert[6], _vert[7]);

        _poly += 4;
    }

    fclose(_f_out);
    return 0;
}

