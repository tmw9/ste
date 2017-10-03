#include "ebuffer.h"

void init_buffer(ebuffer *buff) {
    buff = (ebuffer *) (malloc(sizeof(ebuffer)));
    buff -> prev = NULL;
    buff -> next = NULL;
    (buff -> row_buffer).start = (char *) (calloc(1, 4096));
    (buff -> row_buffer).cursor_ptr = start;
    (buff -> row_buffer).end_ptr = start;
    (buff -> row_buffer).capacity = 512;
    (buff -> row_buffer).gap_end_ptr = start + 512;
    (buff -> row_buffer).len = 0;
}

void add_char_to_buffer(ebuffer *buff, char ch) {
    if(capacity == 0) {
        expand_buffer(buff);
    }
    *cursor_ptr = ch;
    if(cursor_ptr == end_ptr)
        ++end_ptr;
    ++cursor_ptr;
    --capacity;
    ++len;
}

void expand_buffer(ebuffer *buff) {

}
