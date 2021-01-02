#include "marlo_conf.h"

bool marlo_conf_cons (marlo_conf *_conf) {
    _conf->win_w = 640;
    _conf->win_h = 480;
    _conf->full_ = false;
    _conf->fps = 30.0f;
    _conf->_title_s = "marlo";

    _conf->start_map_s = "data/level1";
    _conf->ground_friction = 0.90f;
    _conf->air_friction = 0.95f;
    _conf->rho_clamp = 0.01f;
    _conf->player_ent_type_id = 33;
    _conf->player_state_len = 10.0f;
    _conf->player_tries_n = 10;
    _conf->player_weight = 45.0f;
    _conf->run_max_walk = 100.0f;
    _conf->run_max_run = 300.0f;
    _conf->run_inc = 20.0f;
    _conf->run_air_r = 0.5f;
    _conf->jump_r_min = 10.0f;
    _conf->jump_run_r = 0.015f;
    _conf->brick_weight = 50.0f;
    _conf->brick_jump_r = 6.0f;
    _conf->brick_del_t = 0.8f;
    return true;
}

