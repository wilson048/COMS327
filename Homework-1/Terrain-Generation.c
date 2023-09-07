#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    // Generate board
    char *terrain[21][80];
    int x, y;
    // Generate top-bottom border
    for(x = 0; x < 80; x++) {
        terrain[0][x] = "\%";
        terrain[20][x] = "\%";
    }

    for(x = 1; x < 20; x++) {
        for(y = 0; y < 80; y++) {
            if(y == 0 || y == 79) {
                terrain[x][y] = "\%";
            }
            else {
                terrain[x][y] = ".";
            }
        }
    }

    // Print Grid
    for(x = 0; x < 21; x++) {
        for(y = 0; y < 80; y++) {
            printf("%s ", terrain[x][y]);
        }
        printf("\n");
    }
    
}