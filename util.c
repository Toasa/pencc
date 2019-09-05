#include "util.h"

void error(char *err_msg, ...) {
    va_list ap;
    va_start(ap, err_msg);
    vfprintf(stderr, err_msg, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

void assert(bool cond, char *err_msg, ...) {
    if (!cond) {
        error(err_msg);
    }
}