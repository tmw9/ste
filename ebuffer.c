#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ncurses.h>
#include "ebuffer.h"

void init_ebuffer(ebuffer *eb, FILE *file) {
    eb -> row_start = eb -> row_end = eb -> current_row = NULL;
    eb -> current_line_no = 0;
    copy_files_to_ebuffer(eb, file);
    eb -> current_row = eb -> row_start;
    eb -> current_row -> gb -> cursor_ptr = eb -> current_row -> gb -> buffer;
    eb -> current_line_no = eb -> current_row -> row_no;
}

void copy_files_to_ebuffer(ebuffer *eb, FILE *file) {
    char  str[10240] = { 0 };
    while(fgets(str, 10240, file) != NULL) {
        str[strlen(str)] = '\0';
        if(eb -> row_start == NULL && eb -> row_end == NULL) {
            eb -> row_start = eb -> row_end = eb -> current_row = get_new_row();
            eb -> row_start -> row_no = 1;
        } else {
            row *new_row = get_new_row();
            new_row -> prev = new_row -> next = NULL;
            eb -> current_row = new_row;
            new_row -> prev = eb -> row_end;
            new_row -> row_no = eb -> row_end -> row_no + 1;
            eb -> row_end -> next = new_row;
            eb -> row_end = new_row;
        }
        eb -> current_row -> gb = (gap_buffer *) malloc(sizeof(gap_buffer));
        init_gap_buffer(eb -> current_row -> gb, strlen(str) + GAP_SIZE);
        copy_file_to_gap_buffer(eb -> current_row -> gb, str);
        eb -> current_row -> no_of_chars = strlen(str);
    }
}

row *get_new_row() {
    row *new_row = (row *)malloc(sizeof(row));
    new_row -> gb = (gap_buffer *)malloc(sizeof(gap_buffer));
    new_row -> row_no = 0;
    new_row -> prev = new_row -> next = NULL;
    new_row -> no_of_chars = 0;
    return new_row;
}

void init_gap_buffer(gap_buffer *gb, unsigned long size) {

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

void copy_file_to_gap_buffer(gap_buffer *gb, char *str) {
    str[strlen(str) - 1] = '\0';
    strcpy(gb -> buffer, str);
    gb -> gap_start = gb -> buffer + strlen(str);
    gb -> cursor_ptr = gb -> buffer;
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
        size += GAP_SIZE;
        expand_buffer(gb, size);
        move_chars_to_gap(gb, gb -> gap_end + size, gb -> gap_end, gb -> buffer_end - gb -> gap_end);
        gb -> gap_end += size;
        gb -> buffer_end += size;
    }
}

