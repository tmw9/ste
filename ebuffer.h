typedef struct gap_buffer {
    char *start;
    char *cursor_ptr;
    char *gap_end_ptr;
    char *end_ptr;
    int capacity;
} gap_buffer;

typedef struct list_of_buffers {
    gap_buffer row_buffer;
    gap_buffer *prev, *next;
} ebuffer;

void init_buffer(ebuffer *buff) {
    buff = (ebuffer *) (malloc(sizeof(ebuffer)));
    buff -> prev = NULL;
    buff -> next = NULL;
    (buff -> row_buffer).start = (char *) (calloc(1, 4096));
    (buff -> row_buffer).cursor_ptr = start;
    (buff -> row_buffer).end_ptr = start + 4095;
    (buff -> row_buffer).capacity = 512;
    (buff -> row_buffer).gap_end_ptr = start + 512;
}

void add_char_to_buffer(ebuffer *buff, char ch) {
    if(capacity == 0) {
        expand_buffer(buff);
    }
    *cursor_ptr = ch;
    ++cursor_ptr;
    --capacity;
}

void add_new_line(ebuffer *buff)
