#include "../vgrab.h"

const int w = 640,
    h = 480;

int main (void) {
    vg vg0;
    if (! vg_open (&vg0, "/dev/video0", w, h))
        return 1;

    if (! vg_grab (&vg0))
        return 2;

    img img1;
    img_from_rgb24 (&img1, vg0.w, vg0.h, vg0._b);
    if (! img_save_rwh (&img1, "out1.rwh"))
        return 3;
    img_decons (&img1);

    dlog_debug (_, "sleeping 1s...");
    sleep(1);

    if (! vg_grab (&vg0))
        return 4;

    img img2;
    img_from_rgb24 (&img2, vg0.w, vg0.h, vg0._b);
    if (! img_save_rwh (&img2, "out2.rwh"))
        return 5;
    img_decons (&img2);

    vg_close (&vg0);
    return 0;
}

