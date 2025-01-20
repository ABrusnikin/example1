#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

#define KEY_Q 113
#define KEY_Y 121
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_R 114

#define N 13 // x
#define M 24 // y

#define WALL 88         // 'X'
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

void init_screen();
int start_screen(struct status* Game);
void load_level_screen(struct status* Game);
void print_text(int x, int y, char* text);
void game_loop(struct status* Game);

int download_map(struct status* Game);
void clear_map(int n,int m, char** arr);
// void print_map(int n,int m, char** arr);
void free_map(int n,char** arr);

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
            load_level_screen(&Game);
                /*** play_game(): ***/
            game_loop(&Game);
            Game.want_to_quit = 0;
            // getch();
        }
    }
                /*** finish_game(): ***/
    endwin();
    // printf("%d\n",' ');
    return 0;
}

void init_screen() {
    // system("resize -s 25 80");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
}

int start_screen(struct status* Game) {
    clear();
    getmaxyx(stdscr, Game->max_x, Game->max_y);
    print_text(Game->max_x/2-1, Game->max_y/2,"Want to play");
    print_text(Game->max_x/2+1, Game->max_y/2-5,"y - to play, q - to exit");
    refresh();
    return getch();
}

void load_level_screen(struct status* Game) {
    clear();
    getmaxyx(stdscr, Game->max_x, Game->max_y);
    print_text(Game->max_x/2-2, Game->max_y/2-3,"Loading level 01");
    print_text(Game->max_x/2, Game->max_y/2-3,"W,A,S,D to move");
    print_text(Game->max_x/2+1, Game->max_y/2-3,"r - to restart");
    print_text(Game->max_x/2+2, Game->max_y/2-3,"q - to quit");
    print_text(Game->max_x/2+4, Game->max_y/2-3,"Press any key\n");
    refresh();
    getch();
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

    if (!err) free_map(N,Game->map); 
}

int download_map(struct status* Game) {
    FILE *file = fopen("src/01.txt", "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return 1;
    }   
    Game->map = calloc(N, sizeof(char*));
    for (int i =0; i<N; i++) {
        Game->map[i] = calloc(M,sizeof(char));
    }
    clear_map(N,M,Game->map);

    char temp_ch;
    for (int i =0; i < N; i++) {
        for (int j=0; j<M; j++) {
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

void clear_map(int n,int m, char** arr) {
    for (int i =0; i<n; i++) {
        for (int j=0; j<m; j++) {
            arr[i][j] = EMPTY;
        }
    }
}

// void print_map(int n,int m, char** arr) {
//     for (int i =0; i<n; i++) {
//         for (int j=0; j<m; j++) {
//             printf("%c",arr[i][j]);
//         }
//         printf("\n");
//     }
// }

void free_map(int n, char** arr) {
    for (int i =0; i<n; i++) {
        free(arr[i]);
    }
    free(arr);
}

void display_map(struct status* Game) {
    for (int i =0; i<N; i++) {
        move(i,0);
        for (int j=0; j<M; j++) {
            addch(Game->map[i][j]);
        }
    }
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
        print_text(N,0,"Level done");
        Game->want_to_quit = 1;
        getch();
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