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
    wrefresh(local_win);
    delwin(local_win);
}



void init_editor(editor_config *ec, WINDOW **win) {
    char ch;
    int startx, starty;
    initscr();
    cbreak();
    // WINDOW *win;
    startx = starty = 0;;
    noecho();
    *win = create_newwin(get_screen_row(ec) - 2, get_screen_col(ec), startx, starty);
    keypad(*win, TRUE);
    wrefresh(*win);
    refresh();
}


int main(int argc, char const *argv[])
{
    char inp = 0, next_char = 0, local_x, local_y;
    WINDOW *win;
    gap_buffer gb;
    editor_config ec;
    FILE *fp;
    int temp = 0, valid = 0;
    if(argc != 2) {
        printf("Usage : ./main <filename>\n");
        return -1;
    }
    fp = fopen(argv[1], "r+");
    init_editor(&ec, &win);
    init_editor_config(&ec, win);
    copy_file_to_buffer(&gb, fp);
    print_buffer(&gb);
    // fclose(fp); //close the file currently, will open when writing/
    set_cursor(&ec, &win, 0, 0);
    move(0, 0);
    wrefresh(win);
    while((inp = getch()) != '\032') {
        if(inp == 27) {
            char type = getch(), key = getch();
            if(key == 66) {
                getyx(win, local_y, local_x);
                move_gap_cursor_down(&gb, local_x);
                move_cursor_down(&ec);
            }
            if(key == 65) {
                getyx(win, local_y, local_x);
                if(local_y == 0)
                    continue;
                move_gap_cursor_up(&gb, local_x);
                move_cursor_up(&ec);
            }
            if(key == 67) {
                valid = move_gap_cursor_right(&gb);
                if(valid == 1)
                    move_cursor_right(&ec);
            }
            if(key == 68) {
                valid = move_gap_cursor_left(&gb);
                if(valid == 1)
                    move_cursor_left(&ec);
            }
        } else if(inp == 127) {
            delete_char(&gb);
            move_cursor_left(&ec);
        } else if(inp == 10) {
            add_char(&gb, inp);
            move_cursor_down_start(&ec);
        } else if(inp == 23) {
            // fp2 = fopen("test_op", "w");
            char ans = 0;
            clear();
            print_buffer(&gb);
            want_to_save(win, &ec);
            // printw("HERE I AM\n");
            ans = getch();
            while(1) {
                if(ans == 'y' || ans == 'Y' || ans == 'n' || ans == 'N')
                    break;
                ans = getch();
            }
            printw("%c", ans);
            if(ans == 'y' || ans == 'Y')
                save_buffer_to_file(&gb, fp);
            getch();
            delwin(win);
            endwin();
            fclose(fp);
            return 1;
        }

        else {
            add_char(&gb, inp);
            move_gap_cursor_right(&gb);
            move_cursor_right(&ec);
        }
        clear();
        print_buffer(&gb);
        print_option_bar(win, &ec);
        move(get_cursor_y(&ec), get_cursor_x(&ec));
        wrefresh(win);
        refresh();

    }
    // delwin(win);
    // endwin();
    return 0;
}
