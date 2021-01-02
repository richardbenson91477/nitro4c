#include "../comp.h"

void user_fn (float *v_out, const float *v_in, float t) {
    v_out[0] = cosf(t) * sinf(v_in[0]);
    v_out[1] = sinf(t) * sinf(v_in[1]);
}

int main (void) {
    comp *_comp = malloc(sizeof(comp));
    comp_cons (_comp);

    spline *_spline = malloc(sizeof(spline));
    spline_cons (_spline);

    _spline->dim = 2;
    _spline->type = SPLINE_TYPE_LINEAR;

    float _v [2];
    _v[0] = _v[1] = 0.0;
    spline_add_seg (_spline, 0.0, _v);

    _v[0] = _v[1] = PI2 * 5;
    spline_add_seg (_spline, PI2, _v);

    comp_add_spline (_comp, COMP_ELEM_COMBINE_SET, _spline, 0);

    comp_add_user_fn (_comp, COMP_ELEM_COMBINE_SET, 2, user_fn);

    for (float t = 0; t <= PI2; t += 0.01f) {
        comp_eval (_comp, t, _v);
        printf("%f %f\n", _v[0], _v[1]);
    }
 
    comp_decons (_comp);
    return 0;
}

