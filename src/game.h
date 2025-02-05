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