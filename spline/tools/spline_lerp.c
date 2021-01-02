#include "../spline.h"

int main (int argc, char **argv) {
    if ((argc < 2) || (argc > 5)) {
        fprintf(stderr, "usage: %s spline [step_t [start_t [stop_t]]]",
                argv[0]);
        return 1;
    }

    spline *_spline = malloc(sizeof(spline));
    spline_cons (_spline);
    if (! spline_load (_spline, _, argv[1]))
        return 2;

    float step_t;
    if (argc >= 3) 
        step_t = atof(argv[2]);
    else
        step_t = 0.01f;
    
    float start_t;
    if (argc >= 4) 
        start_t = atof(argv[3]);
    else 
        start_t = _spline->min_t;

    float stop_t;
    if (argc == 5) 
        stop_t = atof(argv[4]);
    else 
        stop_t = _spline->max_t;

    float _v [3];
    const float eps = 0.00000006;
    for (float t = start_t; t <= stop_t + eps; t += step_t) {
        spline_eval (_spline, t, _v);
        for (uint8_t c = 0; c < _spline->dim; c ++)
            printf("%f ", _v[c]);
        printf("\n");
    }

    spline_decons (_spline);
    free(_spline);

    return 0;
}

