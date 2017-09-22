#include <ncurses.h>
#include <stdio.h>
#include <string.h>

int maxrow = 0, maxcol = 0;

WINDOW *create_newwin(int height, int width, int startx, int starty) {
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}

void destroy_mywin(WINDOW *local_win) {
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
    delwin(local_win);
}

void initEditor() {
    initscr();
    cbreak();
    // noecho();
    int row, col, startx, starty, height = 1, width = col, cursorx = 0, cursory = 0;
    char mesg[] = "SHIT";
    getmaxyx(stdscr, row, col);
    // mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
    // mvprintw(row - 2, 0, "this screen has %d rows and %d columns", row, col);
    WINDOW *first;
    startx = 0;
    starty = 0;
    char ch;
    noecho();
    first = create_newwin(height, width, startx, starty);
    keypad(first, TRUE);
    while((ch = getch()) != 'a') {
        switch(ch) {
            case KEY_UP:
                if(height > 1) {
                    destroy_mywin(first);
                    first = create_newwin(--height, width, startx, starty);
                }
                break;
            case KEY_DOWN:
                if(height < row) {
                    destroy_mywin(first);
                    first = create_newwin(++height, width, startx, starty);
                }
                break;
            case KEY_ENTER:
                mvprintw(cursory, cursorx, "\n");
                cursory++;
                cursorx = 0;
                break;
            default:
                mvprintw(cursory, cursorx, "%c", ch);
                cursorx++;
                break;
                // mvprintw()
        }
    }
    // refresh();
    getch();
    endwin();
}


int main(int argc, char const *argv[])
{
    initEditor();
    return 0;
}
