#ifndef NITRO_DLOG_INCLUDED
#define NITRO_DLOG_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "../nitro_defs.h"

typedef struct {
    FILE *_dlog_f;
} dlog;

extern bool dlog_cons (dlog *_dlog, char const *_name_s);
extern bool dlog_decons (dlog *_dlog);
extern void dlog_debug (dlog const *_dlog, char const *s);
extern void dlog_debug_s (dlog const *_dlog, char const *s, char const *s2);
extern void dlog_debug_f (dlog const *_dlog, char const *s, float f);
extern void dlog_debug_d (dlog const *_dlog, char const *s, double d);
extern void dlog_debug_u (dlog const *_dlog, char const *s, uint32_t u);
extern void dlog_debug_i (dlog const *_dlog, char const *s, int32_t i);
extern void dlog_error (dlog const *_dlog, char const *s);
extern void dlog_error_s (dlog const *_dlog, char const *s, char const *s2);
extern void dlog_error_f (dlog const *_dlog, char const *s, float f);
extern void dlog_error_d (dlog const *_dlog, char const *s, double d);
extern void dlog_error_u (dlog const *_dlog, char const *s, uint32_t u);
extern void dlog_error_i (dlog const *_dlog, char const *s, int32_t i);

#endif

