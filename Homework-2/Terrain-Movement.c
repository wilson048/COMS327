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

typedef struct {
    char *terrain[21][80];
    int h_start_index;
    int h_end_index;
    int v_start_index;
    int v_end_index;
    int **m;
} Local_Map;

// void generate_path_and_shops(char *terrain[21][80]) {
//     int h_start_index, h_end_index, h_shift_index;
//     // Generate West-East path
//     h_start_index = (rand()) % (18 - 2 + 1) + 2;
//     h_end_index = (rand()) % (18 - 2 + 1) + 2;
//     // Prevent straight paths
//     while(h_start_index == h_end_index) {
//         h_start_index = (rand()) % (18 - 2 + 1) + 2;
//         h_end_index = (rand()) % (18 - 2 + 1) + 2;
//     }
//     h_shift_index = (rand()) % (60 - 30 + 1) + 30;
//     int x, y;
//     // Generate start of path
//     for(x = 0; x < h_shift_index; x++) {
//         terrain[h_start_index][x] = "#";
//     }
//     // Generate North-South offset upward
//     if(h_start_index > h_end_index) {
//         for(y = h_start_index; y > h_end_index; y--) {
//             terrain[y][h_shift_index] = "#";
//         }
//     }
//     // Generate North-South offset downward
//     else {
//         for(y = h_start_index; y < h_end_index; y++) {
//             terrain[y][h_shift_index] = "#";
//         }
//     }
//     // Generate end of path
//     for(x = h_shift_index; x < 80; x++) {
//         terrain[h_end_index][x] = "#";
//     }
//     int v_start_index, v_end_index, v_shift_index;
//     // Generate North-South path
//     v_start_index = (rand()) % (78 - 2 + 1) + 2;
//     v_end_index = (rand()) % (78 - 2 + 1) + 2;
//     // Prevent straight paths
//     while(v_start_index == v_end_index) {
//         v_start_index = (rand()) % (78 - 1 + 1) + 1;
//         v_end_index = (rand()) % (78 - 1 + 1) + 1;
//     }
//     v_shift_index = v_end_index = (rand()) % (15 - 5 + 1) + 5;
//     // Generate start of path
//     for(y = 0; y < v_shift_index; y++) {
//         terrain[y][v_start_index] = "#";
//     }
//     // Generate North-South offset right
//     if(v_start_index > v_end_index) {
//         for(x = v_start_index; x > v_end_index; x--) {
//             terrain[v_shift_index][x] = "#";
//         }
//     }
//     // Generate North-South offset left
//     else {
//         for(x = v_start_index; x < v_end_index; x++) {
//             terrain[v_shift_index][x] = "#";
//         }
//     }
//     // Generate end of path
//     for(y = v_shift_index; y < 21; y++) {
//         terrain[y][v_end_index] = "#";
//     }
//     // Left/North on 1, Right/South on 2
//     int mart_orientation = (rand()) % (2 - 1 + 1) + 1;
//     int pkm_orientation = (rand()) % (2 - 1 + 1) + 1;
//     // Horizontal on 1, Vertical on 2
//     int path_orientation = (rand()) % (2 - 1 + 1) + 1;
//     int m_location, pkm_location;
//     // Generate on Horizontal path
//     if(path_orientation == 1) {
//         m_location = (rand()) % (78 - 2 + 1) + 2;
//         while(m_location == h_shift_index || m_location == v_start_index || m_location == v_end_index) {
//             m_location = (rand()) % (78 - 2 + 1) + 2;
//         }
//         if(mart_orientation == 1) {
//             // Generate mart at start index side, north of the horizontal path
//             if(m_location > h_shift_index)  {
//                 terrain[h_end_index - 1][m_location] = "M";
//             }
//             // Generate mart at end index side, north of the horizontal path
//             else {
//                 terrain[h_start_index - 1][m_location] = "M";
//             }
//         }
//         else {
//             // Generate mart at start index side, south of the horizontal path
//             if(m_location > h_shift_index)  {
//                 terrain[h_end_index + 1][m_location] = "M";
//             }
//             // Generate mart at end index side, south of the horizontal path
//             else {
//                 terrain[h_start_index + 1][m_location] = "M";
//             }
//         }
//         // Set pokemon center location
//         pkm_location = (rand()) % (78 - 2 + 1) + 2;
//         while(pkm_location == h_shift_index || pkm_location == m_location || pkm_location == v_start_index || pkm_location == v_end_index) {
//             pkm_location = (rand()) % (78 - 2 + 1) + 2;
//         }
//         if(pkm_orientation == 1) {
//             // Generate PokeCenter at start index side, north of the horizontal path
//             if(pkm_location > h_shift_index)  {
//                 terrain[h_end_index - 1][pkm_location] = "P";
//             }
//             // Generate PokeCenter at end index side, north of the horizontal path
//             else {
//                 terrain[h_start_index - 1][pkm_location] = "P";
//             }
//         }
//         else {
//             // Generate PokeCenter at start index side, south of the horizontal path
//             if(pkm_location > h_shift_index)  {
//                 terrain[h_end_index + 1][pkm_location] = "P";
//             }
//             // Generate PokeCenter at end index side, south of the horizontal path
//             else {
//                 terrain[h_start_index + 1][pkm_location] = "P";
//             }
//         }   
//     }
//     else {
//         m_location = (rand()) % (18 - 2 + 1) + 2;
//         while(m_location == v_shift_index || m_location == h_start_index || m_location == h_end_index) {
//             m_location = (rand()) % (18 - 2 + 1) + 2;
//         }
//         if(mart_orientation == 1) {
//             // Generate mart at start index side, left of the vertical path
//             if(m_location < v_shift_index)  {
//                 terrain[m_location][v_start_index - 1] = "M";
//             }
//             // Generate mart at end index side, left of the vertical path
//             else {
//                 terrain[m_location][v_end_index - 1] = "M";
//             }
//         }
//         else {
//             // Generate mart at start index side, right of the vertical path
//             if(m_location < v_shift_index)  {
//                 terrain[m_location][v_start_index + 1] = "M";
//             }
//             // Generate mart at end index side, right of the vertical path
//             else {
//                 terrain[m_location][v_end_index + 1] = "M";
//             }
//         }
//         // Set pokemon center location
//         pkm_location = (rand()) % (18 - 2 + 1) + 2;
//         while(pkm_location == v_shift_index || pkm_location == m_location || pkm_location == h_start_index || pkm_location == h_end_index) {
//             pkm_location = (rand()) % (18 - 2 + 1) + 2;
//         }
//         if(pkm_orientation == 1) {
//             // Generate PokeCenter at start index side, left of the vertical path
//             if(pkm_location < v_shift_index)  {
//                 terrain[pkm_location][v_start_index - 1] = "P";
//             }
//             // Generate PokeCenter at end index side, left of the vertical path
//             else {
//                 terrain[pkm_location][v_end_index - 1] = "P";
//             }
//         }
//         else {
//             // Generate PokeCenter at start index side, right of the vertical path
//             if(pkm_location > h_shift_index)  {
//                 terrain[pkm_location][v_start_index + 1] = "P";
//             }
//             // Generate PokeCenter at end index side, south of the vertical path
//             else {
//                 terrain[pkm_location][v_end_index + 1] = "P";
//             }
//         }
//     }

