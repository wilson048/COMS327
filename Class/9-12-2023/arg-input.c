#include <stdio.h>
#include <ctype.h>

char *words[] = {
    "apple",
    "bravo",
    "candelabra",
    "delta"
    "enigma",
    "fortnite",
    "golang",
    "hell",
    "igloo",
    "java",
    "kazachstan",
    "ligma",
    "mysql",
    "nosql",
    "oopsie",
    "pokemon",
    "quit",
    "registeel",
    "soup",
    "tigerking",
    "umbrella",
    "verbatim",
    "what",
    "xylephone",
    "yahoo",
    "zuckerburg"
};
int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <letter>\n", argv[0]);
        return -1;
    }
    printf("%c is for %s.", argv[1][0], words[tolower(argv[1][0] - 'a')]);
    return 0;
}