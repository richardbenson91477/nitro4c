#ifndef NITRO_VIEW_INCLUDED
#define NITRO_VIEW_INCLUDED

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
#include "../maths/maths.h"

typedef struct {
    char *_name_s;
    uint32_t id;
    uint32_t ref_n;
} view_tex;

typedef struct {
    void (* _ext_cam_tick_fn)(void *, float);
    void (* _ext_cam_apply_fn)(void *, void *);
    void (* _ext_cam_pre_clip_fn)(void *, float *);
    void *_ext_cam_this;

    blaze *_view_texs;

    bool light_;
    bool mtrl_;
    bool tex_;
    bool depth_;

    // viewport and plane normals
    float port_l, port_r, port_t, port_b, port_zn, port_zf;
    float __port_n [4][3];
 
    float _light_pos [4], _light_dif [4], _light_amb [4], _light_spec [4];
    float _light_spot_dir [4], light_spot_exp, light_spot_cutoff;
    float light_atten_c, light_atten_l, light_atten_q;

    float _mtrl_dif [3], _mtrl_amb [3], _mtrl_shin [3];

    int32_t fade_dir;
    float fade_a, fade_a2, fade_r;
    float fade_cr, fade_cg, fade_cb;
} view;

extern bool view_cons (view *_view);
extern bool view_decons (view *_view);
extern bool view_set (view *_view, int32_t w, int32_t h);
extern bool view_tick (view *_view, float t);
extern void view_render (view *_view);
extern void view_port (view *_view, int32_t w, int32_t h);
extern bool view_clip (view *_view, float const *_v, float *_d_out);
extern void view_perspective (view *_view);
extern void view_ortho (view *_view, bool t_, float w_f, float h_f);
extern void view_ortho_norm (view *_view, bool t_);
extern void view_light_defaults (view *_view);
extern void view_light_apply (view *_view);
extern void view_light_setpos (view *_view, float const *_p);
extern void view_mtrl_defaults (view *_view);
extern void view_mtrl_apply (view *_view);
extern void view_color (view *_view, uint32_t c);
extern void view_blend (view *_view, uint32_t type);
extern void view_mv_push (view *_view);
extern void view_mv_pop (view *_view);
extern void view_mv_reset (view *_view);
extern void view_mv_trans (view *_view, float const *_v);
extern void view_mv_negtrans (view *_view, float const *_v);
extern void view_mv_rot (view *_view, float const *_r);
extern void view_mv_rot_m (view *_view, float const (*__m)[4]);
extern void view_fade (view *_view, float a1, float a2, float r,
        float cr, float cg, float cb);
extern void view_fade_tick (view *_view, float t);
extern void view_fade_render (view *_view);
extern void view_render_tris (view *_view, float const *_v, float const *_n,
        uint32_t nv);
extern void view_render_tris_tex (view *_view, float const *_v, float const *_n,
        float const *_t, uint32_t nv);
extern void view_tex_bind (view *_view, view_tex *_view_tex);
extern view_tex *view_tex_load (view *_view, arc *_arc, char const *_name_s);
extern void view_tex_unref (view *_view, view_tex *_view_tex);
extern void view_tex_free (void *_v);

#endif

