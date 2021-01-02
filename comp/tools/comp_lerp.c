#include "../comp.h"

int main (int argc, char **argv) {
    if ((argc < 2) || (argc > 5)) {
        fprintf(stderr, "usage: %s comp [step_t [start_t [stop_t]]]\n",
                argv[0]);
        return 1;
    }

    comp *_comp = malloc(sizeof(comp));
    if (!  comp_cons (_comp))
        return 2;

    if (! comp_load (_comp, _, argv[1]))
        return 3;

    float step_t;
    if (argc >= 3) 
        step_t = atof(argv[2]);
    else
        step_t = 0.01f;
    
    float start_t;
    if (argc >= 4) 
        start_t = atof(argv[3]);
    else 
        start_t = _comp->min_t;

    float stop_t;
    if (argc == 5) 
        stop_t = atof(argv[4]);
    else 
        stop_t = _comp->max_t;

    float _v [3];
    const float eps = 0.00000006;
    for (float t = start_t; t <= stop_t + eps; t += step_t) {
        comp_eval (_comp, t, _v);
        for (uint8_t c = 0; c < _comp->dim; c ++)
            printf("%f ", _v[c]);
        printf("\n");
    }

    comp_decons (_comp);
    free(_comp);

    return 0;
}

