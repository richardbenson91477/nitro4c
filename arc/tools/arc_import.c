#include <stdio.h>
#include "../arc.h"

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s arcfile directory\n", argv[0]);
        return 1;
    }

    arc *_arc = malloc(sizeof(arc));
    arc_cons (_arc);

    if (! arc_import_dir (_arc, argv[2]))
        return 2;

    if (! arc_save_file (_arc, argv[1]))
        return 3;

    arc_decons (_arc);
    free(_arc);
    return 0;
}

