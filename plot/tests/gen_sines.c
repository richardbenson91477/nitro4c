#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../nitro_defs.h"

int main (void) {
    FILE *f_out_txt = fopen("sin.txt", "w");
    FILE *f_out_dat = fopen("sin.dat", "w");

    float out_f;
    float in_f = -PI;
    for (uint32_t c = 0; c < 32; c ++) {
        out_f = sinf(in_f);
        fprintf(f_out_txt, "%f\n", out_f);
        fwrite(&out_f, 4, 1, f_out_dat);

        in_f += (PI2 / 16.0);
    }

    fclose(f_out_txt);
    fclose(f_out_dat);
    return 0;
}

