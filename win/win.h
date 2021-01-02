#ifndef NITRO_WIN_INCLUDED
#define NITRO_WIN_INCLUDED

#include <SDL2/SDL.h>
#ifdef NITRO_GLES
  #include <SDL2/SDL_opengles.h>
#else
  #include <SDL2/SDL_opengl.h>
  #define glFrustumf glFrustum
  #define glOrthof glOrtho
#endif

#include "../nitro_defs.h"
#include "../img/img.h"
#include "../draw/draw.h"

typedef struct {
    int32_t w, h;
    char const *_title_s;
    bool full_;
    bool gl_;

    void (* _ext_key_fn)(void *_v);
    void (* _ext_click_fn)(void *_v);
    void (* _ext_declick_fn)(void *_v);
    void (* _ext_move_fn)(void *_v);
    void (* _ext_close_fn)(void *_v);
} win_conf;

typedef struct {
    win_conf *_conf;

    SDL_Window *_sdl_win;
    SDL_GLContext sdl_gl;
    uint32_t *_p;
    draw *_draw;

    uint32_t sdl_id;
    uint32_t user_id;
    blaze *_user_data;

    SDL_Scancode keyc;
    bool keys [SDL_NUM_SCANCODES];
    int32_t m_x, m_y;
    float m_xf, m_yf;
    bool m_b0_, m_b1_, m_b2_;
 
    bool free_;
} win;

extern float win_run_fps;
extern float win_run_frame_t;

extern bool win_conf_cons (win_conf *_conf);
extern bool win_conf_decons (win_conf *_conf);

extern bool win_cons (win *_win, win_conf *_conf);
extern bool win_decons (win *_win);
extern void win_free (void *_v);
extern bool win_open (win *_win);
extern bool win_icon_load (win *_win, arc *_arc, char const *_file_s);
extern void win_render (win const *_win);
extern bool win_close (win *_win);
extern bool win_init (void);
extern bool win_deinit (void);
extern bool win_tick (void);
extern void win_render_all (void);
extern void win_close_all (void);
#endif

