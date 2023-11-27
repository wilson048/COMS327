#include <unistd.h>
#include <ncurses.h>
#include <cctype>
#include <cstdlib>
#include <climits>
#include <string>
#include <cstring>
#include <fstream>

#include "io.h"
#include "character.h"
#include "poke327.h"
#include "pokemon.h"
#include "db_parse.h"

typedef struct io_message {
  /* Will print " --more-- " at end of line when another message follows. *
   * Leave 10 extra spaces for that.                                      */
  char msg[71];
  struct io_message *next;
} io_message_t;

static io_message_t *io_head, *io_tail;

void io_init_terminal(void)
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void io_reset_terminal(void)
{
  endwin();

  while (io_head) {
    io_tail = io_head;
    io_head = io_head->next;
    free(io_tail);
  }
  io_tail = NULL;
}

void io_queue_message(const char *format, ...)
{
  io_message_t *tmp;
  va_list ap;

  if (!(tmp = (io_message_t *) malloc(sizeof (*tmp)))) {
    perror("malloc");
    exit(1);
  }

  tmp->next = NULL;

  va_start(ap, format);

  vsnprintf(tmp->msg, sizeof (tmp->msg), format, ap);

  va_end(ap);

  if (!io_head) {
    io_head = io_tail = tmp;
  } else {
    io_tail->next = tmp;
    io_tail = tmp;
  }
}

static void io_print_message_queue(uint32_t y, uint32_t x)
{
  while (io_head) {
    io_tail = io_head;
    attron(COLOR_PAIR(COLOR_CYAN));
    mvprintw(y, x, "%-80s", io_head->msg);
    attroff(COLOR_PAIR(COLOR_CYAN));
    io_head = io_head->next;
    if (io_head) {
      attron(COLOR_PAIR(COLOR_CYAN));
      mvprintw(y, x + 70, "%10s", " --more-- ");
      attroff(COLOR_PAIR(COLOR_CYAN));
      refresh();
      getch();
    }
    free(io_tail);
  }
  io_tail = NULL;
}

/**************************************************************************
 * Compares trainer distances from the PC according to the rival distance *
 * map.  This gives the approximate distance that the PC must travel to   *
 * get to the trainer (doesn't account for crossing buildings).  This is  *
 * not the distance from the NPC to the PC unless the NPC is a rival.     *
 *                                                                        *
 * Not a bug.                                                             *
 **************************************************************************/
static int compare_trainer_distance(const void *v1, const void *v2)
{
  const character *const *c1 = (const character * const *) v1;
  const character *const *c2 = (const character * const *) v2;

  return (world.rival_dist[(*c1)->pos[dim_y]][(*c1)->pos[dim_x]] -
          world.rival_dist[(*c2)->pos[dim_y]][(*c2)->pos[dim_x]]);
}

static character *io_nearest_visible_trainer()
{
  character **c, *n;
  uint32_t x, y, count;

  c = (character **) malloc(world.cur_map->num_trainers * sizeof (*c));

  /* Get a linear list of trainers */
  for (count = 0, y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      if (world.cur_map->cmap[y][x] && world.cur_map->cmap[y][x] !=
          &world.pc) {
        c[count++] = world.cur_map->cmap[y][x];
      }
    }
  }

  /* Sort it by distance from PC */
  qsort(c, count, sizeof (*c), compare_trainer_distance);

  n = c[0];

  free(c);

  return n;
}

void io_display()
{
  uint32_t y, x;
  character *c;

  clear();
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if (world.cur_map->cmap[y][x]) {
        mvaddch(y + 1, x, world.cur_map->cmap[y][x]->symbol);
      } else {
        switch (world.cur_map->map[y][x]) {
        case ter_boulder:
          attron(COLOR_PAIR(COLOR_MAGENTA));
          mvaddch(y + 1, x, BOULDER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_MAGENTA));
          break;
        case ter_mountain:
          attron(COLOR_PAIR(COLOR_MAGENTA));
          mvaddch(y + 1, x, MOUNTAIN_SYMBOL);
          attroff(COLOR_PAIR(COLOR_MAGENTA));
          break;
        case ter_tree:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, TREE_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_forest:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, FOREST_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_path:
          attron(COLOR_PAIR(COLOR_YELLOW));
          mvaddch(y + 1, x, PATH_SYMBOL);
          attroff(COLOR_PAIR(COLOR_YELLOW));
          break;
        case ter_gate:
          attron(COLOR_PAIR(COLOR_YELLOW));
          mvaddch(y + 1, x, GATE_SYMBOL);
          attroff(COLOR_PAIR(COLOR_YELLOW));
          break;
        case ter_bailey:
          attron(COLOR_PAIR(COLOR_YELLOW));
          mvaddch(y + 1, x, BAILEY_SYMBOL);
          attroff(COLOR_PAIR(COLOR_YELLOW));
          break;
        case ter_mart:
          attron(COLOR_PAIR(COLOR_BLUE));
          mvaddch(y + 1, x, POKEMART_SYMBOL);
          attroff(COLOR_PAIR(COLOR_BLUE));
          break;
        case ter_center:
          attron(COLOR_PAIR(COLOR_RED));
          mvaddch(y + 1, x, POKEMON_CENTER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_RED));
          break;
        case ter_grass:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, TALL_GRASS_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_clearing:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, SHORT_GRASS_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_water:
          attron(COLOR_PAIR(COLOR_CYAN));
          mvaddch(y + 1, x, WATER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_CYAN));
          break;
        default:
          attron(COLOR_PAIR(COLOR_CYAN));
          mvaddch(y + 1, x, ERROR_SYMBOL);
          attroff(COLOR_PAIR(COLOR_CYAN)); 
       }
      }
    }
  }

  mvprintw(23, 1, "PC position is (%2d,%2d) on map %d%cx%d%c.",
           world.pc.pos[dim_x],
           world.pc.pos[dim_y],
           abs(world.cur_idx[dim_x] - (WORLD_SIZE / 2)),
           world.cur_idx[dim_x] - (WORLD_SIZE / 2) >= 0 ? 'E' : 'W',
           abs(world.cur_idx[dim_y] - (WORLD_SIZE / 2)),
           world.cur_idx[dim_y] - (WORLD_SIZE / 2) <= 0 ? 'N' : 'S');
  mvprintw(22, 1, "%d known %s.", world.cur_map->num_trainers,
           world.cur_map->num_trainers > 1 ? "trainers" : "trainer");
  mvprintw(22, 30, "Nearest visible trainer: ");
  if ((c = io_nearest_visible_trainer())) {
    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(22, 55, "%c at vector %d%cx%d%c.",
             c->symbol,
             abs(c->pos[dim_y] - world.pc.pos[dim_y]),
             ((c->pos[dim_y] - world.pc.pos[dim_y]) <= 0 ?
              'N' : 'S'),
             abs(c->pos[dim_x] - world.pc.pos[dim_x]),
             ((c->pos[dim_x] - world.pc.pos[dim_x]) <= 0 ?
              'W' : 'E'));
    attroff(COLOR_PAIR(COLOR_RED));
  } else {
    attron(COLOR_PAIR(COLOR_BLUE));
    mvprintw(22, 55, "NONE.");
    attroff(COLOR_PAIR(COLOR_BLUE));
  }

  io_print_message_queue(0, 0);

  refresh();
}

uint32_t io_teleport_pc(pair_t dest)
{
  /* Just for fun. And debugging.  Mostly debugging. */

  do {
    dest[dim_x] = rand_range(1, MAP_X - 2);
    dest[dim_y] = rand_range(1, MAP_Y - 2);
  } while (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]                  ||
           move_cost[char_pc][world.cur_map->map[dest[dim_y]]
                                                [dest[dim_x]]] ==
             DIJKSTRA_PATH_MAX                                            ||
           world.rival_dist[dest[dim_y]][dest[dim_x]] < 0);

  return 0;
}

static void io_scroll_trainer_list(char (*s)[40], uint32_t count)
{
  uint32_t offset;
  uint32_t i;

  offset = 0;

  while (1) {
    for (i = 0; i < 13; i++) {
      mvprintw(i + 6, 19, " %-40s ", s[i + offset]);
    }
    switch (getch()) {
    case KEY_UP:
      if (offset) {
        offset--;
      }
      break;
    case KEY_DOWN:
      if (offset < (count - 13)) {
        offset++;
      }
      break;
    case 27:
      return;
    }

  }
}

static void io_list_trainers_display(npc **c, uint32_t count)
{
  uint32_t i;
  char (*s)[40]; /* pointer to array of 40 char */

  s = (char (*)[40]) malloc(count * sizeof (*s));

  mvprintw(3, 19, " %-40s ", "");
  /* Borrow the first element of our array for this string: */
  snprintf(s[0], 40, "You know of %d trainers:", count);
  mvprintw(4, 19, " %-40s ", *s);
  mvprintw(5, 19, " %-40s ", "");

  for (i = 0; i < count; i++) {
    snprintf(s[i], 40, "%16s %c: %2d %s by %2d %s",
             char_type_name[c[i]->ctype],
             c[i]->symbol,
             abs(c[i]->pos[dim_y] - world.pc.pos[dim_y]),
             ((c[i]->pos[dim_y] - world.pc.pos[dim_y]) <= 0 ?
              "North" : "South"),
             abs(c[i]->pos[dim_x] - world.pc.pos[dim_x]),
             ((c[i]->pos[dim_x] - world.pc.pos[dim_x]) <= 0 ?
              "West" : "East"));
    if (count <= 13) {
      /* Handle the non-scrolling case right here. *
       * Scrolling in another function.            */
      mvprintw(i + 6, 19, " %-40s ", s[i]);
    }
  }

  if (count <= 13) {
    mvprintw(count + 6, 19, " %-40s ", "");
    mvprintw(count + 7, 19, " %-40s ", "Hit escape to continue.");
    while (getch() != 27 /* escape */)
      ;
  } else {
    mvprintw(19, 19, " %-40s ", "");
    mvprintw(20, 19, " %-40s ",
             "Arrows to scroll, escape to continue.");
    io_scroll_trainer_list(s, count);
  }

  free(s);
}

static void io_list_trainers()
{
  npc **c;
  uint32_t x, y, count;

  c = (npc **) malloc(world.cur_map->num_trainers * sizeof (*c));

  /* Get a linear list of trainers */
  for (count = 0, y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      if (world.cur_map->cmap[y][x] && world.cur_map->cmap[y][x] !=
          &world.pc) {
        c[count++] = dynamic_cast<npc *> (world.cur_map->cmap[y][x]);
      }
    }
  }

  /* Sort it by distance from PC */
  qsort(c, count, sizeof (*c), compare_trainer_distance);

  /* Display it */
  io_list_trainers_display(c, count);
  free(c);

  /* And redraw the map */
  io_display();
}

void io_pokemart()
{
  mvprintw(0, 0, "Welcome to the Pokemart.  Here's some supplies");
  // Refill pokemon supplies
  world.pc.pokeballs = 3;
  world.pc.potions = 10;
  world.pc.revives = 2;
  refresh();
  getch();
}

void io_pokemon_center()
{
  int x;
  // Fully heal all party Pokemon
  for(x = 0; x < 6; x++) {
    if(world.pc.buddy[x] != NULL) {
      world.pc.buddy[x]->set_current_hp(world.pc.buddy[x]->get_hp());
    }
  }
  mvprintw(0, 0, "Welcome to the Pokemon Center.  Your Pokemon have been resort to fall health");
  refresh();
  getch();
}

