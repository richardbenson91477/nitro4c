#include "view.h"

bool view_cons (view *_view) {
    dlog_debug (_, "view_cons");

    _view->_ext_cam_tick_fn = _;
    _view->_ext_cam_apply_fn = _;
    _view->_ext_cam_pre_clip_fn = _;
    _view->_ext_cam_this = _;
    
    _view->light_ = true;
    _view->mtrl_ = false;
    _view->tex_ = true;
    _view->depth_ = true;
 
    _view->fade_dir = 0;

    _view->_view_texs = malloc(sizeof(blaze));
    if (! blaze_cons (_view->_view_texs))
        return false;

    return true;
}

bool view_decons (view *_view) {
    dlog_debug (_, "view_decons");

    blaze_clear (_view->_view_texs, view_tex_free);
    free(_view->_view_texs);
    _view->_view_texs = _;
    return true;
}

bool view_set (view *_view, int32_t w, int32_t h) {
    dlog_debug (_, "view_set");

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glLineWidth (4.0);
    glPointSize (4.0);

    glEnable (GL_CULL_FACE);

    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GREATER, 0.0);

    if (_view->depth_) {
        glEnable (GL_DEPTH_TEST);
        glDepthFunc (GL_LESS);
    }
    else
        glDisable (GL_DEPTH_TEST);

    if (_view->light_) {
        glEnable (GL_LIGHTING);
        glEnable (GL_LIGHT0);
        glShadeModel (GL_SMOOTH);

        view_light_defaults (_view);
        view_light_apply (_view);
    }
    else {
        glDisable (GL_LIGHT0);
        glDisable (GL_LIGHTING);
        glShadeModel (GL_FLAT);
    }

    if (_view->mtrl_) {
        view_mtrl_defaults (_view);
        view_mtrl_apply (_view);
    }
    else
        glEnable (GL_COLOR_MATERIAL);

    if (_view->tex_) {
        glEnable (GL_TEXTURE_2D);
        glEnableClientState (GL_TEXTURE_COORD_ARRAY);
    }

    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);

    view_port (_view, w, h);
    view_perspective (_view);

    return true;
}

bool view_tick (view *_view, float t) {
    if (_view->_ext_cam_tick_fn)
        _view->_ext_cam_tick_fn (_view->_ext_cam_this, t);

    if (_view->fade_dir) 
        view_fade_tick (_view, t);

    return true;
}

void view_render (view *_view) {
    if (_view->depth_) 
        glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    else 
        glClear (GL_COLOR_BUFFER_BIT);
    
    if (_view->_ext_cam_apply_fn)
        _view->_ext_cam_apply_fn (_view->_ext_cam_this, _view);
}

void view_port (view *_view, int32_t w, int32_t h) {
    dlog_debug (_, "view_port");

    _view->port_l = (float)w / -80000.0;
    _view->port_b = _view->port_l * ((float)h / (float)w);
    _view->port_r = -_view->port_l;
    _view->port_t = -_view->port_b;
    _view->port_zn = 0.01;
    _view->port_zf = 100.0;
    glViewport (0, 0, w, h);
}

bool view_clip (view *_view, float const *_v, float *_d_out) {
    float _v2 [3];
    v_copy (_v2, _v);
    if (_view->_ext_cam_pre_clip_fn) 
        _view->_ext_cam_pre_clip_fn (_view->_ext_cam_this, _v2);

    float d = v_len (_v2);
    if (d >= _view->port_zf) {
        if (_d_out) 
            *_d_out = -d;
        return true;
    }

    d = v_dot (_v2, _view->__port_n[0]);
    if (d > 0.0) 
        d = v_dot (_v2, _view->__port_n[1]);
    if (d > 0.0) 
        d = v_dot (_v2, _view->__port_n[2]);
    if (d > 0.0) 
        d = v_dot (_v2, _view->__port_n[3]);

    if (_d_out) 
        *_d_out = d;

    if (d <= 0.0) 
        return true;
    else 
        return false;
}

