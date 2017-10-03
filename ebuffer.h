typedef struct gap_buffer {
    char *cursor_ptr;                    // location pointer into buffer
    char *buffer;                   // start of text buffer
    char *bufend;                   // first location outside buffer
    char *gap_start;                 // start of gap
    char *gap_end;
    int buffer_size;
    int gap_capacity;
} gap_buffer;

typedef struct list_of_buffers {
    gap_buffer row_buffer;
    gap_buffer *prev, *next;
} ebuffer;

void init_buffer(ebuffer *buff);

void add_char(ebuffer *buff, char ch) ;

void expand_buffer(ebuffer *buff);
