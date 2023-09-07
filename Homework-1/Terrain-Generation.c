#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
    // Set seed
    srand(time(0));
    // Generate board
    char *terrain[21][80];
    int x, y;
    // Generate top-bottom border
    for(x = 0; x < 80; x++) {
        terrain[0][x] = "\%";
        terrain[20][x] = "\%";
    }
    // Generate Side borders and central parts
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
    // Randomly Generate tall grass;
    int random_start_x = (rand()) % (70 - 1 + 1) + 1;
    int random_start_y = (rand()) % (15 - 3 + 1) + 1;
    // Range of 5-40
    int random_length_x = (rand()) % (40 - 5 + 1) + 5;
    // Range of 5-15
    int random_length_y = (rand()) % (15 - 5 + 1) + 5;
    for(y = random_start_y; y < random_start_y + random_length_y; y++) {
        // Stop generating if reached boarder of grid
        if(y == 20) {
            break;
        }
        for(x = random_start_x; x < random_length_x + random_start_x; x++) {
            // Stop generating if reached boarder of grid
            if(x == 79) {
                break;
            }
            terrain[y][x] = ":";
        }
    }
    // Print Grid
    for(x = 0; x < 21; x++) {
        for(y = 0; y < 80; y++) {
            printf("%s", terrain[x][y]);
        }
        printf("\n");
    }

}