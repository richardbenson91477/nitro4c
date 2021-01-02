#include <stdio.h>
#include "../arc.h"

int main (int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s arcfile\n", argv[0]);
        return 1;
    }

    arc *_arc = malloc(sizeof(arc));
    arc_cons (_arc);

    if (! arc_load_file (_arc, argv[1]))
        return 2;

    arc_report (_arc);

    arc_decons (_arc);
    free(_arc);
    return 0;
}