void io_battle(character *aggressor, character *defender)
{
  std::string s;
  npc *n = (npc *) ((aggressor == &world.pc) ? defender : aggressor);
  // int i;

  // if (!n->buddy[1]) {
  //   s = "My pokemon is " + std::string(n->buddy[0]->get_species());
  // } else {
  //   s = "My pokemon are " + std::string(n->buddy[0]->get_species());
  // }

  // for (i = 1; i < 6 && n->buddy[i]; i++) {
  //   s += ", ";
  //   if (i == 4 || !n->buddy[i + 1]) {
  //     s += "and ";
  //   }
  //   s += n->buddy[i]->get_species();
  // }
    
  // s += ".";

  // io_queue_message("%s", s.c_str());

  n->defeated = 1;
  if (n->ctype == char_hiker || n->ctype == char_rival) {
    n->mtype = move_wander;
  }

  int leaveBattle = 0;
  int x;
  // Keep track of highlight on list
  int optionsHighlight;
  // Keep track of position on list
  int cursor = 0;
  // Take player input
  int playerInput;
  // Create trainers menu
  WINDOW * menuwin = newwin(world.char_seq_num, 0, 0, 0);
  clear();
  refresh();
  // wrefresh(menuwin);
  keypad(menuwin, 1);
  // Inital battle menu options
  const char *options[4] = {"Fight", "Bag", "Pokemon", "Run"};
  // Bag Options
  // const char *bag_options[3] = {"Revives", "Potions", "Pokeballs"};
  
  // TODO: Pokemon Revive options
  while(!leaveBattle) {
    // Move stuff
    int num_opponent_moves = 0;
    int player_move_index;
    int opponent_move_index;
    int move_cursor = 0;
    int damage = 0;
    bool cancel_move = false;
    // players typically go first, check for any changes
    bool player_first = true;
    // Players may not use items or switch Pokemon, so check for this change
    bool ignore_player_move = false;

    wclear(menuwin);
    mvwprintw(menuwin, 12, 20, "Player's current pokemon %s, lv%d", world.pc.current_pokemon->get_species(), world.pc.current_pokemon->get_level());
    mvwprintw(menuwin, 13, 20, "HP %d / %d", world.pc.current_pokemon->get_current_hp(), world.pc.current_pokemon->get_hp());
    mvwprintw(menuwin, 15, 20, "NPC's current pokemon %s, lv%d", n->current_pokemon->get_species(), n->current_pokemon->get_level());
    mvwprintw(menuwin, 16, 20, "HP %d / %d", n->current_pokemon->get_current_hp(), n->current_pokemon->get_hp());
    
    optionsHighlight = 0;
    for(x = 0; x <= 3; x++) {
      if(x == cursor) {
        wattron(menuwin, A_REVERSE);
      }
      mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
      wattroff(menuwin, A_REVERSE);
      optionsHighlight++;
    }
    // total_trainers = 0;
    // Output board
    wrefresh(menuwin);
    playerInput = wgetch(menuwin);
    if(playerInput == 27) {
      leaveBattle = 1;
    }
    switch (playerInput) {
      case 27:
        leaveBattle = 1;
        break;
      case KEY_UP:
        if(cursor != 0) {
          cursor--;
        }
        break;
      case KEY_DOWN:
        if(cursor != 3) {
          cursor++;
        }
        break;
      case 10:
        // const char *options[4] = {"Fight", "Bag", "Pokemon", "Run"};
        if(!(strcmp(options[cursor], "Fight"))) {
          // Move options
          int move_options[3];
          
          int move_highlight;

          // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
          // 0 - 3, 1
          int in_move_menu = 1;
          // Load potential moves onto temporary array
          int num_move_options = 1;
          for(x = 0; x < 3; x++) {
            if(strcmp(world.pc.current_pokemon->get_move(x), "")) {
              move_options[num_move_options] = world.pc.current_pokemon->get_move_index(x);
              num_move_options++;
            }
          }
          wrefresh(menuwin);
          // Menu options for switching pokemon
          while (in_move_menu) {
            move_highlight = 0;
            // Cursor logic 
            for(x = 0; x < num_move_options; x++) {
              if(x == move_cursor) {
                wattron(menuwin, A_REVERSE);
              }
              if(x == 0) {
                mvwprintw(menuwin, move_highlight, 10, "Back");
              }
              else {
                mvwprintw(menuwin, move_highlight, 10, "%s", moves[move_options[x]].identifier);
              }
              wattroff(menuwin, A_REVERSE);
              move_highlight++;
            }
            playerInput = wgetch(menuwin);
            switch (playerInput) {
              // Enter key
              case 10:
                if(move_cursor != 0) {
                  // Move logic
                  player_move_index = world.pc.current_pokemon->get_move_index(move_cursor - 1);
                }
                else {
                  cancel_move = true;
                }
                in_move_menu = 0; 
                break;
              // Up and down logic
              case KEY_UP:
                if(move_cursor != 0) {
                  move_cursor--;
                }
                break;
              case KEY_DOWN:
                if(move_cursor != num_move_options - 1) {
                  move_cursor++;
                }
                break;
            }
          } 
          // Opponent chooses random move
          for(x = 0; x < 3; x++) {
            if(strcmp(n->current_pokemon->get_move(x), "")) {
              num_opponent_moves++;
            }
          }
          opponent_move_index = n->current_pokemon->get_move_index(rand() % num_opponent_moves);
          // Check whether player goes first by priority
          if(moves[player_move_index].priority < moves[opponent_move_index].priority) {
            player_first = false;
          }
          // Check whether player goes first by speed
          else if (moves[player_move_index].priority == moves[opponent_move_index].priority && 
          world.pc.current_pokemon->get_speed() < n->current_pokemon->get_speed()) {  
            player_first = false;
          }
          // Check whether player goes first by random
          else if (moves[player_move_index].priority == moves[opponent_move_index].priority && 
          world.pc.current_pokemon->get_speed() == n->current_pokemon->get_speed()){
            player_first = rand() % 2 == 0 ? true : false;
          }
          if(!cancel_move) { 
            
            // Player goes first
            if(!ignore_player_move && player_first) {
              // Feedback for move use
              mvwprintw(menuwin, 9, 20, "%s used %s!                          ", world.pc.current_pokemon->get_species(), moves[player_move_index].identifier);
              wrefresh(menuwin);
              wgetch(menuwin);
              if(rand() % 100 < moves[player_move_index].accuracy) {
                // Damage formula
                damage = ((((2 * world.pc.current_pokemon->get_level()) / 5) + 2) * moves[player_move_index].power * (world.pc.current_pokemon->get_atk() / n->current_pokemon->get_def()));
                damage = (int) damage / 50;
                damage += 2;
                damage = (int) (damage * ((rand() % 100 - 85 + 1) + 85));
                // if(rand() % 256 < world.pc.current_pokemon) {
                //   damage = damage * 1.5;
                //   mvwprintw(menuwin, 9, 20, "A critical hit!                      ");
                //   wrefresh(menuwin);
                //   wgetch(menuwin);
                // }
                // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
                // Hard floor of 1 damage for moves with power
                if(damage == 0 && moves[player_move_index].power != 0) {
                  damage = 1;
                }
                // Win Pokemon Battle if last pokemon logic
                if(damage >= n->current_pokemon->get_current_hp()) {
                  
                  n->current_pokemon->set_current_hp(0);
                  bool pokemon_left = false;
                  for(x = 0; x < 6; x++) {
                    if(n->buddy[x] != NULL && n->buddy[x]->get_current_hp() != 0) {
                      n->current_pokemon = n->buddy[x];
                      pokemon_left = true;
                      break;
                    }
                  }
                  if(!pokemon_left) {
                    mvwprintw(menuwin, 9, 20, "You won!                     ", n->current_pokemon->get_species());
                    wrefresh(menuwin);
                    wgetch(menuwin);
                    leaveBattle = 1;
                    cancel_move = true;
                  }
                  else {
                    mvwprintw(menuwin, 9, 20, "Trainer sent out %s                    ", n->current_pokemon->get_species());
                    wrefresh(menuwin);
                    wgetch(menuwin);
                    cancel_move = true;
                  }
                }
                else {
                  n->current_pokemon->set_current_hp(n->current_pokemon->get_current_hp() - damage);
                }
              }
              // Player Move missed
              else {
                mvwprintw(menuwin, 9, 20, "Your Move missed!                    ");
                wrefresh(menuwin);
                wgetch(menuwin);
              }
              if(!cancel_move) {
                // Feedback for move use
                mvwprintw(menuwin, 9, 20, "%s used %s!                ", n->current_pokemon->get_species(), moves[opponent_move_index].identifier);
                wrefresh(menuwin);
                wgetch(menuwin);
                // Damage formula
                if(rand() % 100 < moves[opponent_move_index].accuracy) {
                  damage = ((((2 * n->current_pokemon->get_level()) / 5) + 2) * moves[opponent_move_index].power * (n->current_pokemon->get_atk() / world.pc.current_pokemon->get_def()));
                  damage = (int) damage / 50;
                  damage += 2;
                  if(rand() % 256 < n->current_pokemon->get_speed()) {
                    damage = damage * 1.5;
                    mvwprintw(menuwin, 9, 20, "A critical hit!                    ");
                    wrefresh(menuwin);
                    wgetch(menuwin);
                  }
                  // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
                  if(damage >= world.pc.current_pokemon->get_current_hp()) {
                    mvwprintw(menuwin, 9, 20, "Your Pokemon fainted!                    ");
                    wrefresh(menuwin);
                    wgetch(menuwin);
                    world.pc.current_pokemon->set_current_hp(0);
                    bool pokemon_left = false;
                    for(x = 0; x < 6; x++) {
                      if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                        pokemon_left = true;
                        break;
                      }
                    }
                    // Exit the game if all pokemon are down
                    if(!pokemon_left) {
                      mvwprintw(menuwin, 9, 20, "You blacked out, exiting game                      ", n->current_pokemon->get_species());
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      leaveBattle = 1;
                      world.quit = 1;
                    }
                    // Swap pokemon before next turn
                    else {
                      ignore_player_move = true;
                      // Pokemon switch options
                      class pokemon *pokemon_options[6];
                      pokemon_options[0] = world.pc.current_pokemon;
                      int switch_cursor = 0;
                      int swtich_highlight;
                      // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                      // 0 - 3, 1
                      int in_switch_menu = 1;
                      // Load potential switch candidates onto temporary array
                      int num_poke_options = 1;
                      for(x = 0; x < 6; x++) {
                        if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
                          pokemon_options[num_poke_options] = world.pc.buddy[x];
                          num_poke_options++;
                        }
                      }
                      
                      // Menu options for switching pokemon
                      while (in_switch_menu) {
                        swtich_highlight = 0;
                        // Cursor logic 
                        for(x = 0; x < num_poke_options; x++) {
                          if(x == switch_cursor) {
                            wattron(menuwin, A_REVERSE);
                          }
                          if(x == 0) {
                            mvwprintw(menuwin, swtich_highlight, 10, "Back");
                          }
                          else {
                            mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
                          }
                          wattroff(menuwin, A_REVERSE);
                          swtich_highlight++;
                        }
                        wrefresh(menuwin);
                        playerInput = wgetch(menuwin);
                        switch (playerInput) {
                          // Enter key
                          case 10:
                            if(switch_cursor != 0) {
                              world.pc.current_pokemon = pokemon_options[switch_cursor];
                              in_switch_menu = 0; 
                            }
                            break;
                          // Up and down logic
                          case KEY_UP:
                            if(switch_cursor != 0) {
                              switch_cursor--;
                            }
                            break;
                          case KEY_DOWN:
                            if(switch_cursor != num_poke_options - 1) {
                              switch_cursor++;
                            }
                            break;
                        }
                      }
                      // Clear menu
                      wclear(menuwin);
                    }
                  }
                  else {
                    world.pc.current_pokemon->set_current_hp(world.pc.current_pokemon->get_current_hp() - damage);
                  }
                }
                else {
                  mvwprintw(menuwin, 9, 20, "%s's move missed!                      ", n->current_pokemon->get_species());
                  wrefresh(menuwin);
                  wgetch(menuwin);
                }
              }
            }
            // Opponent goes first
            else if (!ignore_player_move && !player_first) {
              // Feedback for move use
              mvwprintw(menuwin, 9, 20, "%s used %s!                ", n->current_pokemon->get_species(), moves[opponent_move_index].identifier);
              wrefresh(menuwin);
              wgetch(menuwin);
              // Damage formula
              if(rand() % 100 < moves[opponent_move_index].accuracy) {
                damage = ((((2 * n->current_pokemon->get_level()) / 5) + 2) * moves[opponent_move_index].power * (n->current_pokemon->get_atk() / world.pc.current_pokemon->get_def()));
                damage = (int) damage / 50;
                damage += 2;
                if(rand() % 256 < n->current_pokemon->get_speed()) {
                  damage = damage * 1.5;
                  mvwprintw(menuwin, 9, 20, "A critical hit!                    ");
                  wrefresh(menuwin);
                  wgetch(menuwin);
                }
                // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
                if(damage >= world.pc.current_pokemon->get_current_hp()) {
                  mvwprintw(menuwin, 9, 20, "Your Pokemon fainted!                    ");
                  wrefresh(menuwin);
                  wgetch(menuwin);
                  world.pc.current_pokemon->set_current_hp(0);
                  bool pokemon_left = false;
                  for(x = 0; x < 6; x++) {
                    if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                      pokemon_left = true;
                      break;
                    }
                  }
                  // Exit the game if all pokemon are down
                  if(!pokemon_left) {
                    mvwprintw(menuwin, 9, 20, "You blacked out, exiting game                      ", n->current_pokemon->get_species());
                    wrefresh(menuwin);
                    wgetch(menuwin);
                    leaveBattle = 1;
                    world.quit = 1;
                    cancel_move = true;
                  }
                  // Swap pokemon before next turn
                  else {
                    ignore_player_move = true;
                    // Pokemon switch options
                    class pokemon *pokemon_options[6];
                    pokemon_options[0] = world.pc.current_pokemon;
                    int switch_cursor = 0;
                    int swtich_highlight;
                    // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                    // 0 - 3, 1
                    int in_switch_menu = 1;
                    // Load potential switch candidates onto temporary array
                    int num_poke_options = 1;
                    for(x = 0; x < 6; x++) {
                      if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
                        pokemon_options[num_poke_options] = world.pc.buddy[x];
                        num_poke_options++;
                      }
                    }
              
                    // Menu options for switching pokemon
                    while (in_switch_menu) {
                      swtich_highlight = 0;
                      // Cursor logic 
                      for(x = 0; x < num_poke_options; x++) {
                        if(x == switch_cursor) {
                          wattron(menuwin, A_REVERSE);
                        }
                        if(x == 0) {
                          mvwprintw(menuwin, swtich_highlight, 10, "Back");
                        }
                        else {
                          mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
                        }
                        wattroff(menuwin, A_REVERSE);
                        swtich_highlight++;
                      }
                      wrefresh(menuwin);
                      playerInput = wgetch(menuwin);
                      switch (playerInput) {
                        // Enter key
                        case 10:
                          if(switch_cursor != 0) {
                            world.pc.current_pokemon = pokemon_options[switch_cursor];
                            in_switch_menu = 0; 
                          }
                          break;
                        // Up and down logic
                        case KEY_UP:
                          if(switch_cursor != 0) {
                            switch_cursor--;
                          }
                          break;
                        case KEY_DOWN:
                          if(switch_cursor != num_poke_options - 1) {
                            switch_cursor++;
                          }
                          break;
                      }
                    }
                    // Clear menu
                    wclear(menuwin);
                    // Cancel move
                    cancel_move = true;
                  }
                }
                else {
                  world.pc.current_pokemon->set_current_hp(world.pc.current_pokemon->get_current_hp() - damage);
                }
              }
              else {
                mvwprintw(menuwin, 9, 20, "%s's move missed!                      ", n->current_pokemon->get_species());
                wrefresh(menuwin);
                wgetch(menuwin);
              }
              if(!cancel_move) {
                // Feedback for move use
                mvwprintw(menuwin, 9, 20, "%s used %s!                          ", world.pc.current_pokemon->get_species(), moves[player_move_index].identifier);
                wrefresh(menuwin);
                wgetch(menuwin);
                if(rand() % 100 < moves[player_move_index].accuracy) {
                  // Damage formula
                  damage = ((((2 * world.pc.current_pokemon->get_level()) / 5) + 2) * moves[player_move_index].power * (world.pc.current_pokemon->get_atk() / n->current_pokemon->get_def()));
                  damage = (int) damage / 50;
                  damage += 2;
                  if(rand() % 256 < world.pc.current_pokemon->get_speed()) {
                    damage = damage * 1.5;
                    mvwprintw(menuwin, 9, 20, "A critical hit!                      ");
                    wrefresh(menuwin);
                    wgetch(menuwin);
                  }
                  // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
                  // Hard floor of 1 damage for moves with power
                  if(damage == 0 && moves[player_move_index].power != 0) {
                    damage = 1;
                  }
                  // Win Pokemon Battle if last pokemon logic
                  if(damage >= n->current_pokemon->get_current_hp()) {
                    
                    n->current_pokemon->set_current_hp(0);
                    bool pokemon_left = false;
                    for(x = 0; x < 6; x++) {
                      if(n->buddy[x] != NULL && n->buddy[x]->get_current_hp() != 0) {
                        n->current_pokemon = n->buddy[x];
                        pokemon_left = true;
                        break;
                      }
                    }
                    if(!pokemon_left) {
                      mvwprintw(menuwin, 9, 20, "You won!                     ", n->current_pokemon->get_species());
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      leaveBattle = 1;
                      cancel_move = true;
                    }
                    else {
                      mvwprintw(menuwin, 9, 20, "Trainer sent out %s                    ", n->current_pokemon->get_species());
                      wrefresh(menuwin);
                      wgetch(menuwin);
                    }
                  }
                  else {
                    n->current_pokemon->set_current_hp(n->current_pokemon->get_current_hp() - damage);
                  }
                }
                // Player Move missed
                else {
                  mvwprintw(menuwin, 9, 20, "Your Move missed!                    ");
                  wrefresh(menuwin);
                  wgetch(menuwin);
                }
              }
            }
          }
          // Clear menu
          wclear(menuwin);
        }
        if(!(strcmp(options[cursor], "Bag"))) {
          const char *bag_options[4] = {"Back", "Revives", "Potions", "Pokeballs"};
          // Move options
          
          int bag_highlight;
          // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
          // 0 - 3, 1
          int in_bag_menu = 1;
          // Load potential moves onto temporary array
          int num_bag_options = 4;
          int bag_cursor = 0;
          wrefresh(menuwin);
          // Menu options for switching pokemon
          while (in_bag_menu) {
            bag_highlight = 0;
            // Cursor logic 
            for(x = 0; x < num_bag_options; x++) {
              if(x == bag_cursor) {
                wattron(menuwin, A_REVERSE);
              }
              if(x == 0) {
                mvwprintw(menuwin, bag_highlight, 10, "%s", bag_options[x]);
              }
              else {
                mvwprintw(menuwin, bag_highlight, 10, "%s - %d", bag_options[x], x == 1 ? world.pc.revives : x == 2 ? world.pc.potions : world.pc.pokeballs);
              }
              
              wattroff(menuwin, A_REVERSE);
              bag_highlight++;
            }
            playerInput = wgetch(menuwin);
            switch (playerInput) {
              // Enter key
              case 10:
                if(bag_cursor != 0) {
                  // Revives submenu
                  if(!strcmp(bag_options[bag_cursor], "Revives")) {
                    if(world.pc.revives == 0) {
                      mvwprintw(menuwin, 9, 20, "No revives left!                    ");
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      cancel_move = true;
                    }
                    else {
                      // bool cancel_potion = false;
                      // Pokemon switch options
                      class pokemon *pokemon_options[7];
                      pokemon_options[0] = NULL;
                      int revive_cursor = 0;
                      int revive_highlight;
                      // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                      // 0 - 3, 1
                      int in_revive_menu = 1;
                      // Load potential switch candidates onto temporary array
                      int num_poke_options = 1;
                      for(x = 0; x < 6; x++) {
                        if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() == 0) {
                          pokemon_options[num_poke_options] = world.pc.buddy[x];
                          num_poke_options++;
                        }
                      }
                      wrefresh(menuwin);
                      // Menu options for switching pokemon
                      while (in_revive_menu) {
                        revive_highlight = 0;
                        // Cursor logic 
                        for(x = 0; x < num_poke_options; x++) {
                          if(x == revive_cursor) {
                            wattron(menuwin, A_REVERSE);
                          }
                          if(x == 0) {
                            mvwprintw(menuwin, revive_highlight, 25, "Back");
                          }
                          else {
                            mvwprintw(menuwin, revive_highlight, 25, "%s, HP: 0/%d", pokemon_options[x]->get_species(), pokemon_options[x]->get_hp());
                          }
                          wattroff(menuwin, A_REVERSE);
                          revive_highlight++;
                        }
                        playerInput = wgetch(menuwin);
                        switch (playerInput) {
                          // Enter key
                          case 10:
                            if(revive_cursor != 0) {
                              // std::ofstream myfile;
                              // myfile.open("dary.txt");
                              // myfile << revive_cursor << "\n";
                              // myfile.close();
                              // Revive pokemon
                              pokemon_options[revive_cursor]->set_current_hp((int) pokemon_options[revive_cursor]->get_hp() / 2);
                              world.pc.revives--;
                              mvwprintw(menuwin, 9, 20, "Revived %s to %d                    ", pokemon_options[revive_cursor]->get_species() , 
                                pokemon_options[revive_cursor]->get_current_hp());
                              wrefresh(menuwin);
                              wgetch(menuwin);
                            }
                            else {
                              cancel_move = true;
                            }
                            in_revive_menu = 0; 
                            break;
                          // Up and down logic
                          case KEY_UP:
                            if(revive_cursor != 0) {
                              revive_cursor--;
                            }
                            break;
                          case KEY_DOWN:
                            if(revive_cursor != num_poke_options - 1) {
                              revive_cursor++;
                            }
                            break;
                        }
                      }
                    }
                  }
                  // Potions submenu
                  if(!strcmp(bag_options[bag_cursor], "Potions")) {
                    if(world.pc.potions == 0) {
                      mvwprintw(menuwin, 9, 20, "No potions left!                    ");
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      cancel_move = true;
                    }
                    else {
                      // bool cancel_potion = false;
                      // Pokemon switch options
                      class pokemon *pokemon_options[7];
                      pokemon_options[0] = NULL;
                      int potion_cursor = 0;
                      int potion_highlight;
                      // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                      // 0 - 3, 1
                      int in_potion_menu = 1;
                      // Load potential switch candidates onto temporary array
                      int num_poke_options = 1;
                      for(x = 0; x < 6; x++) {
                        if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                          pokemon_options[num_poke_options] = world.pc.buddy[x];
                          num_poke_options++;
                        }
                      }
                      wrefresh(menuwin);
                      // Menu options for switching pokemon
                      while (in_potion_menu) {
                        potion_highlight = 0;
                        // Cursor logic 
                        for(x = 0; x < num_poke_options; x++) {
                          if(x == potion_cursor) {
                            wattron(menuwin, A_REVERSE);
                          }
                          if(x == 0) {
                            mvwprintw(menuwin, potion_highlight, 25, "Back");
                          }
                          else {
                            mvwprintw(menuwin, potion_highlight, 25, "%s, HP: %d/%d", pokemon_options[x]->get_species(), pokemon_options[x]->get_current_hp(), pokemon_options[x]->get_hp());
                          }
                          wattroff(menuwin, A_REVERSE);
                          potion_highlight++;
                        }
                        playerInput = wgetch(menuwin);
                        switch (playerInput) {
                          // Enter key
                          case 10:
                            if(potion_cursor != 0) {
                              // std::ofstream myfile;
                              // myfile.open("dary.txt");
                              // myfile << potion_cursor << "\n";
                              // myfile.close();

                              if(pokemon_options[potion_cursor]->get_current_hp() + 20 > pokemon_options[potion_cursor]->get_hp()) {
                                pokemon_options[potion_cursor]->set_current_hp(pokemon_options[potion_cursor]->get_hp());
                              }
                              else {
                                pokemon_options[potion_cursor]->set_current_hp(pokemon_options[potion_cursor]->get_current_hp() + 20);
                              }
                              world.pc.potions--;
                              mvwprintw(menuwin, 9, 20, "Healed %s to %d                    ", pokemon_options[potion_cursor]->get_species() , 
                                pokemon_options[potion_cursor]->get_current_hp());
                              wrefresh(menuwin);
                              wgetch(menuwin);
                            }
                            else {
                              cancel_move = true;
                            }
                            in_potion_menu = 0; 
                            break;
                          // Up and down logic
                          case KEY_UP:
                            if(potion_cursor != 0) {
                              potion_cursor--;
                            }
                            break;
                          case KEY_DOWN:
                            if(potion_cursor != num_poke_options - 1) {
                              potion_cursor++;
                            }
                            break;
                        }
                      }
                    }
                  }
                  if(!strcmp(bag_options[bag_cursor], "Pokeballs")) {
                    cancel_move = true;
                  }
                }
                else {
                  cancel_move = true;
                }
                in_bag_menu = 0; 
                break;
              // Up and down logic
              case KEY_UP:
                if(bag_cursor != 0) {
                  bag_cursor--;
                }
                break;
              case KEY_DOWN:
                if(bag_cursor != num_bag_options - 1) {
                  bag_cursor++;
                }
                break;
            }
          } 
          // BATTLE SEQUENCE
          // Opponents move after a player uses an item / switches pokemon
          // Feedback for move use
          // Opponent chooses random move
          if(!cancel_move) {
            for(x = 0; x < 3; x++) {
              if(strcmp(n->current_pokemon->get_move(x), "")) {
                num_opponent_moves++;
              }
            }
            opponent_move_index = n->current_pokemon->get_move_index(rand() % num_opponent_moves);

            mvwprintw(menuwin, 9, 20, "%s used %s!                ", n->current_pokemon->get_species(), moves[opponent_move_index].identifier);
            wrefresh(menuwin);
            wgetch(menuwin);
            // Damage formula
            if(rand() % 100 < moves[opponent_move_index].accuracy) {
              damage = ((((2 * n->current_pokemon->get_level()) / 5) + 2) * moves[opponent_move_index].power * (n->current_pokemon->get_atk() / world.pc.current_pokemon->get_def()));
              damage = (int) damage / 50;
              damage += 2;
              if(rand() % 256 < n->current_pokemon->get_speed()) {
                damage = damage * 1.5;
                mvwprintw(menuwin, 9, 20, "A critical hit!                    ");
                wrefresh(menuwin);
                wgetch(menuwin);
              }
              // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
              if(damage >= world.pc.current_pokemon->get_current_hp()) {
                mvwprintw(menuwin, 9, 20, "Your Pokemon fainted!                    ");
                wrefresh(menuwin);
                wgetch(menuwin);
                world.pc.current_pokemon->set_current_hp(0);
                bool pokemon_left = false;
                for(x = 0; x < 6; x++) {
                  if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                    pokemon_left = true;
                    break;
                  }
                }
                // Exit the game if all pokemon are down
                if(!pokemon_left) {
                  mvwprintw(menuwin, 9, 20, "You blacked out, exiting game                      ", n->current_pokemon->get_species());
                  wrefresh(menuwin);
                  wgetch(menuwin);
                  leaveBattle = 1;
                  world.quit = 1;
                }
                // Swap pokemon before next turn
                else {
                  ignore_player_move = true;
                  // Pokemon switch options
                  class pokemon *pokemon_options[6];
                  pokemon_options[0] = world.pc.current_pokemon;
                  int switch_cursor = 0;
                  int swtich_highlight;
                  // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                  // 0 - 3, 1
                  int in_switch_menu = 1;
                  // Load potential switch candidates onto temporary array
                  int num_poke_options = 1;
                  for(x = 0; x < 6; x++) {
                    if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
                      pokemon_options[num_poke_options] = world.pc.buddy[x];
                      num_poke_options++;
                    }
                  }
                  
                  // Menu options for switching pokemon
                  while (in_switch_menu) {
                    swtich_highlight = 0;
                    // Cursor logic 
                    for(x = 0; x < num_poke_options; x++) {
                      if(x == switch_cursor) {
                        wattron(menuwin, A_REVERSE);
                      }
                      if(x == 0) {
                        mvwprintw(menuwin, swtich_highlight, 10, "Back");
                      }
                      else {
                        mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
                      }
                      wattroff(menuwin, A_REVERSE);
                      swtich_highlight++;
                    }
                    wrefresh(menuwin);
                    playerInput = wgetch(menuwin);
                    switch (playerInput) {
                      // Enter key
                      case 10:
                        if(switch_cursor != 0) {
                          world.pc.current_pokemon = pokemon_options[switch_cursor];
                          in_switch_menu = 0; 
                        }
                        break;
                      // Up and down logic
                      case KEY_UP:
                        if(switch_cursor != 0) {
                          switch_cursor--;
                        }
                        break;
                      case KEY_DOWN:
                        if(switch_cursor != num_poke_options - 1) {
                          switch_cursor++;
                        }
                        break;
                    }
                  }
                  // Clear menu
                  wclear(menuwin);
                }
              }
              else {
                world.pc.current_pokemon->set_current_hp(world.pc.current_pokemon->get_current_hp() - damage);
              }
            }
            else {
              mvwprintw(menuwin, 9, 20, "%s's move missed!                      ", n->current_pokemon->get_species());
              wrefresh(menuwin);
              wgetch(menuwin);
            }
          }
        }
        // Pokemon switch sub-menu
        if(!(strcmp(options[cursor], "Pokemon"))) {
          // Pokemon switch options
          class pokemon *pokemon_options[6];
          pokemon_options[0] = world.pc.current_pokemon;
          int switch_cursor = 0;
          int swtich_highlight;
          // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
          // 0 - 3, 1
          int in_switch_menu = 1;
          // Load potential switch candidates onto temporary array
          int num_poke_options = 1;
          for(x = 0; x < 6; x++) {
            if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
              pokemon_options[num_poke_options] = world.pc.buddy[x];
              num_poke_options++;
            }
          }
          wrefresh(menuwin);
          // Menu options for switching pokemon
          while (in_switch_menu) {
            swtich_highlight = 0;
            // Cursor logic 
            for(x = 0; x < num_poke_options; x++) {
              if(x == switch_cursor) {
                wattron(menuwin, A_REVERSE);
              }
              if(x == 0) {
                mvwprintw(menuwin, swtich_highlight, 10, "Back");
              }
              else {
                mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
              }
              wattroff(menuwin, A_REVERSE);
              swtich_highlight++;
            }
            playerInput = wgetch(menuwin);
            switch (playerInput) {
              // Enter key
              case 10:
                if(switch_cursor != 0) {
                  world.pc.current_pokemon = pokemon_options[switch_cursor];
                }
                else {
                  cancel_move = true;
                }
                in_switch_menu = 0; 
                break;
              // Up and down logic
              case KEY_UP:
                if(switch_cursor != 0) {
                  switch_cursor--;
                }
                break;
              case KEY_DOWN:
                if(switch_cursor != num_poke_options - 1) {
                  switch_cursor++;
                }
                break;
            }
          }

          // BATTLE SEQUENCE
          // Opponents battle after using item / switching pokemon
          // Feedback for move use
          // Opponent chooses random move
          if(!cancel_move) {
            for(x = 0; x < 3; x++) {
              if(strcmp(n->current_pokemon->get_move(x), "")) {
                num_opponent_moves++;
              }
            }
            opponent_move_index = n->current_pokemon->get_move_index(rand() % num_opponent_moves);

            mvwprintw(menuwin, 9, 20, "%s used %s!                ", n->current_pokemon->get_species(), moves[opponent_move_index].identifier);
            wrefresh(menuwin);
            wgetch(menuwin);
            // Damage formula
            if(rand() % 100 < moves[opponent_move_index].accuracy) {
              damage = ((((2 * n->current_pokemon->get_level()) / 5) + 2) * moves[opponent_move_index].power * (n->current_pokemon->get_atk() / world.pc.current_pokemon->get_def()));
              damage = (int) damage / 50;
              damage += 2;
              if(rand() % 256 < n->current_pokemon->get_speed()) {
                damage = damage * 1.5;
                mvwprintw(menuwin, 9, 20, "A critical hit!                    ");
                wrefresh(menuwin);
                wgetch(menuwin);
              }
              // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
              if(damage >= world.pc.current_pokemon->get_current_hp()) {
                mvwprintw(menuwin, 9, 20, "Your Pokemon fainted!                    ");
                wrefresh(menuwin);
                wgetch(menuwin);
                world.pc.current_pokemon->set_current_hp(0);
                bool pokemon_left = false;
                for(x = 0; x < 6; x++) {
                  if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                    pokemon_left = true;
                    break;
                  }
                }
                // Exit the game if all pokemon are down
                if(!pokemon_left) {
                  mvwprintw(menuwin, 9, 20, "You blacked out, exiting game                      ", n->current_pokemon->get_species());
                  wrefresh(menuwin);
                  wgetch(menuwin);
                  leaveBattle = 1;
                  world.quit = 1;
                }
                // Swap pokemon before next turn
                else {
                  ignore_player_move = true;
                  // Pokemon switch options
                  class pokemon *pokemon_options[6];
                  pokemon_options[0] = world.pc.current_pokemon;
                  int switch_cursor = 0;
                  int swtich_highlight;
                  // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                  // 0 - 3, 1
                  int in_switch_menu = 1;
                  // Load potential switch candidates onto temporary array
                  int num_poke_options = 1;
                  for(x = 0; x < 6; x++) {
                    if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
                      pokemon_options[num_poke_options] = world.pc.buddy[x];
                      num_poke_options++;
                    }
                  }
                  
                  // Menu options for switching pokemon
                  while (in_switch_menu) {
                    swtich_highlight = 0;
                    // Cursor logic 
                    for(x = 0; x < num_poke_options; x++) {
                      if(x == switch_cursor) {
                        wattron(menuwin, A_REVERSE);
                      }
                      if(x == 0) {
                        mvwprintw(menuwin, swtich_highlight, 10, "Back");
                      }
                      else {
                        mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
                      }
                      wattroff(menuwin, A_REVERSE);
                      swtich_highlight++;
                    }
                    wrefresh(menuwin);
                    playerInput = wgetch(menuwin);
                    switch (playerInput) {
                      // Enter key
                      case 10:
                        if(switch_cursor != 0) {
                          world.pc.current_pokemon = pokemon_options[switch_cursor];
                          in_switch_menu = 0; 
                        }
                        break;
                      // Up and down logic
                      case KEY_UP:
                        if(switch_cursor != 0) {
                          switch_cursor--;
                        }
                        break;
                      case KEY_DOWN:
                        if(switch_cursor != num_poke_options - 1) {
                          switch_cursor++;
                        }
                        break;
                    }
                  }
                }
              }
              else {
                world.pc.current_pokemon->set_current_hp(world.pc.current_pokemon->get_current_hp() - damage);
              }
            }
            else {
              mvwprintw(menuwin, 9, 20, "%s's move missed!                      ", n->current_pokemon->get_species());
              wrefresh(menuwin);
              wgetch(menuwin);
            }
          }
        }
        // Do nothing on a run
        if(!(strcmp(options[cursor], "Run"))) {
          
        }
        break;
      default:
        break;
    }
  }
  wclear(menuwin);
  wrefresh(menuwin);
  clear();
  refresh();
}

