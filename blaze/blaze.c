#include "blaze.h"

bool blaze_cons (blaze *_b) {
    _b->__p = _;
    _b->item_n = 0;

    _b->blk_len = 8192;
    _b->blk_n = 0;
    _b->auto_compact_ = false;
    return true;
}

bool blaze_decons (blaze *_b) {
    if (_b->__p) {
       if (! blaze_clear (_b, free))
           return false;
    }
    return true;
}

// NOTE: order is significant
bool blaze_add (blaze *_b, void *_v) {
    if (_b->item_n == (_b->blk_len * _b->blk_n)) {
        _b->blk_n ++;

        void **__v = (void **) realloc(_b->__p, 
            (_b->blk_len * _b->blk_n) * sizeof(void *));

        if (! __v)
            return false;

        _b->__p = __v;
    }

    *(_b->__p + _b->item_n) = _v;

    _b->item_n ++;
    return true;
}

bool blaze_insert (blaze *_b, void *_v, uint32_t i) {
    if (i > _b->item_n) 
        return false;

    void **__v;

    if (_b->item_n == _b->blk_len * _b->blk_n) {
        _b->blk_n ++;

        __v = (void **) realloc(_b->__p, 
            (_b->blk_len * _b->blk_n) * sizeof(void *));

        if (! __v)
            return false;

        _b->__p = __v;
    }

    if (_b->item_n) {
        __v = _b->__p + _b->item_n;
        for (uint32_t c = _b->item_n; c > i; c--) {
            *__v = *(__v - 1);
            __v -= 1; 
        }
    }

    *(_b->__p + i) = _v;

    _b->item_n ++;
    return true;
}

void *blaze_get (blaze const *_b, uint32_t i) {
    if (! _b->item_n)
        return _;
    else if (i >= _b->item_n) 
        return _;

    return *(_b->__p + i);
}

bool blaze_del (blaze *_b, void *_v, uint32_t i, void (* free_fn)(void *)) {
    if ((! _b->item_n) || (i >= _b->item_n))
        return false;

    void **__v;
    uint32_t c, found_id;

    if (_v) {
        bool found_ = false;
        __v = _b->__p;
        for (c = 0; c < _b->item_n; c ++) {
            if (*__v == _v) {
                found_ = true;
                break;
            }
            __v ++;
        }
        if (! found_) 
            return false;

        found_id = c;
        if (free_fn) 
            free_fn (_v);
    }
    else {
        found_id = i;
        if (free_fn) 
            free_fn (*(_b->__p + i));
    }

    __v = _b->__p + found_id;
    for (c = found_id; c < (_b->item_n -1); c ++) {
        *__v = *(__v + 1);
        __v ++;
    }

    _b->item_n --;

    if (_b->auto_compact_) 
        blaze_compact (_b);

    return true;
}

bool blaze_clear (blaze *_b, void (* free_fn)(void *)) {
    if (! _b->__p) {
        if (_b->blk_n || _b->item_n)
            return false;
        else
            return true;
    }

    if (free_fn && (_b->item_n > 0)) {
        void **__v = _b->__p;

        for (uint32_t c = 0; c < _b->item_n; c ++) {
            free_fn (*__v);
            __v ++;
        }
    }

    free(_b->__p);
    _b->__p = _;

    _b->blk_n = 0;
    _b->item_n = 0;
    return true;
}

bool blaze_compact (blaze *_b) {
    if (! _b->blk_n)
        return false;

    if (_b->item_n) {
        if (_b->blk_len * (_b->blk_n -1) >= _b->item_n) {
            _b->blk_n --;

            void **__v = (void **) realloc(_b->__p, 
                (_b->blk_len * _b->blk_n) * sizeof(void **));

            if (__v)
                _b->__p = __v;
            else 
                return false;
        }
    }
    else {
        _b->blk_n = 0;

        free(_b->__p);
        _b->__p = _;
    }

    return true;
}

void *blaze_first (blaze *_b) {
    if (! _b->item_n)
        return _;

    _b->iter_c = 0;
    _b->__iter = _b->__p;
    return *(_b->__p);
}

void *blaze_next (blaze *_b) {
    _b->iter_c ++;
    _b->__iter ++;
    return blaze_cur (_b);
}

void *blaze_cur (blaze *_b) {
    if (_b->iter_c >= _b->item_n)
        return _;

    return *(_b->__iter);
}

