#ifndef NITRO_FILE_INCLUDED
#define NITRO_FILE_INCLUDED

#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/mman.h>

#include "../nitro_defs.h"
#include "../dlog/dlog.h"

typedef struct {
    char *_name_s;
    uint8_t *_b;
    uint32_t b_n;
    uint32_t mode;

    bool free_b_;
} file;

extern bool file_cons (file *_f);
extern bool file_cons_from_b (file *_f, uint8_t *_b, uint32_t b_n);
extern bool file_decons (file *_f);
extern void file_free (void *_v);
extern bool file_load (file *_f, char const *_name_s);
extern bool file_save (file const *_f);
extern bool file_save_as (file *_f, char const *_name_s);
extern bool file_export (file const *_f, char const *_name_s);

#endif

