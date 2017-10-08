#include <stdlib.h>
#include <stdio.h>
#include "ebuffer.h"

void init_buffer(gap_buffer *gb) {
    gb -> buffer_size = 2048;
    gb -> buffer = (char *) malloc(sizeof(char) * (gb -> buffer_size));
    if(!(gb -> buffer))
        return;
    gb -> cursor_ptr = gb -> buffer;
    gb -> gap_start = gb -> buffer;
    gb -> gap_end = gb -> buffer + gb -> buffer_size;
    gb -> buffer_end = gb -> gap_end;
    gb -> gap_capacity = gb -> buffer_size;
}

void add_char(gap_buffer *gb, char ch) {
    if(gb -> cursor_ptr != gb -> gap_start)
        move_gap_to_point(gb);

    if(gb -> gap_start == gb -> gap_end || gb -> gap_capacity == 0) {
        expand_gap(gb);
        gb -> gap_capacity = gb -> buffer_size;
    }

    *(gb -> gap_start) = ch;
    ++(gb -> gap_start);
    ++(gb -> cursor_ptr);
    --(gb -> gap_capacity);
}

void expand_gap(gap_buffer *gb) {
    expand_buffer(gb);
    copy_chars_to_gap(gb, gb -> gap_end + gb -> buffer_size, gb -> gap_end, gb -> buffer_end - gb -> gap_end);
    gb -> gap_end += gb -> buffer_size;
    gb -> buffer_end += gb -> buffer_size;
}

void expand_buffer(gap_buffer *gb) {

    char *original_buffer = gb -> buffer;
    int new_buffer_size = gb -> buffer_size * 2;
    gb -> buffer = (char *) realloc(gb -> buffer, new_buffer_size);

    gb -> cursor_ptr += gb -> buffer - original_buffer;
    gb -> buffer_end += gb -> buffer - original_buffer;
    gb -> gap_start += gb -> buffer - original_buffer;
    gb -> gap_end += gb -> buffer - original_buffer;
}

void copy_chars_to_gap(gap_buffer *gb, char *destination, char * source, long length) {

    if(destination == source || length == 0)
        return;

    if(source > destination) {
        for(; length > 0; length--){
            *(destination++) = *(source++);
        }
    }
    else {
        source += length;
        destination += length;

        for(; length > 0; length--){
            *(--destination) == *(--source);
        }
    }
}

void move_gap_to_point(gap_buffer *gb) {

    if(gb -> cursor_ptr == gb -> gap_start)
        return;
    else if(gb -> cursor_ptr == gb -> gap_end) {
        gb -> cursor_ptr = gb -> gap_start;
        return;
    }

    if(gb -> cursor_ptr < gb -> gap_start) {
         copy_chars_to_gap(gb, gb -> cursor_ptr + (gb -> gap_end - gb -> gap_start), gb -> cursor_ptr, gb ->gap_start - gb -> cursor_ptr);
         gb -> gap_end -= (gb -> gap_start - gb -> cursor_ptr);
         gb -> gap_start = gb -> cursor_ptr;
    }
    else {
        copy_chars_to_gap(gb, gb -> gap_start, gb -> gap_end, gb -> cursor_ptr - gb -> gap_end);
        gb -> gap_start += gb -> cursor_ptr - gb -> gap_end;
        gb -> gap_end = gb -> cursor_ptr;
        gb -> cursor_ptr = gb -> gap_start;
    }
}

long size_of_gap(gap_buffer *gb) {
    return (long)gb -> gap_end - (long)gb -> gap_start;
}

char get_char(gap_buffer *gb) {
    if(gb -> cursor_ptr == gb -> gap_start)
        gb -> cursor_ptr = gb -> gap_end;
    char to_ret = *(gb -> cursor_ptr);
    --(gb -> cursor_ptr);
    return to_ret;
}

void delete_char(gap_buffer *gb) {
    if(gb -> cursor_ptr != gb -> gap_start)
        move_gap_to_point(gb);

    gb -> gap_end += 1;
}

void print_buffer(gap_buffer *gb) {
    char *temp = gb -> buffer;


    while (temp < gb -> buffer_end) {

        if ( (temp >= gb -> gap_start) && (temp < gb -> gap_end) ) {
            printf("_");
            temp++;
        } else {
            printf("%c\n", *(temp++));
        }

    }
    printf("\n");
}
