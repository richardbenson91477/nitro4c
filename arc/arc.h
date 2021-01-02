#ifndef NITRO_ARC_INCLUDED
#define NITRO_ARC_INCLUDED

#include <dirent.h>
#include <sys/stat.h>

#include "../nitro_defs.h"
#include "../blaze/blaze.h"
#include "../file/file.h"

typedef struct {
    blaze *_files;
} arc;

extern bool arc_cons (arc *_arc);
extern bool arc_decons (arc *_arc);
extern bool arc_clear (arc *_arc);
extern bool arc_add (arc *_arc, file *_f);
extern bool arc_import_dir (arc *_arc, char const *_dir_s);
extern bool arc_export_dir (arc const *_arc, char const *_dir_s);
extern bool arc_report (arc const *_arc);
extern file *arc_get (arc const *_arc, char const *_name_s);
extern bool arc_get_b (arc const *_arc, char const *_name_s, uint8_t **__b,
        uint32_t *_b_n);
extern bool arc_or_file_get_b (arc const *_arc, char const *_name_s,
        uint8_t **__b, uint32_t *_b_n);
extern bool arc_load_file (arc *_arc, char const *_name_s);
extern bool arc_save_file (arc const *_arc, char const *_name_s);
extern bool arc_load_ser (arc *_arc, const uint8_t *_b, uint32_t b_n);
extern bool arc_save_ser (arc const *_arc, uint8_t **__b, uint32_t *_b_n);

#endif

