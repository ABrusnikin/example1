#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define KEY_Q 113
#define KEY_Y 121
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_R 114

#define WALL 88         // 'X'`
#define BOX 42          // '*' 
#define BOX_ON_POINT 79 // 'O; 
#define PLAYER 64       // '@'
#define POINT 46        // '.'
#define EMPTY 32        // ' '

struct status
{
    bool want_to_quit;
    int level;
    int max_x;
    int max_y;
    char** map;
    int x_map;
    int y_map;
    int points_to_win;
    int me_x;
    int me_y;
    bool me_on_point;
};

int start_screen();
void load_level_screen();

void game_loop(struct status* Game);

int download_map(struct status* Game);
void clear_map(int n,int m, char** arr);
void free_map(int n,char** arr);
void fill_tile(int i, int j, struct status* Game);

void display_map(struct status* Game);
void move_me(int dx, int dy, struct status* Game);
bool can_move_free(int dx, int dy, struct status* Game);
void move_free(int dx, int dy, struct status* Game);
bool can_push_box(int dx, int dy, struct status* Game);
void push_box(int dx, int dy, struct status* Game);
void reload_map(struct status* Game);

void set_square(int dx, int dy, char c, struct status* Game);
char next_square(int dx, int dy, struct status* Game);
char next_2_square(int dx, int dy, struct status* Game);
void move_my_coords(int dx, int dy, struct status* Game);
void make_prev_square_empty(struct status* Game);

void check_points(int dx, int dy, struct status* Game);