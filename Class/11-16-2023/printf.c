#include <stdio.h>
#include <stdarg.h>


__attribute__((constructor))
void init() {
    printf(__FUNCTION__);
    printf("\n");
}

__attribute__((destructor))
void destory() {
    printf(__FUNCTION__);
    printf("\n");
}

 int printf(const char *format, ...) {
    va_list ap;
    int rval;
    fputs("Im in ur library, manipulating ur bites\n", stdout);
    va_start(ap, format);
    rval = vprintf(format, ap);
    va_end(ap);

    return rval;
}