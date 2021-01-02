#include "../model.h"

// model format:
//   uint32_t vert_n, float rad, uint8_t poly_col_, {char tex_s[],} \0, 
//     [float v[3], n[3], t[2]] * vert_n, 
//     [float n[3], float d] * poly_n
// where poly_n = vert_n / 3

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s input output\n", argv[0]);
        return 1;
    }

    dlog_debug_s (_, "opening for read", argv[1]);
    FILE *f_in = fopen(argv[1], "r");
    if (! f_in) {
        dlog_error_s (_, "error reading", argv[1]);
        return 2;
    }
    
    dlog_debug_s (_, "opening for write", argv[2]);
    FILE *f_out = fopen(argv[2], "w");
    if (! f_out) {
        dlog_error_s (_, "error writing", argv[2]);
        return 3;
    }
    
    uint32_t vert_n, poly_n;
    if (! fscanf(f_in, "%u", &vert_n))
        return -1;
    dlog_debug_u (_, "vert_n", vert_n);

    poly_n = vert_n / 3;
    dlog_debug_u (_, "poly_n", poly_n);

    float rad;
    if (! fscanf(f_in, "%f", &rad))
        return -1;
    dlog_debug_f (_, "rad", rad);

    uint32_t poly_col_;
    if (! fscanf(f_in, "%u", &poly_col_))
        return -1;
    dlog_debug_u (_, "poly_col_", poly_col_);

    char tex_s [PATH_MAX];
    if (! fscanf(f_in, "%s", tex_s))
        return -1;
    dlog_debug_s (_, "tex_s", tex_s);

    float *_verts = (float *)malloc(vert_n * 8 * 4);
    float *_poly_norms = (float *)malloc(poly_n * 4 * 4);
    if ((! _verts) || (! _poly_norms)) { 
        dlog_error (_, "malloc");
        return 4;
    }

    float *_vert = _verts;
    for (uint32_t c = 0; c < vert_n * 8; c ++)
        if (! fscanf(f_in, "%f", _vert ++))
            return -1;

    fclose(f_in);

    fwrite(&vert_n, 4, 1, f_out);
    fwrite(&rad, 4, 1, f_out);
    if (poly_col_)
        fputc(1, f_out);
    else 
        fputc(0, f_out);
    if (! strcmp(tex_s, "NONE"))
        fputc(0, f_out);
    else
        fwrite(tex_s, 1, strlen (tex_s) + 1, f_out);
    fwrite(_verts, 4, vert_n * 8, f_out);

    _vert = _verts;
    float *_poly_norm = _poly_norms;
    for (uint32_t c = 0; c < poly_n; c++) {
        poly_plane (_poly_norm, _vert + 8, _vert, _vert + 16);
        _vert += 24;
        _poly_norm += 4;
    }

    fwrite(_poly_norms, 4, poly_n * 4, f_out);
    fclose(f_out);

    free(_verts); 
    free(_poly_norms);
    return 0;
}

