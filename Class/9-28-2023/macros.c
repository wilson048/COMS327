/* Two kinds of macros, value-type and fucntion-type*/
#define PI 3.141592654

#define add(x, y) (x + y)

#define min(x, y) (x < y ? x : y)

#define max(x, y) ({ \
    typeof (x) _x = x; \
    typeof (y) _x = y; \
    _x > _ y : _x : _y; \
})

PI;

add(PI, 10) * 2;
// Don't pass functions, pass variables / constants
min(really_expensive_function(), function_wtih_side_effects());