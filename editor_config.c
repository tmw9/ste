#include "editor_config.h"
#include <ncurses.h>

void set_cursor(editor_config *ec, WINDOW *win, int x, int y) {
    ec -> cursor_x = 0;
    ec -> cursor_y = 0;
    move(ec -> cursor_y, ec -> cursor_x);
    wmove(win, ec -> cursor_y, ec -> cursor_x);
    refresh();
}
