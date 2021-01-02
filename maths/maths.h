#ifndef NITRO_MATH_INCLUDED
#define NITRO_MATH_INCLUDED

#include <stdint.h>
#include <math.h>
#include <float.h>

#include "../nitro_defs.h"

static inline void v_scale (float *_ov, float const *_s) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] *= _s[i];
}
static inline void v_scale2 (float *_ov, float const *_v, float const *s) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[i] * s[i];
}
static inline void v_scalef (float *_ov, float s) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] *= s;
}
static inline void v_scalef2 (float *_ov, float const *_v, float s) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[i] * s;
}
static inline void v_div (float *_ov, float const *s) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] /= s[i];
}
static inline void v_div2 (float *_ov, float const *_v, float const *s) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[i] / s[i];
}
static inline void v_divf (float *_ov, float s) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] /= s;
}
static inline void v_divf2 (float *_ov, float const *_v, float s) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[i] / s;
}
static inline void v_add (float *_ov, float const *_v) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] += _v[i];
}
static inline void v_add2 (float *_ov, float const *_v1, float const *_v2) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v1[i] + _v2[i];
}
static inline void v_addf (float *_ov, float f) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] += f;
}
static inline void v_addf2 (float *_ov, float const *_v, float f) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[i] + f;
}
static inline void v_sub (float *_ov, float const *_v) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] -= _v[i];
}
static inline void v_sub2 (float *_ov, float const *_v1, float const *_v2) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v1[i] - _v2[i];
}
static inline void v_subf (float *_ov, float f) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] -= f;
}
static inline void v_subf2 (float *_ov, float const *_v, float f) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[i] - f;
}
static inline void v_neg (float *_ov) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = -_ov[i];
}
static inline void v_neg2 (float *_ov, float const *_v) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = -_v[i];
}
static inline void v_copy (float *_ov, float const *_v) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[i];
}
static inline bool v_is_clear (float const *_v) {
    return ((_v[0] == 0.0) && (_v[1] == 0.0) && (_v[2] == 0.0));
}
static inline void v_clear (float *_ov) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = 0.0;
}
static inline void v_setf (float *_ov, float f) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = f;
}
static inline float v_len (float const *_v) {
    return sqrtf(_v[0] * _v[0] +
                 _v[1] * _v[1] +
                 _v[2] * _v[2]);
}
static inline bool v_equal (float const *_v1, float const *_v2) {
    return ((_v1[0] == _v2[0]) &&
            (_v1[1] == _v2[1]) &&
            (_v1[2] == _v2[2]));
}
static inline bool v_similar (float const *_v1, float const *_v2, float e) {
    float _dv [3];
    v_sub2 (_dv, _v1, _v2);
    return ((_dv[0] < e) && (_dv[0] > -e) && \
            (_dv[1] < e) && (_dv[1] > -e) && \
            (_dv[2] < e) && (_dv[2] > -e));
}
static inline void v_norm (float *_ov) {
    float len = sqrtf(_ov[0] * _ov[0] +
                      _ov[1] * _ov[1] +
                      _ov[2] * _ov[2]);
    if (len == 0.0) {
        for (uint32_t i = 0; i < 3; i ++)
            _ov[i] = 0.0;
    }
    else {
        for (uint32_t i = 0; i < 3; i ++)
            _ov[i] /= len;
    }
}
static inline void v_norm2 (float *_ov, float const *_v) {
    float len = sqrtf(_v[0] * _v[0] +
                      _v[1] * _v[1] +
                      _v[2] * _v[2]);
    if (len == 0.0) {
        for (uint32_t i = 0; i < 3; i ++)
            _ov[i] = 0.0;
    }
    else {
        for (uint32_t i = 0; i < 3; i ++)
            _ov[i] = _v[i] / len;
    }
}
// returns the cosine of the angle between v1 and v2
static inline float v_dot (float const *_v1, float const *_v2) {
    return (_v1[0] * _v2[0] +
            _v1[1] * _v2[1] +
            _v1[2] * _v2[2]);
}
// returns a vector which is normal to the plane containing vectors v1, v2
// NOTE: if sign ((b - a) cross (c - a)) == sign ((b - a) cross (d - a))
//      then d is on the same side of line (b - a) as c
static inline void v_cross (float *_ov, float const *_v1, float const *_v2) {
    _ov[0] = _v1[1] * _v2[2] - _v1[2] * _v2[1];
    _ov[1] = _v1[2] * _v2[0] - _v1[0] * _v2[2];
    _ov[2] = _v1[0] * _v2[1] - _v1[1] * _v2[0];
}
static inline void v_reflect (float *_ov, float const *_v, float const *_n) {
    float dot = -(v_dot (_n, _v)) * 2.0;
    v_scalef2 (_ov, _n, dot);
    v_add (_ov, _v);
}
static inline void poly_normal (float *_pl, float const *_v1, float const *_v2, 
        float const *_v3) {
    float _a [3], _b [3];
    v_sub2 (_a, _v1, _v2);
    v_sub2 (_b, _v3, _v2);
    v_cross (_pl, _a, _b);
    v_norm (_pl);
}
static inline void poly_plane (float *_pl, float const *_v1, float const *_v2, 
        float const *_v3) {
    float _a [3], _b [3];
    v_sub2 (_a, _v1, _v2);
    v_sub2 (_b, _v3, _v2);
    v_cross (_pl, _a, _b);
    v_norm (_pl);
    _pl[3] = -v_dot (_v1, _pl);
}
static inline void v_from_spherical (float *_ov, float r, float p, float t) {
    _ov[0] = r * sinf(t) * sinf(p);
    _ov[1] = r * cosf(p);
    _ov[2] = r * cosf(t) * sinf(p);
}
static inline void a_bound (float *_a) {
    while (*_a < 0.0)
        *_a += PI2;
    while (*_a >= PI2)
        *_a -= PI2;
}
static inline void v_a_bound (float *_ov) {
    for (uint32_t i = 0; i < 3; i ++)
        a_bound (_ov + i);
}
static inline void spherical_from_v (float const *_v, float *_r, float *_p,
        float *_t){
    if (_r)
        *_r = v_len (_v);
    if (_t) {
        *_t = atan2f(_v[0], _v[2]);
        a_bound (_t);
    }
    if (_p) {
        *_p = atan2f(sqrtf(_v[2] * _v[2] + _v[0] * _v[0]), _v[1]);
        a_bound (_p);
    }
}
static inline float plane_dist (float const *_p, float const *_pl) {
    return v_dot (_p, _pl) + _pl[3];
}
static inline float v_list_farthest (float const *_v, uint32_t nv) {
    float max = 0.0;
    float *_v2 = (float *)_v;
    for (uint32_t c = 0; c < nv; c ++) {
        float l = v_len (_v2);
        if (l > max)
            max = l;
        _v2 += 3;
    }
    return max;
}
static inline void v_slide (float *_ov, float const *_v, float const *_n) {
    float dot = -(v_dot (_n, _v));
    v_scalef2 (_ov, _n, dot);
    v_add (_ov, _v);
}

