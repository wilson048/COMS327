#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <bits/stdc++.h>

#include "file_handler.h"

using namespace std;
// Global to keep track of the array file to load and read
int file_index;
// Array of valid CSV files to read
const char* correct_files[10] = {"pokemon", "pokemon_moves", "pokemon_species", "pokemon_stats", "pokemon_types", "moves", "expereince", "type_names", "stats"};
/* Class for pokemon stats */
class pokemon_stats {
    public:
        int id;
        int stat_id;
        int base_stat;
        int effort;
};

/* Class for stats */
class stats {
    public:
        string id;
        string damage_class_id;
        string identifier;
        string is_battle_only;
        string game_index;
};

/* Class for names of pokemon types */
class type_names {
    public:
        int id;
        int language_id;
        string name;
};

/* Class for experience stats */
class experience {
    public:
        int growth_rate;
        int level;
        int exp;
};

/* Class for move attributes */
class move {
    public:
        int id;
        string identifier;
        int gen_id;
        int type_id;
        int power;
        int pp;
        int accuracy;
        int prioirty;
        int target_id;
        int damage_class_id;
        int effect_id;
        int effect_chance;
        int contest_type_id;
        int contest_effect_id;
        int super_contest_effect_id;
};

/* Class for what moves a single pokemon has */
class pokemon_moves {
    public:
        int pokemon_id;
        int version_id;
        int move_id;
        int pokemon_move_method_id;
        int level;
        int order;
};

/* Class for what intricacies a pokemon has*/
class pokemon_species {
    public:
        int id;
        string identifier;
        int generation_id;
        int evolves_from_species_id;
        int evolution_chain_id;
        int color_id;
        int shape_id;
        int habitat_id;
        int gender_rate;
        int capture_rate;
        int base_happiness;
        int is_baby;
        int hatch_counter;
        int has_gender_differences;
        int growth_rate_id;
        int forms_switchable;
        int is_legendary;
        int is_mythical;
        int order;
        int conquest_order;
};

/* Class for what type a pokemon is*/
class pokemon_types {
    public:
        int pokemon_id;
        int type_id;
        int slot;
};

/* Class for what a pokemon generally represents */
class pokemon {
    public:
        int id;
        string identifier;
        int species_id;
        int height;
        int weight;
        int base_experience;
        int order;
        int is_default;
};
int confirm_file_name(const char* file_name) {
    int isValid = 0;
    int i;
    for (i = 0; i < 9; i++) {
        if(!(strcmp(file_name, correct_files[i]))) {
            isValid = 1;
            file_index = i;
            break;
        }
    }
    return isValid;
}

int parse_file(const char* file_name) {
    string f = file_name;

    // HOME path, default path is stored in file_handler.h
    string alt_path = getenv("HOME");
    alt_path += "/.poke327/pokedex/pokedex/data/csv/";

    struct stat sb;
    // ~/share/cs327/pokedex/pokedex/data/csv/
    if(!(stat(DEFAULT_PATH_ONE, &sb))) {
        read_file(DEFAULT_PATH_ONE + f + ".csv");
    }
    // ~/HOME/.poke327/pokedex/pokedex/data/csv/
    else if (!(stat(strcat(getenv("HOME"), "/.poke327/pokedex/pokedex/data/csv/"), &sb))) {
        read_file(alt_path + f + ".csv");
    }
    else if (!(stat(HARD_PATH, &sb))) {
        read_file(HARD_PATH + f + ".csv");
    }
    else {
        cout << "Database not located" << endl;
    }
    return 0;
}
void read_file(string file_name) {
    cout << "Can print file!" << endl;
}