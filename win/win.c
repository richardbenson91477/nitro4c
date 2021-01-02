#include "win.h"

blaze *_wins = _;

float win_run_fps = 30.0;
float win_run_frame_t;
uint32_t win_run_t_begin, win_run_t_end, win_run_frame_t_ms;

bool win_conf_cons (win_conf *_conf) {
    dlog_debug (_, "win_conf_cons");

    _conf->w = 320;
    _conf->h = 240;
    _conf->_title_s = "n2 win";
    _conf->full_ = false;
    _conf->gl_ = false;

    _conf->_ext_key_fn = (void (*)(void *)) _;
    _conf->_ext_click_fn = (void (*)(void *)) _;
    _conf->_ext_declick_fn = (void (*)(void *)) _;
    _conf->_ext_move_fn = (void (*)(void *)) _;
    _conf->_ext_close_fn = (void (*)(void *)) _;
    return true;
}

bool win_conf_decons (win_conf *_conf) {
    dlog_debug (_, "win_conf_decons");
    return true;
}

bool win_cons (win *_win, win_conf *_conf) {
    dlog_debug (_, "win_cons");

    if (! _conf) {
        dlog_error (_, "win_cons: missing _conf");
        return false;
    }
    _win->_conf = _conf;

    _win->_sdl_win = _;
    _win->sdl_gl = _;
    _win->_p = _;

    _win->keyc = SDL_SCANCODE_UNKNOWN;
    memset (_win->keys, 0, sizeof(_win->keys));
    _win->m_x = _win->m_y = 0;
    _win->m_xf = _win->m_yf = 0.0;
    _win->m_b0_ = _win->m_b1_ = _win->m_b2_ = false;

    _win->free_ = false;
    
    _win->_user_data = malloc(sizeof(blaze));
    if (! blaze_cons (_win->_user_data))
        return false;
 
    _win->_draw = malloc(sizeof(draw));

    if (! blaze_add (_wins, _win)) {
        dlog_error (_, "win_add: blaze_add");
        return false;
    }

    return true;
}

bool win_decons (win *_win) {
    dlog_debug (_, "win_decons");

    if (_win->_sdl_win)
        win_close (_win);

    blaze_clear (_win->_user_data, _);
    free(_win->_user_data);
    _win->_user_data = _;

    if (! blaze_del (_wins, _win, 0, win_free)) {
        dlog_error (_, "win_decons: blaze_del");
        return false;
    }

    return true;
}

void win_free (void *_v) {
    dlog_debug (_, "win_free");
    if (! _v) {
        dlog_error (_, "win_free: missing _v");
        return;
    }
 
    win *_win = (win *)_v;
    if (_win->free_)
        free(_v);
}

bool win_open (win *_win) {
    dlog_debug (_, "win_open");

    _win->_sdl_win = SDL_CreateWindow (_win->_conf->_title_s,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            _win->_conf->w, _win->_conf->h,
            _win->_conf->full_ ?
                    SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE |\
                _win->_conf->gl_ ? SDL_WINDOW_OPENGL : 0);

    if (! _win->_sdl_win) {
        dlog_error (_, "win_open: SDL_CreateWindow");
        return false;
    }

    _win->sdl_id = SDL_GetWindowID (_win->_sdl_win);

    if (_win->_conf->gl_) {
        _win->sdl_gl = SDL_GL_CreateContext (_win->_sdl_win);
        if (! _win->sdl_gl) {
            dlog_error (_, "win_open: SDL_GL_CreateContext");
            return false;
        }
        SDL_GL_MakeCurrent (_win->_sdl_win, _win->sdl_gl);
    }
    else {
        SDL_Surface *_sdl_winsurf = SDL_GetWindowSurface (_win->_sdl_win);
        _win->_p = (uint32_t *)_sdl_winsurf->pixels;

        draw_cons_from (_win->_draw, _win->_p, _win->_conf->w, _win->_conf->h);
    }

    return true;
}

bool win_icon_load (win *_win, arc *_arc, char const *_file_s) {
    dlog_debug_s (_, "win_icon_load", _file_s);

    if (! _win->_sdl_win) {
        dlog_error (_, "win_icon_load: missing _sdl_win");
        return false;
    }

    img *_img = malloc(sizeof(img));
    if (! img_cons (_img))
        return false;

    if (! img_load_rwh (_img, _arc, _file_s))
        return false;

    SDL_Surface *_surf = SDL_CreateRGBSurfaceFrom (_img->_b,
            _img->w, _img->h, 32, 4 * _img->w,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    if (! _surf) {
        dlog_error (_, "win_icon_load: surface create failed");
        return false;
    }

    SDL_SetWindowIcon (_win->_sdl_win, _surf);
    SDL_FreeSurface (_surf);

    if (! img_decons (_img))
        return false;
    free (_img);

    return true;
}

void win_render (win const *_win) {
    if (! _win->_sdl_win)
        return;

    if (_win->_conf->gl_) {
        glFlush ();
        SDL_GL_SwapWindow (_win->_sdl_win);
    }
    else {
        SDL_UpdateWindowSurface (_win->_sdl_win);
    }
}

bool win_close (win *_win) {
    dlog_debug (_, "win_close");

    if (!_win->_sdl_win) {
        dlog_error (_, "win_close: missing _sdl_win");
        return false;
    }

    if (_win->_conf->gl_) {
        if (_win->sdl_gl) {
            SDL_GL_DeleteContext (_win->sdl_gl);
            _win->sdl_gl = _;
        }
        else {
            dlog_error (_, "win_close: missing sdl_gl");
        }
    }
    else {
        if (! draw_decons (_win->_draw))
            return false;
    }

    SDL_DestroyWindow (_win->_sdl_win);
    _win->_sdl_win = _;
    return true;
}

bool win_init (void) {
    dlog_debug (_, "win_init");
    
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        dlog_error (_, "win_init: SDL_Init");
        return false;
    }
 
    if (win_run_fps > 0.0) {
        win_run_frame_t = 1.0 / win_run_fps;
        win_run_frame_t_ms = (uint32_t)(win_run_frame_t * 1000.0);
        win_run_t_begin = win_run_t_end = SDL_GetTicks ();
    } 
    
    SDL_GL_SetSwapInterval (0);
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
    #ifdef NITRO_GLES
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_EGL, 1);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 1);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1);
    #endif

    _wins = malloc(sizeof(blaze));
    if (! blaze_cons (_wins))
        return false;

    return true;
}

