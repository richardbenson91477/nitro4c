#ifndef NITRO_BLAZE_INCLUDED
#define NITRO_BLAZE_INCLUDED

#include "../nitro_defs.h"

typedef struct {
    void **__p;
    uint32_t item_n;

    uint32_t blk_n;
    uint32_t blk_len;
    bool auto_compact_;

    // NOTE: to nest iters you must manually save/restore
    uint32_t iter_c;
    void **__iter;
} blaze;

extern bool blaze_cons (blaze *_b);
extern bool blaze_decons (blaze *_b);
extern bool blaze_add (blaze *_b, void *_v);
extern bool blaze_insert (blaze *_b, void *_v, uint32_t i);
extern void *blaze_get (blaze const *_b, uint32_t i);
extern bool blaze_del (blaze *_b, void *_v, uint32_t i,
        void (* free_fn)(void *));
extern bool blaze_clear (blaze *_b, void (* free_fn)(void *));
extern bool blaze_compact (blaze *_b);
extern void *blaze_first (blaze *_b);
extern void *blaze_next (blaze *_b);
extern void *blaze_cur (blaze *_b);

#endif

