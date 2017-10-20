#ifndef EBUFFER

#define EBUFFER
#include <stdio.h>
#include <ncurses.h>
#include "editor_config.h"



typedef struct gap_buffer {
    char *cursor_ptr;                    // location pointer into buffer
    char *buffer;                   // start of text buffer
    char *buffer_end;                   // first location outside buffer
    char *gap_start;                 // start of gap
    char *gap_end;

} gap_buffer;

void copy_file_to_buffer(gap_buffer *gb, FILE *file);

void init_buffer(gap_buffer *gb, unsigned long size);

void add_char(gap_buffer *gb, char ch) ;

void expand_buffer(gap_buffer *gb, unsigned long size);

void expand_gap(gap_buffer *gb, unsigned long size);

void move_chars_to_gap(gap_buffer *gb, char *destination, char *source, unsigned int length);

void move_gap_to_point(gap_buffer *gb);

unsigned long size_of_gap(gap_buffer *gb);

// char get_char(gap_buffer *gb);

// void delete_char(gap_buffer *gb);

// void print_buffer(gap_buffer *gb, editor_config *ec, WINDOW *win);

void print_buffer(gap_buffer *gb);

void save_buffer_to_file(gap_buffer *gb, FILE *file);

unsigned long size_of_gap(gap_buffer *gb);

unsigned long size_of_buffer(gap_buffer *gb);

#endif
