#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// *((int *) p)
void insertion_sort(void *a, int n, int s, 
                    int (*compare)(const void *, const void *)) {
    int i, j;
    char *p;
    p = a;
    void *t;

    t = malloc(s);

    // p + (s * i);
    for(i = 1; i < n; i++) {
        for(memcpy(t, p + (s * i), s), j = i - 1; 
        j > -1 && compare(p + (s * j), t) > 0; j--) {
            memcpy(p + (s * (j + 1)), p + (s * j), s);
        }
        memcpy(p + (s * (j + 1)), t, s);
    }
    free(t);
}

void insertion_sort_int(int *a, int n) {
    int i, j, t;
    for(i = 1; i < n; i++) {
        for(t = a[i], j = i - 1; j > -1 && a[j] > t; j--) {
            a[j + 1] = a[j];
        }
        a[j + 1] = t;
    }
}

int compare_int(const void *a, const void *b) {
    return *((int *) a) - *((int *) b);
}

int compare_char(const void *a, const void *b) {
    return *((char *) a) - *((char *) b);
}

int compare_string(const void *a, const void *b) {
    return strcmp(*(char **) a, *(char **) b);
}

int main(int argc, char *argv[]) {
    int a[] = {10, 9, 8, 7, 6, 5, 4 , 3, 2, 1};
    int i;
    char s[] = "Hello World!";
    char *t[] = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
        "ten"
    };
    insertion_sort(a, sizeof(a) / sizeof(a[0]), sizeof(a[0]), compare_int);
    for(i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
        printf("%d\n", a[i]);
    }
    // Strings have null byte
    insertion_sort(s, sizeof(s) / sizeof(s[0]) - 1, sizeof(s[0]), compare_char);
    printf("%s\n", s);

    insertion_sort(t, sizeof(t) / sizeof(t[0]), sizeof(t[0]), compare_string);
    for(i = 0; i < sizeof(t) / sizeof(t[0]); i++) {
        printf("%s\n", t[i]);
    }
    return 0;
}