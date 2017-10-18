#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ebuffer.h"

void copy_file_to_buffer(FILE *file, gap_buffer *gb) {

    // determine the size of the file then create
    // a buffer of size + GAP_SIZE
    struct stat buf;

    fstat(fileno(file), &buf);
    long file_size = buf.st_size;
    init_buffer(gb, file_size + 2048);
    move_gap_to_point(gb);
    expand_gap(gb, (int)file_size );
    unsigned int amount = fread(gb -> gap_start, 1, file_size, file);
    gb -> gap_start += amount;
};


void init_buffer(gap_buffer *gb, unsigned long size) {
    gb -> buffer_size = size;
    // printf("%lld\n", size);
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
        expand_gap(gb, 1);
        gb -> gap_capacity = gb -> buffer_size;
    }

    *(gb -> gap_start) = ch;
    ++(gb -> gap_start);
    ++(gb -> cursor_ptr);
    --(gb -> gap_capacity);
}

void expand_gap(gap_buffer *gb, unsigned long size) {
    expand_buffer(gb, size);
    copy_chars_to_gap(gb, gb -> gap_end + gb -> buffer_size, gb -> gap_end, gb -> buffer_end - gb -> gap_end);
    gb -> gap_end += size;
    gb -> buffer_end += size;
}

void expand_buffer(gap_buffer *gb, unsigned long size) {

    char *original_buffer = gb -> buffer;
    int new_buffer_size = gb -> buffer_size + 1;
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
            printf("%c", *(temp++));
        }

    }
    printf("\n");
}

void save_buffer_to_file(FILE *file, gap_buffer *gb) {
    char *temp = gb -> buffer;
    while(*temp) {
        if(temp == gb -> gap_start)
            temp = gb -> gap_end;
        fwrite(temp, 1, 1, file);
        temp++;
        if(temp == gb -> buffer_end)
            break;
    }
    free(gb -> buffer);
}
