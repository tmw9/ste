#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "editor_config.h"

editor_config ec;

WINDOW *create_newwin(int height, int width, int startx, int starty) {
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    // box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}

void destroy_mywin(WINDOW *local_win) {
    // wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
    delwin(local_win);
}

WINDOW *init_editor(const char *filename) {
    ec.cursor_x = 0;
    ec.cursor_y = 0;
    ec.screen_row = 0;
    ec.screen_col = 0;
    ec.no_rows = 1;
    initscr();
    cbreak();
    // noecho();
    getmaxyx(stdscr, ec.screen_row, ec.screen_col);
    // mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
    // mvprintw(row - 2, 0, "this screen has %d rows and %d columns", row, col);
    WINDOW *win;
    int startx, starty;
    startx = starty = 0;
    // char c;
    noecho();
    win = create_newwin(ec.screen_row - 2, ec.screen_col, startx, starty);
    keypad(win, TRUE);
    int fd = open(filename, O_RDONLY);
    char ch;
    while(read(fd, &ch, 1) != 0) {
        mvprintw(ec.cursor_y, ec.cursor_x, "%c", ch);
        // printw("%c", ch);
        if(ch == 10){
            ec.cursor_x = 0;
            ec.cursor_y++;
        }
        else
            ec.cursor_x++;
    }
    refresh();
    close(fd);
    return win;
}


int main(int argc, char const *argv[])
{
    char inp = 0;
    if(argc != 2) {
        printf("Usage : ./main <filename>\n");
        return -1;
    }
    WINDOW *win;
    win = init_editor(argv[1]);
    set_cursor(&ec, win, 0, 0);
    refresh();
    while((inp = getch()) != '\032') {
        if(inp == 27) {
            char type = getch(), key = getch();
            printw("%d\n", key);
        }
        mvprintw(ec.cursor_y, ec.cursor_x++, "%c", inp);
    }
    delwin(win);
    endwin();
    return 0;
}
