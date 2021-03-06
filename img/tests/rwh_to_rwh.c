#include "../img.h"

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s input output\n", argv[0]);
        return 1;
    }

    img *_img = malloc(sizeof(img));
    if (! img_cons (_img))
        return 2;

    if (! img_load_rwh (_img, _, argv[1]))
        return 3;
    
    if (! img_save_rwh (_img, argv[2]))
        return 4;
    
    if (! img_decons (_img))
        return 5;

    free(_img);
    return 0;
}

