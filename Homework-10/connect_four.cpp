#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ncurses.h>

typedef enum {
    PLAYER_ONE_TOKEN,
    PLAYER_TWO_TOKEN,
    NO_ONE_TOKEN 
} type_token;

int grid[6][7];
void display_board() {
    int i, j;
    attron(COLOR_PAIR(COLOR_CYAN));
    
    for(i = 5; i < 35; i += 5) {
        for(j = 15; j < 50; j += 5) {
            // Border
            
            
            // Top
            mvprintw(i, j, "/---\\");
            // Sides
            mvprintw(i + 1, j, "|");
            mvprintw(i + 2, j, "|");
            mvprintw(i + 3, j, "|");
            if(grid[(i / 5) - 1][(j / 5) - 3] != NO_ONE_TOKEN) {
                attroff(COLOR_PAIR(COLOR_CYAN));
                switch(grid[(i / 5) - 1][(j / 5) - 3]) {
                    case PLAYER_ONE_TOKEN:
                        attron(COLOR_PAIR(COLOR_RED));
                        mvprintw(i + 1, j + 1, "###");
                        mvprintw(i + 2, j + 1, "###");
                        mvprintw(i + 3, j + 1, "###");
                        attroff(COLOR_PAIR(COLOR_RED));
                        break;
                    case PLAYER_TWO_TOKEN:
                        attron(COLOR_PAIR(COLOR_YELLOW));
                        mvprintw(i + 1, j + 1, "###");
                        mvprintw(i + 2, j + 1, "###");
                        mvprintw(i + 3, j + 1, "###");
                        attroff(COLOR_PAIR(COLOR_YELLOW));
                        break;
                }
                attron(COLOR_PAIR(COLOR_CYAN));
            }
            mvprintw(i + 1, j + 4, "|");
            mvprintw(i + 2, j + 4, "|");
            mvprintw(i + 3, j + 4, "|");
            // Bottom
            mvprintw(i + 4, j, "\\---/");
        }
    }
    // Extremety boarder
    for(i = 3; i < 36; i++) {
        mvprintw(i, 14, "|");
        mvprintw(i, 50, "|");
    }
    mvprintw(4, 13, "-+-----------------------------------+-");
    mvprintw(35, 13, "---------------------------------------");
    attroff(COLOR_PAIR(COLOR_CYAN));
    refresh();
}

void play_game(const char *game_type) {
    
}

int main(int arg, char *argv[]) {
    initscr();
    raw();
    noecho();
    curs_set(0);
    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);

    int x;
    int player_input;
    int menu_cursor = 0;
    int in_main_menu = 1;
    
    mvprintw(2, 10, " CCCCC                                       tt           44");    
    mvprintw(3, 10, "CC    C  oooo  nn nnn  nn nnn    eee    cccc tt          444");
    mvprintw(4, 10, "CC      oo  oo nnn  nn nnn  nn ee   e cc     tttt      44  4 ");
    mvprintw(5, 10, "CC    C oo  oo nn   nn nn   nn eeeee  cc     tt       44444444");
    mvprintw(6, 10, " CCCCC   oooo  nn   nn nn   nn  eeeee  ccccc  tttt       444  ");

    mvprintw(8, 10, "Options: ");     
    mvprintw(9, 10, "Left and Right Arrow keys to move, enter to select"); 

    const char *main_menu_options[2] = {"Play VS Computer", "Play VS Other Player"};
    // Menu options for switching pokemon
    while (in_main_menu) {
        // // Cursor logic 
        
        for(x = 0; x < 2; x++) {
            if(x == menu_cursor) {
                attron(A_REVERSE);
            }
            mvprintw(10, 10 + (x * 20), main_menu_options[x]);
            attroff(A_REVERSE);
        }
        refresh();
        player_input = getch();
        // std::ofstream myfile;
        // myfile.open("dary.txt");
        // myfile << player_input << "\n";
        // myfile.close();
        switch (player_input) {
            // Enter key
            case 10:
                in_main_menu = 0; 
                break;
            // left key
            case 68: {
                if(menu_cursor != 0) {
                    menu_cursor--;
                }
                break;
            }
            // Right Key
            case 67: {
                if(menu_cursor != 2) {
                    menu_cursor++;
                }
                break; 
            }
        }
    }

    int i, j;
    for(i = 0; i < 6; i++) {
        for(j = 0; j < 7; j++) {
            grid[i][j] = PLAYER_TWO_TOKEN;
        }
    }
    clear();
    display_board();
    getch();
    play_game(main_menu_options[menu_cursor]);
    endwin();
    return 0;
}   
