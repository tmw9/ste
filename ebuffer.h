typedef struct gap_buffer {
    char *cursor_ptr;                    // location pointer into buffer
    char *buffer;                   // start of text buffer
    char *buffer_end;                   // first location outside buffer
    char *gap_start;                 // start of gap
    char *gap_end;
    int buffer_size;
    int gap_capacity;
} gap_buffer;


void init_buffer(gap_buffer *gb);

void add_char(gap_buffer *gb, char ch) ;

void expand_buffer(gap_buffer *gb);

void expand_gap(gap_buffer *gb);

void copy_chars_to_gap(gap_buffer *gb, char *destination, char * source, long length);

void move_gap_to_point(gap_buffer *gb);

void move_gap_to_point(gap_buffer *gb);

long size_of_gap(gap_buffer *gb);

char get_char(gap_buffer *gb);

void delete_char(gap_buffer *gb);

void print_buffer(gap_buffer *gb);
