#include "../app.h"

app *_app = _;
map_ent *_ent_ball = _;
map_ent *_ent_cam = _;

void on_key (void *_v);
void on_click (void *_v);

int main (int argc, char **argv) {
    dlog_debug (_, "main begin");
 
    app_conf *_app_conf = malloc(sizeof(app_conf));
    app_conf_cons (_app_conf);
 
    _app_conf->_win_conf->w = 640;
    _app_conf->_win_conf->h = 400;
    _app_conf->_win_conf->_title_s = "app_1";
    _app_conf->_win_conf->_ext_key_fn = on_key;
    _app_conf->_win_conf->_ext_click_fn = on_click;

    app *_app = malloc(sizeof(app));
    if (! app_cons (_app, _app_conf)) {
        dlog_error (_, "main: app_cons");
        return 1;
    }

    view_fade (_app->_view, 1.0, 0.0, 2.0, 0.0, 0.0, 0.0);
 
    if (! map_load (_app->_map, _app->_view, _, "data/map_1.map"))
        return 2;

    _ent_ball = blaze_get (_app->_map->_ents, 1);
    _ent_ball->ph_grav = 9.8;
    _ent_ball->ph_air_fric = 1.0;
    _ent_ball->ph_bounciness = 0.8;
 
    _ent_cam = _app->_map->_ent_cam;
    _ent_cam->ph_grav = 9.8;

    while (app_tick (_app)) {
        app_render (_app);
    }
 
    app_decons (_app);
    free(_app);
 
    app_conf_decons (_app_conf);
    free (_app_conf);

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
}

void on_click (void *_v) {
    map_ent_reset_v_r (_ent_ball);
}

