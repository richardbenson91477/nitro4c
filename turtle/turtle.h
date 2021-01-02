#ifndef NITRO_TURTLE_INCLUDED
#define NITRO_TURTLE_INCLUDED

#include "../nitro_defs.h"
#include "../win/win.h" 
#include "../colors/colors.h"

typedef struct {
    uint32_t bg_color;
    uint32_t pen_color;

    bool down_;
    bool show_;

    float x_init, y_init, a_init;
    float x, y, a;

    win_conf *_win_conf;
    win *_win;

    blaze *_stack;
} turtle;

extern bool turtle_cons (turtle *_t, int32_t w, int32_t h);
extern bool turtle_decons (turtle *_t);
extern void turtle_goto (turtle *_t, float x, float y);
extern void turtle_forward (turtle *_t, float n);
extern void turtle_back (turtle *_t, float n);
extern void turtle_push (turtle *_t);
extern void turtle_pop (turtle *_t);
extern void turtle_home (turtle *_t);
extern void turtle_left (turtle *_t, float a);
extern void turtle_right (turtle *_t, float a);
extern void turtle_reset (turtle *_t);
extern bool turtle_tick (turtle const *_t);
extern void turtle_render (turtle const *_t);
extern void turtle_clear (turtle const *_t);
extern void turtle_bg_color (turtle *_t, uint32_t c);
extern void turtle_pen_color (turtle *_t, uint32_t c);
extern void turtle_up (turtle *_t);
extern void turtle_down (turtle *_t);
extern void turtle_show (turtle *_t);
extern void turtle_hide (turtle *_t);

#endif

