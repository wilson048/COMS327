/* Two kinds of macros, value-type and fucntion-type*/
#define PI 3.141592654

#define add(x, y) (x + y)

#define min(x, y) (x < y ? x : y)

#define max(x, y) ({ \
    typeof (x) _x = x; \
    typeof (y) _x = y; \
    _x > _ y : _x : _y; \
})
#define stringify(x) #x

struct lutable_entry {
    char *name;
    void (*func)();
};

#define build_entry(x) #x x

struct lutable_entry lookup_table[] = {
    /*
    {"a", a}, 
    {"c", c},
    {"foo", foo},
    {"go", go},
    {"stop", stop}
    */
    build_entry(a),
    build_entry(c),
    build_entry(foo),
    build_entry(go),
    build_entry(stop),
}

bsearch("c", lookup_table, num elements in table, size of an element, comparitor)->func()

PI;
#define concatenate(x) x ## foo

add(PI, 10) * 2;
// Don't pass functions, pass variables / constants
min(really_expensive_function(), function_wtih_side_effects());

stringify(Hello World!);

concatenate(Hello);

enum trainer_type {
    hiker,
    rival,
    swimmer,
};

#define move(x) trainer_move_func[trainer_ # x]()

move(rival);

#define FOO
#ifndef FOO_H
#define FOO_H

#endif