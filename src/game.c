#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

struct status
{
    bool want_to_quit;
    int level;
    int max_x;
    int max_y;
};

int start_screen(struct status* Game);
void level_screen(struct status* Game);
void print(int x, int y, char* text);

int main() {
    system("resize -s 25 80");
                /*** init screen ***/
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    
    struct status Game = {0,1,0,0};
    getmaxyx(stdscr, Game.max_x, Game.max_y);
    
                /*** start_screen(): ***/
    while (!Game.want_to_quit) {
        int key_pressed = start_screen(&Game);
        if (key_pressed == 113) Game.want_to_quit = 1;
        if (key_pressed == 121) {
                /*** level_screen(): ***/
            level_screen(&Game);
                /*** play_game(): ***/

        }

    }
                /*** finish_game(): ***/
    endwin();
    return 0;
}

int start_screen(struct status* Game) {
    clear();
    getmaxyx(stdscr, Game->max_x, Game->max_y);
    print(Game->max_x/2-1, Game->max_y/2,"Want to play");
    print(Game->max_x/2+1, Game->max_y/2-5,"y - to play, q - to exit");
    refresh();
    return getch();
}

void level_screen(struct status* Game) {
    clear();
    getmaxyx(stdscr, Game->max_x, Game->max_y);
    print(Game->max_x/2-2, Game->max_y/2-3,"Loading level 01");
    print(Game->max_x/2, Game->max_y/2-3,"Arrows to move");
    print(Game->max_x/2+1, Game->max_y/2-3,"r - to restart");
    print(Game->max_x/2+2, Game->max_y/2-3,"q - to quit");
    print(Game->max_x/2+4, Game->max_y/2-3,"Press any key");
    refresh();
    getch();
}

void print(int x, int y, char* message_text) {
    move(x,y);
    printw("%s",message_text);
}