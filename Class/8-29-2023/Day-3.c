#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    srand(time(NULL)); // Call srand exactly once! 
    //Probably in main, or in an initiation function that is called from main
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", RAND_MAX);
    printf("%d\n", rand() % 11); // 0-10
    printf("%d\n", rand() % 10 + 1) // 1-10
    return 0;
}