#include "snd.h"

bool snd_active_ = false;
uint32_t snd_mix_rate = 48000;
uint32_t snd_mix_format = AUDIO_S16;
uint32_t snd_mix_chans = 2;
uint32_t snd_mix_buf_n = 2048;
float _snd_listener_pos [3];
float _snd_listener_rot [3];

bool snd_cons (snd *_snd) {
    dlog_debug (_, "snd_cons");

    _snd->_sdl_mix_chunk = _;
    _snd->sdl_chan = -1;
    _snd->_b = _;
    _snd->b_n = 0;
    _snd->free_b_ = true;
    return true;
}

bool snd_decons (snd *_snd) {
    dlog_debug (_, "snd_decons");
    
    if (_snd->sdl_chan >= 0) {
        dlog_debug (_, "snd_decons: calling snd_stop");
        snd_stop (_snd);
    }

    if (_snd->_sdl_mix_chunk) {
        Mix_FreeChunk (_snd->_sdl_mix_chunk);
        _snd->_sdl_mix_chunk = _;
    }

    if (_snd->free_b_ && _snd->_b) {
        free(_snd->_b);
        _snd->_b = _;
    }

    return true;
}

bool snd_init (void) {
    dlog_debug (_, "snd_init");

    if (snd_active_) {
        dlog_error (_, "snd_init: already active");
        return false;
    }

    if (SDL_InitSubSystem (SDL_INIT_AUDIO)) {
        dlog_error (_, "snd_init: SDL_InitSubSystem");
        return false;
    }

    if (Mix_OpenAudio (snd_mix_rate, snd_mix_format, snd_mix_chans,
            snd_mix_buf_n)) {
        dlog_error (_, "snd_init: Mix_OpenAudio");
        return false;
    }

    snd_active_ = true;
    return true;
}

bool snd_deinit (void) {
    dlog_debug (_, "snd_deinit");

    if (! snd_active_) {
        dlog_error (_, "snd_deinit: not active");
        return false;
    }

    Mix_CloseAudio ();

    snd_active_ = false;
    return true;
}

bool snd_orient3d (float const *_pos, float const *_rot) {
    if ((! _pos) || (! _rot)) {
        dlog_error (_, "snd_orient3d: missing _pos or _rot");
        return false;
    }
    v_copy (_snd_listener_pos, _pos);
    v_copy (_snd_listener_rot, _rot);
    return true;
}

bool snd_load_raw (snd *_snd, arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "snd_load_raw", _name_s);

    if (! _name_s) {
        dlog_error (_, "snd_load_raw: missing _name_s");
        return false;
    }
 
    if (! arc_or_file_get_b (_arc, _name_s, &_snd->_b, &_snd->b_n)) {
        dlog_error_s (_, "model_load: arc_or_fs_get_b", _name_s);
        return false;
    }

    if (_arc)
        _snd->free_b_ = false;

    _snd->sdl_chan = -1;
    _snd->_sdl_mix_chunk = Mix_QuickLoad_RAW (_snd->_b, _snd->b_n);
    if (! _snd->_sdl_mix_chunk) {
        dlog_error (_, "snd_load_raw: Mix_QuickLoad_RAW");
        return false;
    }

    return true;
}

bool snd_play (snd *_snd, uint32_t loop_n) {
    dlog_debug (_, "snd_play");

    if (! snd_active_) {
        dlog_error (_, "snd_play: not active");
        return false;
    }

    if (! _snd->_sdl_mix_chunk) {
        dlog_error (_, "snd_play: missing chunk");
        return false;
    }

    _snd->sdl_chan = Mix_PlayChannel (-1, _snd->_sdl_mix_chunk, loop_n);
    if (_snd->sdl_chan == -1) {
        dlog_error (_, "snd_play: Mix_PlayChannel");
        return false;
    }

    return true;
}

bool snd_stop (snd *_snd) {
    dlog_debug (_, "snd_stop");

    if (! snd_active_) {
        dlog_error (_, "snd_stop: not active");
        return false;
    }

    if (! _snd->_sdl_mix_chunk) {
        dlog_error (_, "snd_stop: missing chunk");
        return false;
    }

    if (_snd->sdl_chan < 0) {
        dlog_error (_, "snd_stop: sdl_chan < 0");
        return false;
    }
 
    if (Mix_HaltChannel (_snd->sdl_chan)) {
        dlog_error (_, "snd_stop: Mix_HaltChannel");
        return false;
    }
    _snd->sdl_chan = -1;

    return true;
}

bool snd_pos3d (snd *_snd, float const *_pos) {
    if (! _pos) {
        dlog_error (_, "snd_pos3d: missing _pos");
        return false;
    }

    if (! snd_active_) {
        dlog_error (_, "snd_pos3d: not active");
        return false;
    }

    float _pos_rel [3];
    v_sub2 (_pos_rel, _snd_listener_pos, _pos);
    float d = v_len (_pos_rel);
 
    float _listener_rmat [4][4];
    mat_from_r (_listener_rmat, _snd_listener_rot);

    float _rot_pos [3];
    v_inv_rotate_m2 (_rot_pos, _pos_rel, _listener_rmat);
    float theta = atan2f(_rot_pos[0], _rot_pos[2]);
    a_bound (&theta);

    float const d_scale = 10.0;
    d *= d_scale;
    if (d > 255.0)
        d = 255.0;

    if (Mix_SetPosition (_snd->sdl_chan, (int16_t)DEGREES(theta),
            (int16_t)(d * d_scale))) {
        dlog_error (_, "snd_pos3d: Mix_SetPosition");
        return false;
    }

    return true;
}

