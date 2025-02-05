#define WALL_COLOR 3
#define BOX_COLOR 1
#define PLAYER_COLOR 2
#define POINT_COLOR 4

void init_screen();
void clear_screen();
void refresh_screen();
void getmax_yx_screen(int* x,int* y);

void yellow_1();
void green_2();
void return_white(int n);
void magenta_3();
void cyan_4();
void print_text(int x, int y, char* text);
void move_cursor_to(int a, int b);
int get_pressed_key();
void print_char(char c);
void close_screen();
void print(char* text);