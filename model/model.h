#ifndef NITRO_MODEL_INCLUDED
#define NITRO_MODEL_INCLUDED

#include "../nitro_defs.h"
#include "../view/view.h"

typedef struct {
    uint32_t vert_n, poly_n;
    float *_verts, *_v_norms, *_t_coords, *_p_norms;
    float rad;
    bool poly_col_;
    char *_tex_name_s;

    view_tex *_view_tex;
} model;

extern bool model_cons (model *_model);
extern bool model_decons (model *_model, view *_view);
extern bool model_load (model *_model, view *_view, arc *_arc,
        char const *_name_s);
extern bool model_tick (model *_model);
extern void model_render (model const *_model, view *_view);

#endif

