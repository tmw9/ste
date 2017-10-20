#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "ebuffer.h"
#include "editor_config.h"


WINDOW *create_newwin(int height, int width, int startx, int starty) {
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    wrefresh(local_win);
    return local_win;
}

void destroy_mywin(WINDOW *local_win) {
    // wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
    delwin(local_win);
}



WINDOW *init_editor(editor_config *ec, WINDOW *win) {
    char ch;
    int startx, starty;
    initscr();
    cbreak();
    // mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
    // mvprintw(10 - 2, 0, "this screen has %d rows and %d columns", 10, 10);
    // WINDOW *win;
    startx = starty = 0;
    // char c;
    noecho();
    win = create_newwin(get_screen_row(ec) - 2, get_screen_col(ec), startx, starty);
    keypad(win, TRUE);
    wrefresh(win);
}


int main(int argc, char const *argv[])
{
    char inp = 0;
    WINDOW *win;
    gap_buffer gb;
    editor_config ec;
    FILE *fp;
    if(argc != 2) {
        printf("Usage : ./main <filename>\n");
        return -1;
    }
    fp = fopen(argv[1], "r");
    init_editor(&ec, win);
    copy_file_to_buffer(&gb, fp);
    print_buffer(&gb, &ec, win);
    fclose(fp); //close the file currently, will open when writing/
    set_cursor(&ec, win, 0, 0);
    wrefresh(win);
    while((inp = getch()) != '\032') {
        if(inp == 27) {
            char type = getch(), key = getch();
            if(key == 66) {
                move_cursor_down(&ec);
                wrefresh(win);
            }
            if(key == 65) {
                move_cursor_up(&ec);
            }
        }
        else{
            mvwprintw(win, get_cursor_y(&ec), get_cursor_x(&ec), "%c", inp);
            move_cursor_right(&ec);
        }
    }
    delwin(win);
    endwin();
    return 0;
}
