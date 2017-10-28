#ifndef EBUFFER

#define EBUFFER
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include "editor_config.h"

#define GAP_SIZE 64

typedef struct gap_buffer {
    char *cursor_ptr;                    // location pointer into buffer
    char *buffer;                   // start of text buffer
    char *buffer_end;                   // first location outside buffer
    char *gap_start;                 // start of gap
    char *gap_end;
} gap_buffer;

typedef struct row {
    gap_buffer *gb;
    int row_no, no_of_chars, spanning_terminal_rows;
    struct row *prev, *next;
} row;

typedef struct ebuffer {
    row *row_start, *row_end, *current_row, *copied_row;
    int current_line_no;
} ebuffer;

void init_ebuffer(ebuffer *eb, FILE *file);
void copy_files_to_ebuffer(ebuffer *eb, FILE *file);
row *get_new_row();
void init_gap_buffer(gap_buffer *gb, unsigned long size);
void copy_file_to_gap_buffer(gap_buffer *gb, char *str);
void print_ebuffer(ebuffer *eb);
void print_gap_buffer(gap_buffer *gb);
void save_ebuffer_to_file(ebuffer *eb, FILE *file);
void save_gap_buffer_to_file(gap_buffer *gb, FILE *file);
unsigned long size_of_gap(gap_buffer *gb);
unsigned long size_of_buffer(gap_buffer *gb);
void expand_gap(gap_buffer *gb, unsigned long size);
void expand_buffer(gap_buffer *gb, unsigned long size);
void move_gap_to_point(gap_buffer *gb);
void move_chars_to_gap(gap_buffer *gb, char *destination, char *source, unsigned int length);
void add_char_to_gap_buffer(gap_buffer *gb, char ch);
void add_char(ebuffer *eb, char ch);
int delete_char(ebuffer *eb);
int move_gap_cursor_right(ebuffer *eb);
int move_gap_cursor_left(ebuffer *eb);
int move_gap_cursor_down(ebuffer *eb);
int move_gap_cursor_up(ebuffer *eb);
int cut_line(ebuffer *eb);
int copy_line(ebuffer *eb);
void find_str(ebuffer *eb, char *str);

#endif
