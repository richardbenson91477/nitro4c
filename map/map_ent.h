#ifndef NITRO_ENT_INCLUDED
#define NITRO_ENT_INCLUDED
    
typedef struct {
    blaze *_parent_ents;

    sys *_sys;
    model *_model;

    float _v [3], __r_mat [4][4];
    float _init_v [3], _init_r [3];
    float _inertia_v [3];

    bool path_ignore_;
    comp *_path_comp;
    float path_t, path_len;
    uint32_t path_cur;
 
    bool lookat_ignore_;
    float _lookat_v [3];

    bool ph_ignore_;
    bool ph_poly_col_;
    float ph_rad, ph_grav, ph_air_fric, ph_bounciness;

    bool render_ignore_;
    bool cam_;
    uint32_t del_;
} map_ent;

extern bool map_ent_cons (map_ent *_e);
extern bool map_ent_decons (map_ent *_e);
extern void map_ent_free (void *_v);
extern bool map_ent_set_sys (map_ent *_e, sys *_sys);
extern bool map_ent_set_path (map_ent *_e, uint32_t id);
extern bool map_ent_set_init_v_r (map_ent *_e,
        float const *_init_v, float const *_init_r);
extern bool map_ent_reset_v_r (map_ent *_e);
extern bool map_ent_rotate (map_ent *_e, float const *_r);
extern bool map_ent_rotate_3f (map_ent *_e, float r0, float r1, float r2);
extern bool map_ent_model_load (map_ent *_e, view *_view,
        arc *_arc, char const *_name_s);
extern bool map_ent_tick (map_ent *_e, float t);
extern bool map_ent_tick_path (map_ent *_e, float t);
extern void map_ent_render (map_ent *_e, view *_view);
extern bool map_ent_move (map_ent *_e, float t);

extern bool map_ent_process_msg (void *_this, sys_msg *_msg);

extern void map_ent_cam_tick (void *_this, float t);
extern void map_ent_cam_apply (void *_this, void *_v);
extern void map_ent_cam_pre_clip (void *_this, float *_v);

#endif

