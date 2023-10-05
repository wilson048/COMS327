#include <stdio.h>

static int foo; // Static globals are scope-limited to the "compliation unit" (file)
                // It cannot be accessed outside of this file (linker doesn't export the symbol)
                // Also applicable to functions

int func() {
    static int num_calls; // zero by placement ("by placement" because the linker puts this variable in the "data segement")
                          // Which is initialized to all bits 0
    return ++num_calls;
}
int main(int argc, char *argv[]) {
    int x;
    for(x = 0; x < 1000; x++) {
        printf("%d\n", func());
    }
    return 0;
}