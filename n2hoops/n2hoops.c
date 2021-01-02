#include "../app/app.h"
#include "n2hoops_arc.h"

map_ent *_ent_ball = _;
map_ent *_ent_cam = _;
uint32_t mouse_mode = 0;
float m_xf_prev, m_yf_prev;

void on_key (void *_v);
void on_click (void *_v);
void on_declick (void *_v);

int main (int argc, char **argv) {
    dlog_debug (_, "main begin");

    arc *_arc = malloc(sizeof(arc));
    if (! arc_cons (_arc))
        return 1;
    if (! arc_load_ser (_arc, n2hoops_arc, n2hoops_arc_size))
        return 2;

    app_conf *_app_conf = malloc(sizeof(app_conf));
    app_conf_cons (_app_conf);
 
    _app_conf->_win_conf->w = 320;
    _app_conf->_win_conf->h = 480;
    _app_conf->_win_conf->_title_s = "n2 hoops";
    _app_conf->_win_conf->_ext_key_fn = on_key;
    _app_conf->_win_conf->_ext_click_fn = on_click;
    _app_conf->_win_conf->_ext_declick_fn = on_declick;

    app *_app = malloc(sizeof(app));
    if (! app_cons (_app, _app_conf)) {
        dlog_error (_, "main: app_cons");
        return 3;
    }

    view_fade (_app->_view, 1.0, 0.0, 2.0, 0.0, 0.0, 0.0);
 
    if (! map_load (_app->_map, _app->_view, _arc, "data/map_1.map"))
        return 4;

    _ent_ball = blaze_get (_app->_map->_ents, 1);
    _ent_ball->ph_grav = 9.8;
    _ent_ball->ph_air_fric = 1.0;
    _ent_ball->ph_bounciness = 0.8;
 
    _ent_cam = _app->_map->_ent_cam;
    _ent_cam->ph_grav = 0.0;

    while (app_tick (_app)) {
        if (v_len (_ent_ball->_v) > 8.0) {
            map_ent_reset_v_r (_ent_ball);
            v_clear (_ent_ball->_inertia_v);
        }

        app_render (_app);
    }
 
    app_decons (_app);
    free(_app);
 
    app_conf_decons (_app_conf);
    free (_app_conf);

    arc_decons (_arc);
    free (_arc);

    dlog_debug (_, "main end");
    return 0;
}

void on_key (void *_v) {
    win *_win = (win *)_v;
    SDL_Scancode const k = _win->keyc;

    if (k == SDL_SCANCODE_J)
        map_ent_rotate_3f (_ent_cam, 0.1, 0.0, 0.0);
    else if (k == SDL_SCANCODE_K)
        map_ent_rotate_3f (_ent_cam, -0.1, 0.0, 0.0);
    else if (k == SDL_SCANCODE_H)
        map_ent_rotate_3f (_ent_cam, 0.0, -0.1, 0.0);
    else if (k == SDL_SCANCODE_L)
        map_ent_rotate_3f (_ent_cam, 0.0, 0.1, 0.0);
    else if (k == SDL_SCANCODE_U)
        map_ent_rotate_3f (_ent_cam, 0.0, 0.0, 0.1);
    else if (k == SDL_SCANCODE_O)
        map_ent_rotate_3f (_ent_cam, 0.0, 0.0, -0.1);
    else if (k == SDL_SCANCODE_A)
        _ent_cam->_v[0] -= 0.1;
    else if (k == SDL_SCANCODE_D)
        _ent_cam->_v[0] += 0.1;
    else if (k == SDL_SCANCODE_Q)
        _ent_cam->_v[1] += 0.1;
    else if (k == SDL_SCANCODE_E)
        _ent_cam->_v[1] -= 0.1;
    else if (k == SDL_SCANCODE_W)
        _ent_cam->_v[2] -= 0.1;
    else if (k == SDL_SCANCODE_S)
        _ent_cam->_v[2] += 0.1;
    else if (k == SDL_SCANCODE_R)
        map_ent_reset_v_r (_ent_cam);
    else if (k == SDL_SCANCODE_1) {
        mouse_mode += 1;
        if (mouse_mode > 2) 
            mouse_mode = 0;
    }
}

void on_click (void *_v) {
    win *_win = (win *)_v;

    if (mouse_mode == 2) {
        _ent_ball->_v[0] = _win->m_xf * 10.0;
        _ent_ball->_v[1] = _win->m_yf * -10.0;
        v_clear (_ent_ball->_inertia_v);
    }
    else {
        m_xf_prev = _win->m_xf;
        m_yf_prev = _win->m_yf;
    }
}

void on_declick (void *_v) {
    win *_win = (win *)_v;

    if (mouse_mode == 0) {
        _ent_ball->_inertia_v[0] += 
            (_win->m_xf - m_xf_prev) * 0.4;
        _ent_ball->_inertia_v[1] -=
            (_win->m_yf - m_yf_prev) * 1.2;
        _ent_ball->_inertia_v[2] += 
            (_win->m_yf - m_yf_prev) * 0.4;
    }
    else if (mouse_mode == 1) {
        _ent_ball->_inertia_v [0] += 
            (_win->m_xf - m_xf_prev) * 0.4;
        _ent_ball->_inertia_v [1] -= 
            (_win->m_yf - m_yf_prev) * 1.2;
    }
}