void expand_buffer(gap_buffer *gb, unsigned long size) {
    if (((gb -> buffer_end - gb -> buffer) + size) > size_of_buffer(gb)) {
        char *origbuffer = gb -> buffer;
        int new_buffer_size = (gb -> buffer_end - gb -> buffer) + size  + GAP_SIZE;
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

void add_char(ebuffer *eb, char ch) {
    add_char_to_gap_buffer(eb -> current_row -> gb, ch);
    eb -> current_row -> no_of_chars += 1;
}

void add_char_to_gap_buffer(gap_buffer *gb, char ch) {
    if (gb -> cursor_ptr != gb -> gap_start)
        move_gap_to_point(gb);
    if (gb -> gap_start == gb -> gap_end)
        expand_gap(gb, 1);
    *(gb -> gap_start) = ch;
    (gb -> gap_start)++;
    (gb -> cursor_ptr)++;
}

void print_ebuffer(ebuffer *eb) {
    int y, x;
    row *r = eb -> row_start;
    while(r != NULL) {
        getmaxyx(stdscr, y, x);
        r -> spanning_terminal_rows = r -> no_of_chars / x + 1;
        printf("ROW NO : %d  ", r -> row_no);
        printf("SPANNING ROWS : %d  \n", r -> spanning_terminal_rows);
        print_gap_buffer(r -> gb);
        r = r -> next;
    }
    // printf("CURRENT LINE NUMBER : %d\n", eb -> current_line_no);
}

void print_gap_buffer(gap_buffer *gb) {
    char *temp = gb -> buffer;
    while (temp < gb -> buffer_end) {
        // if(temp >= gb -> gap_start && temp < gb -> gap_end)
        //     printf("_");
        if (temp < gb -> gap_start || temp >= gb -> gap_end)
            printf("%c",*(temp));
        ++temp;
    }
    // printf("%s", gb -> buffer);
    printf("\n");
}

void save_ebuffer_to_file(ebuffer *eb, FILE *file) {
    row *r = eb -> row_start;
    while(r != NULL) {
        save_gap_buffer_to_file(r -> gb, file);
        r = r -> next;
    }
}

void save_gap_buffer_to_file(gap_buffer *gb, FILE *file) {
    char *temp = gb -> buffer;
    while(*temp) {
        if(temp == gb -> gap_start)
            temp = gb -> gap_end;
        fwrite(temp, 1, 1, file);
        temp++;
    }
    fputc('\n', file);
}

int move_gap_cursor_right(ebuffer *eb) {
    row *current_row = eb -> current_row;
    gap_buffer *gb = current_row -> gb;
    if(gb -> cursor_ptr >= gb -> gap_start && gb -> cursor_ptr < gb -> gap_end) {
        gb -> cursor_ptr = gb -> gap_end;
    }
    // if(*(gb -> cursor_ptr) == '\n' || *(gb -> cursor_ptr) == '\0') {
    if(gb -> cursor_ptr == gb -> buffer_end) {
        // printf("CUR : %c\n", *(gb -> cursor_ptr - 1));
        return 0;
    }
    (gb -> cursor_ptr)++;
    return 1;
}


int move_gap_cursor_left(ebuffer *eb) {
    row *current_row = eb -> current_row;
    gap_buffer *gb = current_row -> gb;
    char *temp = gb -> cursor_ptr;
    if(gb -> cursor_ptr == gb -> buffer) {
        return 0;
    }
    if(gb -> cursor_ptr >= gb -> gap_start && gb -> cursor_ptr < gb -> gap_end)
        gb -> cursor_ptr = gb -> gap_start - 1;
    else{
        (gb -> cursor_ptr)--;
    }
    return 1;
}

int move_gap_cursor_down(ebuffer *eb) {
    row *current_row = eb -> current_row;
    if(current_row -> next == NULL)
        return 0;
    int x = 0;
    char *temp = current_row -> gb -> cursor_ptr;
    while(temp != current_row -> gb -> buffer) {
        ++x;
        if(temp >= current_row -> gb -> gap_start && temp < current_row -> gb -> gap_end)
            temp = current_row -> gb -> gap_start - 1;
        else
            --temp;
    }
    current_row = current_row -> next;
    eb -> current_row = current_row;
    if(current_row -> no_of_chars < x) {
        current_row -> gb -> cursor_ptr = current_row -> gb -> buffer_end;
        return 2;
    }
    current_row -> gb -> cursor_ptr = current_row -> gb -> buffer + x;
    if(current_row -> gb -> cursor_ptr >= current_row -> gb -> gap_start && current_row -> gb -> cursor_ptr < current_row -> gb -> gap_end)
        current_row -> gb -> cursor_ptr += current_row -> gb -> cursor_ptr - current_row -> gb -> gap_start;
    return 1;
}

int move_gap_cursor_up(ebuffer *eb) {
    row *current_row = eb -> current_row;
    gap_buffer *gb = current_row -> gb;
    char *temp = gb -> cursor_ptr;
    int x = 0;

    //check if previous row exists
    if(current_row -> prev == NULL)
        return 0;

    while(temp > gb -> buffer) {
        ++x;
        if(temp >= gb -> gap_start && temp < gb -> gap_end)
            temp = gb -> gap_start - 1;
        else
            --temp;
    }
    current_row = current_row -> prev;
    eb -> current_row = current_row;
    gb = current_row -> gb;
    temp = gb -> buffer;
    if(current_row -> no_of_chars < x) {
        gb -> cursor_ptr = gb -> buffer_end;
        return 2;
    }
    gb -> cursor_ptr = gb -> buffer + x;
    if(gb -> cursor_ptr >= gb -> gap_start && gb -> cursor_ptr < gb -> gap_end)
        gb -> cursor_ptr += gb -> cursor_ptr - gb -> gap_start;
    return 1;
}


int delete_char(ebuffer *eb) {
    row *current_row = eb -> current_row;
    gap_buffer *gb = current_row -> gb;
    if(gb -> cursor_ptr == gb -> buffer)
        return 0;
    if(gb -> cursor_ptr >= gb -> gap_start && gb -> gap_end)
        gb -> cursor_ptr = gb -> gap_start - 1;
    (gb -> cursor_ptr)--;
    (gb -> gap_start)--;
    return 1;
}

int cut_line(ebuffer *eb) {
    row *current_row = eb -> current_row, *temp;
    eb -> copied_row = current_row;
    if(current_row -> prev == NULL  && current_row -> next == NULL){
        eb -> current_row = eb -> row_start = eb -> row_end = NULL;
        eb -> current_line_no = 0;
    } else if(current_row -> prev == NULL) {
        eb -> current_row = current_row -> next;
        eb -> row_start = eb -> current_row;
        current_row -> next -> prev = NULL;
        temp = eb -> current_row;
        while(temp != NULL) {
            temp -> row_no -= 1;
            temp = temp -> next;
        }
        eb -> current_line_no = eb -> current_row -> row_no;
    } else if(current_row -> next == NULL) {
        eb -> current_row = current_row -> prev;
        current_row -> prev -> next = NULL;
        eb -> current_line_no = eb -> current_row -> row_no;
        eb -> row_end = eb -> current_row;
    } else {
        eb -> current_row = current_row -> next;
        current_row -> next -> prev = current_row -> prev;
        current_row -> prev -> next = current_row -> next;
        temp = current_row -> next;
        while(temp != NULL) {
            temp -> row_no -= 1;
            temp = temp -> next;
        }
        eb -> current_line_no = eb -> current_row -> row_no;
    }
    return 1;
}

int copy_line(ebuffer *eb) {
    if(eb -> current_row == NULL)
        return 0;
    eb -> copied_row = eb -> current_row;
}

// void find_str(ebuffer *eb, char *str) {

// }

/*
char get_next_char(gap_buffer *gb) {
    return *(gb -> cursor_ptr + 1);
}

char get_prev_char(gap_buffer *gb) {
    if(gb -> cursor_ptr - 1 == gb -> buffer)
        return '\n';
    return *(gb -> cursor_ptr - 1);
}

*/
