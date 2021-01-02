#include <stdint.h>
#include <stdio.h>

#include "data_1.h"

int main (void) {
    uint8_t *_b = (uint8_t *)data_1;
    for (uint32_t c = 0; c < data_1_size; c ++)
        putchar(*_b++);

    return 0;
}

