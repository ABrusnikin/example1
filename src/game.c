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

#define N 12
#define M 23

struct status
{
    bool want_to_quit;
    int level;
    int max_x;
    int max_y;
    char** map;
};

void init_screen();
int start_screen(struct status* Game);
void load_level_screen(struct status* Game);
void print_text(int x, int y, char* text);
void game_loop(struct status* Game);

int download_map(struct status* Game);
void clear_map(int n,int m, char** arr);
void print_map(int n,int m, char** arr);
void free_map(int n,char** arr);

int main() {
    
    init_screen();

    struct status Game = {0,1,0,0,NULL};
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
            getch();

        }
    }
                /*** finish_game(): ***/
    endwin();
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
        
            if (temp_ch == '\n' || temp_ch == EOF) break;
            Game->map[i][j] = temp_ch;
        }
        if (temp_ch == EOF) break;
    }
    fclose(file);
    return 0;
}

void clear_map(int n,int m, char** arr) {
    for (int i =0; i<n; i++) {
        for (int j=0; j<m; j++) {
            arr[i][j] = ' ';
        }
    }
}

void print_map(int n,int m, char** arr) {
    for (int i =0; i<n; i++) {
        for (int j=0; j<m; j++) {
            printf("%c",arr[i][j]);
        }
        printf("\n");
    }
}

void free_map(int n, char** arr) {
    for (int i =0; i<n; i++) {
        free(arr[i]);
    }
    free(arr);
}