#include "ebuffer.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *f = fopen(argv[1], "r");
    ebuffer eb;
    init_ebuffer(&eb, f);
    fclose(f);
    FILE *f1 = fopen("test_op", "w");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    wrefresh(stdscr);
    refresh();

    // move_gap_cursor_right(&eb);
    // move_gap_cursor_down(&eb);
    move_gap_cursor_down(&eb);
    // move_gap_cursor_down(&eb);
    // move_gap_cursor_down(&eb);
    // move_gap_cursor_down(&eb);
    // move_gap_cursor_down(&eb);
    // move_gap_cursor_down(&eb);
    // move_gap_cursor_down(&eb);
    // move_gap_cursor_down(&eb);
    cut_line(&eb);
    // printf("ROW NO :%d\n", );
    // add_char(&eb, 's');
    // delete_char(&eb);
    print_ebuffer(&eb);
    move(0, 0);
    refresh();
    save_ebuffer_to_file(&eb, f1);
    getch();
    endwin();

    fclose(f1);
    return 0;
}
