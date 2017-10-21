#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ebuffer.h"

void copy_file_to_buffer(gap_buffer *gb, FILE *file) {
    struct stat buf;

    fstat(fileno(file), &buf);
    long file_size = buf.st_size;
    init_buffer(gb, file_size + 128);
    move_gap_to_point(gb);
    expand_gap(gb, (int)file_size );
    unsigned int amount = fread(gb -> gap_start, 1, file_size, file);
    gb -> gap_start += amount;
    gb -> cursor_ptr = gb -> buffer;
}

void init_buffer(gap_buffer *gb, unsigned long size) {

    gb -> buffer = (char *) malloc(sizeof(char) * size);

    if (!(gb ->buffer)) {
        return;
    }

    gb -> cursor_ptr = gb -> buffer;
    gb -> gap_start = gb -> buffer;

    // initially gapend is outside of buffer
    gb -> gap_end = gb -> buffer + size;
    gb -> buffer_end = gb -> gap_end;
}

void move_gap_to_point(gap_buffer *gb) {
    if (gb -> cursor_ptr == gb -> gap_start) {
        return;
    }

    if (gb -> cursor_ptr == gb -> gap_end) {
        gb -> cursor_ptr = gb -> gap_start;
        return;
    }
    if (gb -> cursor_ptr < gb -> gap_start) {
        move_chars_to_gap(gb, gb -> cursor_ptr + (gb -> gap_end-gb -> gap_start), gb -> cursor_ptr, gb -> gap_start - gb -> cursor_ptr);
        gb -> gap_end -= (gb -> gap_start - gb -> cursor_ptr);
        gb -> gap_start = gb -> cursor_ptr;
    } else {
        move_chars_to_gap(gb, gb -> gap_start, gb -> gap_end, gb -> cursor_ptr - gb -> gap_end);
        gb -> gap_start += gb -> cursor_ptr - gb -> gap_end;
        gb -> gap_end = gb -> cursor_ptr;
        gb -> cursor_ptr = gb -> gap_start;
    }
}

void expand_gap(gap_buffer *gb, unsigned long size) {
    if (size > size_of_gap(gb)) {
        size += 128;
        expand_buffer(gb, size);
        move_chars_to_gap(gb, gb -> gap_end + size, gb -> gap_end, gb -> buffer_end - gb -> gap_end);

        gb -> gap_end += size;
        gb -> buffer_end += size;
    }
}

void expand_buffer(gap_buffer *gb, unsigned long size) {
    if (((gb -> buffer_end - gb -> buffer) + size) > size_of_buffer(gb)) {
        char *origbuffer = gb -> buffer;
        int new_buffer_size = (gb -> buffer_end - gb -> buffer) + size  + 128;
        gb -> buffer = (char *) realloc(gb -> buffer, new_buffer_size);
        gb -> cursor_ptr += gb -> buffer - origbuffer;
        gb -> buffer_end += gb -> buffer - origbuffer;
        gb -> gap_start += gb -> buffer - origbuffer;
        gb -> gap_end += gb -> buffer - origbuffer;
    }
}

unsigned long size_of_gap(gap_buffer *gb) {
    return (unsigned long)(gb -> gap_end - gb -> gap_start);
}

unsigned long size_of_buffer(gap_buffer *gb) {
     return (unsigned long)((gb -> buffer_end - gb -> buffer) - (gb -> gap_end - gb -> gap_start));
}

void move_chars_to_gap(gap_buffer *gb, char *destination, char *source, unsigned int length)
{
    if (destination == source || length == 0 )
        return;
    if (source > destination) {
        if (source + length >= gb -> buffer_end)
            return;
        while(length--)
            *(destination++) = *(source++);
    } else {
        source += length;
        destination += length;
        while(length--)
            *(--destination) = *(--source);
    }
}

void print_buffer(gap_buffer *gb) {
    char *temp = gb -> buffer;
    while (temp < gb -> buffer_end) {
        if (temp < gb -> gap_start || temp >= gb -> gap_end)
            printw("%c",*(temp));
        ++temp;
    }
    printw("\n");
}

void add_char(gap_buffer *gb, char ch) {
    if (gb -> cursor_ptr != gb -> gap_start)
        move_gap_to_point(gb);
    if (gb -> gap_start == gb -> gap_end)
        expand_gap(gb, 1);
    *(gb -> gap_start) = ch;
    (gb -> gap_start)++;
    (gb -> cursor_ptr)++;
}

void save_buffer_to_file(gap_buffer *gb, FILE *file) {
    char *temp = gb -> buffer;
    while(*temp) {
        if(temp == gb -> gap_start)
            temp = gb -> gap_end;
        fputc(*temp, file);
        temp++;
    }
}

void delete_char(gap_buffer *gb) {
    (gb -> cursor_ptr)--;
    (gb -> gap_start)--;
}

char get_next_char(gap_buffer *gb) {
    return *(gb -> cursor_ptr + 1);
}

char get_prev_char(gap_buffer *gb) {
    return *(gb -> cursor_ptr - 1);
}

int move_gap_cursor_right(gap_buffer *gb) {
    char *temp = gb -> cursor_ptr + 1;
    if(*temp == '\n' || *temp == '\0')
        return 2;
    if(temp >= gb -> gap_start && temp < gb -> gap_end) {
        temp = gb -> gap_end;
        gb -> cursor_ptr = gb -> gap_end;
        return 1;
    }
    gb -> cursor_ptr = temp;
    return 1;
}

int move_gap_cursor_left(gap_buffer *gb) {
    char *temp = gb -> cursor_ptr - 1;
    if(gb -> cursor_ptr == gb -> buffer || *temp == '\n')
        return 2;
    if(temp >= gb -> gap_start && temp < gb -> gap_end) {
        temp = gb -> gap_start - 1;
        gb -> cursor_ptr = gb -> gap_start - 1;
        return 1;
    }
    gb -> cursor_ptr = temp;
    return 1;
}

void move_gap_cursor_up(gap_buffer *gb, int x) {
    char *temp = gb -> cursor_ptr;
    while(*temp != '\n')
        --temp;
    --temp;
    while(temp != gb -> buffer || *temp != '\n')
        --temp;
    gb -> cursor_ptr = temp;
    while(x--) {
        gb -> cursor_ptr += 1;
        if(*(gb -> cursor_ptr + 1) == '\n')
            break;
    }
}

void move_gap_cursor_down(gap_buffer *gb, int x) {
    char *temp = gb -> cursor_ptr;
    while(temp != gb -> buffer_end || *temp != '\n') {
        ++temp;
    }
    if(gb -> buffer_end == temp)
        return;
    ++temp;
    while(x--) {
        gb -> cursor_ptr += 1;
        if(*(gb -> cursor_ptr + 1) == '\n' || gb -> cursor_ptr + 1 == gb -> buffer_end)
            break;
    }
}
