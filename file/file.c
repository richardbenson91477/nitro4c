#include "file.h"

bool file_cons (file *_f) {
    dlog_debug (_, "file_cons");

    _f->_b = _;
    _f->b_n = 0;
    _f->_name_s = _;
    _f->mode = 0644;
    _f->free_b_ = true;

    return true;
}

bool file_cons_from_b (file *_f, uint8_t *_b, uint32_t b_n) {
    dlog_debug (_, "file_cons_from_b");

    if (! _b) {
        dlog_error (_, "file_cons_from_b: missing _b");
        return false;
    }

    _f->_b = _b;
    _f->b_n = b_n;
    _f->_name_s = _;
    _f->mode = 0644;
    _f->free_b_ = false;

    return true;
}

bool file_decons (file *_f) {
    dlog_debug (_, "file_decons");

    if (_f->_name_s)
        free(_f->_name_s);

    if (_f->free_b_ && _f->_b)
        free(_f->_b);

    return true;
}

void file_free (void *_v) {
    dlog_debug (_, "file_free");

    if (! _v) {
        dlog_error (_, "file_free: missing _v");
        return;
    }

    if (! file_decons ((file *)_v))
        return;

    free(_v);
}

bool file_load (file *_f, char const *_name_s) {
    dlog_debug_s (_, "file_load", _name_s);

    if (! _name_s) {
        dlog_error (_, "file_load: missing name");
        return false;
    }

    int f_in = open(_name_s, O_RDONLY);
    if (f_in < 0) {
        dlog_error_s (_, "file_load: open", _name_s);
        return false;
    }

    struct stat st;
    if (fstat(f_in, &st)) {
        dlog_error (_, "file_load: fstat");
        return false;
    }
    _f->mode = st.st_mode & 0777;
 
    _f->b_n = lseek(f_in, 0, SEEK_END);
    lseek(f_in, 0, SEEK_SET);

    _f->_b = (uint8_t *)malloc(_f->b_n);
    if (! _f->_b) {
        dlog_error_u (_, "file_load: malloc", _f->b_n);
        return false;
    }
    _f->free_b_ = true;

    if (read(f_in, _f->_b, _f->b_n) <= 0) {
        dlog_error_u (_, "file_load: read", _f->b_n);
        return false;
    }
    close(f_in);

    _f->_name_s = strdup(_name_s);
    return true;
}

bool file_save (file const *_f) {
    dlog_debug_s (_, "file_save", _f->_name_s);

    if (! _f->_name_s) {
        dlog_error (_, "file_save: missing filename");
        return false;
    }

    int f_out = open(_f->_name_s, O_RDWR | O_CREAT, _f->mode);
    if (f_out < 0) {
        dlog_error_s (_, "file_save: open", _f->_name_s);
        return false;
    }

    if (write(f_out, _f->_b, _f->b_n) <= 0) {
        dlog_error_u (_, "file_save: write", _f->b_n);
        return false;
    }

    close(f_out);
    return true;
}

bool file_save_as (file *_f, char const *_name_s) {
    dlog_debug_s (_, "file_save_as", _name_s);

    if (_f->_name_s)
        free(_f->_name_s);
    _f->_name_s = strdup(_name_s);
    return file_save (_f);
}

bool file_export (file const *_f, char const *_name_s) {
    dlog_debug_s (_, "file_export", _name_s);

    int f_out = open(_name_s, O_RDWR | O_CREAT, _f->mode);
    if (f_out < 0) {
        dlog_error_s (_, "file_export: open", _name_s);
        return false;
    }

    if (write(f_out, _f->_b, _f->b_n) <= 0) {
        dlog_error_u (_, "file_export: write", _f->b_n);
        return false;
    }

    close(f_out);
    return true;
}

