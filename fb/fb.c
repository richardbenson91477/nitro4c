#include "fb.h"

bool fb_cons (fb *_fb, char const *fb_s) {
    dlog_debug (_, "fb_cons");

    int fb_fd = open(fb_s ? fb_s : "/dev/fb0", O_RDWR);
    if (fb_fd == -1) {
        dlog_error_s (_, "fb_cons: open", fb_s ? fb_s : "/dev/fb0");
        return false;
    }

    struct fb_var_screeninfo fb_vsi;
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &fb_vsi) == -1) {
        dlog_error (_, "fb_cons: ioctl FBIOGET_VSCREENINFO");
        return false;
    }

    if (fb_vsi.bits_per_pixel != 32) {
        dlog_error_u (_, "fb_cons: bits per pixel is not 32",
            fb_vsi.bits_per_pixel);
        return false;
    }

    _fb->w = fb_vsi.xres;
    _fb->h = fb_vsi.yres;
    _fb->n = _fb->w * _fb->h * 4;

    _fb->_p = (uint32_t *)mmap(0, _fb->n, PROT_READ | PROT_WRITE, MAP_SHARED,
            fb_fd, 0);

    if (_fb->_p == MAP_FAILED) {
        dlog_error (_, "fb_cons: mmap");
        return false;
    }
    
    close(fb_fd);
    return true;
}

bool fb_decons (fb *_fb) {
    munmap(_fb->_p, _fb->n);
    return true;
}

