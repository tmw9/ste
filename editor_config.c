#include <ncurses.h>
#include "editor_config.h"

void set_cursor(editor_config *ec, WINDOW **win, int x, int y) {
    ec -> cursor_x = 0;
    ec -> cursor_y = 0;
    ec -> cursor_row = 0;
    // move(ec -> cursor_y, ec -> cursor_x);
    move(ec -> cursor_y, ec -> cursor_x);
    wrefresh(*(win));
}

void init_editor_config(editor_config *ec, WINDOW *win) {
    ec -> cursor_x = 0;
    ec -> cursor_y = 0;
    ec -> screen_row = 0;
    ec -> screen_col = 0;
    ec -> cursor_row = 0;
    getmaxyx(stdscr, ec -> screen_row, ec -> screen_col);
    ec -> screen_row -= 3;
    print_option_bar(win, ec);
}

void print_option_bar(WINDOW *win, editor_config *ec) {
    mvaddstr(ec -> screen_row + 1, 0, "CTRL + B : SAVE          CTRL + F : FIND          CTRL + L : COPY           CTRL + U : PASTE");
    mvaddstr(ec -> screen_row + 2, 0, "CTRL + X : CUT           CTRL + H : REPLACE       CTRL + W : QUIT");
    move(ec -> cursor_y, ec -> cursor_x);
    wrefresh(win);
    refresh();
}

void want_to_save(WINDOW *win, editor_config *ec) {
    mvaddstr(ec -> screen_row + 1, 0, "Want to save the file : ");
    mvaddstr(ec -> screen_row + 2, 0, "Y or N : ");
}

int get_screen_row(editor_config *ec) {
    return ec -> screen_row;
}

int get_screen_col(editor_config *ec) {
    return ec -> screen_col;
}

int get_cursor_row(editor_config *ec) {
    return ec -> cursor_row;
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

void move_cursor_down_start(editor_config *ec) {
    ec -> cursor_x = 0;
    (ec -> cursor_y)++;
}

void move_cursor_up_start(editor_config *ec) {
    ec -> cursor_x = 0;
    (ec -> cursor_y)--;
}
