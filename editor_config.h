#ifndef EDITOR_CONFIG

#define EDITOR_CONFIG

typedef struct editor_config {
    int cursor_x;   //cursor's x co ordinate
    int cursor_y;   // cursor's y co oridnate
    int screen_row; // max number of rows that can be displayed on screen
    int screen_col; //max number of columns that can be displayed on screen
    int no_rows; //number of rows
} editor_config;

void set_cursor(editor_config *ec, WINDOW *win, int x, int y);
void print_at_cursor(editor_config *ec, char ch);
int get_screen_row(editor_config *ec);
int get_screen_col(editor_config *ec);
void init_editor_config(editor_config *ec);
int get_screen_row(editor_config *ec);
int get_screen_col(editor_config *ec);
void move_cursor_left(editor_config *ec);
void move_cursor_right(editor_config *ec);
void move_cursor_up(editor_config *ec);
void move_cursor_down(editor_config *ec);
void enter_new_line(editor_config *ec);
int get_cursor_x(editor_config *ec);
int get_cursor_y(editor_config *ec);

#endif
