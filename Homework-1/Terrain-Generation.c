#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

void initalize_grid(char *terrain[21][80]) {
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
}
void generate_terrain(char *terrain[21][80], char *tile, bool gen_twice) {
    int x, y;
    // Randomly Generate tall grass;
    int random_start_x = (rand()) % (70 - 1 + 1) + 1;
    // Range of 5-40
    int random_length_x = (rand()) % (40 - 5 + 1) + 5;
    
    int random_start_y = (rand()) % (15 - 3 + 1) + 1;
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
            terrain[y][x] = tile;
        }
    }
    if(!gen_twice) {
        return;
    }
    // Get random X ends
    int rand_x_end = random_start_x + random_length_x;
    // Reroll start X
    if(random_start_x > 35) {
        random_start_x = (rand()) % (23 - 1 + 1) + 1;
    }
    
    else {
        random_start_x = (rand()) % (70 -(rand_x_end + 1) + 1) + (rand_x_end + 1);
    }

    // Range of 5-11
    random_length_x = (rand()) % (11 - 5 + 1) + 5;
    
    random_start_y = (rand()) % (15 - 3 + 1) + 1;
    // Range of 5-15
    random_length_y = (rand()) % (15 - 5 + 1) + 5;
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
            terrain[y][x] = tile;
        }
    }
}

int main(int argc, char *argv[]) {
    // Set seed
    srand(time(0));
    // Generate board
    char *terrain[21][80];
    initalize_grid(terrain);
    // Generate Grass
    generate_terrain(terrain, *&":", true);
    // Generate Water
    generate_terrain(terrain, *&"~", false);
    // Generate Rocks
    generate_terrain(terrain, *&"\%", false);
    // Generate Trees
    generate_terrain(terrain, *&"\"", false);
    int x, y;
    // Print Grid
    for(x = 0; x < 21; x++) {
        for(y = 0; y < 80; y++) {
            printf("%s", terrain[x][y]);
        }
        printf("\n");
    }
}
