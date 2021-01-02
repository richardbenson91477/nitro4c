#ifndef NITRO_OR_MENU_INCLUDED
#define NITRO_OR_MENU_INCLUDED

#include "or.h"
#include "or_bg.h"

typedef struct {
    bool on_;
    or_bg *_bg;
    snd *_snd_mus;
} or_menu;

extern bool or_menu_cons (or_menu *_menu);
extern bool or_menu_decons (or_menu *_menu);
extern bool or_menu_load (or_menu *_menu, char const *_bg_name_s,
        char const *_mus_name_s);
extern bool or_menu_toggle (or_menu *_menu);
extern uint32_t or_menu_tick (or_menu *_menu, win *_win);
extern bool or_menu_render (or_menu const *_menu, win *_win);

#endif

