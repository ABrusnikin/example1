#include <stdio.h>
#include <ncurses.h>

struct status
{
    bool want_to_quit;
};


int main() {
        /*** start_screen(): ***/
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    int row, col;
    getmaxyx(stdscr, row, col);
    move(row/2, col/2);
    printw("Want to play?");

    int key_pressed;
    while (1) {
        key_pressed = getch();
        if (key_pressed == 113) break;
        if (key_pressed == 121) {
                /*** play_game(): ***/
            clear();
            move(row/2, col/2);
            printw("Loading...");
            refresh();
            getch();
        }

    }
      
        /*** finish_game(): ***/
    endwin();
    return 0;
}