uint32_t move_pc_dir(uint32_t input, pair_t dest)
{
  dest[dim_y] = world.pc.pos[dim_y];
  dest[dim_x] = world.pc.pos[dim_x];

  switch (input) {
  case 1:
  case 2:
  case 3:
    dest[dim_y]++;
    break;
  case 4:
  case 5:
  case 6:
    break;
  case 7:
  case 8:
  case 9:
    dest[dim_y]--;
    break;
  }
  switch (input) {
  case 1:
  case 4:
  case 7:
    dest[dim_x]--;
    break;
  case 2:
  case 5:
  case 8:
    break;
  case 3:
  case 6:
  case 9:
    dest[dim_x]++;
    break;
  case '>':
    if (world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]] ==
        ter_mart) {
      io_pokemart();
    }
    if (world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]] ==
        ter_center) {
      io_pokemon_center();
    }
    break;
  }

  if (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]) {
    if (dynamic_cast<npc *> (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]) &&
        ((npc *) world.cur_map->cmap[dest[dim_y]][dest[dim_x]])->defeated) {
      // Some kind of greeting here would be nice
      return 1;
    } else if ((dynamic_cast<npc *>
                (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]))) {
      io_battle(&world.pc, world.cur_map->cmap[dest[dim_y]][dest[dim_x]]);
      // Not actually moving, so set dest back to PC position
      dest[dim_x] = world.pc.pos[dim_x];
      dest[dim_y] = world.pc.pos[dim_y];
    }
  }
  
  if (move_cost[char_pc][world.cur_map->map[dest[dim_y]][dest[dim_x]]] ==
      DIJKSTRA_PATH_MAX) {
    return 1;
  }

  if (world.cur_map->map[dest[dim_y]][dest[dim_x]] == ter_gate &&
      dest[dim_y] != world.pc.pos[dim_y]                       &&
      dest[dim_x] != world.pc.pos[dim_x]) {
    return 1;
  }

  return 0;
}

