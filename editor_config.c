#include <ncurses.h>
#include "editor_config.h"
#include "ebuffer.h"

void set_cursor(editor_config *ec, WINDOW **win, int x, int y) {
    ec -> cursor_x = x;
    ec -> cursor_y = y;
    // move(ec -> cursor_y, ec -> cursor_x);
    move(ec -> cursor_y, ec -> cursor_x);
    wrefresh(*(win));
}

void init_editor_config(editor_config *ec, WINDOW *win) {
    ec -> cursor_x = 0;
    ec -> cursor_y = 0;
    ec -> screen_row = 0;
    ec -> screen_col = 0;
    getmaxyx(stdscr, ec -> screen_row, ec -> screen_col);
    ec -> screen_row -= 3;
    print_option_bar(win, ec);
}

void print_option_bar(WINDOW *win, editor_config *ec) {

    mvaddstr(ec -> screen_row + 1, 0, "CTRL + B : SAVE         CTRL + F : FIND         CTRL + U : PASTE");
    mvaddstr(ec -> screen_row + 2, 0, "CTRL + X : CUT          CTRL + H : REPLACE      CTRL + W : QUIT");
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

void move_cursor_left(editor_config *ec) {
    (ec -> cursor_x)--;
    refresh();
}

void move_cursor_right(editor_config *ec) {
    (ec -> cursor_x)++;
    refresh();
}


//not workign
int move_cursor_up(editor_config *ec, ebuffer *eb, int x) {
    if(ec -> cursor_y == 0) {
        return -1;
    }
    ec -> cursor_y -= get_spanning_rows(eb);
    ec -> cursor_x = x;
    refresh();
}

int move_cursor_down(editor_config *ec, ebuffer *eb, int x) {
    int foo = 0;
    if(ec -> cursor_y < ec -> screen_row - 1)
        (ec -> cursor_y) += get_spanning_rows(eb);
    else{
        // (ec -> cursor_y)--;
        return 1;
    }
    if(x == -1)
        return 0;
    ec -> cursor_x = x;
    refresh();
}

void move_gap_cursor_down_enter(editor_config *ec) {
    (ec -> cursor_y)++;
    (ec -> cursor_x) = 0;
    refresh();
}

int get_cursor_x(editor_config *ec) {
    return ec -> cursor_x;
}

int get_cursor_y(editor_config *ec) {
    return ec -> cursor_y;
}

int get_max_size_y(editor_config *ec) {
    return ec -> screen_row;
}

void move_cursor_down_start(editor_config *ec) {
    ec -> cursor_x = 0;
    (ec -> cursor_y)++;
}

void move_cursor_up_start(editor_config *ec) {
    ec -> cursor_x = 0;
    (ec -> cursor_y)--;
}

void print_save_bar(editor_config *ec) {
    mvaddstr(ec -> screen_row + 1, 0, "Do you want to save");
    mvaddstr(ec -> screen_row + 2, 0, "Y or N : ");
    // move(ec -> cursor_y, ec -> cursor_x);
}

void move_x_to_start(editor_config *ec) {
    ec -> cursor_x = 0;
}
