#include <stdlib.h>
#include "../fb.h"

int main (int argc, char **argv) {
    fb fb1;
    if (! fb_cons (&fb1, _))
        return 1;

    uint32_t *_p, c;
    for (;;) {
        c = rand ();
        _p = fb1._p;
        for (int p_c = 0; p_c < (fb1.w * fb1.h); p_c ++)
           *_p++ = c;

        //sleep (1);
        break;
    }

    fb_decons (&fb1);
    return 0;
}

