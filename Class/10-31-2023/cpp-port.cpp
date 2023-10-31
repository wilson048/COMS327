#include "cpp-port.h"

ostream *get_cout() {
    return &cout;
}

void use_cout(ostream *o, const char *s) {
    *o << s;
}