#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>

typedef enum {
    PLAYER_ONE_TOKEN,
    PLAYER_TWO_TOKEN,
    NO_ONE_TOKEN 
} type_token;

int grid[6][7];

int main(int arg, char *argv[]) {
    initscr();
    raw();
    noecho();
    int i, j;
    for(i = 0; i < 6; i++) {
        for(j = 0; j < 7; j++) {
            grid[i][j] = NO_ONE_TOKEN;
        }
    }

    return 0;
}   
