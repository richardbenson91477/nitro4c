#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../nitro_defs.h"

int main (void) {
    FILE *f_out_txt = fopen("circle.txt", "w");
    FILE *f_out_dat = fopen("circle.dat", "w");

    float out_f;
    float in_f = 0.0;
    for (uint32_t c = 0; c < 64; c ++) {
        out_f = cosf(in_f);
        fprintf(f_out_txt, "%f ", out_f);
        fwrite(&out_f, 4, 1, f_out_dat);

        out_f = sinf(in_f);
        fprintf(f_out_txt, "%f\n", out_f);
        fwrite(&out_f, 4, 1, f_out_dat);

        in_f += (PI2 / 64.0);
    }

    fclose (f_out_txt);
    fclose (f_out_dat);
    return 0;
}

