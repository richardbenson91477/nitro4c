#include "../file.h"

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s input-file data-name\n", argv[0]);
        return 1;
    }

    file *_f = malloc(sizeof(file));
    if (! file_cons (_f))
        return 2;

    if (! file_load (_f, argv[1]))
        return 3;

    printf("#include <stdint.h>\n");
    printf("const uint8_t %s [] = {\n", argv[2]);

    uint8_t *_b = _f->_b;
    uint32_t b_c = 0;
    for (; b_c < _f->b_n ;) {
        printf(" 0x%02X,", *_b++);
        b_c ++;
        if (! (b_c % 12)) 
            printf("\n");
    }

    if (b_c % 12) 
        printf("\n");

    printf("};\nconst uint32_t %s_size = %u;\n\n", argv[2], _f->b_n);
 
    file_decons (_f);
    free(_f);

    return 0;
}

