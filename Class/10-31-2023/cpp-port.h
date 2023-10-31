#ifndef CPP_PORT_H
#define CPP_PORT_H

// if c++ code
#ifdef __cplusplus
#include <iostream>

using namespace std;

extern "C" {
// If c code
#else 
typedef void ostream;
#endif
ostream *get_cout();
void use_cout(ostream *o, const char *s);

#ifdef __cplusplus
}
#endif

#endif