void view_perspective (view *_view) {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glFrustumf (_view->port_l, _view->port_r, _view->port_b, _view->port_t,
        _view->port_zn, _view->port_zf);
    glMatrixMode (GL_MODELVIEW);

    // calculate viewport plane normals
    // NOTE: all intersect origin; D value assumed 0 
    float p [3][3];
    float a [3], b [3];

    // left
    p[0][0] = 0.0; p[0][1] = 0.0; p[0][2] = 0.0;
    p[1][0] = _view->port_l;
    p[1][1] = _view->port_t;
    p[1][2] = _view->port_zn;
    p[2][0] = _view->port_l;
    p[2][1] = _view->port_b;
    p[2][2] = _view->port_zn;
    v_sub2 (a, p[0], p[1]);
    v_sub2 (b, p[2], p[1]);
    v_cross (_view->__port_n[0], a, b);
    v_norm (_view->__port_n[0]);

    // right
    p[0][0] = 0.0; p[0][1] = 0.0; p[0][2] = 0.0;
    p[1][0] = _view->port_r;
    p[1][1] = _view->port_b;
    p[1][2] = _view->port_zn;
    p[2][0] = _view->port_r;
    p[2][1] = _view->port_t;
    p[2][2] = _view->port_zn;
    v_sub2 (a, p[0], p[1]);
    v_sub2 (b, p[2], p[1]);
    v_cross (_view->__port_n[1], a, b);
    v_norm (_view->__port_n[1]);

    // bottom
    p[0][0] = 0.0; p[0][1] = 0.0; p[0][2] = 0.0;
    p[1][0] = _view->port_l;
    p[1][1] = _view->port_b;
    p[1][2] = _view->port_zn;
    p[2][0] = _view->port_r;
    p[2][1] = _view->port_b;
    p[2][2] = _view->port_zn;
    v_sub2 (a, p[0], p[1]);
    v_sub2 (b, p[2], p[1]);
    v_cross (_view->__port_n[2], a, b);
    v_norm (_view->__port_n[2]);

    // top
    p[0][0] = 0.0; p[0][1] = 0.0; p[0][2] = 0.0;
    p[1][0] = _view->port_r;
    p[1][1] = _view->port_t;
    p[1][2] = _view->port_zn;
    p[2][0] = _view->port_l;
    p[2][1] = _view->port_t;
    p[2][2] = _view->port_zn;
    v_sub2 (a, p[0], p[1]);
    v_sub2 (b, p[2], p[1]);
    v_cross (_view->__port_n[3], a, b);
    v_norm (_view->__port_n[3]);
}

void view_ortho (view *_view, bool t_, float w_f, float h_f) {
    if (t_) {
        glMatrixMode (GL_PROJECTION);
        glPushMatrix ();
        glLoadIdentity ();

        glMatrixMode (GL_MODELVIEW);
        glPushMatrix ();
        glLoadIdentity ();

        glOrthof (0.0, w_f, 0.0, h_f, -1.0, 1.0);
    }
    else {
        glMatrixMode (GL_PROJECTION);
        glPopMatrix ();
        glMatrixMode (GL_MODELVIEW);
        glPopMatrix ();
    }
}

void view_ortho_norm (view *_view, bool t_) {
    view_ortho (_view, t_, 1.0, 1.0);
}

void view_light_defaults (view *_view) {
    v_clear (_view->_light_pos);
    _view->_light_pos[3] = 1.0;

    v_setf (_view->_light_dif, 0.8);
    _view->_light_dif[3] = 1.0;

    v_setf (_view->_light_amb, 0.2);
    _view->_light_amb[3] = 1.0;

    v_setf (_view->_light_spec, 0.5);
    _view->_light_spec[3] = 1.0;

    v_setf (_view->_light_spot_dir, 0.0);
    _view->_light_spot_dir[3] = 1.0;
    _view->light_spot_exp = 0.0;
    _view->light_spot_cutoff = 180.0;

    _view->light_atten_c = 1.0;
    _view->light_atten_l = 0.0;
    _view->light_atten_q = 0.0;
}

