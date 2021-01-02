#ifndef NITRO_DEFS
#define NITRO_DEFS

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef uint8_t bool;
#define true 1
#define false 0
#define _ NULL

#define DEGREES(x) ((x) * 57.29577951308232)
#define RADIANS(x) ((x) * 0.017453292519943295)

#ifndef PI2_78
    #define PI (3.14159265)
    #define PI14 (0.78539816) // 1/4 PI
    #define PI12 (1.57079633) // 1/2 PI
    #define PI34 (2.3561945) // 3/4 PI
    #define PI2 (6.28318531) // 2PI
    #define PI2_34 (4.71238898) // 3/4 2PI
    #define PI2_78 (5.49778714) // 7/8 2PI
#endif

#endif

