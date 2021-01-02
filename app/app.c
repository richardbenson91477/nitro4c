#include "app.h"

bool app_conf_cons (app_conf *_app_conf) {
    dlog_debug (_, "app_conf_cons");

    _app_conf->fps = 30.0f;

    _app_conf->_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_app_conf->_win_conf))
        return false;

    _app_conf->_win_conf->gl_ = true;

    return true;
}

bool app_conf_decons (app_conf *_app_conf) {
    dlog_debug (_, "app_conf_decons");

    if (! win_conf_decons (_app_conf->_win_conf))
        return false;

    free(_app_conf->_win_conf);
    return true;
}

bool app_cons (app *_a, app_conf *_app_conf) {
    dlog_debug (_, "app_cons");

    if (! _app_conf) {
        dlog_error (_, "app_cons: missing app_conf");
        return false;
    }
    _a->_conf = _app_conf;

    srand(time(_));
 
    win_run_fps = _a->_conf->fps;
    if (! win_init ())
        return false;
    
    if (! snd_init ())
        return false;

    _a->_win = malloc(sizeof(win));
    if (! win_cons (_a->_win, _a->_conf->_win_conf))
        return false;
 
    if (! win_open (_a->_win))
        return false;

    _a->_view = malloc(sizeof(view));
    if (! view_cons (_a->_view))
        return false;
    
    if (! view_set (_a->_view, _a->_conf->_win_conf->w, _a->_conf->_win_conf->h))
        return false;

    _a->_map = malloc(sizeof(map));
    if (! map_cons (_a->_map))
        return false;
 
    return true;
}

bool app_decons (app *_a) {
    dlog_debug (_, "app_decons");
    
    if (! map_decons (_a->_map))
        return false;
    free(_a->_map);
    _a->_map = _;
 
    if (! view_decons (_a->_view))
        return false;
    free(_a->_view);
    _a->_view = _;
 
    if (! snd_deinit ()) {
        dlog_error (_, "app_decons: snd_deinit");
        return false;
    }

    if (! win_decons (_a->_win))
        return false;
    free(_a->_win);
    _a->_win = _;

    win_deinit ();
    return true;
}

bool app_tick (app *_a) {
    if (! map_tick (_a->_map, win_run_frame_t))
        return false;

    if (! view_tick (_a->_view, win_run_frame_t))
        return false;
 
    if (! win_tick ())
        return false;

    return true;
}

void app_render (app *_a) {
    view_render (_a->_view);
    map_render (_a->_map, _a->_view);
    view_fade_render (_a->_view);
    win_render (_a->_win);
}

