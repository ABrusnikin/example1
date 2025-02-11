#include "gui.h"
#include <ncurses.h>


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

void getmax_yx_screen(int* x, int* y) {
getmaxyx(stdscr, *x, *y);
}

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

void move_cursor_to(int a, int b) {
    move(a,b);
}

void print_text(int x, int y, char* text) {
    move_cursor_to(x,y);
    printw("%s",text);
}

int get_pressed_key() {
    return getch();
}

void print_char(char c) {
    addch(c);
}

void close_screen() {
    endwin();
}

void print(char* text) {
    printw("%s", text);
}

void print_level(int x, int lvl) {
    print_text(x+1,0,"level ");
    printw("%d",lvl);
}