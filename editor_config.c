#include <ncurses.h>
#include "editor_config.h"

void set_cursor(editor_config *ec, WINDOW *win, int x, int y) {
    ec -> cursor_x = 0;
    ec -> cursor_y = 0;
    move(ec -> cursor_y, ec -> cursor_x);
    wmove(win, ec -> cursor_y, ec -> cursor_x);
    wrefresh(win);
}

void init_editor_config(editor_config *ec) {
    ec -> cursor_x = 0;
    ec -> cursor_y = 0;
    ec -> screen_row = 0;
    ec -> screen_col = 0;
    ec -> no_rows = 1;
    getmaxyx(stdscr, ec -> screen_row, ec -> screen_col);

}

void print_at_cursor(editor_config *ec, char ch) {
    mvprintw(ec -> cursor_y, ec -> cursor_x, "%c", ch);
    if(ch == 10){
        ec -> cursor_x = 0;
        (ec -> cursor_y)++;
    }
    else
        (ec -> cursor_x)++;
}

int get_screen_row(editor_config *ec) {
    return ec -> screen_row;
}

int get_screen_col(editor_config *ec) {
    return ec -> screen_col;
}

void move_cursor_left(editor_config *ec) {
    (ec -> cursor_x)--;
    refresh();
}

void move_cursor_right(editor_config *ec) {
    (ec -> cursor_x)++;
    refresh();
}

void move_cursor_up(editor_config *ec) {
    (ec -> cursor_y)--;
    refresh();
}

void move_cursor_down(editor_config *ec) {
    (ec -> cursor_y)++;
    refresh();
}

int get_cursor_x(editor_config *ec) {
    return ec -> cursor_x;
}

int get_cursor_y(editor_config *ec) {
    return ec -> cursor_y;
}

void enter_new_line(editor_config *ec) {
    ec -> cursor_x = 0;
    (ec -> cursor_y)++;
}
