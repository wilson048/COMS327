#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(int *a, int n) {
    int i, j, t;
    for(i = 1; i < n; i++) {
        for(t = a[i], j = i - 1; j > -1 && a[j] > t; j--) {
            a[j + 1] = a[j];
        }
        a[j + 1] = t;
    }
}

int main(int argc, char *argv[]) {
    int a[] = {10, 9, 8, 7, 6, 5, 4 , 3, 2, 1}l
    int i;
    insertion_sort(a, sizeof(a) / sizeof(a[0]));
    for(i = 0; i < sizeof(a) / sizeof(a[0]); i++) {

    }
}