#ifndef MARLO_CONF_INCLUDED
#define MARLO_CONF_INCLUDED

#include "../or.h"

typedef struct {
    int32_t win_w, win_h;
    bool full_;
    float fps;
    char const *_title_s;

    char const *start_map_s;
    float ground_friction;
    float air_friction;
    float rho_clamp;
    uint32_t player_ent_type_id;
    uint32_t player_tries_n;
    float player_weight;
    float player_state_len;
    float run_max_walk;
    float run_max_run;
    float run_inc;
    float run_air_r;
    float jump_r_min;
    float jump_run_r;
    float brick_weight;
    float brick_jump_r;
    float brick_del_t;
} marlo_conf;
    
extern bool marlo_conf_cons (marlo_conf *_conf);

#endif

