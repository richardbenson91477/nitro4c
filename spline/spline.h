#ifndef NITRO_SPLINE_INCLUDED
#define NITRO_SPLINE_INCLUDED

#include "../nitro_defs.h"
#include "../arc/arc.h"
#include "../maths/maths.h"

#define SPLINE_MAX_DIM 4

enum {
// enums_map_begin
    SPLINE_TYPE_LINEAR = 1,
    SPLINE_TYPE_CUBIC_BEZIER
// enums_map_end
};

typedef struct {
    float t;
    float _v [SPLINE_MAX_DIM * 3];
} spline_seg;

typedef struct {
    uint8_t dim;
    uint32_t type;
    float min_t;
    float max_t;

    blaze *_segs;
} spline;

extern bool spline_cons (spline *_spline);
extern bool spline_decons (spline *_spline);
extern bool spline_eval (spline const *_spline, float t, float *_v_out);
extern bool spline_add_seg (spline *_spline, float t, float *_v);
extern bool spline_load (spline *_spline, arc *_arc, char const *_name_s);

#endif

