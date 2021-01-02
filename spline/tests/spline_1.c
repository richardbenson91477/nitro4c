#include "../spline.h"

int main (void) {
    spline *_spline = malloc(sizeof(spline));
    spline_cons (_spline);

    _spline->dim = 2;
    _spline->type = SPLINE_TYPE_CUBIC_BEZIER;

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
    spline_add_seg (_spline, 0.0, _v);

    _v[0] = pt2x;
    _v[1] = pt2y;
    _v[2] = 0.0;
    _v[3] = 0.0;
    _v[4] = h2x;
    _v[5] = h2y;
    spline_add_seg (_spline, 1.0, _v);

    const float eps = 0.00000006;
    for (float t = _spline->min_t; t <= _spline->max_t + eps; t += 0.02) {
        spline_eval (_spline, t, _v);
        printf("%f %f\n", _v[0], _v[1]);
    }

    spline_decons (_spline);
    free(_spline);

    return 0;
}

