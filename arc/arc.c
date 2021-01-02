#include "arc.h"

bool arc_cons (arc *_arc) {
    dlog_debug (_, "arc_cons");

    _arc->_files = malloc(sizeof(blaze));
    if (! blaze_cons (_arc->_files))
        return false;

    return true;
}

bool arc_decons (arc *_arc) {
    dlog_debug (_, "arc_decons");

    arc_clear (_arc);
    if (! blaze_decons (_arc->_files))
        return false;

    free(_arc->_files);
    _arc->_files = _;
    return true;
}

bool arc_clear (arc *_arc) {
    dlog_debug (_, "arc_clear");
    return blaze_clear (_arc->_files, file_free);
}

bool arc_add (arc *_arc, file *_f) {
    dlog_debug (_, "arc_add");

    if (! _f) {
        dlog_error (_, "arc_add: missing file");
        return false;
    }

    return blaze_add (_arc->_files, _f);
}

bool arc_import_dir (arc *_arc, char const *_dir_s) {
    dlog_debug_s (_, "arc_import_dir", _dir_s);

    char _name_s [PATH_MAX];
    if (_dir_s)
        snprintf(_name_s, PATH_MAX, "%s/", _dir_s);
    else 
        snprintf(_name_s, PATH_MAX, "./");

    DIR *_dir = opendir(_name_s);
    if (! _dir) {
        dlog_error_s (_, "arc_import_dir: opendir", _name_s);
        return false;
    }

    struct dirent *_dirent = readdir(_dir);
    if (! _dirent) {
        dlog_error_s (_, "arc_import_dir: readdir", _name_s);
        closedir (_dir);
        return false;
    }

    while (_dirent) {
        if (_dir_s) 
            snprintf(_name_s, PATH_MAX, "%s/%s", _dir_s, _dirent->d_name);
        else 
            snprintf(_name_s, PATH_MAX, "%s", _dirent->d_name);
    
        struct stat st;
        if (stat(_name_s, &st)) {
            dlog_error_s (_, "arc_import_dir: stat", _name_s);
            return false;
        }

        if (! S_ISDIR(st.st_mode)) {
            file *_f = malloc(sizeof(file));
            if (! file_cons (_f))
                return false;

            if (! file_load (_f, _name_s)) {
                dlog_error_s (_, "arc_import_dir: file_load", _name_s);
                return false;
            }
            if (! arc_add (_arc, _f))
                return false;
        }
        else if (strcmp(_dirent->d_name, ".") &&
                strcmp(_dirent->d_name, ".."))
            if (! arc_import_dir (_arc, _name_s))
                return false;

        _dirent = readdir(_dir); 
    }

    closedir (_dir);
    return true;
}

bool arc_export_dir (arc const *_arc, char const *_dir_s) {
    dlog_debug_s (_, "arc_export_dir", _dir_s);

    if (! _dir_s) {
        dlog_error (_, "arc_export_dir: missing _dir_s");
        return false;
    }

    char _name_s [PATH_MAX];
    char _sub_dir_s [PATH_MAX - 1];

    for (file *_f = blaze_first (_arc->_files); _f;
            _f = blaze_next (_arc->_files)) {
        char *_m = strchr(_f->_name_s, '/');
        while (_m) {
            memcpy(_sub_dir_s, _f->_name_s, _m - _f->_name_s);
            _sub_dir_s[_m - _f->_name_s] = 0;
            snprintf(_name_s, PATH_MAX, "%s/%s", _dir_s, _sub_dir_s);
            mkdir (_name_s, 0755);
            _m = strchr(_m + 1, '/');
        }

        snprintf(_name_s, PATH_MAX, "%s/%s", _dir_s, _f->_name_s);
        if (! file_export (_f, _name_s))
            return false;
    }

    return true;
}

bool arc_report (arc const *_arc) {
    dlog_debug (_, "arc_report");

    for (file *_f = blaze_first (_arc->_files); _f;
            _f = blaze_next (_arc->_files))
        printf ("name: %s, size: %u, mode: %u\n",
                _f->_name_s, _f->b_n, _f->mode);

    return true;
}

file *arc_get (arc const *_arc, char const *_name_s) {
    dlog_debug_s (_, "arc_get_b", _name_s);

    if (! _name_s) {
        dlog_error (_, "arc_get: missing name");
        return _;
    }

    for (file *_f = blaze_first (_arc->_files); _f;
            _f = blaze_next (_arc->_files)) {
        if (! strcmp(_name_s, _f->_name_s)) {
            return _f;
        }
    }

    dlog_error_s (_, "arc_get", _name_s);
    return _;
}

bool arc_get_b (arc const *_arc, char const *_name_s, uint8_t **__b,
        uint32_t *_b_n) {
    dlog_debug_s (_, "arc_get_b", _name_s);

    if (! _name_s) {
        dlog_error (_, "arc_get_b: missing name");
        return false;
    }

    file *_f = arc_get (_arc, _name_s);
    if (! _f)
        return false; 

    *__b = _f->_b;
    *_b_n = _f->b_n;
    return true;
}