void io_teleport_world(pair_t dest)
{
  /* mvscanw documentation is unclear about return values.  I believe *
   * that the return value works the same way as scanf, but instead   *
   * of counting on that, we'll initialize x and y to out of bounds   *
   * values and accept their updates only if in range.                */
  int x = INT_MAX, y = INT_MAX;
  
  world.cur_map->cmap[world.pc.pos[dim_y]][world.pc.pos[dim_x]] = NULL;

  echo();
  curs_set(1);
  do {
    mvprintw(0, 0, "Enter x [-200, 200]:           ");
    refresh();
    mvscanw(0, 21, (char *) "%d", &x);
  } while (x < -200 || x > 200);
  do {
    mvprintw(0, 0, "Enter y [-200, 200]:          ");
    refresh();
    mvscanw(0, 21, (char *) "%d", &y);
  } while (y < -200 || y > 200);

  refresh();
  noecho();
  curs_set(0);

  x += 200;
  y += 200;

  world.cur_idx[dim_x] = x;
  world.cur_idx[dim_y] = y;

  new_map(1);
  io_teleport_pc(dest);
}

void io_handle_input(pair_t dest)
{
  uint32_t turn_not_consumed;
  int key;

  do {
    switch (key = getch()) {
    case '7':
    case 'y':
    case KEY_HOME:
      turn_not_consumed = move_pc_dir(7, dest);
      break;
    case '8':
    case 'k':
    case KEY_UP:
      turn_not_consumed = move_pc_dir(8, dest);
      break;
    case '9':
    case 'u':
    case KEY_PPAGE:
      turn_not_consumed = move_pc_dir(9, dest);
      break;
    case '6':
    case 'l':
    case KEY_RIGHT:
      turn_not_consumed = move_pc_dir(6, dest);
      break;
    case '3':
    case 'n':
    case KEY_NPAGE:
      turn_not_consumed = move_pc_dir(3, dest);
      break;
    case '2':
    case 'j':
    case KEY_DOWN:
      turn_not_consumed = move_pc_dir(2, dest);
      break;
    case '1':
    case 'b':
    case KEY_END:
      turn_not_consumed = move_pc_dir(1, dest);
      break;
    case '4':
    case 'h':
    case KEY_LEFT:
      turn_not_consumed = move_pc_dir(4, dest);
      break;
    case '5':
    case ' ':
    case '.':
    case KEY_B2:
      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      turn_not_consumed = 0;
      break;
    case '>':
      turn_not_consumed = move_pc_dir('>', dest);
      break;
    case 'Q':
      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      world.quit = 1;
      turn_not_consumed = 0;
      break;
      break;
    case 't':
      io_list_trainers();
      turn_not_consumed = 1;
      break;
    case 'p':
      /* Teleport the PC to a random place in the map.              */
      io_teleport_pc(dest);
      turn_not_consumed = 0;
      break;
    case 'f':
      /* Fly to any map in the world.                                */
      io_teleport_world(dest);
      turn_not_consumed = 0;
      break;    
    case 'q':
      /* Demonstrate use of the message queue.  You can use this for *
       * printf()-style debugging (though gdb is probably a better   *
       * option.  Not that it matters, but using this command will   *
       * waste a turn.  Set turn_not_consumed to 1 and you should be *
       * able to figure out why I did it that way.                   */
      io_queue_message("This is the first message.");
      io_queue_message("Since there are multiple messages, "
                       "you will see \"more\" prompts.");
      io_queue_message("You can use any key to advance through messages.");
      io_queue_message("Normal gameplay will not resume until the queue "
                       "is empty.");
      io_queue_message("Long lines will be truncated, not wrapped.");
      io_queue_message("io_queue_message() is variadic and handles "
                       "all printf() conversion specifiers.");
      io_queue_message("Did you see %s?", "what I did there");
      io_queue_message("When the last message is displayed, there will "
                       "be no \"more\" prompt.");
      io_queue_message("Have fun!  And happy printing!");
      io_queue_message("Oh!  And use 'Q' to quit!");

      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      turn_not_consumed = 0;
      break;
    case 'B':
    {
        clear();
        const char *bag_options[4] = {"Back", "Revives", "Potions", "Pokeballs"};
        // Move options
        int x;
        int bag_highlight;
        // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
        // 0 - 3, 1
        int in_bag_menu = 1;
        // Load potential moves onto temporary array
        int num_bag_options = 4;
        int bag_cursor = 0;
        int playerInput;
        // Menu options for switching pokemon
        while (in_bag_menu) {
          bag_highlight = 0;
          // Cursor logic 
          for(x = 0; x < num_bag_options; x++) {
            if(x == bag_cursor) {
              attron(A_REVERSE);
            }
            if(x == 0) {
              mvprintw(bag_highlight, 1, "%s", bag_options[x]);
            }
            else {
              mvprintw(bag_highlight, 1, "%s - %d", bag_options[x], x == 1 ? world.pc.revives : x == 2 ? world.pc.potions : world.pc.pokeballs);
            }
            
            attroff(A_REVERSE);
            bag_highlight++;
          }
          refresh();
          playerInput = getch();
          switch (playerInput) {
            // Enter key
            case 10:
              if(bag_cursor != 0) {
                // Revives submenu
                if(!strcmp(bag_options[bag_cursor], "Revives")) {
                  if(world.pc.revives == 0) {
                    mvprintw(9, 20, "No revives left!                    ");
                    refresh();
                    getch();
                    
                  }
                  else {
                    // bool cancel_potion = false;
                    // Pokemon switch options
                    class pokemon *pokemon_options[7];
                    pokemon_options[0] = NULL;
                    int revive_cursor = 0;
                    int revive_highlight;
                    // mvprintw(optionsHighlight, 1, "%s", options[x]);
                    // 0 - 3, 1
                    int in_revive_menu = 1;
                    // Load potential switch candidates onto temporary array
                    int num_poke_options = 1;
                    for(x = 0; x < 6; x++) {
                      if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() == 0) {
                        pokemon_options[num_poke_options] = world.pc.buddy[x];
                        num_poke_options++;
                      }
                    }
                    refresh();
                    // Menu options for switching pokemon
                    while (in_revive_menu) {
                      revive_highlight = 0;
                      // Cursor logic 
                      for(x = 0; x < num_poke_options; x++) {
                        if(x == revive_cursor) {
                          attron(A_REVERSE);
                        }
                        if(x == 0) {
                          mvprintw(revive_highlight, 17, "Back");
                        }
                        else {
                          mvprintw(revive_highlight, 17, "%s, HP: 0/%d", pokemon_options[x]->get_species(), pokemon_options[x]->get_hp());
                        }
                        attroff(A_REVERSE);
                        revive_highlight++;
                      }
                      playerInput = getch();
                      switch (playerInput) {
                        // Enter key
                        case 10:
                          if(revive_cursor != 0) {
                            // std::ofstream myfile;
                            // myfile.open("dary.txt");
                            // myfile << revive_cursor << "\n";
                            // myfile.close();
                            // Revive pokemon
                            pokemon_options[revive_cursor]->set_current_hp((int) pokemon_options[revive_cursor]->get_hp() / 2);
                            world.pc.revives--;
                            mvprintw(9, 20, "Revived %s to %d                    ", pokemon_options[revive_cursor]->get_species() , 
                              pokemon_options[revive_cursor]->get_current_hp());
                            refresh();
                            getch();
                            in_bag_menu = 0;
                          }
                          else {
                            
                          }
                          in_revive_menu = 0; 
                          break;
                        // Up and down logic
                        case KEY_UP:
                          if(revive_cursor != 0) {
                            revive_cursor--;
                          }
                          break;
                        case KEY_DOWN:
                          if(revive_cursor != num_poke_options - 1) {
                            revive_cursor++;
                          }
                          break;
                      }
                    }
                  }
                }
                // Potions submenu
                if(!strcmp(bag_options[bag_cursor], "Potions")) {
                  if(world.pc.potions == 0) {
                    mvprintw(9, 20, "No potions left!                    ");
                    refresh();
                    getch();
                    
                  }
                  else {
                    // bool cancel_potion = false;
                    // Pokemon switch options
                    class pokemon *pokemon_options[7];
                    pokemon_options[0] = NULL;
                    int potion_cursor = 0;
                    int potion_highlight;
                    // mvprintw(optionsHighlight, 1, "%s", options[x]);
                    // 0 - 3, 1
                    int in_potion_menu = 1;
                    // Load potential switch candidates onto temporary array
                    int num_poke_options = 1;
                    for(x = 0; x < 6; x++) {
                      if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                        pokemon_options[num_poke_options] = world.pc.buddy[x];
                        num_poke_options++;
                      }
                    }
                    refresh();
                    // Menu options for switching pokemon
                    while (in_potion_menu) {
                      potion_highlight = 0;
                      // Cursor logic 
                      for(x = 0; x < num_poke_options; x++) {
                        if(x == potion_cursor) {
                          attron(A_REVERSE);
                        }
                        if(x == 0) {
                          mvprintw(potion_highlight, 17, "Back");
                        }
                        else {
                          mvprintw(potion_highlight, 17, "%s, HP: %d/%d", pokemon_options[x]->get_species(), pokemon_options[x]->get_current_hp(), pokemon_options[x]->get_hp());
                        }
                        attroff(A_REVERSE);
                        potion_highlight++;
                      }
                      playerInput = getch();
                      switch (playerInput) {
                        // Enter key
                        case 10:
                          if(potion_cursor != 0) {
                            // std::ofstream myfile;
                            // myfile.open("dary.txt");
                            // myfile << potion_cursor << "\n";
                            // myfile.close();

                            if(pokemon_options[potion_cursor]->get_current_hp() + 20 > pokemon_options[potion_cursor]->get_hp()) {
                              pokemon_options[potion_cursor]->set_current_hp(pokemon_options[potion_cursor]->get_hp());
                            }
                            else {
                              pokemon_options[potion_cursor]->set_current_hp(pokemon_options[potion_cursor]->get_current_hp() + 20);
                            }
                            world.pc.potions--;
                            mvprintw(9, 20, "Healed %s to %d                    ", pokemon_options[potion_cursor]->get_species() , 
                              pokemon_options[potion_cursor]->get_current_hp());
                            in_bag_menu = 0;
                            getch();
                            refresh();
                          }
                          in_potion_menu = 0; 
                          break;
                        // Up and down logic
                        case KEY_UP:
                          if(potion_cursor != 0) {
                            potion_cursor--;
                          }
                          break;
                        case KEY_DOWN:
                          if(potion_cursor != num_poke_options - 1) {
                            potion_cursor++;
                          }
                          break;
                      }
                    }
                  }
                }
              }
              else {
                in_bag_menu = 0;
              }
              break;
            // Up and down logic
            case KEY_UP:
              if(bag_cursor != 0) {
                bag_cursor--;
              }
              break;
            case KEY_DOWN:
              if(bag_cursor != num_bag_options - 1) {
                bag_cursor++;
              }
              break;
          }
        }
        turn_not_consumed = 1;
        clear();
        io_display();
        refresh();
        break;
      }
    default:
      /* Also not in the spec.  It's not always easy to figure out what *
       * key code corresponds with a given keystroke.  Print out any    *
       * unhandled key here.  Not only does it give a visual error      *
       * indicator, but it also gives an integer value that can be used *
       * for that key in this (or other) switch statements.  Printed in *
       * octal, with the leading zero, because ncurses.h lists codes in *
       * octal, thus allowing us to do reverse lookups.  If a key has a *
       * name defined in the header, you can use the name here, else    *
       * you can directly use the octal value.                          */
      mvprintw(0, 0, "Unbound key: %#o ", key);
      turn_not_consumed = 1;
      break;
    }
    refresh();
  } while (turn_not_consumed);
}

