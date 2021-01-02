#include "or_app.h"

bool or_app_conf_cons (or_app_conf *_app_conf) {
    dlog_debug (_, "or_app_conf_cons");

    _app_conf->fps = 30.0f;

    _app_conf->_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_app_conf->_win_conf))
        return false;

    _app_conf->_win_conf->w = 640;
    _app_conf->_win_conf->h = 400;
    _app_conf->_win_conf->_title_s = _;
    _app_conf->_win_conf->full_ = false;

    return true;
}

bool or_app_conf_decons (or_app_conf *_app_conf) {
    dlog_debug (_, "or_app_conf_decons");

    free(_app_conf->_win_conf);
    return true;
}

bool or_app_cons (or_app *_a, or_app_conf *_app_conf) {
    dlog_debug (_, "or_app_cons");

    if (! _app_conf) {
        dlog_error (_, "or_app_cons: missing app_conf");
        return false;
    }
    _a->_conf = _app_conf;

    srand(time(_));

    _a->_menu = malloc(sizeof(or_menu));
    if (! or_menu_cons (_a->_menu))
        return false;

    win_run_fps = _a->_conf->fps;
    if (! win_init ())
        return false;
    
    _a->_win = malloc(sizeof(win));
    if (! win_cons (_a->_win, _a->_conf->_win_conf))
        return false;
 
    if (! snd_init ())
        return false;

    if (! win_open (_a->_win))
        return false;

    _a->_map = malloc(sizeof(or_map));
    if (! or_map_cons (_a->_map,
                _a->_conf->_win_conf->w, _a->_conf->_win_conf->h))
        return false;

    return true;
}

bool or_app_decons (or_app *_a) {
    dlog_debug (_, "or_app_decons");

    if (! or_map_decons (_a->_map))
        return false;
    free(_a->_map);
    _a->_map = _;

    if (! or_menu_decons (_a->_menu))
        return false;
    free(_a->_menu);
    _a->_menu = _;

    if (! win_decons (_a->_win))
        return false;
    free(_a->_win);
    _a->_win = _;

    if (! snd_deinit ()) {
        dlog_error (_, "or_app_decons: snd_deinit");
        return false;
    }

    if (! win_deinit ()) {
        dlog_error (_, "or_app_decons: win_deinit");
        return false;
    }

    return true;
}

bool or_app_tick (or_app const *_a) {
    return win_tick ();
}

void or_app_render (or_app const *_a) {
    win_render (_a->_win);
}

