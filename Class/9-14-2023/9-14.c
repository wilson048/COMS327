#include <stdio.h>

char *strcpy327(char *dest, const char *src) {
    // int i;
    // for(i = 0; src[i]; i++) {
    //     dest[i] = src[i];
    // }
    // dest[i] = '\0';

    char *tmp = dest;

    while((*tmp++ = *src++));

    return dest;
}
int strcmp327(const char *s1, const char *s2) {
    while(*s1 && *s1++ == *s2++);
    
    return *s1 - *s2;
}
int main(int argc, char *argv[]) {
    char *s = "Foo";
    char t[] = "Bar";
    char x[100];

    t[0] = 'C';
    printf("%s\n", t);

    strcpy327(x, s);
    printf("%s\n", x); 
    printf("%s\n", s); // Foo
    printf("%p\n", &s); // Address of variable s

    char *s1 = "abcd";
    char *s2 = "abcd";
    // Prints s2, which is not a true statement
    printf("%s is bigger\n", strcmp327(s1, s2) > 0 ? s1 : s2);
    // Output of this is undefined
    if(s1 == s2) {
        printf("Equal\n");
    }
    else {
        printf("Not equal\n");
    }
    s1 = "abcd";
    s2 = "abcde";
    printf("%s is bigger\n", strcmp327(s1, s2) > 0 ? s1 : s2);
    // These compare teh addresses stored in the pointers, not the strings! 
    if(s1 > s2) {
        printf("s1 greater\n");
    }
    else {
        printf("s2 greater\n");
    }
    s1 = "abcc";
    s2 = "abcd";
    printf("%s is bigger\n", strcmp327(s1, s2) > 0 ? s1 : s2);
    if(s1 > s2) {
        printf("s1 greater\n");
    }
    else {
        printf("s2 greater\n");
    }
    return 0; 
}