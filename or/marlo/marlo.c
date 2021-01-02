#include "../or.h"
#include "../or_app.h"
#include "marlo_conf.h"
#include "brick.h"
#include "player.h"

int main (int argc, char **argv) {
    dlog_debug (_, "main: begin");
 
    marlo_conf *_marlo_conf = malloc(sizeof(marlo_conf));
    marlo_conf_cons (_marlo_conf);

    if (argc > 1) {
        if (! strcmp (argv[1], "full"))
            _marlo_conf->full_ = true;
    }

    or_app_conf *_app_conf = malloc(sizeof(or_app_conf));
    or_app_conf_cons (_app_conf);
 
    _app_conf->_win_conf->w = _marlo_conf->win_w;
    _app_conf->_win_conf->h = _marlo_conf->win_h;
    _app_conf->_win_conf->_title_s = _marlo_conf->_title_s;
    _app_conf->_win_conf->full_ = _marlo_conf->full_;

    or_app *_app = malloc(sizeof(or_app));
    if (! or_app_cons (_app, _app_conf)) {
        dlog_error (_, "main: or_app_cons");
        return 1;
    }

    player *_player = malloc(sizeof(player));
    if (! player_cons (_player, _marlo_conf)) {
        dlog_error (_, "main: player_cons");
        return 2;
    }
    _player->_ent->_parent_map = _app->_map;

    brick *_brick = malloc(sizeof(brick));
    if (! brick_cons (_brick, _marlo_conf)) {
        dlog_error (_, "main: brick_cons");
        return 3;
    }

    if (! or_menu_load (_app->_menu, "data/menu.rwh", "data/menu.raw")) {
        dlog_error (_, "main: or_menu load");
        return 4;
    }
    or_menu_toggle (_app->_menu);

    float step_t = 1.0 / _app_conf->fps;
    or_app_tick (_app);

    while (true) {
        if (_app->_menu->on_) {
            uint32_t res_u =
                or_menu_tick (_app->_menu, _app->_win);
            if (! res_u) {
                dlog_error (_, "main: or_menu_tick");
                break;
            }
            else if (res_u == 2) {
                or_menu_toggle (_app->_menu);

                if (! or_map_load (_app->_map, _marlo_conf->start_map_s)) {
                    dlog_error (_, "main: or_map_load");
                    break;
                }
 
                for (or_ent_type *_ent_type =
                        blaze_first (_app->_map->_ent_types); _ent_type;
                        _ent_type = blaze_next (_app->_map->_ent_types))
                    if (_ent_type->id == 1) {
                        _ent_type->_self = _brick;
                        _ent_type->ext_tick_fn = brick_tick;
                        _ent_type->ext_on_col_fn = brick_on_col;
                    }

                player_reset (_player);
            }
        }
        else {
            if (! player_tick (_player, _app->_win, step_t)) {
                dlog_error (_, "main: player_tick");
                break;
            }

            if (! or_map_tick (_app->_map, step_t)) {
                dlog_error (_, "main: or_map_tick");
                break;
            }
        }

        if (! or_app_tick (_app)) {
            dlog_error (_, "main: or_app_tick");
            break;
        }

        if (_app->_menu->on_) {
            if (! or_menu_render (_app->_menu, _app->_win)) {
                dlog_error (_, "main: or_menu_render");
                break;
            }
        }
        else {
            if (! or_map_render (_app->_map, _app->_win)) {
                dlog_error (_, "main: or_app_render");
                break;
            }

            if (! player_render (_player, _app->_win)) {
                dlog_error (_, "main: player_render");
                break;
            }
        }

        or_app_render (_app);
    }

    player_decons (_player);
    free(_player);

    brick_decons (_brick);
    free(_brick);

    or_app_decons (_app);
    free(_app);
    
    or_app_conf_decons (_app_conf);
    free(_app_conf);

    dlog_debug (_, "main: end");
    return 0;
} 

