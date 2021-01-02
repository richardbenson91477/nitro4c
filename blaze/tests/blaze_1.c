#include <stdio.h>
#include "../blaze.h"

typedef struct { 
    int x, y;
} p;
blaze ps;
  
void ps_free (void *v) {
    printf("in ps_free\n");
    free(v);
}

void ps_cons (void) {
    printf ("in ps_cons\n");
    blaze_cons (&ps);
}

void ps_add (int x, int y) {
    printf ("in ps_add(%d,%d)\n", x, y);

    p *_p = malloc(sizeof(p));
    if (! _p)
        return;

    _p->x = x;
    _p->y = y;
    blaze_add (&ps, _p);
}

void ps_print (int i) {
    printf ("in ps_print(%d)\n", i);

    p *_p = blaze_get (&ps, i);
    if (_p) {
        printf("ps_print: %d %d\n", _p->x, _p->y);
    }
    else {
        printf("ps_print: missing item %d\n", i);
    }
}

void ps_print_all (void) {
    printf ("in ps_print_all\n");

    for (p *_p = blaze_first (&ps); _p; _p = blaze_next (&ps)) {
       printf("ps_print_all: %d %d\n", _p->x, _p->y); 
    }
}

void ps_del (int i) {
    printf ("in ps_del(%d)\n", i);

    blaze_del (&ps, _, i, free);
}

void ps_clear (void) {
    printf ("in ps_clear\n");

    blaze_clear (&ps, ps_free);
    blaze_decons (&ps);
}

int main (void) {
    ps_cons ();

    ps_add (1, 2);
    ps_add (3, 4);

    ps_print (0);
    ps_print (1);

    ps_del (0);
    ps_print (0);

    ps_add (1, 2);
    ps_print_all ();

    ps_clear ();
    ps_print (0);

    ps_add (0, 1);
    ps_print (0);

    ps_clear ();
    return 0;
}

