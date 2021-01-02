#ifndef MARLO_BRICK_INCLUDED
#define MARLO_BRICK_INCLUDED

#include "../or.h"
#include "../or_map.h"
#include "marlo_conf.h"

typedef struct {
    snd *_snd_break;
    marlo_conf *_conf;
} brick;

bool brick_cons (brick *_b, marlo_conf *_conf);
bool brick_decons (brick *_b);
bool brick_tick (void *_self, void *_ent_v, float t);
void brick_on_col (void *_self, void *_ent_v, void *_ent_col_v);

#endif

