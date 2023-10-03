#include <ncurses.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i;
    char a[4] = "\\|/-";
    initscr();
    for(i = 0; i < 320; i++) {
        mvaddch(0, i, a[i % 4]);

    }
}