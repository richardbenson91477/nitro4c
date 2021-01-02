#include "../img.h"
#include "save_ppm.c"

int main (int argc, char **argv) {
    if ((argc < 3) || (argc > 4)) {
        fprintf(stderr, "usage: %s input output [alpha-output]\n", argv[0]);
        return 1;
    }
 
    img *_img = malloc(sizeof(img));
    if (! img_cons (_img))
        return 2;

    if (! img_load_rwh (_img, _, argv[1]))
        return 3;
    
    if (! save_ppm (_img, argv[2], 0))
        return 4;

    if (argc == 4) {
        if (! save_ppm (_img, argv[3], 1))
            return 5;
    }

    if (! img_decons (_img))
        return 6;

    free(_img);
    return 0;
}