void io_encounter_pokemon()
{
  class pokemon *p;

  p = new class pokemon();

  // int i;

  // if (!n->buddy[1]) {
  //   s = "My pokemon is " + std::string(n->buddy[0]->get_species());
  // } else {
  //   s = "My pokemon are " + std::string(n->buddy[0]->get_species());
  // }

  // for (i = 1; i < 6 && n->buddy[i]; i++) {
  //   s += ", ";
  //   if (i == 4 || !n->buddy[i + 1]) {
  //     s += "and ";
  //   }
  //   s += n->buddy[i]->get_species();
  // }
    
  // s += ".";

  // io_queue_message("%s", s.c_str());

  int leaveBattle = 0;
  int x;
  // Keep track of highlight on list
  int optionsHighlight;
  // Keep track of position on list
  int cursor = 0;
  // Take player input
  int playerInput;
  // Create trainers menu
  WINDOW * menuwin = newwin(world.char_seq_num, 0, 0, 0);
  clear();
  refresh();
  // wrefresh(menuwin);
  keypad(menuwin, 1);
  // Inital battle menu options
  const char *options[4] = {"Fight", "Bag", "Pokemon", "Run"};
  // Bag Options
  // const char *bag_options[3] = {"Revives", "Potions", "Pokeballs"};
  
  // TODO: Pokemon Revive options
  while(!leaveBattle) {
    // Move stuff
    int num_opponent_moves = 0;
    int player_move_index;
    int opponent_move_index;
    int move_cursor = 0;
    int damage = 0;
    bool cancel_move = false;
    // players typically go first, check for any changes
    bool player_first = true;
    // Players may not use items or switch Pokemon, so check for this change
    bool ignore_player_move = false;

    wclear(menuwin);
    mvwprintw(menuwin, 12, 20, "Player's current pokemon %s, lv%d", world.pc.current_pokemon->get_species(), world.pc.current_pokemon->get_level());
    mvwprintw(menuwin, 13, 20, "HP %d / %d", world.pc.current_pokemon->get_current_hp(), world.pc.current_pokemon->get_hp());
    mvwprintw(menuwin, 15, 20, "Wild pokemon %s, lv%d", p->get_species(), p->get_level());
    mvwprintw(menuwin, 16, 20, "HP %d / %d", p->get_current_hp(), p->get_hp());
    
    optionsHighlight = 0;
    for(x = 0; x <= 3; x++) {
      if(x == cursor) {
        wattron(menuwin, A_REVERSE);
      }
      mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
      wattroff(menuwin, A_REVERSE);
      optionsHighlight++;
    }
    // total_trainers = 0;
    // Output board
    wrefresh(menuwin);
    playerInput = wgetch(menuwin);
    if(playerInput == 27) {
      leaveBattle = 1;
    }
    switch (playerInput) {
      case 27:
        leaveBattle = 1;
        break;
      case KEY_UP:
        if(cursor != 0) {
          cursor--;
        }
        break;
      case KEY_DOWN:
        if(cursor != 3) {
          cursor++;
        }
        break;
      case 10:
        // const char *options[4] = {"Fight", "Bag", "Pokemon", "Run"};
        if(!(strcmp(options[cursor], "Fight"))) {
          // Move options
          int move_options[3];
          
          int move_highlight;

          // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
          // 0 - 3, 1
          int in_move_menu = 1;
          // Load potential moves onto temporary array
          int num_move_options = 1;
          for(x = 0; x < 3; x++) {
            if(strcmp(world.pc.current_pokemon->get_move(x), "")) {
              move_options[num_move_options] = world.pc.current_pokemon->get_move_index(x);
              num_move_options++;
            }
          }
          wrefresh(menuwin);
          // Menu options for switching pokemon
          while (in_move_menu) {
            move_highlight = 0;
            // Cursor logic 
            for(x = 0; x < num_move_options; x++) {
              if(x == move_cursor) {
                wattron(menuwin, A_REVERSE);
              }
              if(x == 0) {
                mvwprintw(menuwin, move_highlight, 10, "Back");
              }
              else {
                mvwprintw(menuwin, move_highlight, 10, "%s", moves[move_options[x]].identifier);
              }
              wattroff(menuwin, A_REVERSE);
              move_highlight++;
            }
            playerInput = wgetch(menuwin);
            switch (playerInput) {
              // Enter key
              case 10:
                if(move_cursor != 0) {
                  // Move logic
                  player_move_index = world.pc.current_pokemon->get_move_index(move_cursor - 1);
                }
                else {
                  cancel_move = true;
                }
                in_move_menu = 0; 
                break;
              // Up and down logic
              case KEY_UP:
                if(move_cursor != 0) {
                  move_cursor--;
                }
                break;
              case KEY_DOWN:
                if(move_cursor != num_move_options - 1) {
                  move_cursor++;
                }
                break;
            }
          } 
          // Opponent chooses random move
          for(x = 0; x < 3; x++) {
            if(strcmp(p->get_move(x), "")) {
              num_opponent_moves++;
            }
          }
          opponent_move_index = p->get_move_index(rand() % num_opponent_moves);
          // Check whether player goes first by priority
          if(moves[player_move_index].priority < moves[opponent_move_index].priority) {
            player_first = false;
          }
          // Check whether player goes first by speed
          else if (moves[player_move_index].priority == moves[opponent_move_index].priority && 
          world.pc.current_pokemon->get_speed() < p->get_speed()) {  
            player_first = false;
          }
          // Check whether player goes first by random
          else if (moves[player_move_index].priority == moves[opponent_move_index].priority && 
          world.pc.current_pokemon->get_speed() == p->get_speed()){
            player_first = rand() % 2 == 0 ? true : false;
          }
          if(!cancel_move) { 
            
            // Player goes first
            if(!ignore_player_move && player_first) {
              // Feedback for move use
              mvwprintw(menuwin, 9, 20, "%s used %s!                          ", world.pc.current_pokemon->get_species(), moves[player_move_index].identifier);
              wrefresh(menuwin);
              wgetch(menuwin);
              if(rand() % 100 < moves[player_move_index].accuracy) {
                // Damage formula
                damage = ((((2 * world.pc.current_pokemon->get_level()) / 5) + 2) * moves[player_move_index].power * (world.pc.current_pokemon->get_atk() / p->get_def()));
                damage = (int) damage / 50;
                damage += 2;
                damage = (int) (damage * ((rand() % 100 - 85 + 1) + 85));
                // if(rand() % 256 < world.pc.current_pokemon) {
                //   damage = damage * 1.5;
                //   mvwprintw(menuwin, 9, 20, "A critical hit!                      ");
                //   wrefresh(menuwin);
                //   wgetch(menuwin);
                // }
                // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
                // Hard floor of 1 damage for moves with power
                if(damage == 0 && moves[player_move_index].power != 0) {
                  damage = 1;
                }
                // Win Pokemon Battle if last pokemon logic
                if(damage >= p->get_current_hp()) {
                  
                  p->set_current_hp(0);
                  
                  
                  mvwprintw(menuwin, 9, 20, "You won!                     ", p->get_species());
                  wrefresh(menuwin);
                  wgetch(menuwin);
                  leaveBattle = 1;
                  cancel_move = true;
                  delete p;
                  
                }
                else {
                  p->set_current_hp(p->get_current_hp() - damage);
                }
              }
              // Player Move missed
              else {
                mvwprintw(menuwin, 9, 20, "Your Move missed!                    ");
                wrefresh(menuwin);
                wgetch(menuwin);
              }
              if(!cancel_move) {
                // Feedback for move use
                mvwprintw(menuwin, 9, 20, "%s used %s!                ", p->get_species(), moves[opponent_move_index].identifier);
                wrefresh(menuwin);
                wgetch(menuwin);
                // Damage formula
                if(rand() % 100 < moves[opponent_move_index].accuracy) {
                  damage = ((((2 * p->get_level()) / 5) + 2) * moves[opponent_move_index].power * (p->get_atk() / world.pc.current_pokemon->get_def()));
                  damage = (int) damage / 50;
                  damage += 2;
                  if(rand() % 256 < p->get_speed()) {
                    damage = damage * 1.5;
                    mvwprintw(menuwin, 9, 20, "A critical hit!                    ");
                    wrefresh(menuwin);
                    wgetch(menuwin);
                  }
                  // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
                  if(damage >= world.pc.current_pokemon->get_current_hp()) {
                    mvwprintw(menuwin, 9, 20, "Your Pokemon fainted!                    ", p->get_species());
                    wrefresh(menuwin);
                    wgetch(menuwin);
                    world.pc.current_pokemon->set_current_hp(0);
                    bool pokemon_left = false;
                    for(x = 0; x < 6; x++) {
                      if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                        pokemon_left = true;
                        break;
                      }
                    }
                    // Exit the game if all pokemon are down
                    if(!pokemon_left) {
                      mvwprintw(menuwin, 9, 20, "You blacked out, exiting game                      ", p->get_species());
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      leaveBattle = 1;
                      world.quit = 1;
                    }
                    // Swap pokemon before next turn
                    else {
                      ignore_player_move = true;
                      // Pokemon switch options
                      class pokemon *pokemon_options[6];
                      pokemon_options[0] = world.pc.current_pokemon;
                      int switch_cursor = 0;
                      int swtich_highlight;
                      // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                      // 0 - 3, 1
                      int in_switch_menu = 1;
                      // Load potential switch candidates onto temporary array
                      int num_poke_options = 1;
                      for(x = 0; x < 6; x++) {
                        if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
                          pokemon_options[num_poke_options] = world.pc.buddy[x];
                          num_poke_options++;
                        }
                      }
                      
                      // Menu options for switching pokemon
                      while (in_switch_menu) {
                        swtich_highlight = 0;
                        // Cursor logic 
                        for(x = 0; x < num_poke_options; x++) {
                          if(x == switch_cursor) {
                            wattron(menuwin, A_REVERSE);
                          }
                          if(x == 0) {
                            mvwprintw(menuwin, swtich_highlight, 10, "Back");
                          }
                          else {
                            mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
                          }
                          wattroff(menuwin, A_REVERSE);
                          swtich_highlight++;
                        }
                        wrefresh(menuwin);
                        playerInput = wgetch(menuwin);
                        switch (playerInput) {
                          // Enter key
                          case 10:
                            if(switch_cursor != 0) {
                              world.pc.current_pokemon = pokemon_options[switch_cursor];
                              in_switch_menu = 0;
                            }
                            break;
                          // Up and down logic
                          case KEY_UP:
                            if(switch_cursor != 0) {
                              switch_cursor--;
                            }
                            break;
                          case KEY_DOWN:
                            if(switch_cursor != num_poke_options - 1) {
                              switch_cursor++;
                            }
                            break;
                        }
                      }
                      // Clear menu
                      wclear(menuwin);
                    }
                  }
                  else {
                    world.pc.current_pokemon->set_current_hp(world.pc.current_pokemon->get_current_hp() - damage);
                  }
                }
                else {
                  mvwprintw(menuwin, 9, 20, "%s's move missed!                      ", p->get_species());
                  wrefresh(menuwin);
                  wgetch(menuwin);
                }
              }
            }
            // Opponent goes first
            else if (!ignore_player_move && !player_first) {
              // Feedback for move use
              mvwprintw(menuwin, 9, 20, "%s used %s!                ", p->get_species(), moves[opponent_move_index].identifier);
              wrefresh(menuwin);
              wgetch(menuwin);
              // Damage formula
              if(rand() % 100 < moves[opponent_move_index].accuracy) {
                damage = ((((2 * p->get_level()) / 5) + 2) * moves[opponent_move_index].power * (p->get_atk() / world.pc.current_pokemon->get_def()));
                damage = (int) damage / 50;
                damage += 2;
                if(rand() % 256 < p->get_speed()) {
                  damage = damage * 1.5;
                  mvwprintw(menuwin, 9, 20, "A critical hit!                    ");
                  wrefresh(menuwin);
                  wgetch(menuwin);
                }
                // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
                if(damage >= world.pc.current_pokemon->get_current_hp()) {
                  mvwprintw(menuwin, 9, 20, "Your Pokemon fainted!                    ", p->get_species());
                  wrefresh(menuwin);
                  wgetch(menuwin);
                  world.pc.current_pokemon->set_current_hp(0);
                  bool pokemon_left = false;
                  for(x = 0; x < 6; x++) {
                    if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                      pokemon_left = true;
                      break;
                    }
                  }
                  // Exit the game if all pokemon are down
                  if(!pokemon_left) {
                    mvwprintw(menuwin, 9, 20, "You blacked out, exiting game                      ", p->get_species());
                    wrefresh(menuwin);
                    wgetch(menuwin);
                    leaveBattle = 1;
                    world.quit = 1;
                    cancel_move = true;
                  }
                  // Swap pokemon before next turn
                  else {
                    ignore_player_move = true;
                    // Pokemon switch options
                    class pokemon *pokemon_options[6];
                    pokemon_options[0] = world.pc.current_pokemon;
                    int switch_cursor = 0;
                    int swtich_highlight;
                    // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                    // 0 - 3, 1
                    int in_switch_menu = 1;
                    // Load potential switch candidates onto temporary array
                    int num_poke_options = 1;
                    for(x = 0; x < 6; x++) {
                      if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
                        pokemon_options[num_poke_options] = world.pc.buddy[x];
                        num_poke_options++;
                      }
                    }
                    // Menu options for switching pokemon
                    while (in_switch_menu) {
                      swtich_highlight = 0;
                      // Cursor logic 
                      for(x = 0; x < num_poke_options; x++) {
                        if(x == switch_cursor) {
                          wattron(menuwin, A_REVERSE);
                        }
                        if(x == 0) {
                          mvwprintw(menuwin, swtich_highlight, 10, "Back");
                        }
                        else {
                          mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
                        }
                        wattroff(menuwin, A_REVERSE);
                        swtich_highlight++;
                      }
                      wrefresh(menuwin);
                      playerInput = wgetch(menuwin);
                      switch (playerInput) {
                        // Enter key
                        case 10:
                          if(switch_cursor != 0) {
                            world.pc.current_pokemon = pokemon_options[switch_cursor];
                            in_switch_menu = 0;
                          }
                          break;
                        // Up and down logic
                        case KEY_UP:
                          if(switch_cursor != 0) {
                            switch_cursor--;
                          }
                          break;
                        case KEY_DOWN:
                          if(switch_cursor != num_poke_options - 1) {
                            switch_cursor++;
                          }
                          break;
                      }
                    }
                    // Clear menu
                    wclear(menuwin);
                    // Cancel move
                    cancel_move = true;
                  }
                }
                else {
                  world.pc.current_pokemon->set_current_hp(world.pc.current_pokemon->get_current_hp() - damage);
                }
              }
              else {
                mvwprintw(menuwin, 9, 20, "%s's move missed!                      ", p->get_species());
                wrefresh(menuwin);
                wgetch(menuwin);
              }
              if(!cancel_move) {
                // Feedback for move use
                mvwprintw(menuwin, 9, 20, "%s used %s!                          ", world.pc.current_pokemon->get_species(), moves[player_move_index].identifier);
                wrefresh(menuwin);
                wgetch(menuwin);
                if(rand() % 100 < moves[player_move_index].accuracy) {
                  // Damage formula
                  damage = ((((2 * world.pc.current_pokemon->get_level()) / 5) + 2) * moves[player_move_index].power * (world.pc.current_pokemon->get_atk() / p->get_def()));
                  damage = (int) damage / 50;
                  damage += 2;
                  if(rand() % 256 < world.pc.current_pokemon->get_speed()) {
                    damage = damage * 1.5;
                    mvwprintw(menuwin, 9, 20, "A critical hit!                      ");
                    wrefresh(menuwin);
                    wgetch(menuwin);
                  }
                  // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
                  // Hard floor of 1 damage for moves with power
                  if(damage == 0 && moves[player_move_index].power != 0) {
                    damage = 1;
                  }
                  // Win Pokemon Battle if last pokemon logic
                  if(damage >= p->get_current_hp()) {
                    
                    p->set_current_hp(0);
                  
                    mvwprintw(menuwin, 9, 20, "You won!                     ", p->get_species());
                    wrefresh(menuwin);
                    wgetch(menuwin);
                    leaveBattle = 1;
                    cancel_move = true;
                    delete p;
                  }
                  else {
                    p->set_current_hp(p->get_current_hp() - damage);
                  }
                }
                // Player Move missed
                else {
                  mvwprintw(menuwin, 9, 20, "Your Move missed!                    ");
                  wrefresh(menuwin);
                  wgetch(menuwin);
                }
              }
            }
          }
          // Clear menu
          wclear(menuwin);
        }
        if(!(strcmp(options[cursor], "Bag"))) {
          const char *bag_options[4] = {"Back", "Revives", "Potions", "Pokeballs"};
          // Move options
          
          int bag_highlight;
          // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
          // 0 - 3, 1
          int in_bag_menu = 1;
          // Load potential moves onto temporary array
          int num_bag_options = 4;
          int bag_cursor = 0;
          wrefresh(menuwin);
          // Menu options for switching pokemon
          while (in_bag_menu) {
            bag_highlight = 0;
            // Cursor logic 
            for(x = 0; x < num_bag_options; x++) {
              if(x == bag_cursor) {
                wattron(menuwin, A_REVERSE);
              }
              if(x == 0) {
                mvwprintw(menuwin, bag_highlight, 10, "%s", bag_options[x]);
              }
              else {
                mvwprintw(menuwin, bag_highlight, 10, "%s - %d", bag_options[x], x == 1 ? world.pc.revives : x == 2 ? world.pc.potions : world.pc.pokeballs);
              }
              
              wattroff(menuwin, A_REVERSE);
              bag_highlight++;
            }
            playerInput = wgetch(menuwin);
            switch (playerInput) {
              // Enter key
              case 10:
                if(bag_cursor != 0) {
                  // Revives submenu
                  if(!strcmp(bag_options[bag_cursor], "Revives")) {
                    if(world.pc.revives == 0) {
                      mvwprintw(menuwin, 9, 20, "No revives left!                    ");
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      cancel_move = true;
                    }
                    else {
                      // bool cancel_potion = false;
                      // Pokemon switch options
                      class pokemon *pokemon_options[7];
                      pokemon_options[0] = NULL;
                      int revive_cursor = 0;
                      int revive_highlight;
                      // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                      // 0 - 3, 1
                      int in_revive_menu = 1;
                      // Load potential switch candidates onto temporary array
                      int num_poke_options = 1;
                      for(x = 0; x < 6; x++) {
                        if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() == 0) {
                          pokemon_options[num_poke_options] = world.pc.buddy[x];
                          num_poke_options++;
                        }
                      }
                      wrefresh(menuwin);
                      // Menu options for switching pokemon
                      while (in_revive_menu) {
                        revive_highlight = 0;
                        // Cursor logic 
                        for(x = 0; x < num_poke_options; x++) {
                          if(x == revive_cursor) {
                            wattron(menuwin, A_REVERSE);
                          }
                          if(x == 0) {
                            mvwprintw(menuwin, revive_highlight, 25, "Back");
                          }
                          else {
                            mvwprintw(menuwin, revive_highlight, 25, "%s, HP: 0/%d", pokemon_options[x]->get_species(), pokemon_options[x]->get_hp());
                          }
                          wattroff(menuwin, A_REVERSE);
                          revive_highlight++;
                        }
                        playerInput = wgetch(menuwin);
                        switch (playerInput) {
                          // Enter key
                          case 10:
                            if(revive_cursor != 0) {
                              // std::ofstream myfile;
                              // myfile.open("dary.txt");
                              // myfile << revive_cursor << "\n";
                              // myfile.close();
                              // Revive pokemon
                              pokemon_options[revive_cursor]->set_current_hp((int) pokemon_options[revive_cursor]->get_hp() / 2);
                              world.pc.revives--;
                              mvwprintw(menuwin, 9, 20, "Revived %s to %d                    ", pokemon_options[revive_cursor]->get_species() , 
                                pokemon_options[revive_cursor]->get_current_hp());
                              wrefresh(menuwin);
                              wgetch(menuwin);
                            }
                            else {
                              cancel_move = true;
                            }
                            in_revive_menu = 0; 
                            break;
                          // Up and down logic
                          case KEY_UP:
                            if(revive_cursor != 0) {
                              revive_cursor--;
                            }
                            break;
                          case KEY_DOWN:
                            if(revive_cursor != num_poke_options - 1) {
                              revive_cursor++;
                            }
                            break;
                        }
                      }
                    }
                  }
                  // Potions submenu
                  if(!strcmp(bag_options[bag_cursor], "Potions")) {
                    if(world.pc.potions == 0) {
                      mvwprintw(menuwin, 9, 20, "No potions left!                    ");
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      cancel_move = true;
                    }
                    else {
                      // bool cancel_potion = false;
                      // Pokemon switch options
                      class pokemon *pokemon_options[7];
                      pokemon_options[0] = NULL;
                      int potion_cursor = 0;
                      int potion_highlight;
                      // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                      // 0 - 3, 1
                      int in_potion_menu = 1;
                      // Load potential switch candidates onto temporary array
                      int num_poke_options = 1;
                      for(x = 0; x < 6; x++) {
                        if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                          pokemon_options[num_poke_options] = world.pc.buddy[x];
                          num_poke_options++;
                        }
                      }
                      wrefresh(menuwin);
                      // Menu options for switching pokemon
                      while (in_potion_menu) {
                        potion_highlight = 0;
                        // Cursor logic 
                        for(x = 0; x < num_poke_options; x++) {
                          if(x == potion_cursor) {
                            wattron(menuwin, A_REVERSE);
                          }
                          if(x == 0) {
                            mvwprintw(menuwin, potion_highlight, 25, "Back");
                          }
                          else {
                            mvwprintw(menuwin, potion_highlight, 25, "%s, HP: %d/%d", pokemon_options[x]->get_species(), pokemon_options[x]->get_current_hp(), pokemon_options[x]->get_hp());
                          }
                          wattroff(menuwin, A_REVERSE);
                          potion_highlight++;
                        }
                        playerInput = wgetch(menuwin);
                        switch (playerInput) {
                          // Enter key
                          case 10:
                            if(potion_cursor != 0) {
                              // std::ofstream myfile;
                              // myfile.open("dary.txt");
                              // myfile << potion_cursor << "\n";
                              // myfile.close();

                              if(pokemon_options[potion_cursor]->get_current_hp() + 20 > pokemon_options[potion_cursor]->get_hp()) {
                                pokemon_options[potion_cursor]->set_current_hp(pokemon_options[potion_cursor]->get_hp());
                              }
                              else {
                                pokemon_options[potion_cursor]->set_current_hp(pokemon_options[potion_cursor]->get_current_hp() + 20);
                              }
                              world.pc.potions--;
                              mvwprintw(menuwin, 9, 20, "Healed %s to %d                    ", pokemon_options[potion_cursor]->get_species() , 
                                pokemon_options[potion_cursor]->get_current_hp());
                              wrefresh(menuwin);
                              wgetch(menuwin);
                            }
                            else {
                              cancel_move = true;
                            }
                            in_potion_menu = 0; 
                            break;
                          // Up and down logic
                          case KEY_UP:
                            if(potion_cursor != 0) {
                              potion_cursor--;
                            }
                            break;
                          case KEY_DOWN:
                            if(potion_cursor != num_poke_options - 1) {
                              potion_cursor++;
                            }
                            break;
                        }
                      }
                    }
                  }
                  // Out of pokeballs
                  if(!strcmp(bag_options[bag_cursor], "Pokeballs")) {
                    if(world.pc.pokeballs == 0) {
                      mvwprintw(menuwin, 9, 20, "Darn, out of Pokeballs!                   ");
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      cancel_move = true;
                    }
                    int open_slots = 0;
                    // Caught
                    for(int x = 0; x < 6; x++) {
                      if(world.pc.buddy[x] == NULL) {
                        // mvwprintw(menuwin, 9, 20, "Healed %s to %d                    ", pokemon_options[potion_cursor]->get_species())
                        open_slots = x;
                        break;
                      }
                    }
                    if(open_slots != 0) {
                      world.pc.buddy[open_slots] = p;
                      mvwprintw(menuwin, 9, 20, "Caught %s!                   ", p->get_species());
                      wrefresh(menuwin);
                      wgetch(menuwin);
                      cancel_move = true;
                      leaveBattle = 1;
                    }
                    // Party is full
                    else {
                      mvwprintw(menuwin, 9, 20, "Darn, party's full!                   ");
                      wrefresh(menuwin);
                      wgetch(menuwin);
                    }
                    world.pc.pokeballs--;
                  }
                }
                else {
                  cancel_move = true;
                }
                in_bag_menu = 0; 
                break;
              // Up and down logic
              case KEY_UP:
                if(bag_cursor != 0) {
                  bag_cursor--;
                }
                break;
              case KEY_DOWN:
                if(bag_cursor != num_bag_options - 1) {
                  bag_cursor++;
                }
                break;
            }
          } 
          // BATTLE SEQUENCE
          // Opponents move after a player uses an item / switches pokemon
          // Feedback for move use
          // Opponent chooses random move
          if(!cancel_move) {
            for(x = 0; x < 3; x++) {
              if(strcmp(p->get_move(x), "")) {
                num_opponent_moves++;
              }
            }
            opponent_move_index = p->get_move_index(rand() % num_opponent_moves);

            mvwprintw(menuwin, 9, 20, "%s used %s!                ", p->get_species(), moves[opponent_move_index].identifier);
            wrefresh(menuwin);
            wgetch(menuwin);
            // Damage formula
            if(rand() % 100 < moves[opponent_move_index].accuracy) {
              damage = ((((2 * p->get_level()) / 5) + 2) * moves[opponent_move_index].power * (p->get_atk() / world.pc.current_pokemon->get_def()));
              damage = (int) damage / 50;
              damage += 2;
              if(rand() % 256 < p->get_speed()) {
                damage = damage * 1.5;
                mvwprintw(menuwin, 9, 20, "A critical hit!                    ");
                wrefresh(menuwin);
                wgetch(menuwin);
              }
              // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
              if(damage >= world.pc.current_pokemon->get_current_hp()) {
                mvwprintw(menuwin, 9, 20, "Your Pokemon fainted!                    ", p->get_species());
                wrefresh(menuwin);
                wgetch(menuwin);
                world.pc.current_pokemon->set_current_hp(0);
                bool pokemon_left = false;
                for(x = 0; x < 6; x++) {
                  if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                    pokemon_left = true;
                    break;
                  }
                }
                // Exit the game if all pokemon are down
                if(!pokemon_left) {
                  mvwprintw(menuwin, 9, 20, "You blacked out, exiting game                      ", p->get_species());
                  wrefresh(menuwin);
                  wgetch(menuwin);
                  leaveBattle = 1;
                  world.quit = 1;
                }
                // Swap pokemon before next turn
                else {
                  ignore_player_move = true;
                  // Pokemon switch options
                  class pokemon *pokemon_options[6];
                  pokemon_options[0] = world.pc.current_pokemon;
                  int switch_cursor = 0;
                  int swtich_highlight;
                  // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                  // 0 - 3, 1
                  int in_switch_menu = 1;
                  // Load potential switch candidates onto temporary array
                  int num_poke_options = 1;
                  for(x = 0; x < 6; x++) {
                    if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
                      pokemon_options[num_poke_options] = world.pc.buddy[x];
                      num_poke_options++;
                    }
                  }
                  
                  // Menu options for switching pokemon
                  while (in_switch_menu) {
                    swtich_highlight = 0;
                    // Cursor logic 
                    for(x = 0; x < num_poke_options; x++) {
                      if(x == switch_cursor) {
                        wattron(menuwin, A_REVERSE);
                      }
                      if(x == 0) {
                        mvwprintw(menuwin, swtich_highlight, 10, "Back");
                      }
                      else {
                        mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
                      }
                      wattroff(menuwin, A_REVERSE);
                      swtich_highlight++;
                    }
                    wrefresh(menuwin);
                    playerInput = wgetch(menuwin);
                    switch (playerInput) {
                      // Enter key
                      case 10:
                        if(switch_cursor != 0) {
                          world.pc.current_pokemon = pokemon_options[switch_cursor];
                          in_switch_menu = 0; 
                        }
                        break;
                      // Up and down logic
                      case KEY_UP:
                        if(switch_cursor != 0) {
                          switch_cursor--;
                        }
                        break;
                      case KEY_DOWN:
                        if(switch_cursor != num_poke_options - 1) {
                          switch_cursor++;
                        }
                        break;
                    }
                  }
                  // Clear menu
                  wclear(menuwin);
                }
              }
              else {
                world.pc.current_pokemon->set_current_hp(world.pc.current_pokemon->get_current_hp() - damage);
              }
            }
            else {
              mvwprintw(menuwin, 9, 20, "%s's move missed!                      ", p->get_species());
              wrefresh(menuwin);
              wgetch(menuwin);
            }
          }
        }
        // Pokemon switch sub-menu
        if(!(strcmp(options[cursor], "Pokemon"))) {
          // Pokemon switch options
          class pokemon *pokemon_options[6];
          pokemon_options[0] = world.pc.current_pokemon;
          int switch_cursor = 0;
          int swtich_highlight;
          // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
          // 0 - 3, 1
          int in_switch_menu = 1;
          // Load potential switch candidates onto temporary array
          int num_poke_options = 1;
          for(x = 0; x < 6; x++) {
            if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
              pokemon_options[num_poke_options] = world.pc.buddy[x];
              num_poke_options++;
            }
          }
          wrefresh(menuwin);
          // Menu options for switching pokemon
          while (in_switch_menu) {
            swtich_highlight = 0;
            // Cursor logic 
            for(x = 0; x < num_poke_options; x++) {
              if(x == switch_cursor) {
                wattron(menuwin, A_REVERSE);
              }
              if(x == 0) {
                mvwprintw(menuwin, swtich_highlight, 10, "Back");
              }
              else {
                mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
              }
              wattroff(menuwin, A_REVERSE);
              swtich_highlight++;
            }
            playerInput = wgetch(menuwin);
            switch (playerInput) {
              // Enter key
              case 10:
                if(switch_cursor != 0) {
                  world.pc.current_pokemon = pokemon_options[switch_cursor];
                }
                else {
                  cancel_move = true;
                }
                in_switch_menu = 0; 
                break;
              // Up and down logic
              case KEY_UP:
                if(switch_cursor != 0) {
                  switch_cursor--;
                }
                break;
              case KEY_DOWN:
                if(switch_cursor != num_poke_options - 1) {
                  switch_cursor++;
                }
                break;
            }
          }

          // BATTLE SEQUENCE
          // Opponents battle after using item / switching pokemon
          // Feedback for move use
          // Opponent chooses random move
          if(!cancel_move) {
            for(x = 0; x < 3; x++) {
              if(strcmp(p->get_move(x), "")) {
                num_opponent_moves++;
              }
            }
            opponent_move_index = p->get_move_index(rand() % num_opponent_moves);

            mvwprintw(menuwin, 9, 20, "%s used %s!                ", p->get_species(), moves[opponent_move_index].identifier);
            wrefresh(menuwin);
            wgetch(menuwin);
            // Damage formula
            if(rand() % 100 < moves[opponent_move_index].accuracy) {
              damage = ((((2 * p->get_level()) / 5) + 2) * moves[opponent_move_index].power * (p->get_atk() / world.pc.current_pokemon->get_def()));
              damage = (int) damage / 50;
              damage += 2;
              if(rand() % 256 < p->get_speed()) {
                damage = damage * 1.5;
                mvwprintw(menuwin, 9, 20, "A critical hit!                    ");
                wrefresh(menuwin);
                wgetch(menuwin);
              }
              // damage = (int) ((rand() % 100 - 85 + 1) + 85) / 100 * damage;
              if(damage >= world.pc.current_pokemon->get_current_hp()) {
                mvwprintw(menuwin, 9, 20, "Your Pokemon fainted!                    ", p->get_species());
                wrefresh(menuwin);
                wgetch(menuwin);
                world.pc.current_pokemon->set_current_hp(0);
                bool pokemon_left = false;
                for(x = 0; x < 6; x++) {
                  if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0) {
                    pokemon_left = true;
                    break;
                  }
                }
                // Exit the game if all pokemon are down
                if(!pokemon_left) {
                  mvwprintw(menuwin, 9, 20, "You blacked out, exiting game                      ", p->get_species());
                  wrefresh(menuwin);
                  wgetch(menuwin);
                  leaveBattle = 1;
                  world.quit = 1;
                }
                // Swap pokemon before next turn
                else {
                  ignore_player_move = true;
                  // Pokemon switch options
                  class pokemon *pokemon_options[6];
                  pokemon_options[0] = world.pc.current_pokemon;
                  int switch_cursor = 0;
                  int swtich_highlight;
                  // mvwprintw(menuwin, optionsHighlight, 1, "%s", options[x]);
                  // 0 - 3, 1
                  int in_switch_menu = 1;
                  // Load potential switch candidates onto temporary array
                  int num_poke_options = 1;
                  for(x = 0; x < 6; x++) {
                    if(world.pc.buddy[x] != NULL && world.pc.buddy[x]->get_current_hp() != 0 && world.pc.buddy[x] != world.pc.current_pokemon) {
                      pokemon_options[num_poke_options] = world.pc.buddy[x];
                      num_poke_options++;
                    }
                  }
                  
                  // Menu options for switching pokemon
                  while (in_switch_menu) {
                    swtich_highlight = 0;
                    // Cursor logic 
                    for(x = 0; x < num_poke_options; x++) {
                      if(x == switch_cursor) {
                        wattron(menuwin, A_REVERSE);
                      }
                      if(x == 0) {
                        mvwprintw(menuwin, swtich_highlight, 10, "Back");
                      }
                      else {
                        mvwprintw(menuwin, swtich_highlight, 10, "%s", pokemon_options[x]->get_species());
                      }
                      wattroff(menuwin, A_REVERSE);
                      swtich_highlight++;
                    }
                    wrefresh(menuwin);
                    playerInput = wgetch(menuwin);
                    switch (playerInput) {
                      // Enter key
                      case 10:
                        if(switch_cursor != 0) {
                          world.pc.current_pokemon = pokemon_options[switch_cursor];
                          in_switch_menu = 0; 
                        }
                        break;
                      // Up and down logic
                      case KEY_UP:
                        if(switch_cursor != 0) {
                          switch_cursor--;
                        }
                        break;
                      case KEY_DOWN:
                        if(switch_cursor != num_poke_options - 1) {
                          switch_cursor++;
                        }
                        break;
                    }
                  }
                }
              }
              else {
                world.pc.current_pokemon->set_current_hp(world.pc.current_pokemon->get_current_hp() - damage);
              }
            }
            else {
              mvwprintw(menuwin, 9, 20, "%s's move missed!                      ", p->get_species());
              wrefresh(menuwin);
              wgetch(menuwin);
            }
          }
        }
        // Do nothing on a run
        if(!(strcmp(options[cursor], "Run"))) {
          leaveBattle = 1;
          delete p;
        }
        break;
      default:
        break;
    }
  }
  wclear(menuwin);
  wrefresh(menuwin);
  clear();
  refresh();

  // // Later on, don't delete if captured
}