static inline void mat_ident (float (*__om)[4]) {
    for (uint32_t i = 0; i < 4; i ++)
        __om[i][i] = 1.0;

    __om[0][1] = __om[0][2] = __om[0][3] = 0.0;
    __om[1][0] = __om[1][2] = __om[1][3] = 0.0;
    __om[2][0] = __om[2][1] = __om[2][3] = 0.0;
    __om[3][0] = __om[3][1] = __om[3][2] = 0.0;
}

static inline void mat_copy (float (*__om)[4], float const (*__m)[4]) {
    for (uint32_t i = 0; i < 4; i ++)
        for (uint32_t j = 0; j < 4; j ++)
            __om[i][j] = __m[i][j];
}

static inline void mat_concat (float (*__om)[4], float const (*__m1)[4],
        float const (*__m2)[4]) {
    for (uint32_t i = 0; i < 4; i ++)
        for (uint32_t j = 0; j < 4; j ++)
            __om[i][j] = __m1[i][0] * __m2[0][j] +
                       __m1[i][1] * __m2[1][j] +
                       __m1[i][2] * __m2[2][j] +
                       __m1[i][3] * __m2[3][j];
}

static inline void mat_invert (float (*__om)[4], float const (*__m)[4]) {
    for (uint32_t i = 0; i < 4; i ++)
        for (uint32_t j = 0; j < 4; j ++)
            __om[i][j] = __m[j][i];
}

