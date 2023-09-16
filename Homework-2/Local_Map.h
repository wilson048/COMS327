#ifndef LOCAL_MAP_H
#define LOCAL_MAP_H

typedef struct {
    int x;
    int y;
    int tileType;
} Voroni_Point;

struct Local_Map {
    char **terrain;
    int h_start_index;
    int h_end_index;
    int v_start_index;
    int v_end_index;
};

void local_map_init(struct Local_Map *map);
void generate_voronoi_terrain(struct Local_Map *map);
void generate_paths_and_shops(struct Local_Map *map);
void local_map_destroy(struct Local_Map *map);

#endif