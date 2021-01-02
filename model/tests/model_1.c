#include "../model.h"
#include "../../win/win.h"
#include "../../colors/colors.h"

const uint32_t win_w = 640,
         win_h = 480;

int main (void) {
    win_conf *_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_win_conf))
        return 2;

    _win_conf->w = win_w;
    _win_conf->h = win_h;
    _win_conf->_title_s = "model_1";
    _win_conf->gl_ = true;
    
//    win_run_fps = 60.0;
    if (! win_init ())
        return 1;

    win *_win = malloc(sizeof(win));
    if (! win_cons (_win, _win_conf))
        return 3;

    if (! win_open (_win))
        return 4;

    view *_view = malloc(sizeof(view));
    if (! view_cons (_view))
        return 5;

    if (! view_set (_view, win_w, win_h))
        return 6;
 
    model *_model = malloc(sizeof(model));
    if (! model_cons (_model))
        return 7;

    if (! model_load (_model, _view, _, "test1.model"))
        return 8;

    float _model_pos [] = { 0.0, 0.0, -4.0 };
    float _eye_v [] = { 0.0, 0.0, 4.0 };
    float _light_v [4] = { 0.0, 0.0, 0.0, 0.0};
    float _model_r [] = { 0.0, 0.0, 0.0 };
    
    float frame_t = 1.0 / win_run_fps;
    
    float zr = 0.0;
    while (win_tick ()) {
        if (! view_tick (_view, frame_t))
            break;

        zr += frame_t / 2.0;
        if (zr > PI2)
            zr -= PI2;

        view_render (_view);

        view_mv_reset (_view);

        _light_v[0] = _win->m_xf * 4.0;
        _light_v[1] = -_win->m_yf * 4.0;
        view_light_setpos (_view, _light_v);
        
        view_mv_negtrans (_view, _eye_v);
        view_mv_push (_view);
            view_mv_trans (_view, _model_pos);
            _model_r[0] = _win->m_yf * 1.8;
            _model_r[1] = _win->m_xf * 1.8;
            _model_r[2] = 0.0;
            view_mv_rot (_view, _model_r);
            _model_r[0] = 0.0;
            _model_r[1] = 0.0;
            _model_r[2] = zr;
            view_mv_rot (_view, _model_r);
            model_render (_model, _view);
        view_mv_pop (_view);

        win_render (_win);
    }
    
    model_decons (_model, _view);
    free(_model);

    view_decons (_view);
    free(_view);

    win_deinit ();

    win_conf_decons (_win_conf);
    free(_win_conf);

    return 0;
}