void io_choose_starter()
{
  class pokemon *choice[3];
  int i;
  bool again = true;
  
  choice[0] = new class pokemon();
  choice[1] = new class pokemon();
  choice[2] = new class pokemon();

  echo();
  curs_set(1);
  do {
    mvprintw( 4, 20, "Before you are three Pokemon, each of");
    mvprintw( 5, 20, "which wants absolutely nothing more");
    mvprintw( 6, 20, "than to be your best buddy forever.");
    mvprintw( 8, 20, "Unfortunately for them, you may only");
    mvprintw( 9, 20, "pick one.  Choose wisely.");
    mvprintw(11, 20, "   1) %s", choice[0]->get_species());
    mvprintw(12, 20, "   2) %s", choice[1]->get_species());
    mvprintw(13, 20, "   3) %s", choice[2]->get_species());
    mvprintw(15, 20, "Enter 1, 2, or 3: ");

    refresh();
    i = getch();

    if (i == '1' || i == '2' || i == '3') {
      world.pc.buddy[0] = choice[(i - '0') - 1];
      // Assign starter as current pokemon
      world.pc.current_pokemon = choice[(i - '0') - 1];
      delete choice[(i - '0') % 3];
      delete choice[((i - '0') + 1) % 3];
      again = false;
    }
  } while (again);
  noecho();
  curs_set(0);
}
