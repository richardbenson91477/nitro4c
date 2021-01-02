#ifndef MARLO_PLAYER_INCLUDED
#define MARLO_PLAYER_INCLUDED

#include "../or.h"
#include "../or_map.h"
#include "marlo_conf.h"

typedef struct {
    float run_x, run_max;
    uint32_t tries_c;
    or_ent *_ent;
    snd *_snd_jump, *_snd_fail;
    marlo_conf *_conf;
} player;

extern bool player_cons (player *_p, marlo_conf *_conf);
extern bool player_decons (player *_p);
extern void player_reset (player *_p);
extern void player_calc_view (player *_p);
extern bool player_render (player const *_p, win *_win);
extern bool player_tick (player *_p, win *_win, float t);

#endif

