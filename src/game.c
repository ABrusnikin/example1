#include <stdio.h>
#include <ncurses.h>
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
// #define N 13 // x
// #define M 24 // y
#define WALL 88         // 'X'`
#define BOX 42          // '*' 
#define BOX_ON_POINT 79 // 'O; 
#define PLAYER 64       // '@'
#define POINT 46        // '.'
#define EMPTY 32        // ' '

#define WALL_COLOR 3
#define BOX_COLOR 1
#define PLAYER_COLOR 2
#define POINT_COLOR 4


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

void init_screen();
int start_screen();
void load_level_screen();
void print_text(int x, int y, char* text);

void yellow_1();
void green_2();
void return_white(int n);
void magenta_3();
void cyan_4();

void game_loop(struct status* Game);

int download_map(struct status* Game);
void clear_map(int n,int m, char** arr);
// void print_map(int n,int m, char** arr);
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

int main() {
    
    init_screen();

    struct status Game = {0,1,0,0,NULL,0,0,0,0,0,0};
    getmaxyx(stdscr, Game.max_x, Game.max_y);
    
                /*** start_screen(): ***/
    while (!Game.want_to_quit) {
        int key_pressed = start_screen(&Game);
        if (key_pressed == KEY_Q) Game.want_to_quit = 1;
        if (key_pressed == KEY_Y) {
                /*** level_screen(): ***/
            load_level_screen();
                /*** play_game(): ***/
            game_loop(&Game);
            Game.want_to_quit = 0;
        }
    }
                /*** finish_game(): ***/
    endwin();
    return 0;
}

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

int start_screen() {
    clear();
    int x,y;
    getmaxyx(stdscr, x, y);

    print_text(x/2-1, y/2,"Want to play?");

    yellow_1();
    print_text(x/2+1, y/2-5,"Y");

    return_white(1);
    printw(" - to play, ");

    yellow_1();
    printw("Q");

    return_white(1);
    printw(" - to exit");
    refresh();
    return getch();
}

void load_level_screen() {
    clear();
    int x,y;
    getmaxyx(stdscr, x, y);
    
    print_text(x/2-2, y/2,"Sokoban");
    
    yellow_1();
    print_text(x/2, y/2-3,"W A S D");
    
    return_white(1);
    printw(" to move");
    
    yellow_1();
    print_text(x/2+1, y/2-3,"R");
    
    return_white(1);
    printw(" - to restart");
    
    yellow_1();
    print_text(x/2+2, y/2-3,"Q");
    
    return_white(1);
    printw(" - to quit");
    print_text(x/2+4, y/2-3,"Press any key\n");
    refresh();
    getch();
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

void print_text(int x, int y, char* text) {
    move(x,y);
    printw("%s",text);
}

void game_loop(struct status* Game) {
    int err = download_map(Game);

    /*** a game ***/
    clear();
    display_map(Game);
    refresh();

    int key;
    while (!Game->want_to_quit) {
        key = getch();
        switch (key)
        {
        case KEY_Q:
            Game->want_to_quit = 1;
            break;
        case KEY_A:
            move_me(0,-1,Game);
            break;
        case KEY_S:
            move_me(1,0,Game);
            break;
        case KEY_D:
            move_me(0,1,Game);
            break;
        case KEY_W:
            move_me(-1,0,Game);
            break;
        case KEY_R:
            reload_map(Game);
            break;
        default:
            break;
        }
    }

    if (!err) free_map(Game->x_map,Game->map); 
}

int download_map(struct status* Game) {

    /* create a string-level to find */
    int level_number = Game->level;
    char maze_n[8] = "Maze: ";
    char add_maze[5];
    sprintf(add_maze,"%d",level_number);
    strcat(maze_n,add_maze);

    /* open map-file */
    FILE *maps = fopen("src/01.txt", "r");
    if (maps == NULL) {
        perror("Ошибка при открытии файла");
        return 1;
    }

    /* find current map-level */
    char line[50];
    while (fgets(line,sizeof(line),maps)!=NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line,maze_n)==0) break;
    }
    
    /* set y-size map */
    while (fgets(line,sizeof(line),maps)!=NULL) {
        if (strstr(line,"Size X:")!=NULL) break;
    }
    sscanf(line+8,"%d",&Game->y_map);
    // printf("y - %d\n",Game->y_map);

    /* set x-size map */
    while (fgets(line,sizeof(line),maps)!=NULL) {
        if (strstr(line,"Size Y:")!=NULL) break;
    }
    sscanf(line+8,"%d",&Game->x_map);
    // printf("x - %d\n",Game->x_map);

    /* find current map */
    while (fgets(line,sizeof(line),maps)!=NULL) {
        if (strlen(line)==1) break;
    }

    /* create a map-matrix */
    Game->map = calloc(Game->x_map, sizeof(char*));
    for (int i =0; i<Game->x_map; i++) {
        Game->map[i] = calloc(Game->y_map+1,sizeof(char));
    }

    /* fill the map-matrix */
    clear_map(Game->x_map,Game->y_map,Game->map);
    for (int i =0; i < Game-> x_map; i++) {
        for (int j=0; j < Game-> y_map; j++) {
            char temp_ch = fgetc(maps);
            if (temp_ch == '.') Game->points_to_win++;
            if (temp_ch == '\n') break;
            Game->map[i][j] = temp_ch;
            if (temp_ch == PLAYER) {
                Game->me_x = i;
                Game->me_y = j;
            }
        }
        // Game->map[i][Game->y_map] = '\0';
            fgetc(maps);
    }

    fclose(maps);
    return 0;
}

