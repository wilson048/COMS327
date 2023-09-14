#include <stdio.h>

char *strcpy327(char *dest, const char *src) {
    int i;
    for(i = 0; src[i]; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';

    return dest;
}

int main(int argc, char *argv[]) {
    char *s = "Foo";
    char t[] = "Bar";
    char x[100];

    t[0] = 'C';
    printf("%s\n", t);

    strcpy327(x, s);
    printf("%s\n", x);
    return 0;
}