#include <stdio.h>
void wrong_swap(int a, int b) {
    int tmp;
    tmp = a;
    a = b;
    b = tmp;
    printf("Inside wrong_swap: i = %d, j = %d\n", a, b);
    printf("Address of i = %p, j = %p\n", &a, &b);
}

void swap(int *a, int *b) {
    int tmp;
    tmp = *a; // * is the dereference operator
    *a = *b; // It gives the user the value stored at an address (i.e. behind a pointer)
    *b = tmp;
}
void swap_pointers(int **a, int **b) {
    int *tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
// Records in C are called structs. 
struct foo {
    int i;
    float f;
    char *s;
};

void init_foo(struct foo *f, int i, float g, char *s) {
    f->i = i;
    f->f = g;
    f->s = s;
}
int main(int argc, char *argv[]) {
    int i;
    int a[50]; // To read declarations, go right when you can, left when you must
    int *p; // p is a pointer to int. A pointer is a variable that holds an address
    int j;
    struct foo f;


    printf("i is located at this address%p\n", &i);
    printf("a is located at this address%p\n", a);
    printf("a[0] is located at this address%p\n", &a[0]);
    p = &i;
    printf("p holds the address %p\n", p);
    printf("p is located at address %p\n", &p);
    i = 0;
    j = 1;
    printf("Before wrong_swap: i = %d, j = %d\n", i, j);
    wrong_swap(i, j);
    printf("After wrong_swap: i = %d, j = %d\n", i, j);
    printf("Address of i = %p, j = %p\n", &i, &j);
    printf("Before swap: i = %d, j = %d\n", i, j);
    swap(&i, &j);
    printf("After swap: i = %d, j = %d\n", i, j);
    printf("Address of i = %p, j = %p\n", &i, &j);
    for(i = 0; i < 50; i++) {
        a[i] = i;
    }
    // p++ <-> p = p + 1 (go to next int in a)
    for(p = a; p < a + 50; p++) {
        printf("%d\n",  *p);
    }

    for(p = a, i = 0; i < 50; i++) {
        // Array index notation works on pointers, too
        printf("%d\n",  p[i]);
    }

    for(p = a + 25, i = -25; i < 25; i++) {
        printf("%d\n", p[i]);
    }

    f.i = 0;
    f.f = 3.14;
    f.s = "Hello.c";

    printf("Printing f: i = %d, f = %f, s = %s\n", f.i, f.f, f.s);
    init_foo(&f, 1, 1.68, "Goodbye.c");
    printf("Printing f: i = %d, f = %f, s = %s\n", f.i, f.f, f.s);
    p = &a[0];
    q = &a[1];

    printf("p - %p, q = %p\n", p, q);

    swap_pointers(&p, &q);

    printf("p - %p, q = %p\n", p, q);
    return 0;
}