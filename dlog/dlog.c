#include "dlog.h"

bool dlog_cons (dlog *_dlog, char const *_name_s) {
    if (! _dlog)
        return false;

    if (_name_s) {
        _dlog->_dlog_f = fopen(_name_s, "w");
        if (! _dlog->_dlog_f)
            return false;
    }
    else 
        _dlog->_dlog_f = stderr;

    return true;
}

bool dlog_decons (dlog *_dlog) {
    if (! _dlog)
        return false;

    if (_dlog->_dlog_f != stderr)
        fclose(_dlog->_dlog_f);

    _dlog->_dlog_f = stderr;
    return true;
}

void dlog_debug (dlog const *_dlog, char const *s) {
    #ifndef NO_DEBUG_MSGS
        fprintf(_dlog ? _dlog->_dlog_f : stderr, "> %s\n", s);
        fflush(_dlog ? _dlog->_dlog_f : stderr);
    #endif
}
void dlog_debug_s (dlog const *_dlog, char const *s, char const *s2) {
    #ifndef NO_DEBUG_MSGS
        fprintf(_dlog ? _dlog->_dlog_f : stderr, "> %s: %s\n", s, s2);
        fflush(_dlog ? _dlog->_dlog_f : stderr);
    #endif
}
void dlog_debug_f (dlog const *_dlog, char const *s, float f) {
    #ifndef NO_DEBUG_MSGS
        fprintf(_dlog ? _dlog->_dlog_f : stderr, "> %s: %f\n", s, f);
        fflush(_dlog ? _dlog->_dlog_f : stderr);
    #endif
}
void dlog_debug_d (dlog const *_dlog, char const *s, double d) {
    #ifndef NO_DEBUG_MSGS
        fprintf(_dlog ? _dlog->_dlog_f : stderr, "> %s: %lf\n", s, d);
        fflush(_dlog ? _dlog->_dlog_f : stderr);
    #endif
}
void dlog_debug_u (dlog const *_dlog, char const *s, uint32_t u) {
    #ifndef NO_DEBUG_MSGS
        fprintf(_dlog ? _dlog->_dlog_f : stderr, "> %s: %u\n", s, u);
        fflush(_dlog ? _dlog->_dlog_f : stderr);
    #endif
}
void dlog_debug_i (dlog const *_dlog, char const *s, int32_t i) {
    #ifndef NO_DEBUG_MSGS
        fprintf(_dlog ? _dlog->_dlog_f : stderr, "> %s: %d\n", s, i);
        fflush(_dlog ? _dlog->_dlog_f : stderr);
    #endif
}
void dlog_error (dlog const *_dlog, char const *s) {
    fprintf(_dlog ? _dlog->_dlog_f : stderr, "ERROR: %s\n", s);
    fflush(_dlog ? _dlog->_dlog_f : stderr);
}
void dlog_error_s (dlog const *_dlog, char const *s, char const *s2) {
    fprintf(_dlog ? _dlog->_dlog_f : stderr, "ERROR: %s: %s\n", s, s2);
    fflush(_dlog ? _dlog->_dlog_f : stderr);
}
void dlog_error_f (dlog const *_dlog, char const *s, float f) {
    fprintf(_dlog ? _dlog->_dlog_f : stderr, "ERROR: %s: %f\n", s, f);
    fflush(_dlog ? _dlog->_dlog_f : stderr);
}
void dlog_error_d (dlog const *_dlog, char const *s, double d) {
    fprintf(_dlog ? _dlog->_dlog_f : stderr, "ERROR: %s: %f\n", s, d);
    fflush(_dlog ? _dlog->_dlog_f : stderr);
}
void dlog_error_u (dlog const *_dlog, char const *s, uint32_t u) {
    fprintf(_dlog ? _dlog->_dlog_f : stderr, "ERROR: %s: %u\n", s, u);
    fflush(_dlog ? _dlog->_dlog_f : stderr);
}
void dlog_error_i (dlog const *_dlog, char const *s, int32_t i) {
    fprintf(_dlog ? _dlog->_dlog_f : stderr, "ERROR: %s: %d\n", s, i);
    fflush(_dlog ? _dlog->_dlog_f : stderr);
}