static inline void mat_from_r (float (*__om)[4], float const *_r) {
    float _s [3], _c [3];

    for (uint32_t i = 0; i < 3; i ++) {
        _s[i] = sinf(_r[i]);
        _c[i] = cosf(_r[i]);
    }

    __om[0][0] = _c[1] * _c[2];
    __om[1][0] = _c[1] * _s[2];
    __om[2][0] = -_s[1];
    __om[3][0] = 0.0;
    __om[0][1] = _s[0] * _s[1] * _c[2] + _c[0] * -_s[2];
    __om[1][1] = _s[0] * _s[1] * _s[2] + _c[0] * _c[2];
    __om[2][1] = _s[0] * _c[1];
    __om[3][1] = 0.0;
    __om[0][2] = _c[0] * _s[1] * _c[2] + _s[0] * _s[2];
    __om[1][2] = _c[0] * _s[1] * _s[2] + -_s[0] * _c[2];
    __om[2][2] = _c[0] * _c[1];
    __om[3][2] = 0.0;
    __om[0][3] = 0.0;
    __om[1][3] = 0.0;
    __om[2][3] = 0.0;
    __om[3][3] = 0.0;
}

static inline void mat_from_p (float (*__om)[4], float const *_p) {
    __om[0][3] = _p[0];
    __om[1][3] = _p[1];
    __om[2][3] = _p[2];
}

static inline void mat_from_r_p (float (*__om)[4], float const *_r,
        float const *_p) {
    mat_from_r (__om, _r);
    mat_from_p (__om, _p);
}

// NOTE: r returned is not generally r that created m
static inline void r_from_m (float *_r, float const (*__m)[4]) {
    float cy = sqrtf(__m[0][0] * __m[0][0] + __m[1][0] * __m[1][0]);

    if (cy > 16 * FLT_EPSILON) {
        _r[0] = atan2f(__m[2][1], __m[2][2]);
        _r[1] = atan2f(-__m[2][0], cy);
        _r[2] = atan2f(__m[1][0], __m[0][0]);
    } 
    else {
        _r[0] = atan2f(-__m[1][2], __m[1][1]);
        _r[1] = atan2f(-__m[2][0], cy);
        _r[2] = 0.0;
    }
}

static inline void v_rotate_m2 (float *_ov, float const *_v,
        float const (*__m)[4]) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[0] * __m[i][0] +
                 _v[1] * __m[i][1] +
                 _v[2] * __m[i][2];
}

static inline void v_rotate_m (float *_v, float const (*__m)[4]) {
    float _v2 [3];
    v_rotate_m2 (_v2, _v, __m);
    v_copy (_v, _v2);
}

static inline void v_list_rotate_m2 (float *_ov, float const *_v, uint32_t nv, 
        float const (*__m)[4]) {
    float *_ov2 = _ov;
    float *_v2 = (float *)_v;
    for (uint32_t c = 0; c < nv; c ++) {
        v_rotate_m2 (_ov2, _v2, __m);
        _ov2 += 3;
        _v2 += 3;
    }
}

static inline void v_apply_m2 (float *_ov, float const *_v,
        float const (*__m)[4]) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[i] * __m[i][0] +
                 _v[i] * __m[i][1] +
                 _v[i] * __m[i][2] +
                         __m[i][3];
}

static inline void v_apply_m (float *_v, float const (*__m)[4]) {
    float _v2 [3];
    v_apply_m2 (_v2, _v, __m);    
    v_copy (_v, _v2);
}

static inline void v_inv_rotate_m2 (float *_ov, float const *_v,
        float const (*__m)[4]) {
    for (uint32_t i = 0; i < 3; i ++)
        _ov[i] = _v[0] * __m[0][i] +
                 _v[1] * __m[1][i] +
                 _v[2] + __m[2][i];
}

static inline void v_inv_rotate_m (float *_v, float const (*__m)[4]) {
    float _v2 [3];
    v_inv_rotate_m2 (_v2, _v, __m);
    v_copy (_v, _v2);
}

static inline void v_inv_apply_m2 (float *_ov, float const *_v,
        float const (*__m)[4]) {
    float _v2 [3];
    uint32_t i;
    for (i = 0; i < 3; i ++)
        _v2[i] = _v[i] - __m[i][3];

    for (i = 0; i < 3; i ++)
        _ov[i] = _v2[0] * __m[0][i] +
                 _v2[1] * __m[1][i] +
                 _v2[2] * __m[2][i];
}

