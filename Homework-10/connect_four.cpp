#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ncurses.h>

typedef enum {
    NO_ONE_TOKEN,
    PLAYER_ONE_TOKEN,
    PLAYER_TWO_TOKEN
} type_token;

int grid[6][7];
/* Display board here, then refresh*/
void display_board() {
    int i, j;
    attron(COLOR_PAIR(COLOR_CYAN));
    
    for(i = 5; i < 35; i += 5) {
        for(j = 15; j < 50; j += 5) {
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
            // Border
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
/* Detect a connect 4 for any player, if it doesn't exist, then return not found 
   It's a cool algorithm I found online here https://codereview.stackexchange.com/questions/127091/java-connect-four-four-in-a-row-detection-algorithms*/
int someone_win() {
    int x, y;
    for (y = 0; y < 6; y++) { // iterate rows, bottom to top
        for (x = 0; x < 7; x++) { // iterate columns, left to right
            int player = grid[y][x];
            if (player == NO_ONE_TOKEN)
                continue; // don't check empty slots

            if (x + 3 < 7 &&
                player == grid[y][x+1] && // look right
                player == grid[y][x+2] &&
                player == grid[y][x+3])
                return player;
            if (y + 3 < 6) {
                if (player == grid[y+1][x] && // look up
                    player == grid[y+2][x] &&
                    player == grid[y+3][x])
                    return player;
                if (x + 3 < 7 &&
                    player == grid[y+1][x+1] && // look up & right
                    player == grid[y+2][x+2] &&
                    player == grid[y+3][x+3])
                    return player;
                if (x - 3 >= 0 &&
                    player == grid[y+1][x-1] && // look up & left
                    player == grid[y+2][x-2] &&
                    player == grid[y+3][x-3])
                    return player;
            }
        }
    }
    return NO_ONE_TOKEN;
}

/** Detect a potential connect 4 for the computer, if no connect 4 exists, then return -1**/
int find_connect_four(int player) {
    // int grid_cpy[6][7];
    int x;
    // // Simulate moves on connect four grid
    // for(y = 0; y < 6; y++) {
    //     for(x = 0; x < 7; x++) {
    //         grid_cpy[y][x] = grid_cpy[y][x];
    //     }
    // }
    for(x = 0; x < 7; x++) {
        // Skip any full columns
        if(grid[0][x] != NO_ONE_TOKEN) {
            continue;
        }
        // Simulate move
        int bottom_of_board = 5;
        while(grid[bottom_of_board][x] != NO_ONE_TOKEN && bottom_of_board > 0) {
            bottom_of_board--;
        }
        grid[bottom_of_board][x] = player;
        // Win detected, return position to drop coin
        if(someone_win() == player) {
            // Undo move
            grid[bottom_of_board][x] = NO_ONE_TOKEN;
            return x;
        }
        // Undo move
        grid[bottom_of_board][x] = NO_ONE_TOKEN;
    }
    return -1;
}

/* Game is played here*/
void play_game(const char *game_type) {
    // int x;
    int bottom_of_board;
    int player_input;
    int menu_cursor = 0;
    int is_game_over = 1;
    int turn_number = 1;
    bool player_turn;
    int player_num = PLAYER_ONE_TOKEN;

    int temp;
    while (is_game_over) {
                // std::ofstream myfile;
        // myfile.open("dary.txt");
        // myfile << player_input << "\n";
        // myfile.close();
        player_turn = true;
        while (player_turn) {
            clear();
            // Display board
            // Place a token randomly

            if(!strcmp(game_type, "Play VS Computer") && player_num == PLAYER_TWO_TOKEN) {

                int com_num = find_connect_four(PLAYER_TWO_TOKEN);
                // No connect four found for computer, do random move
                if(com_num < 0) {
                    com_num = find_connect_four(PLAYER_ONE_TOKEN);
                    if(com_num < 0) {
                        do {
                            com_num = rand() % 7;
                        } while(grid[0][com_num] != NO_ONE_TOKEN);
                    }
                }
                // Place coin
                bottom_of_board = 5;
                while(grid[bottom_of_board][com_num] != NO_ONE_TOKEN && bottom_of_board > 0) {
                    bottom_of_board--;
                }
                grid[bottom_of_board][com_num] = player_num;
                mvprintw(1, 15, "Computer's Turn");
                mvprintw(2, 15, "Computer Placed Coin at Column %d, Press any key to continue", com_num);
                mvprintw(4, 55, "Use Left and Right arrow keys to move cursor");
                mvprintw(5, 55, "Enter to drop coin");
                mvprintw(1, 44, "Turn %d", turn_number);
                display_board();
                getch();
                player_turn = false;
            }
            else {
                mvprintw(1, 15, "Player %d's Turn", player_num);
                mvprintw(4, 55, "Use Left and Right arrow keys to move cursor");
                mvprintw(5, 55, "Enter to drop coin");
                mvprintw(1, 44, "Turn %d", turn_number);
                display_board();
                // Cursor logic 
                mvprintw(3, (5 * (menu_cursor)) + 17 , "V");
                refresh();
                player_input = getch();
                switch (player_input) {
                    // Enter key
                    case 10:
                        bottom_of_board = 5;
                        while(grid[bottom_of_board][menu_cursor] != NO_ONE_TOKEN && bottom_of_board > 0) {
                            bottom_of_board--;
                        }
                        grid[bottom_of_board][menu_cursor] = player_num;
                        // Move to first open cursor
                        if(grid[0][menu_cursor] != NO_ONE_TOKEN) {
                            menu_cursor = 0;
                            while(grid[0][menu_cursor] != NO_ONE_TOKEN) {
                                menu_cursor++;
                            }
                        }
                        player_turn = false;
                        // is_game_over = 0; 
                        break;
                    // left key
                    case KEY_LEFT: {
                        if(menu_cursor != 0) {
                            temp = menu_cursor;
                            menu_cursor--;
                            // Prevent Cursor from landing on full space
                            while(grid[0][menu_cursor] != NO_ONE_TOKEN && menu_cursor > 0) {
                                menu_cursor--;
                            }
                            if(menu_cursor == 0 && grid[0][menu_cursor] != NO_ONE_TOKEN) {
                                menu_cursor = temp;
                            }
                        }
                        break;
                    }
                    // Right Key
                    case KEY_RIGHT: {
                        if(menu_cursor != 6) {
                            temp = menu_cursor;
                            menu_cursor++;
                            // Prevent Cursor from landing on full space
                            while(grid[0][menu_cursor] != NO_ONE_TOKEN && menu_cursor < 6) {
                                menu_cursor++;
                            }
                            if(menu_cursor == 6 && grid[0][menu_cursor] != NO_ONE_TOKEN) {
                                menu_cursor = temp;
                            }
                        }
                        break; 
                    }
                }
            }
        }
        clear();
        // Display board
        mvprintw(1, 15, "Player %d's Turn", player_num);
        mvprintw(2, 15, "Press any key to continue");
        mvprintw(4, 55, "Use Left and Right arrow keys to move cursor");
        mvprintw(5, 55, "Enter to drop coin");
        mvprintw(1, 44, "Turn %d", turn_number);
        display_board();
        // Cursor logic 
        mvprintw(3, (5 * (menu_cursor)) + 17 , "V");
        refresh();
        // Check for win
        int player_win = someone_win();
        if(player_win != NO_ONE_TOKEN) {
            mvprintw(2, 15, "Player %d Wins!                  ", player_win);
            refresh();
            getch();
            is_game_over = 0;
        }
        // Swap player turn
        player_num = player_num == PLAYER_ONE_TOKEN ? PLAYER_TWO_TOKEN : PLAYER_ONE_TOKEN;
        turn_number++;
        if(turn_number == 42) {
            is_game_over = 0;
            refresh();
            getch();
        }
    }
    clear();
    display_board();
}

int main(int arg, char *argv[]) {
    initscr();
    raw();
    noecho();
    curs_set(0);
    start_color();
    keypad(stdscr, TRUE);
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

    const char *main_menu_options[2] = {"Play VS Computer", "Play VS Other Player"};
    // Menu options for switching pokemon
    while (in_main_menu) {
        clear();

        mvprintw(2, 10, " CCCCC                                       tt           44    ");    
        mvprintw(3, 10, "CC    C  oooo  nn nnn  nn nnn    eee    cccc tt          444    ");
        mvprintw(4, 10, "CC      oo  oo nnn  nn nnn  nn ee   e cc     tttt      44  4    ");
        mvprintw(5, 10, "CC    C oo  oo nn   nn nn   nn eeeee  cc     tt       44444444  ");
        mvprintw(6, 10, " CCCCC   oooo  nn   nn nn   nn  eeeee  ccccc  tttt       444    ");

        mvprintw(8, 10, "Options: ");     
        mvprintw(9, 10, "Left and Right Arrow keys to move, enter to select"); 

        mvprintw(13, 10, "BBBBB   yy   yy    WW      WW iii lll  sss                     CCCCC  hh              ");    
        mvprintw(14, 10, "BB   B  yy   yy    WW      WW     lll s      oooo  nn nnn     CC    C hh      uu   uu ");
        mvprintw(15, 10, "BBBBBB   yyyyyy    WW   W  WW iii lll  sss  oo  oo nnn  nn    CC      hhhhhh  uu   uu ");
        mvprintw(16, 10, "BB   BB      yy     WW WWW WW iii lll     s oo  oo nn   nn    CC    C hh   hh uu   uu ");
        mvprintw(17, 10, "BBBBBB   yyyyy      WW   WW   iii lll  sss   oooo  nn   nn     CCCCC  hh   hh  uuuu u ");
        // Cursor logic 
        
        for(x = 0; x < 2; x++) {
            if(x == menu_cursor) {
                mvprintw(10, 10 + (x * 20) - 3, "-->");
            }
            mvprintw(10, 10 + (x * 20), main_menu_options[x]);
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
            case KEY_LEFT: {
                if(menu_cursor != 0) {
                    menu_cursor--;
                }
                break;
            }
            // Right Key
            case KEY_RIGHT: {
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
            grid[i][j] = NO_ONE_TOKEN;
        }
    }
    clear();
    play_game(main_menu_options[menu_cursor]);
    endwin();
    return 0;
}   
