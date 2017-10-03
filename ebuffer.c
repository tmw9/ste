#include "ebuffer.h"

void init_buffer(ebuffer *buff) {

    buff = (ebuffer *) (malloc(sizeof(ebuffer)));
    buff -> prev = NULL;
    buff -> next = NULL;

    (buff -> row_buffer).buffer_size = 4096;
    (buff -> row_buffer).buffer = (char *) (calloc(1, (buff -> row_buffer).buffer_size));
    if(!(buff -> row_buffer).buffer)
        return;
    (buff -> row_buffer).cursor_ptr = (buff -> row_buffer).buffer;
    (buff -> row_buffer).gap_end = (buff -> row_buffer).buffer + (buff -> row_buffer).buffer_size;
    (buff -> row_buffer).gap_capacity = 512;
    (buff -> row_buffer).gap_start = (buff -> row_buffer).buffer;
    (buff -> row_buffer).bufend = (buff -> row_buffer).gap_end;
    // (buff -> row_buffer).len = 0;
}

void add_char(ebuffer *buff, char ch) {
    if((buff -> row_buffer).cursor_ptr != (buff -> row_buffer).gap_start)
        move_gap_to_cursor_ptr(buff);

    if(is_full_gap(buff))
        expand_buffer(buff);

    *((buff -> row_buffer).gap_start++) = ch;
    *((buff -> row_buffer).cursor_ptr++);
}

void expand_buffer(ebuffer *buff) {

}