void view_light_apply (view *_view) {
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT0, GL_POSITION, _view->_light_pos);
    glLightfv (GL_LIGHT0, GL_AMBIENT, _view->_light_amb);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, _view->_light_dif); 
    glLightfv (GL_LIGHT0, GL_SPECULAR, _view->_light_spec);
    glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, _view->_light_spot_dir);
    glLightfv (GL_LIGHT0, GL_SPOT_EXPONENT, &_view->light_spot_exp);
    glLightfv (GL_LIGHT0, GL_SPOT_CUTOFF, &_view->light_spot_cutoff);
    glLightfv (GL_LIGHT0, GL_CONSTANT_ATTENUATION, &_view->light_atten_c);
    glLightfv (GL_LIGHT0, GL_LINEAR_ATTENUATION, &_view->light_atten_l);
    glLightfv (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, &_view->light_atten_q);
}

void view_light_setpos (view *_view, float const *_p) {
    v_copy (_view->_light_pos, _p);
    glLightfv (GL_LIGHT0, GL_POSITION, _p);
}

void view_mtrl_defaults (view *_view) {
    v_setf (_view->_mtrl_dif, 0.9);
    v_setf (_view->_mtrl_amb, 0.2);
    v_setf (_view->_mtrl_shin, 0.5);
}

void view_mtrl_apply (view *_view) {
    #ifndef NITRO_GLES
        glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    #endif
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, _view->_mtrl_dif);
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, _view->_mtrl_amb);
    glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, _view->_mtrl_shin);
}

void view_color (view *_view, uint32_t c) {
    unsigned char const *_c_b = (unsigned char *)&c;
    glColor4ub (_c_b[2], _c_b[1], _c_b[0], _c_b[3]);
}

void view_blend (view *_view, uint32_t type) {
    if (type == 0) {
        glDisable (GL_BLEND);
    }
    else if (type == 1) {
        glEnable (GL_BLEND);
        glBlendFunc (GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    }
    else if (type == 2) {
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    }
}

void view_mv_push (view *_view) {
    glPushMatrix ();
}

void view_mv_pop (view *_view) {
    glPopMatrix ();
}

void view_mv_reset (view *_view) {
    glLoadIdentity ();
}

void view_mv_negtrans (view *_view, float const *_v) {
    glTranslatef (-_v[0], -_v[1], -_v[2]);
}

void view_mv_trans (view *_view, float const *_v) {
    glTranslatef (_v[0], _v[1], _v[2]);
}

void view_mv_rot (view *_view, float const *_r) {
    float __m [4][4];

    mat_from_r (__m, _r);
    view_mv_rot_m (_view, __m);
}

void view_mv_rot_m (view *_view, float const (*__m)[4]) {
    float const __m2[16] = {
        __m[0][0], __m[1][0], __m[2][0], 0.0,
        __m[0][1], __m[1][1], __m[2][1], 0.0,
        __m[0][2], __m[1][2], __m[2][2], 0.0,
        __m[0][3], __m[1][3], __m[2][3], 1.0
    };
    glMultMatrixf (__m2);
}

void view_fade (view *_view, float a1, float a2, float r, 
        float cr, float cg, float cb) {
    _view->fade_a = a1; _view->fade_a2 = a2; _view->fade_r = r;
    _view->fade_cr = cr; _view->fade_cg = cg; _view->fade_cb = cb; 

    if (_view->fade_a > _view->fade_a2) 
        _view->fade_dir = -1;
    else 
        _view->fade_dir = 1;
}

void view_fade_tick (view *_view, float t) {
    if (_view->fade_dir > 0) {
        _view->fade_a += _view->fade_r * t;
        if (_view->fade_a >= _view->fade_a2) { 
            _view->fade_dir = 0; 
            _view->fade_a = _view->fade_a2;
        }
    }
    else {
        _view->fade_a -= _view->fade_r * t;
        if (_view->fade_a <= _view->fade_a2) {
            _view->fade_dir = 0;
            _view->fade_a = _view->fade_a2;
        }
    }
}

void view_fade_render (view *_view) {
    float color_prev [4];
    static float const veil_v_a [] = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 1.0
    };
    static float const veil_n_a [] = {
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0
    };

    if (! _view->fade_dir)
        return;

    if (_view->tex_) 
        glDisable (GL_TEXTURE_2D);

    if (_view->light_)
        glDisable (GL_LIGHTING);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);

    glGetFloatv (GL_CURRENT_COLOR, color_prev);
    glColor4f (_view->fade_cr, _view->fade_cg, _view->fade_cb, _view->fade_a);

    view_ortho_norm (_view, 1);

    glVertexPointer (2, GL_FLOAT, 0, veil_v_a);
    glNormalPointer (GL_FLOAT, 0, veil_n_a);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
    
    view_ortho_norm (_view, 0);
    
    glColor4f (color_prev[0], color_prev[1], color_prev[2], color_prev[3]);

    glDisable (GL_BLEND);

    if (_view->tex_) 
        glEnable (GL_TEXTURE_2D);

    if (_view->light_)
        glEnable (GL_LIGHTING);
}

