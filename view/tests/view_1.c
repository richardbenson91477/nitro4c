#include "../view.h"
#include "../../win/win.h"
#include "../../colors/colors.h"

const uint32_t win_w = 640,
      win_h = 480;

float _tri_v [] = {
    0.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.5, 1.0, 0.0
};
float _tri_n [] = {
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0
};
float _tri_pos [] = { 0.0, 0.0, -1.0 };
float _tri_rot_m [4][4];
float _tri_rot_m_prev [4][4];

float _frame_rot_m [4][4];
float _frame_rot_v [3];

float _eye_v [] = { 0.0, 0.0, 4.0 };

int main (void) {
    win_conf *_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_win_conf))
        return 2;

    _win_conf->w = win_w;
    _win_conf->h = win_h;
    _win_conf->_title_s = "view_1";
    _win_conf->gl_ = true;

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

    _view->tex_ = false;
    if (! view_set (_view, win_w, win_h))
        return 5;

    mat_ident (_tri_rot_m);
    _frame_rot_v[0] = 0.0;
    _frame_rot_v[1] = 0.0;
    _frame_rot_v[2] = 0.1;

    mat_from_r (_frame_rot_m, _frame_rot_v);

    view_color (_view, COLOR_OrangeRed1);

    view_mv_reset (_view);
    view_mv_negtrans (_view, _eye_v);
    view_light_setpos (_view, _eye_v);

    view_mv_push (_view);
    view_mv_trans (_view, _tri_pos);

    float frame_t = 1.0 / win_run_fps;

    while (win_tick ()) {
        if (! view_tick (_view, frame_t))
            break;

        mat_copy (_tri_rot_m_prev, _tri_rot_m);
        mat_concat (_tri_rot_m, _tri_rot_m_prev, _frame_rot_m);

        view_render (_view);

        view_mv_push (_view);
        view_mv_rot_m (_view, _tri_rot_m);
        view_render_tris (_view, _tri_v, _tri_n, 3);
        view_mv_pop (_view);

        win_render (_win);
    }

    view_decons (_view);
    free(_view);

    win_deinit ();

    win_conf_decons (_win_conf);
    free(_win_conf);

    return 0;
}

