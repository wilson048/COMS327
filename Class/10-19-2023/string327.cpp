#include <cstring>

#include "string327.h"
string327::string327() {
    str = strdup("");
}
string327::string327(const char *s) {
    str = strdup(s);
} // take a string constructor
string327::string327(const string327 &s) {
    str = strdup (s.str);
} // copy constructor
string327::~string327() {
    free(str);
}  // Destroy function
int string327::length() {
    return strlen(str);
}  // length of string
bool string327::operator < (const string327 &rhs) {
    return strcmp(str, rhs.str) < 0;
}
bool string327::operator > (const string327 &rhs) {
    return strcmp(str, rhs.str) > 0;
}
bool string327::operator == (const string327 &rhs) {
    return strcmp(str, rhs.str) == 0;
}
bool string327::operator != (const string327 &rhs) {
    return strcmp(str, rhs.str) != 0;
}
bool string327::operator <= (const string327 &rhs) {
    return strcmp(str, rhs.str) <= 0;
}
bool string327::operator >= (const string327 &rhs) {
    return strcmp(str, rhs.str) >= 0;
}
string327 string327::operator+(const string327 &rhs) {
    string327 *s;
    free(s.str)
    s.str = malloc(strlen(str) + strlen(rhs.str) + 1); // memory leak
    strcpy(s, str);
    strcat(s, rhs.str);
    return s;
}
string327 string327::operator+(const char *rhs) {

}
string327 string327::&operator+=(const string327 &rhs) {

}
string327 string327::&operator+=(const char *rhs) {

}
string327 string327::&operator=(const string327 &rhs) {

}
string327 string327::&operator=(const char *rhs) {

}
char string327::&operator[](const int i) {

}
const string327::char *c_str() {

}

// Friends have accesss to the privates
friend std::istream &operator>>(std::istream &lhs, const string327 &rhs);