void view_render_tris (view *_view, float const *_v, float const *_n,
        uint32_t nv) {
    glVertexPointer (3, GL_FLOAT, 0, _v);
    glNormalPointer (GL_FLOAT, 0, _n);
    glDrawArrays (GL_TRIANGLES, 0, nv);
}

void view_render_tris_tex (view *_view, float const *_v, float const *_n,
        float const *_t, uint32_t nv) {
    // skipping sanity checks for performance
    glVertexPointer (3, GL_FLOAT, 0, _v);
    glNormalPointer (GL_FLOAT, 0, _n);
    glTexCoordPointer (2, GL_FLOAT, 0, _t);
    glDrawArrays (GL_TRIANGLES, 0, nv);
}

void view_tex_bind (view *_view, view_tex *_view_tex) {
    // skipping sanity checks for performance
    glBindTexture (GL_TEXTURE_2D, _view_tex->id);
}

view_tex *view_tex_load (view *_view, arc *_arc, char const *_name_s) {
    dlog_debug_s (_, "view_tex_load", _name_s);

    if (! _view->tex_) {
        dlog_error (_, "view_tex_load: tex_ not enabled");
        return _;
    }
    else if (! _name_s) {
        dlog_error (_, "view_tex_load: missing _name_s");
        return _;
    }

    for (view_tex *_view_tex = blaze_first (_view->_view_texs); _view_tex;
            _view_tex = blaze_next (_view->_view_texs))
        if (! strcmp (_name_s, _view_tex->_name_s)) {
            _view_tex->ref_n ++;
            return _view_tex;
        }

    img img1;
    if (! img_load_rwh (&img1, _arc, _name_s)) {
        dlog_error (_, "view_tex_load: img_load_rwh");
        return _;
    }

    view_tex *_view_tex = (view_tex *)malloc(sizeof(view_tex));
    _view_tex->_name_s = strdup(_name_s);
    _view_tex->ref_n = 1;

    glGenTextures (1, &(_view_tex->id));
    glBindTexture (GL_TEXTURE_2D, _view_tex->id);
    
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, img1.w, img1.h, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, img1._b);

    if (! img_decons (&img1))
        return _;

    blaze_add (_view->_view_texs, _view_tex);
    return _view_tex;
}

void view_tex_unref (view *_view, view_tex *_view_tex) {
    dlog_debug (_, "view_tex_unref");
    
    if (! _view->tex_) {
        dlog_error (_, "view_tex_unref: tex_ not enabled");
        return;
    }
    else if (! _view_tex) {
        dlog_error (_, "view_tex_unref: missing _tex");
        return;
    }
    else if (_view_tex->ref_n < 1) {
        dlog_error (_, "view_tex_unref: missing reference");
        return;
    }
    
    _view_tex->ref_n -= 1;
    if (! _view_tex->ref_n)
        blaze_del (_view->_view_texs, _view_tex, 0, view_tex_free);
}

void view_tex_free (void *_v) {
    dlog_debug (_, "view_tex_free");

    view_tex *_view_tex = (view_tex *)_v;
    if (! _view_tex) {
        dlog_error (_, "view_tex_free: missing _tex");
        return;
    }
    else if (_view_tex->_name_s) {
        dlog_debug_s (_, "view_tex_free: _name_s", _view_tex->_name_s);
        free(_view_tex->_name_s);
    }

    glDeleteTextures (1, &_view_tex->id);
    free(_view_tex);
}

