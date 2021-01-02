#include "or_menu.h"

bool or_menu_cons (or_menu *_menu) {
    dlog_debug (_, "or_menu_cons");

    _menu->on_ = false;
    
    _menu->_bg = malloc(sizeof(or_bg));
    if (! or_bg_cons (_menu->_bg))
        return false;
    
    _menu->_snd_mus = malloc(sizeof(snd));
    if (! snd_cons (_menu->_snd_mus))
        return false;

    return true;
}

bool or_menu_decons (or_menu *_menu) {
    dlog_debug (_, "or_menu_decons");

    _menu->on_ = false;
    
    if (_menu->_bg) {
        if (! or_bg_decons (_menu->_bg))
            return false;

        free(_menu->_bg);
        _menu->_bg = _;
    }

    if (_menu->_snd_mus) {
        if (! snd_decons (_menu->_snd_mus))
            return false;

        free(_menu->_snd_mus);
        _menu->_snd_mus = _;
    }

    return true;
}

bool or_menu_load (or_menu *_menu, char const *_bg_name_s,
        char const *_mus_name_s) {
    dlog_debug_s (_, "or_menu_load", _bg_name_s);

    if (! or_bg_load (_menu->_bg, _bg_name_s))
        return false;

    if (! snd_load_raw (_menu->_snd_mus, _, _mus_name_s))
        return false;

    return true;
}

bool or_menu_toggle (or_menu *_menu) {
    dlog_debug (_, "or_menu_toggle");

    if (! _menu->on_) {
        _menu->on_ = true;
        if (! snd_play (_menu->_snd_mus, -1))
            return false;
    }
    else {
        _menu->on_ = false;
        if (! snd_stop (_menu->_snd_mus))
            return false;
    }

    return true;
}

uint32_t or_menu_tick (or_menu *_menu, win *_win) {
    // check input
    if (_win->keys[SDL_SCANCODE_RETURN]) {
        _win->keys[SDL_SCANCODE_RETURN] = false;
        return 2;
    }
    else if (_win->keys[SDL_SCANCODE_ESCAPE]) {
        return 0;
    }

    return 1;
}

bool or_menu_render (or_menu const *_menu, win *_win) {
    if (! _menu->on_)
        return true;

    if (! or_bg_render (_menu->_bg, _win, 0, 0))
        return false;

    return true;
}

