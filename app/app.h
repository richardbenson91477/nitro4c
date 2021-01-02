#ifndef NITRO_APP_INCLUDED
#define NITRO_APP_INCLUDED

#include "../nitro_defs.h"
#include "../map/map.h"
#include "../win/win.h"
#include "../snd/snd.h"
#include "time.h"

typedef struct {
    win_conf *_win_conf;
    float fps;
} app_conf;

typedef struct {
    app_conf *_conf;
    //menu *_menu;
    map *_map;
    win *_win;
    view *_view;
} app;

extern bool app_conf_cons (app_conf *_app_conf);
extern bool app_conf_decons (app_conf *_app_conf);
extern bool app_cons (app *_a, app_conf *_app_conf);
extern bool app_decons (app *_a);
extern bool app_tick (app *_a);
extern void app_render (app *_a);

#endif

