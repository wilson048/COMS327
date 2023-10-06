#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"


// Enum for representing npc types
typedef enum {
    player,
    hiker,
    rival,
    wanderer,
    sentry,
    explorer,
    pacer,
    swimmer,
    other
} npc_type;
// Voronoi points for generating terrain
typedef struct {
    int x;
    int y;
    int tileType;
} Voroni_Point;

typedef struct characters {
    heap_node_t *hn;
    npc_type type;
    int pos_x;
    int pos_y;
    int cost;
    int sequence_num;
} Character;

// Heap nodes that keep track of cost and previous nodes
typedef struct npc_heap {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
} npc_node_t;
// Heap nodes without the heap nodes, solution to store Dijkstra generations
typedef struct {
  uint8_t x_pos;
  uint8_t y_pos;
  uint8_t x_from;
  uint8_t y_from;
  int32_t cost;
} npc_tile;
// The local map struct
typedef struct {
    char *terrain[21][80];
    int west_index;
    int east_index;
    int north_index;
    int south_index;
    npc_tile *hiker_paths[21][80];
    npc_tile *rival_paths[21][80];
} Local_Map;


// The world map
Local_Map *world_map[401][401];
// Player Coordinates
int playerX = 0;
int playerY = 0;
// Integer Maximum
int32_t INT_MAX = 2000000;
// NPCs to place on the map
npc_type NPCs[11] = {player, hiker, hiker, rival, rival, pacer, pacer, explorer, sentry, wanderer, explorer};
// Character array for collision detection
Character *character_map[21][80];
// Array of characters to malloc
Character *characters[11];


void initalize_grid(Local_Map *map) {
    int x, y;
    // Generate top-bottom border
    for(x = 0; x < 80; x++) {
        map->terrain[0][x] = "\%";
        map->terrain[20][x] = "\%";
    }
    // Generate Side borders and central parts
    for(x = 1; x < 20; x++) {
        for(y = 0; y < 80; y++) {
            if(y == 0 || y == 79) {
                map->terrain[x][y] = "\%";
            }
            else {
                map->terrain[x][y] = ".";
            }
        }
    }
}

