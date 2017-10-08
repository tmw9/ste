#include <ncurses.h>

typedef struct editor_config {
    int cursor_x;   //cursor's x co ordinate
    int cursor_y;   // cursor's y co oridnate
    int screen_row; // max number of rows that can be displayed on screen
    int screen_col; //max number of columns that can be displayed on screen
    int no_rows; //number of rows
} editor_config;

void set_cursor(editor_config *ec, WINDOW *win, int x, int y);
