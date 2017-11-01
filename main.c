#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "ebuffer.h"
#include "editor_config.h"

#define MY_KEY_ENTER 10
#define CTRLB 2
#define CTRLW 23
#define CTRLX 24
#define CTRLU 21

void init_editor() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    wrefresh(stdscr);
    refresh();
}


int main(int argc, char const *argv[])
{
    char next_char = 0, no_file = 0;
    int inp;
    char save = 0;
    int local_x, local_y, new_line = 0;
    ebuffer eb;
    editor_config ec;
    FILE *fp;
    int temp = 0, valid = 0;
    noecho();
    if(argc != 2) {
        printf("Usage : ./main <filename>\n");
        return -1;
    }
    fp = fopen(argv[1], "r+");
    if(fp == NULL) {
        no_file = 1;
    }
    init_editor();
    init_editor_config(&ec, stdscr);
    init_ebuffer(&eb, fp);
    print_ebuffer(&eb, get_max_size_y(&ec), 0);
    move(0, 0);
    wrefresh(stdscr);
    while((inp = getch()) != '\032') {
        if(inp == KEY_RIGHT) {
            temp = move_gap_cursor_right(&eb);
            if(temp)
                move_cursor_right(&ec);
        } else if(inp == KEY_LEFT) {
            temp = move_gap_cursor_left(&eb);
            if(temp)
                move_cursor_left(&ec);
        } else if(inp == KEY_DOWN) {
            temp = move_gap_cursor_down(&eb);
            if(temp != -1)
                new_line = move_cursor_down(&ec, &eb, temp);
        } else if(inp == KEY_UP) {
            temp = move_gap_cursor_up(&eb);
            if(temp != 0)
                new_line = move_cursor_up(&ec, &eb, temp);
        }

        else if(inp == KEY_BACKSPACE) {
            temp = delete_char(&eb);
            if(temp)
                move_cursor_left(&ec);
        } else if(inp == MY_KEY_ENTER) {
            // printw("ENTER");
            temp = move_gap_enter_key(&eb);
            // if(temp)
                move_gap_cursor_down_enter(&ec);
        } else if(inp == CTRLB) {
            rewind(fp);
            save_ebuffer_to_file(&eb, fp);
        } else if(inp == CTRLW) {
            clear();
            printw("Do you want to save : \n");
            save = getch();
            if(save == 'y' || save == 'Y') {
                rewind(fp);
                save_ebuffer_to_file(&eb, fp);
            }
            break;
        } else if(inp == CTRLX) {
            cut_line(&eb);
            move_x_to_start(&ec);
        } else if(inp == CTRLU) {
            paste_row(&eb);
        }
        else {
            move_cursor_right(&ec);
            add_char(&eb, inp);
        }
        clear();
        print_ebuffer(&eb, get_max_size_y(&ec), new_line);
        print_option_bar(stdscr, &ec);
        move(get_cursor_y(&ec), get_cursor_x(&ec));
        wrefresh(stdscr);
        refresh();
        new_line = 0;
    }
    endwin();
    fclose(fp);
    return 0;
}