// }

void generate_voronoi_terrain(Local_Map *map) {
    int x, y, z;
    if(!(map == malloc(sizeof(map)))) {
        return;
    }
    if(!(map->terrain == malloc(sizeof(map->terrain)))) {
        return;
    }
    // Terrain types defined by numbers
    int terrain_types[12] = {1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    // Points are in the form [x, y, terrain_type]
    Voroni_Point points[12];
    // Generate voronoi seed points
    for(x = 0; x < 12; x ++) {
        // Set voronoi x to 1-79
        points[x].x = (rand()) % (81);
        // Set voronoi y to 1-19
        points[x].y = (rand()) % (21);
        // Set voronoi terrain to a predetermined int
        points[x].tileType = terrain_types[x];
    }

    // 1 = short grass (.), 2 = tall grass (:), 3 = water (~), 4 = mountains (%), 5 = trees (^)
    for(y = 0; y < 21; y++) {
        for(x = 0; x < 80; x++) {
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
                    map->terrain[y][x] = ".";
                    break;
                case 2:
                    map->terrain[y][x] = ":";
                    break;
                case 3:
                    map->terrain[y][x] = "~";
                    break;
                case 4:
                    map->terrain[y][x] = "\%";
                    break;
                case 5:
                    map->terrain[y][x] = "^";
                    break;    
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // The world map
    Local_Map *world_map[401][401];
    
    // Set seed
    srand(time(0));
    // Generate board
    int current_x = 200;
    int current_y = 200;
    int x, y;
    // Print Grid
    generate_voronoi_terrain(world_map[current_y][current_x]);
    char input = ' ';
    while(input != 'q') {
        for(x = 0; x < 21; x++) {
            for(y = 0; y < 80; y++) {
                printf("%s", world_map[current_y][current_x]->terrain[x][y]);
            }
        printf("\n");
        }
        scanf("%c", &input);
    }
    free(world_map[200][200]->terrain);
    free(world_map[200][200]);
    return 0;
}