static inline void v_inv_apply_m (float *_v, float const (*__m)[4]) {
    float _v2 [3];
    v_inv_apply_m2 (_v2, _v, __m);
    v_copy (_v, _v2);
}

static inline float randf (float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX * (max - min));
}

static inline bool pt_in_box (float const *_p, float const *_hb2) {
    return ((_p[0] <= _hb2[0]) && (_p[0] >= -_hb2[0]) &&
            (_p[1] <= _hb2[1]) && (_p[1] >= -_hb2[1]) &&
            (_p[2] <= _hb2[2]) && (_p[2] >= -_hb2[2]));
}

static inline bool pt_in_sph (float const *_p, float sr2) {
    return (sr2 >= v_dot (_p, _p));
}

// p is relative to sphere (sphere_pos - ray_origin)
// ray must be normalized
// isec_n = p + tn * ray
static inline bool ray_to_sph (float const *_p, float const *_ray, float sr2,
        float *_t1, float *_t2) {
    float t_ca, t_hc, d_sqr;

    // algorithm care of cs.princeton.edu (img013.gif)
    // ray: isec = ray_origin + t(ray)
    // sphere: |isec - sphere_origin|^2 - sphere_rad^2 = 0
    // p is line from sphere_origin to ray_origin
    // t_ca is line from ray_origin to midway between intersection points
    // t_ca = p . ray
    // d is line from sphere_origin to t_ca
    // pythagorean wizardry
    // p . p = d^2 + t_ca^2
    // d^2 = p . p - t_ca^2
    // t_hc is the distance from t_ca to the intersection points
    // more pythagorean wizardry
    // t_hc = sqrt (sphere_rad^2 - d^2)
    // t = t_ca - t_hc and t_ca + t_hc

    t_ca = v_dot (_p, _ray);
    if (t_ca <= 0.0)
        return false;

    d_sqr = v_dot (_p, _p) - (t_ca * t_ca);
    if (d_sqr > sr2)
        return false;

    if (_t1 || _t2) {
        t_hc = sqrtf(sr2 - d_sqr);
        if (_t1)
            *_t1 = t_ca - t_hc;
        if (_t2)
            *_t2 = t_ca + t_hc;
    }
    return true;
}

static inline bool ray_to_poly (float const *_p, float const *_ray,
        float const *_v1, float const *_v2, float const *_v3, float const *_pn, 
        float *_t_out, float *_isec_out) {
    float _ev0 [3], _ev1 [3], _ev2 [3], _isec [3];
    float dot00, dot01, dot02, dot11, dot12, u, v;
    float t, d;

    // find ray intersection point isec
    // algorithm mostly care of cs.princeton.edu (img017.gif)
    // ray: isec = p + t(ray)
    // plane: isec . pn + pn[3] = 0
    // substituting for isec
    // (p + t(ray)) . pn + pn[3] = 0
    // (p . pn) + (t(ray) . pn) = -pn[3]
    // t(ray) . pn = -(p . pn + pn[3])
    // t = -(p . pn + pn[3]) / (ray . pn)
    // t = plane_dit (p, pn) / -(ray . pn)
    d = v_dot (_ray, _pn);
    if (d > 0.0)
        return false;

    t = plane_dist (_p, _pn) / -d;

    if (t <= 0.0)
        return false;

    v_scalef2 (_isec, _ray, t);
    v_add (_isec, _p);

    // algorithm care of http://www.blackpawn.com/texts/pointinpoly/
    // test whether isec is within polygon (using barycentric coordinates)
    // P   = A   + u (C - A) + p (B - A)
    // P - A     = u (C - A) + p (B - A)
    // ev2       = u (ev0) + p (ev1)
    // ev2 . ev0 = (u (ev0) + p (ev1)) . ev0
    // ev2 . ev0 = u (ev0 . ev0) + p (ev1 . ev0)
    // ev2 . ev1 = (u (ev0) + p (ev1)) . ev1
    // ev2 . ev1 = u (ev0 . ev1) + p (ev1 . ev1)
    // solve for u and v
    // u = ((ev1 . ev1)(ev2 . ev0) - (ev1 . ev0)(ev2 . ev1)) /
    //    ((ev0 . ev0)(ev1 . ev1) - (ev0 . ev1)(ev1 . ev0))
    // v = ((ev0 . ev0)(ev2 . ev1) - (ev0 . ev1)(ev2 . ev0)) /
    //    ((ev0 . ev0)(ev1 . ev1) - (ev0 . ev1)(ev1 . ev0))
    v_sub2 (_ev0, _v3, _v1);
    v_sub2 (_ev1, _v2, _v1);
    v_sub2 (_ev2, _isec, _v1);
    dot00 = v_dot (_ev0, _ev0);
    dot01 = v_dot (_ev0, _ev1);
    dot02 = v_dot (_ev0, _ev2);
    dot11 = v_dot (_ev1, _ev1);
    dot12 = v_dot (_ev1, _ev2);
    d = dot00 * dot11 - dot01 * dot01;
    if (d == 0.0)
        return true;
    u = (dot11 * dot02 - dot01 * dot12) / d;
    v = (dot00 * dot12 - dot01 * dot02) / d;
    if ((u < 0.0) || (v < 0.0) || (u + v > 1.0))
        return false;

    if (_t_out)
        *_t_out = t;
    if (_isec_out)
        v_copy (_isec_out, _isec);
    return true;
}

