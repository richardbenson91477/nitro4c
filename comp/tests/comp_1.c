#include "../comp.h"

int main (void) {
    comp *_comp = malloc(sizeof(comp));
    comp_cons (_comp);

    spline *_spline1 = malloc(sizeof(spline));
    spline_cons (_spline1);
    _spline1->dim = 2;
    _spline1->type = SPLINE_TYPE_CUBIC_BEZIER;

    float pt1x = 0.0, pt1y = 0.0;
    float pt2x = 1.0, pt2y = 0.0;
    float h1x = 0.0, h1y = 1.0;
    float h2x = 1.0, h2y = -1.0;

    float _v [6];
    _v[0] = pt1x;
    _v[1] = pt1y;
    _v[2] = h1x;
    _v[3] = h1y;
    _v[4] = 0.0; 
    _v[5] = 0.0;
    spline_add_seg (_spline1, 0.0, _v);

    _v[0] = pt2x;
    _v[1] = pt2y;
    _v[2] = 0.0;
    _v[3] = 0.0;
    _v[4] = h2x;
    _v[5] = h2y;
    spline_add_seg (_spline1, 1.0, _v);

    comp_add_spline (_comp, COMP_ELEM_COMBINE_SET, _spline1, 0);

    spline *_spline2 = malloc(sizeof(spline));
    spline_cons (_spline2);

    _spline2->dim = 2;
    _spline2->type = SPLINE_TYPE_LINEAR;

    _v[0] = _v[1] = 2.0;
    spline_add_seg (_spline2, 0.0, _v);

    comp_add_spline (_comp, COMP_ELEM_COMBINE_MUL, _spline2, 0);
 
    _v[0] = _v[1] = 0.5;
    comp_add_consts (_comp, COMP_ELEM_COMBINE_MUL, 2, _v);

    const float eps = 0.00000006;
    for (float t = _comp->min_t; t <= _comp->max_t + eps; t += 0.01f) {
        comp_eval (_comp, t, _v);
        printf("%f %f\n", _v[0], _v[1]);
    }
 
    comp_decons (_comp);
    free(_comp);

    spline_decons (_spline2);
    free(_spline2);

    spline_decons (_spline1);
    free(_spline1);
    
    return 0;
}

