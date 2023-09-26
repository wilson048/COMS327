#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_VAL 185
void populate_times_table(int table[MAX_VAL][MAX_VAL]) {
    int i, j;
    for (j = 0; j < MAX_VAL; j++) {
        for (i = 0; i < MAX_VAL; i++) {
            table[j][i] = (j + 1) * (i + 1);
        }
    }
}

/*
    Recursion
*/
unsigned int fib(unsigned n) {
    if(n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char *argv[]) {
    int a[MAX_VAL][MAX_VAL];

    populate_times_table(a);
    int i, j;
    for (j = 0; j < MAX_VAL; j++) {
        for (i = 0; i < MAX_VAL; i++) {
            printf("%*d ", (int) floor(log(a[MAX_VAL - 1][MAX_VAL - 1]) + 1), a[j][i]);
        }
        printf("\n");
    }
    /*
    Print error to standard error file
    */
    if(argc != 2 || argv[1][0] == '-') {
        fprintf(stderr, "Usage: %s <positive number>\n", argv[0]);
        return -1;
    }
    printf("%u\n", fib(atoi(argv[1])));
    return 0;
}