static inline bool sph_in_sph (float const *_p, float r1, float r2) {
    float t = r1 + r2;
    return ((t * t) >= v_dot (_p, _p));
}

static inline bool sph_in_plane (float const *_p, float r, float const *_pl) {
    float t = plane_dist (_p, _pl);
    return ((t < -1.0) || (t > r));
}

static inline float pt_on_line_closest_to_pt (float const *_p,
        float const *_v1, float const *_v2, float *_p_out) {
    float _v [3], _ray [3];
    float d, t;

    // algorithm care of DigiBen@GameTutorials.com

    v_sub2 (_ray, _v2, _v1);
    d = v_len (_ray);
    v_divf (_ray, d);

    v_sub2 (_v, _p, _v1);
    t = v_dot (_v, _ray);
    if (t <= 0.0)
        v_copy (_v, _v1);
    else if (t >= d)
        v_copy (_v, _v2);
    else {
        v_scalef2 (_v, _ray, t);
        v_add (_v, _v1);
    }

    if (_p_out)
        v_copy (_p_out, _v);
    
    v_sub (_v, _p);
    return v_len (_v);
}

static inline bool sph_to_poly (float const *_p, float r,
        float const *_ray, float move_t,
        float const *_v1, float const *_v2, float const *_v3, float const *_pl,
        float *_t_out, float *_isec_out) {
    float _ev0 [3], _ev1 [3], _ev2 [3];
    float dot00, dot01, dot02, dot11, dot12, u, v;
    float _isec [3], d, t;

    // algorithm somewhat care of cs.princeton.edu (img017.gif)

    d = v_dot (_ray, _pl);
    if (d >= 0.0)
        return false;

    t = plane_dist (_p, _pl) / -d;
    if (t < -r)
        return false;
    
    if (_t_out)
        *_t_out = t;

    if (((t + r * 2.0) < move_t) || ((t - r) > move_t))
        return false;
    
    v_scalef2 (_isec, _ray, t);
    v_add (_isec, _p);
    if (_isec_out)
        v_copy (_isec_out, _isec);

    // algorithm care of http://www.blackpawn.com/texts/pointinpoly/
    v_sub2 (_ev0, _v3, _v1);
    v_sub2 (_ev1, _v2, _v1);
    v_sub2 (_ev2, _isec, _v1);
    dot00 = v_dot (_ev0, _ev0);
    dot01 = v_dot (_ev0, _ev1);
    dot02 = v_dot (_ev0, _ev2);
    dot11 = v_dot (_ev1, _ev1);
    dot12 = v_dot (_ev1, _ev2);
    d = dot00 * dot11 - dot01 * dot01;
    if (d == 0.0)
        return true;
    u = (dot11 * dot02 - dot01 * dot12) / d;
    v = (dot00 * dot12 - dot01 * dot02) / d;
    if ((u >= 0.0) && (v >= 0.0) && (u + v <= 1.0))
        return true;

    else if (pt_on_line_closest_to_pt (_isec, _v1, _v2, _) < r)
        return true;
    else if (pt_on_line_closest_to_pt (_isec, _v2, _v3, _) < r)
        return true;
    else if (pt_on_line_closest_to_pt (_isec, _v3, _v1, _) < r)
        return true;
    
    return false;
}

#endif

