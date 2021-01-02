#include "vgrab.h"

bool vg_open (vg *_vg, char const *_dev_s, uint32_t w, uint32_t h) {
    dlog_debug (_, "vg_open");

    _vg->dev = v4l2_open (_dev_s, O_RDWR | O_NONBLOCK, 0);
    if (_vg->dev < 0) {
        dlog_error_s (_, "vg_open: open", _dev_s);
        return false;
    }

    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = w;
    fmt.fmt.pix.height = h;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if (! vg_ioctl (_vg->dev, VIDIOC_S_FMT, &fmt)) {
        dlog_error (_, "vg_open: vg_ioctl VIDIOC_S_FMT");
        return false;
    }

    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) {
        dlog_error (_, "vg_open: pixelformat is not V4L2_PIX_FMT_RGB24");
        return false;
    }
    else if ((fmt.fmt.pix.width != w) || (fmt.fmt.pix.height != h)) {
        dlog_error_u (_, "vg_open: unrequested x resolution",
            fmt.fmt.pix.width);
        dlog_error_u (_, "vg_open: unrequested y resolution",
            fmt.fmt.pix.height);
        
        return false;
    }

    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (! vg_ioctl (_vg->dev, VIDIOC_REQBUFS, &req)) {
        dlog_error (_, "vg_open: vg_ioctl VIDIOC_REQBUFS");
        return false;
    }

    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (! vg_ioctl (_vg->dev, VIDIOC_QUERYBUF, &buf)) {
        dlog_error (_, "vg_open: vg_ioctl VIDIOC_QUERYBUF");
        return false;
    }

    _vg->_b = (unsigned char *)v4l2_mmap (_, buf.length,
        PROT_READ | PROT_WRITE, MAP_SHARED,
        _vg->dev, buf.m.offset);
    if (_vg->_b == MAP_FAILED) {
        dlog_error_u (_, "vg_open: mmap", buf.length);
        return false;
    }
    
    _vg->b_n = buf.length;
    _vg->w = w;
    _vg->h = h;
    return true;
}

bool vg_grab (vg *_vg) {
    dlog_debug (_, "vg_grab");

    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (! vg_ioctl (_vg->dev, VIDIOC_QBUF, &buf)) {
        dlog_error (_, "vg_grab: vg_ioctl VIDIOC_QBUF");
        return false;
    }

    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (! vg_ioctl (_vg->dev, VIDIOC_STREAMON, &type)) {
        dlog_error (_, "vg_grab: vg_ioctl VIDIOC_STREAMMON");
        return false;
    }

    fd_set fds;
    struct timeval tv;
    int res;
    do {
        FD_ZERO (&fds);
        FD_SET (_vg->dev, &fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        res = select (_vg->dev + 1, &fds, _, _, &tv);
    } while ((res == -1) &&\
            (errno == EINTR));
    if (res == -1) {
        dlog_error (_, "vg_grab: select");
        return false;
    }

    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (! vg_ioctl (_vg->dev, VIDIOC_DQBUF, &buf)) {
        dlog_error (_, "vg_grab: vg_ioctl");
        return false;
    }
    if (! vg_ioctl (_vg->dev, VIDIOC_QBUF, &buf)) {
        dlog_error (_, "vg_grab: vg_ioctl VIDIOC_QBUF");
        return false;
    }
    
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (! vg_ioctl (_vg->dev, VIDIOC_STREAMOFF, &type)) {
        dlog_error (_, "vg_grab: vg_ioctl VIDIOC_STREAMOFF");
        return false;
    }

    return true;
}

void vg_close (vg *_vg) {
    dlog_debug (_, "vg_close");

    v4l2_munmap (_vg->_b, _vg->b_n);
    v4l2_close (_vg->dev);
}

