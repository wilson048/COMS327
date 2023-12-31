#ifndef CHARACTER_H
# define CHARACTER_H

# include <cstdint>

# include "pair.h"
# include "db_parse.h"

#define DIJKSTRA_PATH_MAX (INT_MAX / 2)

typedef enum __attribute__ ((__packed__)) movement_type {
  move_hiker,
  move_rival,
  move_pace,
  move_wander,
  move_sentry,
  move_explore,
  move_swim,
  move_pc,
  num_movement_types
} movement_type_t;

typedef enum __attribute__ ((__packed__)) character_type {
  char_pc,
  char_hiker,
  char_rival,
  char_swimmer,
  char_other,
  num_character_types
} character_type_t;

extern const char *char_type_name[num_character_types];


class char_pokemon {
  public:
    virtual ~char_pokemon() {}
    // Pokemon name 
    // The max length pokemon name is 29 characters long
    char name[30];
    // Pokemon movesets and expereince thresholds
    move_db moves[10];
    int num_moves;
    experience_db exp_level_thresholds[101];
    // Pokemon face value stats
    int is_shiny;
    int exp;
    int level;
    // Identifiers
    int poke_id;
    int species_id;
    int growth_rate_id;
    // Male is 0
    // Female is 1
    int gender;
    // Pokemon Main Stats that change
    int hp;
    int attack;
    int defense;
    int special_attack;
    int special_defense;
    int speed;
    // Pokemon base stats that don't change
    int base_hp;
    int base_attack;
    int base_defense;
    int base_special_attack;
    int base_special_defense;
    int base_speed;
    // Pokemon IVs
    int hp_iv;
    int attack_iv;
    int defense_iv;
    int special_attack_iv;
    int special_defense_iv;
    int speed_iv;
};

class character {
 public:
  virtual ~character() {}
  char_pokemon current_pokemon[6];
  int num_pokemon;
  pair_t pos;
  char symbol;
  int next_turn;
  int seq_num;
};

class npc : public character {
 public:
  character_type_t ctype;
  movement_type_t mtype;
  int defeated;
  pair_t dir;
};

class pc : public character {
};

/* character is defined in poke327.h to allow an instance of character
 * in world without including character.h in poke327.h                 */

int32_t cmp_char_turns(const void *key, const void *with);
void delete_character(void *v);

extern void (*move_func[num_movement_types])(character *, pair_t);

int pc_move(char);
bool is_pc(character *c);

#endif