bool arc_or_file_get_b (arc const *_arc, char const *_name_s,
        uint8_t **__b, uint32_t *_b_n) {
    dlog_debug_s (_, "arc_or_file_get_b", _name_s);

    if (_arc) {
        if (! arc_get_b (_arc, _name_s, __b, _b_n)) {
            dlog_error_s (_, "arc_or_file_get_b: arc_get_b", _name_s);
            return false;
        }
    }
    else {
        file *_f = malloc(sizeof(file));
        if (! file_cons (_f))
            return false;

        if (! file_load (_f, _name_s)) {
            dlog_error_s (_, "arc_or_file_get_b: file_load", _name_s);
            return false;
        }
        *__b = _f->_b;
        *_b_n = _f->b_n;
        _f->free_b_ = false;

        if (! file_decons (_f))
            return false;
        free(_f);
    }

    return true;
}

bool arc_load_file (arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "arc_load_file", _name_s);

    file *_f = malloc(sizeof(file));
    if (! file_cons (_f))
        return false;

    if (! file_load (_f, _name_s)) {
        dlog_error_s (_, "arc_load_file: file_load", _name_s);
        return false;
    }

    if (! arc_load_ser (_arc, _f->_b, _f->b_n))
        return false;

    if (! file_decons (_f))
        return false;

    return true;
}

bool arc_save_file (arc const *_arc, char const *_name_s) {
    dlog_debug_s (_, "arc_save_file", _name_s);

    uint8_t *_b;
    uint32_t b_n;
    if (! arc_save_ser (_arc, &_b, &b_n))
        return false;

    file *_f = malloc(sizeof(file));
    if (! file_cons_from_b (_f, _b, b_n))
        return false;

    // free_b_ is safe because arc_save_ser malloced _b
    _f->free_b_ = 1;

    if (! file_export (_f, _name_s)) {
        file_decons (_f);
        return false;
    }

    if (! file_decons (_f))
        return false;

    return true;
}

bool arc_load_ser (arc *_arc, const uint8_t *_b, uint32_t b_n) {
    dlog_debug (_, "arc_load_ser");

    uint8_t *_m = (uint8_t *) _b;

    if (memcmp(_m, "N2ARCHIV", 8)) {
        dlog_error (_, "arc_load_ser: invalid header");
        return false;
    }

    _m += 8;

    uint32_t item_n = *(uint32_t *)_m;
    _m += 4;

    for (uint32_t c = 0; c < item_n; c ++) {
        uint32_t name_n = *(uint32_t *)_m;
        _m += 4;

        char *_name_s = malloc(name_n + 1);
        memcpy(_name_s, _m, name_n);
        _name_s[name_n] = 0;
        _m += name_n;

        uint32_t b_n = *(uint32_t *)_m;
        _m += 4;

        uint8_t *_b = malloc(b_n);
        if (! _b) {
            dlog_error_u (_, "arc_load_ser: malloc", b_n);
            return false;
        }
        memcpy(_b, _m, b_n);
        _m += b_n;

        file *_f = malloc(sizeof(file));
        if (! file_cons_from_b (_f, _b, b_n))
            return false;

        _f->mode = *(uint32_t *)_m;
        _m += 4;

        _f->_name_s = _name_s;
        _f->free_b_ = 1;

        if (! arc_add (_arc, _f))
            return false;
    }

    return true;
}

bool arc_save_ser (arc const *_arc, uint8_t **__b, uint32_t *_b_n) {
    dlog_debug (_, "arc_save_ser");

    // calculate total archive data size
    uint32_t len = 12;
    for (file *_f = blaze_first (_arc->_files); _f;
            _f = blaze_next (_arc->_files))
        len += 12 + strlen(_f->_name_s) + _f->b_n;

    *__b = (uint8_t *)malloc(len);
    if (! *__b) {
        dlog_error_u (_, "arc_save_ser: malloc", len);
        return false;
    }
    *_b_n = len;

    uint8_t *_m = *__b;

    memcpy(_m, "N2ARCHIV", 8);
    _m += 8;

    *(uint32_t *)_m = _arc->_files->item_n;
    _m += 4;

    for (file *_f = blaze_first (_arc->_files); _f;
            _f = blaze_next (_arc->_files)) {
        uint32_t name_n = strlen(_f->_name_s);
        *(uint32_t *)_m = name_n;
        _m += 4;
 
        memcpy(_m, _f->_name_s, name_n);
        _m += name_n;
 
        *(uint32_t *)_m = _f->b_n;
        _m += 4;
 
        memcpy(_m, _f->_b, _f->b_n);
        _m += _f->b_n;

        *(uint32_t *)_m = _f->mode;
        _m += 4;
    }

    return true;
}

