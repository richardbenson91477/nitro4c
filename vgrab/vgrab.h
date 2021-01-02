#ifndef NITRO_VGRAB_INCLUDED
#define NITRO_VGRAB_INCLUDED

#include <errno.h>
#include <libv4l2.h>
#include <linux/videodev2.h>

#include "../nitro_defs.h"
#include "../img/img.h"

typedef struct {
    int32_t w;
    int32_t h;
    uint8_t *_b;
    uint32_t b_n;

    int32_t dev;
} vg;

static inline bool vg_ioctl (int f, int req, void *arg) {
    int res;
    do {
        res = v4l2_ioctl (f, req, arg);
    } while ((res == -1) &&\
            ((errno == EINTR) || (errno == EAGAIN)));
    return (res != -1);
}

extern bool vg_open (vg *_vg, char const *_dev_s, uint32_t w, uint32_t h);
extern bool vg_grab (vg *_vg);
extern void vg_close (vg *_vg);

#endif

