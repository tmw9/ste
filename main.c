#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "ebuffer.h"
#include "editor_config.h"

#define KEYUP 65
#define KEYDOWN 66
#define KEYRIGHT 67
#define KEYLEFT 68


void init_editor() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    wrefresh(stdscr);
    refresh();
}


int main(int argc, char const *argv[])
{
    char inp = 0, next_char = 0;
    int local_x, local_y;
    ebuffer eb;
    editor_config ec;
    FILE *fp;
    int temp = 0, valid = 0;
    if(argc != 2) {
        printf("Usage : ./main <filename>\n");
        return -1;
    }
    fp = fopen(argv[1], "r+");
    init_editor();
    init_editor_config(&ec);
    init_buffer(&eb, fp);
    print_ebuffer(&eb);
    move(0, 0);
    wrefresh(stdscr);
    while((inp = getch()) != '\032') {
        if(inp == 'a') {
            // char type = getch(), key = getch();
            // if(key == KEYRIGHT) {
                temp = move_buffer_cursor_right(&eb);
                if(temp != 0)
                    move_cursor_right(&ec);
            // }
        } else {
            move_cursor_right(&ec);
            add_char(&eb, inp);
            move_buffer_cursor_right(&eb);
        }
        clear();
        print_ebuffer(&eb);
        // print_option_bar(stdscr, &ec);
        move(get_cursor_y(&ec), get_cursor_x(&ec));
        wrefresh(stdscr);
        refresh();

    }
    // delwin(win);
    // endwin();
    return 0;
}