/*
int download_map(struct status* Game) {
    FILE *file = fopen("src/01.txt", "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return 1;
    }

    Game->x_map = 13;
    Game->y_map = 24;

    Game->map = calloc(Game->x_map, sizeof(char*));
    for (int i =0; i<Game->x_map; i++) {
        Game->map[i] = calloc(Game->y_map,sizeof(char));
    }
    clear_map(Game->x_map,Game->y_map,Game->map);

    char temp_ch;
    for (int i =0; i < Game->x_map; i++) {
        for (int j=0; j<Game->y_map; j++) {
            temp_ch = fgetc(file);
            if (temp_ch == '.') Game->points_to_win++;
            if (temp_ch == '\n' || temp_ch == EOF) break;
            Game->map[i][j] = temp_ch;
            if (temp_ch == PLAYER) {
                Game->me_x = i;
                Game->me_y = j;
            }
        }
        if (temp_ch == EOF) break;
    }

    fclose(file);
    return 0;
}
*/

void clear_map(int n,int m, char** arr) {
    for (int i =0; i<n; i++) {
        for (int j=0; j<m; j++) {
            arr[i][j] = EMPTY;
        }
    }
}

void free_map(int n, char** arr) {
    for (int i =0; i<n; i++) {
        free(arr[i]);
    }
    free(arr);
}

void display_map(struct status* Game) {
    for (int i =0; i<Game->x_map; i++) {
        move(i,0);
        for (int j=0; j<Game->y_map; j++) {
            fill_tile(i,j,Game);
        }
    }
    // printw("%d\n",Game->x_map);
    // printw("%d\n",Game->y_map);
}

void move_me(int dx, int dy, struct status* Game) {
    if (can_move_free(dx,dy,Game)) move_free(dx,dy,Game);
        else 
            if (can_push_box(dx,dy,Game)) push_box(dx,dy,Game);
}

bool can_move_free(int dx, int dy, struct status* Game) {
    bool can = false;
    if (next_square(dx,dy,Game) != WALL && next_square(dx,dy,Game) != BOX 
        && next_square(dx,dy,Game) != BOX_ON_POINT) can = true;
    return can;
}

void move_free(int dx, int dy, struct status* Game) {
    clear();
    make_prev_square_empty(Game);
    if (next_square(dx,dy,Game) == POINT) Game->me_on_point = true;
    set_square(dx,dy,PLAYER,Game);
    move_my_coords(dx,dy,Game);
    display_map(Game);
    refresh();
}

bool can_push_box(int dx, int dy, struct status* Game) {
    bool can = false;
    if ((next_square(dx,dy,Game) == BOX || next_square(dx,dy,Game) == BOX_ON_POINT) && 
        next_2_square(dx,dy,Game) != WALL &&
        next_2_square(dx,dy,Game) != BOX &&
        next_2_square(dx,dy,Game) != BOX_ON_POINT) can = true;
    return can;
}

void push_box(int dx, int dy, struct status* Game) { 
    clear();
    check_points(dx,dy,Game);
    make_prev_square_empty(Game);
    if (next_square(dx,dy,Game) == BOX_ON_POINT) Game->me_on_point = true;
    set_square(dx,dy,PLAYER,Game);
    if (next_2_square(dx,dy,Game)==POINT) {
        set_square(2*dx,2*dy,BOX_ON_POINT,Game);
    }
        else set_square(2*dx,2*dy,BOX,Game);
    move_my_coords(dx,dy,Game);
    display_map(Game);
    if (Game->points_to_win==0) {
        print_text(Game->x_map+1,0,"Level done");
        Game->level++;
        // Game->want_to_quit = 1;
        getch();
        reload_map(Game);
    }
    refresh();
}

void reload_map(struct status* Game) {
    clear();
    Game->points_to_win = 0;
    download_map(Game);
    display_map(Game);
    refresh();
}

void set_square(int dx, int dy,char c, struct status* Game) {
 Game->map[Game->me_x+dx][Game->me_y+dy] = c;
}

char next_square(int dx, int dy, struct status* Game) {
 return Game->map[Game->me_x+dx][Game->me_y+dy];
}

char next_2_square(int dx, int dy, struct status* Game) {
 return Game->map[Game->me_x+2*dx][Game->me_y+2*dy];
}

void move_my_coords(int dx, int dy, struct status* Game) {
    Game->me_x += dx;
    Game->me_y += dy;
}

void check_points(int dx, int dy, struct status* Game) {
    if (next_square(dx,dy, Game)==BOX && next_2_square(dx,dy, Game)==POINT) Game->points_to_win--;
    if (next_square(dx,dy, Game)==BOX_ON_POINT && next_2_square(dx,dy, Game)!=POINT) Game->points_to_win++;
}

void make_prev_square_empty(struct status* Game) {
    if (Game->me_on_point) {
        set_square(0,0,POINT,Game);
        Game->me_on_point = false;
    }
        else set_square(0,0,EMPTY,Game);
}

void fill_tile(int i, int j, struct status* Game) {
    char sym = Game->map[i][j];
    switch (sym)
    {
    case WALL:
        magenta_3();
        addch(sym);
        return_white(3);
        break;

    case BOX:
        yellow_1();
        addch(sym);
        return_white(1);
        break;

    case PLAYER:
        green_2();
        addch(sym);
        return_white(2);
        break;

    case POINT:
        cyan_4();
        addch(sym);
        return_white(4);
        break;

    case BOX_ON_POINT:
        yellow_1();
        addch(sym);
        return_white(1);
        break;

    default:
        addch(sym);
        break;
    }
}