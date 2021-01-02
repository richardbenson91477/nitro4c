#ifndef NITRO_OR_APP_INCLUDED
#define NITRO_OR_APP_INCLUDED

#include "or.h"
#include "or_menu.h"
#include "or_map.h"
#include <time.h>

typedef struct {
    win_conf *_win_conf;
    float fps;
} or_app_conf;

typedef struct {
    or_app_conf *_conf;
    or_menu *_menu;
    or_map *_map;
    win *_win;
} or_app;

extern bool or_app_conf_cons (or_app_conf *_app_conf);
extern bool or_app_conf_decons (or_app_conf *_app_conf);
extern bool or_app_cons (or_app *_a, or_app_conf *_app_conf);
extern bool or_app_decons (or_app *_a);
extern bool or_app_tick (or_app const *_a);
extern void or_app_render (or_app const *_a);

#endif

