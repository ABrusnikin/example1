#include <stdio.h>
#include <ncurses.h>
// #include <string.h>

struct status
{
    bool want_to_quit;
    int level;
};

void level_screen(int max_x, int max_y);
void print(int x, int y, char* text);

int main() {
                /*** start_screen(): ***/
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);
    
    struct status game = {0,1};
    
    int key_pressed;
                /*** start_screen(): ***/
    while (!game.want_to_quit) {
        clear();
        print(max_row/2-1, max_col/2,"Want to play");
        print(max_row/2+1, max_col/2-5,"y - to play, q - to exit");
        refresh();
        key_pressed = getch();
        if (key_pressed == 113) game.want_to_quit = 1;
        if (key_pressed == 121) {
                /*** level_screen(): ***/
            level_screen(max_row, max_col);
                /*** play_game(): ***/

        }

    }
      
                /*** finish_game(): ***/
    endwin();
    return 0;
}

void level_screen(int max_x, int max_y) {
    clear();
    print(max_x/2-2, max_y/2,"Loading...");
    print(max_x/2, max_y/2-3,"Arrows to move");
    print(max_x/2+1, max_y/2-3,"r - to restart");
    print(max_x/2+2, max_y/2-3,"q - to quit");
    print(max_x/2+4, max_y/2-3,"Press any key");
    refresh();
    getch();
}

void print(int x, int y, char* mes_text) {
    move(x,y);
    printw("%s",mes_text);
}