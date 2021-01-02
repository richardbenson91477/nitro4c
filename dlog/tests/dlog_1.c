#include "../dlog.h"

#define LOG_FNAME "test1.log"

void emit (dlog *_dlog);

int main (void) {
    emit (_);

    dlog *_dlog = malloc(sizeof(dlog));
    if (! dlog_cons (_dlog, LOG_FNAME)) {
        dlog_error_s (_, "open", LOG_FNAME);
        return 1;
    }

    emit (_dlog);

    dlog_decons (_dlog);
    free(_dlog);

    return 0;
}

void emit (dlog *_dlog) {
    dlog_debug (_dlog, "s");
    dlog_debug_s (_dlog, "s", "s2");
    dlog_debug_f (_dlog, "s", 1.0);
    dlog_debug_d (_dlog, "s", 1.0);
    dlog_debug_u (_dlog, "s", -1);
    dlog_debug_i (_dlog, "s", -1);

    dlog_error (_dlog, "s");
    dlog_error_s (_dlog, "s", "s2");
    dlog_error_f (_dlog, "s", 1.0);
    dlog_error_d (_dlog, "s", 1.0);
    dlog_error_u (_dlog, "s", -1);
    dlog_error_i (_dlog, "s", -1);
}