void generate_path_and_shops(int mapX, int mapY) {
    // Check for surrounding maps. If no map exists, set a new random start index
    // Northern map
    if(mapY != 0 && world_map[mapY - 1][mapX] != NULL) {
        world_map[mapY][mapX]->north_index = world_map[mapY - 1][mapX]->south_index;
    }
    else {
        // Number from 2 - 78
        world_map[mapY][mapX]->north_index = (rand()) % (78 - 2 + 1) + 2;
    }
    // Southern map
    if(mapY != 400 && world_map[mapY + 1][mapX] != NULL) {
        world_map[mapY][mapX]->south_index = world_map[mapY + 1][mapX]->north_index;
    }
    else {
        // Number from 2 - 78
        world_map[mapY][mapX]->south_index = (rand()) % (78 - 2 + 1) + 2;
    }
    // Western Map
    if(mapX != 0 && world_map[mapY][mapX - 1] != NULL) {
        world_map[mapY][mapX]->west_index = world_map[mapY][mapX - 1]->east_index;
    }
    else {
        // Number from 2 - 18
        world_map[mapY][mapX]->west_index = (rand()) % (18 - 2 + 1) + 2;
    }
    // Eastern Map
    if(mapX != 400 && world_map[mapY][mapX + 1] != NULL) {
        world_map[mapY][mapX]->east_index = world_map[mapY][mapX + 1]->west_index;
    }
    else {
        world_map[mapY][mapX]->east_index = (rand()) % (18 - 2 + 1) + 2;
    }
    // Generate West-East path
    int h_shift_index;
    // Shift at indicie ranging from 30 - 60
    h_shift_index = (rand()) % (60 - 30 + 1) + 30;
    int x, y;

    // Stop start gen if map is on left edge of world
    int start_number = 0;
    if(mapX == 0) {
        start_number++;
    }
    // Generate start of path
    for(x = start_number; x < h_shift_index; x++) {
        // Mark edge paths with "-" to represent gates
        if(x == 0) {
            world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->west_index][x] = "-";
        }
        else {
            world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->west_index][x] = "#";
        }
    }
    // Generate North-South offset upward
    if(world_map[mapY][mapX]->west_index > world_map[mapY][mapX]->east_index) {
        for(y = world_map[mapY][mapX]->west_index; y > world_map[mapY][mapX]->east_index; y--) {
            world_map[mapY][mapX]->terrain[y][h_shift_index] = "#";
        }
    }
    // Generate North-South offset downward
    else {
        for(y = world_map[mapY][mapX]->west_index; y < world_map[mapY][mapX]->east_index; y++) {
            world_map[mapY][mapX]->terrain[y][h_shift_index] = "#";
        }
    }
    // Stop end gen if on right edge of world
    int stop_number = 80;
    if(mapX == 400) {
        stop_number--;
    }
    // Generate end of path
    for(x = h_shift_index; x < stop_number; x++) {
        // Mark edge paths with "-" to represent gates
        if(x == 79) {
            world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->east_index][x] = "-";
        }
        else {
            world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->east_index][x] = "#";
        }
    }


    int v_shift_index;
    // Generate North-South path
    // Shift path from indicie in range of 5 - 15
    v_shift_index = (rand()) % (15 - 5 + 1) + 5;
    // Stop start gen if on top edge of world
    start_number = 0;
    if(mapY == 0) {
        start_number++;
    }
    // Generate start of path
    for(y = start_number; y < v_shift_index; y++) {
        if(y == 0) {
            // Mark edge paths with "-" to represent gates
            world_map[mapY][mapX]->terrain[y][world_map[mapY][mapX]->north_index] = "-";
        }
        else {
            world_map[mapY][mapX]->terrain[y][world_map[mapY][mapX]->north_index] = "#";
        }
    }
    // Generate North-South offset right
    if(world_map[mapY][mapX]->north_index > world_map[mapY][mapX]->south_index) {
        for(x = world_map[mapY][mapX]->north_index; x > world_map[mapY][mapX]->south_index; x--) {
            world_map[mapY][mapX]->terrain[v_shift_index][x] = "#";
        }
    }
    // Generate North-South offset left
    else {
        for(x = world_map[mapY][mapX]->north_index; x < world_map[mapY][mapX]->south_index; x++) {
            world_map[mapY][mapX]->terrain[v_shift_index][x] = "#";
        }
    }
    // Stop south gen end if on south edge of world
    stop_number = 21;
    if(mapY == 400) {
        stop_number--;
    }
    // Generate end of path
    for(y = v_shift_index; y < stop_number; y++) {
        // Mark edge paths with "-" to represent gates
        if(y == 20) {
            world_map[mapY][mapX]->terrain[y][world_map[mapY][mapX]->south_index] = "-";
        } 
        else {
            world_map[mapY][mapX]->terrain[y][world_map[mapY][mapX]->south_index] = "#";
        }
    }
    // Get distance from spawn
    int distance_from_spawn = (int) sqrt((double) ((mapX - 200) * (mapX - 200) + (mapY - 200) * (mapY - 200)));
    // Spawn probability
    int spawn_probability = ((((int) (-45 * (distance_from_spawn)) / 200) + 50));

    // Left/North on 1, Right/South on 2
    int mart_orientation = (rand()) % (2 - 1 + 1) + 1;
    int pkm_orientation = (rand()) % (2 - 1 + 1) + 1;
    // Horizontal on 1, Vertical on 2
    int path_orientation = (rand()) % (2 - 1 + 1) + 1;
    int m_location, pkm_location, m_chance, pkm_chance;
    // Get spawn probability for PokeCenters and PokeMarts
    m_chance = (rand()) % (100 - 1 + 1) + 1;
    pkm_chance = (rand()) % (100 - 1 + 1) + 1;
    // Generate on Horizontal path
    if(path_orientation == 1) {
        m_location = (rand()) % (78 - 2 + 1) + 2;
        while(m_location == h_shift_index || m_location == world_map[mapY][mapX]->north_index || m_location == world_map[mapY][mapX]->south_index) {
            m_location = (rand()) % (78 - 2 + 1) + 2;
        }
        // Generate mart if probability allows it
        if((m_chance < spawn_probability) || ((mapX == 200) && (mapY == 200))) {
            if(mart_orientation == 1) {
                // Generate mart at start index side, north of the horizontal path
                if(m_location > h_shift_index)  {
                    world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->east_index - 1][m_location] = "M";
                }
                // Generate mart at end index side, north of the horizontal path
                else {
                    world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->west_index - 1][m_location] = "M";
                }
            }
            else {
                // Generate mart at start index side, south of the horizontal path
                if(m_location > h_shift_index)  {
                    world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->east_index + 1][m_location] = "M";
                }
                // Generate mart at end index side, south of the horizontal path
                else {
                    world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->west_index + 1][m_location] = "M";
                }
            }
        }
        // Set pokemon center location if probability is there / Guarantee both at spawn
        if((pkm_chance < spawn_probability) || ((mapX == 200) && (mapY == 200))) {
            pkm_location = (rand()) % (78 - 2 + 1) + 2;
            while(pkm_location == h_shift_index || pkm_location == m_location || pkm_location == world_map[mapY][mapX]->north_index || pkm_location == world_map[mapY][mapX]->south_index) {
                pkm_location = (rand()) % (78 - 2 + 1) + 2;
            }
            if(pkm_orientation == 1) {
                // Generate PokeCenter at start index side, north of the horizontal path
                if(pkm_location > h_shift_index)  {
                    world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->east_index - 1][pkm_location] = "P";
                }
                // Generate PokeCenter at end index side, north of the horizontal path
                else {
                    world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->west_index - 1][pkm_location] = "P";
                }
            }
            else {
                // Generate PokeCenter at start index side, south of the horizontal path
                if(pkm_location > h_shift_index)  {
                    world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->east_index + 1][pkm_location] = "P";
                }
                // Generate PokeCenter at end index side, south of the horizontal path
                else {
                    world_map[mapY][mapX]->terrain[world_map[mapY][mapX]->west_index + 1][pkm_location] = "P";
                }
            }
        }   
    }
    else {
        m_location = (rand()) % (18 - 2 + 1) + 2;
        while(m_location == v_shift_index || m_location == world_map[mapY][mapX]->west_index || m_location == world_map[mapY][mapX]->east_index) {
            m_location = (rand()) % (18 - 2 + 1) + 2;
        }
        if((m_chance < spawn_probability) || ((mapX == 200) && (mapY == 200))) {
            if(mart_orientation == 1) {
                // Generate mart at start index side, left of the vertical path
                if(m_location < v_shift_index)  {
                    world_map[mapY][mapX]->terrain[m_location][world_map[mapY][mapX]->north_index - 1] = "M";
                }
                // Generate mart at end index side, left of the vertical path
                else {
                    world_map[mapY][mapX]->terrain[m_location][world_map[mapY][mapX]->south_index - 1] = "M";
                }
            }
            else {
                // Generate mart at start index side, right of the vertical path
                if(m_location < v_shift_index)  {
                    world_map[mapY][mapX]->terrain[m_location][world_map[mapY][mapX]->north_index + 1] = "M";
                }
                // Generate mart at end index side, right of the vertical path
                else {
                    world_map[mapY][mapX]->terrain[m_location][world_map[mapY][mapX]->south_index + 1] = "M";
                }
            }
        }
        // Set pokemon center location
        pkm_location = (rand()) % (18 - 2 + 1) + 2;
        while(pkm_location == v_shift_index || pkm_location == m_location || pkm_location == world_map[mapY][mapX]->west_index || pkm_location == world_map[mapY][mapX]->east_index) {
            pkm_location = (rand()) % (18 - 2 + 1) + 2;
        }
        if((pkm_chance < spawn_probability) || ((mapX == 200) && (mapY == 200))) {
            if(pkm_orientation == 1) {
                // Generate PokeCenter at start index side, left of the vertical path
                if(pkm_location < v_shift_index)  {
                    world_map[mapY][mapX]->terrain[pkm_location][world_map[mapY][mapX]->north_index - 1] = "P";
                }
                // Generate PokeCenter at end index side, left of the vertical path
                else {
                    world_map[mapY][mapX]->terrain[pkm_location][world_map[mapY][mapX]->south_index - 1] = "P";
                }
            }
            else {
                // Generate PokeCenter at start index side, right of the vertical path
                if(pkm_location < v_shift_index)  {
                    world_map[mapY][mapX]->terrain[pkm_location][world_map[mapY][mapX]->north_index + 1] = "P";
                }
                // Generate PokeCenter at end index side, south of the vertical path
                else {
                    world_map[mapY][mapX]->terrain[pkm_location][world_map[mapY][mapX]->south_index + 1] = "P";
                }
            }
        }    
    }
}

