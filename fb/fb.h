#ifndef NITRO_FB_INCLUDED
#define NITRO_FB_INCLUDED

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include "../nitro_defs.h"
#include "../dlog/dlog.h"

typedef struct {
    uint32_t *_p;
    uint32_t n;
    int32_t w, h;
} fb;

extern bool fb_cons (fb *_fb, char const *fb_s);
extern bool fb_decons (fb *_fb);

#endif

