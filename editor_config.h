#ifndef EDITOR_CONFIG

#define EDITOR_CONFIG
#include "ebuffer.h"
#include <ncurses.h>

typedef struct editor_config {
    int cursor_x;   //cursor's x co ordinate
    int cursor_y;   // cursor's y co oridnate
    int screen_row; // max number of rows that can be displayed on screen
    int screen_col; //max number of columns that can be displayed on screen
} editor_config;

void set_cursor(editor_config *ec, WINDOW **win, int x, int y);
void print_option_bar(WINDOW *win, editor_config *ec);
int get_screen_row(editor_config *ec);
int get_screen_col(editor_config *ec);
void init_editor_config(editor_config *ec, WINDOW *win);
int get_screen_row(editor_config *ec);
int get_screen_col(editor_config *ec);
void move_cursor_left(editor_config *ec);
void move_cursor_right(editor_config *ec);
int move_cursor_up(editor_config *ec, ebuffer *eb, int x);
int move_cursor_down(editor_config *ec, ebuffer *eb, int x);
void move_gap_cursor_down_enter(editor_config *ec);
void move_cursor_down_start(editor_config *ec);
void move_cursor_up_start(editor_config *ec);
int get_cursor_x(editor_config *ec);
int get_cursor_y(editor_config *ec);
int get_cursor_row(editor_config *ec);
void want_to_save(WINDOW *win, editor_config *ec);
int get_max_size_y(editor_config *ec);
void print_save_bar(editor_config *ec);
void move_x_to_start(editor_config *ec);

#endif