void generate_voronoi_terrain(Local_Map *map) {
    int x, y, z;
    // Terrain types defined by numbers
    int terrain_types[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    // Points are in the form [x, y, terrain_type]
    Voroni_Point points[20];
    // Generate voronoi seed points
    for(x = 0; x < 20; x ++) {
        // Set voronoi x to 1-79
        points[x].x = (rand()) % (80 - 1 + 1) + 1;
        // Set voronoi y to 1-19
        points[x].y = (rand()) % (20 - 1 + 1) + 1;
        // Set voronoi terrain to a predetermined int
        points[x].tileType = terrain_types[x];
    }

    // 1 = short grass (.), 2 = tall grass (:), 3 = water (~), 4 = mountains (%), 5 = trees (^)
    for(y = 1; y < 20; y++) {
        for(x = 1; x < 79; x++) {
            int point_index = 0;
            // Find closet point
            double closest_distance = sqrt((double) ((points[0].x - x) * (points[0].x - x)) + ((points[0].y - y) * (points[0].y - y)));
            for(z = 1; z < 20; z++) {
                double distance = sqrt((double) ((points[z].x - x) * (points[z].x - x)) + ((points[z].y - y) * (points[z].y - y)));;
                if(distance < closest_distance) {
                    closest_distance = distance;
                    point_index = z;
                }
            }
            // Set tile
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
// Place player randomly on path
void placePlayer(Local_Map *map) {
    // Generate random numbers until the player ends up on a path
    while(strcmp(map->terrain[playerY][playerX], "#")) {
        playerX = (rand()) % (78 - 2 + 1) + 2;
        playerY = (rand()) % (18 - 2 + 1) + 2;
    }
}
// Terrain comparator for NPCs
static int32_t npc_cmp(const void *tile1, const void *tile2) {
    return ((npc_node_t*) tile1)->cost - ((npc_node_t*) tile2)->cost;
}
// Calculate tile weights for numerous NPCs
uint32_t tile_weight(char *tile, npc_type n_type) {
    if((!(strcmp(tile, "."))) ||
    (!(strcmp(tile, "#")))) {
        switch (n_type) {
            case swimmer:
                return INT_MAX;
                break;
            default:
                return 10;
                break;
        }
    }   
     // Pokemart and Pokemon Center costs
    if((!(strcmp(tile, "P"))) ||
    (!(strcmp(tile, "M")))) {
        switch (n_type) {
            case player:
                return 10;
                break;
            case swimmer:
                return INT_MAX;
                break;
            default:
                return 50;
                break;
        }
    }
    // Tall grass cost
    if((!(strcmp(tile, ":")))) {
        switch (n_type) {
            case swimmer:
                return INT_MAX;
                break;
            case hiker:
                return 15;
                break;
            default:
                return 20;
                break;
        }
    }
    // Dashes "-" represent gates
    if(!(strcmp(tile, "P"))) {
        switch (n_type) {
            case player:
                return 10;
                break;
            default:
                return INT_MAX;
                break;
        }
    }
    // Water, Boulders, and Tree costs
    return INT_MAX;
}

void dijkstras_generation(Local_Map *map, npc_type n_type, npc_tile *npc_maps[21][80]) {
    int x, y;
    heap_t h;
    npc_node_t npc_heap[21][80], *p;
    uint32_t current_cost = 10;
    
    for(y = 0; y < 21; y++) {
        for(x = 0; x < 80; x++) { 
            // So this parts makes it impossible to reuse this method, unless...
            npc_maps[y][x] = (npc_tile*) malloc(sizeof(npc_tile));
            
            npc_heap[y][x].pos[0] = y;
            npc_heap[y][x].pos[1] = x;
            // Fill nodes with max cost unless node is the player character
            npc_heap[y][x].cost = INT_MAX;
            // Non-Heap nodes
            npc_maps[y][x]->y_pos = npc_heap[y][x].pos[0];
            npc_maps[y][x]->x_pos = npc_heap[y][x].pos[1];
            npc_maps[y][x]->cost = INT_MAX;
        }
    }
    // Set player node to 0
    npc_heap[playerY][playerX].cost = 0;
    heap_init(&h, npc_cmp, NULL);
    // Fill Heap
    for(y = 0; y < 21; y++) {
        for(x = 0; x < 80; x++) {
            npc_heap[y][x].hn = heap_insert(&h, &npc_heap[y][x]);
        }
    }
    // Temporary int checking variable
    uint32_t tile_checker;
    // Dijkstra's Algorithm
    while((p = heap_remove_min(&h))) {
        p->hn = NULL;
        // Path and short grass cost
        current_cost = tile_weight(map->terrain[p->pos[0]][p->pos[1]], n_type);
        // printf("%d Y, %d X\n", p->pos[0], p->pos[1]);
        // Loop through all surrounding nodes
        for(y = p->pos[0] - 1; y <= p->pos[0] + 1; y++) {
            for(x = p->pos[1] - 1; x <= p->pos[1] + 1; x++) {
                // Bounds checking
                if((y == p->pos[0]) && (x == p->pos[1])) {
                    continue;
                }
                if((y < 0) || (x < 0) || (y > 20) || (x > 79)) {
                    continue;
                }
                if((npc_heap[y][x].hn) && 
                (npc_heap[y][x].cost > npc_heap[p->pos[0]][p->pos[1]].cost)) {
                    // Set position of previous node here
                    npc_maps[y][x]->y_from = p->pos[0];
                    npc_maps[y][x]->x_from = p->pos[1];
                    tile_checker = tile_weight(map->terrain[y][x], n_type);
                    npc_heap[y][x].cost = tile_checker == INT_MAX ? INT_MAX : npc_heap[p->pos[0]][p->pos[1]].cost + current_cost;
                    npc_maps[y][x]->cost = npc_heap[y][x].cost;
                    // Remove node from heap
                    heap_decrease_key_no_replace(&h, npc_heap[y][x].hn);
                }
            }
        }
    }
    heap_delete(&h);
}
// The NPC comparator for the min-heap
static int32_t character_cmp(const void *c_one, const void *c_two) {
    if(((Character*)c_one)->cost < ((Character*)c_two)->cost) {
        return -1;
    }
    if(((Character*)c_one)->cost > ((Character*)c_two)->cost) {
        return 1;
    }
    if(((Character*)c_one)->sequence_num > ((Character*)c_two)->sequence_num) {
        return -1;
    }
    return 1;
}
// Generate 10 NPCs to place into the heap
void generate_npcs(heap_t *char_heap, Local_Map *map) {
    int x; 
    int rand_x = 0;
    int rand_y = 0;
    for(x = 0; x < sizeof(NPCs) / sizeof(NPCs[0]); x++) {
        characters[x] = (Character*) malloc(sizeof(Character));
        if(NPCs[x] == player) {
            characters[x]->pos_x = playerX;
            characters[x]->pos_y = playerY;
        }
        else {
            // Generate numbers from 1-79 for now
            do {
                printf("Randomly Generating nums\n");
                rand_x = (rand()) % (79 - 1 + 1) + 1;
                rand_y = (rand()) % (19 - 1 + 1) + 1;
            } while(tile_weight(map->terrain[rand_y][rand_x], NPCs[x]) == INT_MAX || character_map[rand_y][rand_x] != NULL);
            characters[x]->pos_x = rand_x;
            characters[x]->pos_y = rand_y;
            printf("Done\n");

        }
        characters[x]->type = NPCs[x];
        printf("%d\n", NPCs[x]);
        characters[x]->sequence_num = x;
        characters[x]->cost = 0;
        characters[x]->hn = heap_insert(char_heap, characters[x]->hn);
    }
    // playerX = (rand()) % (78 - 2 + 1) + 2;
    // playerY = (rand()) % (18 - 2 + 1) + 2;
}

int main(int argc, char *argv[]) {
    int x, y;
    Local_Map* startMap = (Local_Map*) malloc(sizeof(Local_Map));
    // Character* chracter_map[21][80];
    // Set seed
    srand(time(0));
    // Generate board
    int current_x = 200;
    int current_y = 200;
    world_map[current_y][current_x] = startMap;
    // Initalize Grid
    initalize_grid(world_map[current_y][current_x]);
    // Generate Terrain
    generate_voronoi_terrain(world_map[current_y][current_x]);
    generate_path_and_shops(current_y, current_x);
    placePlayer(world_map[current_y][current_x]);
    // Print Dijkstra's for hikers and rivals
    dijkstras_generation(world_map[current_y][current_x], hiker, world_map[current_y][current_x]->hiker_paths);
    dijkstras_generation(world_map[current_y][current_x], rival, world_map[current_y][current_x]->rival_paths);
    // DEBUG: Show hiker and rival paths
    // for(y = 0; y < 21; y++) {
    //     for(x = 0; x < 80; x++) {
    //         if(world_map[current_y][current_x]->rival_paths[y][x]->cost == INT_MAX) {
    //             printf("   ");
    //         }
    //         else {
    //             printf("%02d ", world_map[current_y][current_x]->rival_paths[y][x]->cost % 100);
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("\n\n\nHiker Paths:\n");
    // for(y = 0; y < 21; y++) {
    //     for(x = 0; x < 80; x++) {
    //         if(world_map[current_y][current_x]->hiker_paths[y][x]->cost == INT_MAX) {
    //             printf("   ");
    //         }
    //         else {
    //             printf("%02d ", world_map[current_y][current_x]->hiker_paths[y][x]->cost % 100);
    //         }
    //     }
    //     printf("\n");
    // }
    heap_t character_heap;
    heap_init(&character_heap, character_cmp, NULL);
    generate_npcs(&character_heap, world_map[current_y][current_x]);
    for(y = 0; y < 21; y++) {
        for(x = 0; x < 80; x++) {
            if((y == playerY) && (x == playerX)) {
                printf("@");                
            }
            else {
                printf("%s", world_map[current_y][current_x]->terrain[y][x]);
            }
        }
        printf("\n");
    }
    // Exit loop if input is q
    while(1) {
        // Character charact = heap_remove_min(&char_heap);

    }
    // Free all memory
    int i, j;
    for(y = 0; y < 401; y++) {
        for(x = 0; x < 401; x++) {
            if(world_map[y][x] != NULL) {
                for(i = 0; i < 21; i++) {
                    for(j = 0; j < 80; j++) {
                        free(world_map[y][x]->hiker_paths[y][x]);
                        free(world_map[y][x]->rival_paths[y][x]);
                    }
                }
                free(world_map[y][x]);
            }
        }
    }
    for(x = 0; x < sizeof(NPCs) / sizeof(NPCs[0]); x++) {
        free(characters[x]);
    }
    heap_delete(&character_heap);
    return 0;
}
