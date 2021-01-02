#ifndef NITRO_SND_INCLUDED
#define NITRO_SND_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../nitro_defs.h"
#include "../dlog/dlog.h"
#include "../arc/arc.h"
#include "../maths/maths.h"

typedef struct {
    Mix_Chunk *_sdl_mix_chunk;
    int32_t sdl_chan;

    uint8_t *_b;
    uint32_t b_n;
    bool free_b_;
} snd;

extern bool snd_cons (snd *_snd);
extern bool snd_decons (snd *_snd);
extern bool snd_init (void);
extern bool snd_deinit (void);
extern bool snd_orient3d (float const *_pos, float const *_rot);
extern bool snd_load_raw (snd *_snd, arc *_arc, char const *_name_s);
extern bool snd_play (snd *_snd, uint32_t loop_n);
extern bool snd_stop (snd *_snd);
extern bool snd_pos3d (snd *_snd, float const *_pos);

#endif

