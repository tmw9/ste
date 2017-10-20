#include "ebuffer.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *f = fopen(argv[1], "r");
    FILE *f1 = fopen("test_op", "w");
    gap_buffer gb;
    copy_file_to_buffer(&gb, f);
    print_buffer(&gb);
    save_buffer_to_file(&gb, f1);
    fclose(f);
    fclose(f1);
    return 0;
}
