#ifndef NITRO_COMP_INCLUDED
#define NITRO_COMP_INCLUDED

#include "../nitro_defs.h"
#include "../spline/spline.h"

#define COMP_MAX_DIM 4

enum {
// enums_map_begin
    COMP_ELEM_COMBINE_SET = 1,
    COMP_ELEM_COMBINE_ADD,
    COMP_ELEM_COMBINE_MUL,
    COMP_ELEM_TYPE_SPLINE = 1,
    COMP_ELEM_TYPE_CONSTANTS,
    COMP_ELEM_TYPE_USER_FN,
// enums_map_end
};

typedef struct {
    uint32_t combiner;
    uint32_t type;

    spline *_spline; 
    bool spline_free_;

    uint8_t consts_dim;
    float _consts [COMP_MAX_DIM];

    uint8_t user_fn_dim;
    void (* _user_fn)(float *, float const *, float);
} comp_elem;

typedef struct {
    uint8_t dim;
    float min_t;
    float max_t;

    blaze *_elems;
} comp;

extern bool comp_cons (comp *_comp);
extern bool comp_decons (comp *_comp);
extern void comp_free (void *_v);
extern bool comp_eval (comp const *_comp, float t, float *_v_out);
extern bool comp_add_spline (comp *_comp, uint32_t combiner,
        spline *_spline, bool spline_free_);
extern bool comp_add_consts (comp *_comp, uint32_t combiner,
        uint8_t consts_dim, float const *_consts);
extern bool comp_add_user_fn (comp *_comp, uint32_t combiner,
        uint8_t user_fn_dim, void (* _user_fn)(float *, float const *, float));
extern bool comp_load (comp *_comp, arc *_arc, char const *_name_s);
extern void comp_elem_free (void *_v);

#endif

