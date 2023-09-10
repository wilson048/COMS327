#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int tileType;
} Voroni_Point;

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
void generate_path_and_shops(char *terrain[21][80]) {
    int start_index, end_index, shift_index;
    // Generate West-East path
    start_index = (rand()) % (19 - 1 + 1) + 1;
    end_index = (rand()) % (19 - 1 + 1) + 1;
    // Prevent straight paths
    while(start_index == end_index) {
        start_index = (rand()) % (19 - 1 + 1) + 1;
        end_index = (rand()) % (19 - 1 + 1) + 1;
    }
    shift_index = end_index = (rand()) % (60 - 30 + 1) + 30;
    int x, y;
    // Generate start of path
    for(x = 0; x < shift_index; x++) {
        terrain[start_index][x] = "#";
    }
    // Generate North-South offset upward
    if(start_index > end_index) {
        for(y = start_index; y > end_index; y--) {
            terrain[y][shift_index] = "#";
        }
    }
    // Generate North-South offset downward
    else {
        for(y = start_index; y < end_index; y++) {
            terrain[y][shift_index] = "#";
        }
    }
    // Generate end of path
    for(x = shift_index; x < 80; x++) {
        terrain[end_index][x] = "#";
    }
    // Generate North-South path
    start_index = (rand()) % (79 - 1 + 1) + 1;
    end_index = (rand()) % (79 - 1 + 1) + 1;
    // Prevent straight paths
    while(start_index == end_index) {
        start_index = (rand()) % (19 - 1 + 1) + 1;
        end_index = (rand()) % (19 - 1 + 1) + 1;
    }
    shift_index = end_index = (rand()) % (60 - 30 + 1) + 30;
    // Generate start of path
    for(y = 0; y < shift_index; y++) {
        terrain[y][start_index] = "#";
    }
    // Generate North-South offset right
    if(start_index > end_index) {
        for(x = start_index; x < end_index; x++) {
            terrain[shift_index][x] = "#";
        }
    }
    // Generate North-South offset left
    else {
        for(x = start_index; x > end_index; x--) {
            terrain[shift_index][x] = "#";
        }
    }
    // Generate end of path
    for(y = shift_index; y < 21; y++) {
        terrain[y][end_index] = "#";
    }


}

void generate_voronoi_terrain(char *terrain[21][80]) {
    int x, y, z;
    // Terrain types defined by numbers
    int terrain_types[12] = {1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    // Points are in the form [x, y, terrain_type]
    Voroni_Point points[12];
    // Generate voronoi seed points
    for(x = 0; x < 12; x ++) {
        // Set voronoi x to 1-79
        points[x].x = (rand()) % (79 - 1 + 1) + 1;
        // Set voronoi y to 1-19
        points[x].y = (rand()) % (19 - 1 + 1) + 1;
        // Set voronoi terrain to a predetermined int
        points[x].tileType = terrain_types[x];
    }

    // 1 = short grass (.), 2 = tall grass (:), 3 = water (~), 4 = mountains (%), 5 = trees (^)
    for(y = 1; y < 20; y++) {
        for(x = 1; x < 79; x++) {
            int point_index = 0;
            double closest_distance = sqrt((double) ((points[0].x - x) * (points[0].x - x)) + ((points[0].y - y) * (points[0].y - y)));
            for(z = 1; z < 12; z++) {
                double distance = sqrt((double) ((points[z].x - x) * (points[z].x - x)) + ((points[z].y - y) * (points[z].y - y)));;
                if(distance < closest_distance) {
                    closest_distance = distance;
                    point_index = z;
                }
            }
            switch (points[point_index].tileType) {
                case 1:
                    terrain[y][x] = ".";
                    break;
                case 2:
                    terrain[y][x] = ":";
                    break;
                case 3:
                    terrain[y][x] = "~";
                    break;
                case 4:
                    terrain[y][x] = "\%";
                    break;
                case 5:
                    terrain[y][x] = "^";
                    break;    
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // Set seed
    srand(time(0));
    // Generate board
    char *terrain[21][80];
    int x, y;
    int **m; 
    m = malloc(sizeof(terrain));
    initalize_grid(terrain);
    generate_voronoi_terrain(terrain);

    generate_path_and_shops(terrain);

    // Print Grid
    for(x = 0; x < 21; x++) {
        for(y = 0; y < 80; y++) {
            printf("%s", terrain[x][y]);
        }
        printf("\n");
    }

    free(m);
}