bool win_deinit (void) {
    dlog_debug (_, "win_deinit");

    for (win *_win = blaze_first (_wins); _win;
              _win = blaze_first (_wins))
        if (! win_decons (_win))
            return false;

    free(_wins);
    _wins = _;

    SDL_Quit ();
    return true;
}

bool win_tick (void) {
    static win *_win = _;

    SDL_Event event;
    while (SDL_PollEvent (&event) != 0) {
        if (event.type == SDL_WINDOWEVENT) {
            if ((event.window.event == SDL_WINDOWEVENT_HIDDEN) ||
                (event.window.event == SDL_WINDOWEVENT_MOVED) ||
                (event.window.event == SDL_WINDOWEVENT_RESIZED) ||
                (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) ||
                (event.window.event == SDL_WINDOWEVENT_MINIMIZED) ||
                (event.window.event == SDL_WINDOWEVENT_MAXIMIZED) ||
                (event.window.event == SDL_WINDOWEVENT_RESTORED) ||
                (event.window.event == SDL_WINDOWEVENT_LEAVE) ||
                (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST))
                    continue;

            _win = _;
            for (win *_win_i = blaze_first (_wins); _win_i;
                    _win_i = blaze_next (_wins))
                if (_win_i->sdl_id == event.window.windowID) {
                    _win = _win_i;
                    break;
                }
            if (! _win) {
                dlog_error (_, "win_tick: couldn't locate window");
                return false;
            }

            if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                if (_win->_conf->_ext_close_fn)
                    _win->_conf->_ext_close_fn ((void *)_win);
            }

            continue;
        }
        else if (! _win) {
            dlog_debug (_, "win_tick: no window set");
            _win = blaze_first (_wins);
            if (! _win) {
                dlog_error (_, "win_tick: no windows");
                return false;
            }
        }

        if (event.type == SDL_QUIT) {
            return false;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                return false;

            _win->keyc = event.key.keysym.scancode;
            _win->keys[_win->keyc] = 1;

            if (_win->_conf->_ext_key_fn)
                _win->_conf->_ext_key_fn ((void *)_win);
        }
        else if (event.type == SDL_KEYUP) {
            _win->keyc = SDL_SCANCODE_UNKNOWN;
            _win->keys[event.key.keysym.scancode] = 0;
        } 
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT)
                _win->m_b0_ = true;
            else if (event.button.button == SDL_BUTTON_MIDDLE)
                _win->m_b2_ = true;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                _win->m_b1_ = true;
            _win->m_x = event.button.x;
            _win->m_y = event.button.y;
            _win->m_xf = ((float)_win->m_x / (float)_win->_conf->w) - 0.5;
            _win->m_yf = ((float)_win->m_y / (float)_win->_conf->h) - 0.5;
 
            if (_win->_conf->_ext_click_fn)
                _win->_conf->_ext_click_fn ((void *)_win);
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            _win->m_x = event.button.x;
            _win->m_y = event.button.y;
            _win->m_xf = ((float)_win->m_x / (float)_win->_conf->w) - 0.5;
            _win->m_yf = ((float)_win->m_y / (float)_win->_conf->h) - 0.5;

            if (_win->_conf->_ext_declick_fn)
                _win->_conf->_ext_declick_fn ((void *)_win);

            if (event.button.button == SDL_BUTTON_LEFT)
                _win->m_b0_ = false;
            else if (event.button.button == SDL_BUTTON_MIDDLE)
                _win->m_b2_ = false;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                _win->m_b1_ = false;
        }
        else if (event.type == SDL_MOUSEMOTION) {
            _win->m_x = event.motion.x;
            _win->m_y = event.motion.y;
            _win->m_xf = ((float)_win->m_x / (float)_win->_conf->w) - 0.5;
            _win->m_yf = ((float)_win->m_y / (float)_win->_conf->h) - 0.5;

            if (_win->_conf->_ext_move_fn)
                _win->_conf->_ext_move_fn ((void *)_win);
        }
    }

    if (win_run_fps > 0.0) {
        win_run_t_end = SDL_GetTicks ();
        int32_t t_i = win_run_t_begin - win_run_t_end + win_run_frame_t_ms;
        if (t_i > 0)
            SDL_Delay (t_i);
        win_run_t_begin = SDL_GetTicks ();
    }

    return true;
}

void win_render_all (void) {
    for (win *_win = blaze_first (_wins); _win; _win = blaze_next (_wins))
        win_render (_win);
}

void win_close_all (void) {
    dlog_debug (_, "win_close_all");

    for (win *_win = blaze_first (_wins); _win; _win = blaze_next (_wins))
        if (_win->_sdl_win)
            win_close (_win);
}

