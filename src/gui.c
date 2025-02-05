#include "game.h"
// #include <ncurses.h>


void init_screen() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(BOX_COLOR,COLOR_YELLOW,COLOR_BLACK);
    init_pair(PLAYER_COLOR,COLOR_GREEN,COLOR_BLACK);
    init_pair(WALL_COLOR,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(POINT_COLOR,COLOR_CYAN,COLOR_BLACK);
}

void clear_screen() {
    clear();
}

void refresh_screen() {
    refresh();
}

// void getmax_yx_screen(int* x, int* y) {

void yellow_1() {
    attron(COLOR_PAIR(1));
}

void return_white(int n) {
    attroff(COLOR_PAIR(n));
}

void green_2() {
    attron(COLOR_PAIR(2));
}

void magenta_3() {
    attron(COLOR_PAIR(3));
}

void cyan_4() {
    attron(COLOR_PAIR(4));
}

void print_text(int x, int y, char* text) {
    move(x,y);
    printw("%s",text);
}