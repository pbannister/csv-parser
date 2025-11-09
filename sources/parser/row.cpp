#include "parser/row.h"
#include <string.h>

using namespace csv_parser;

void row_o::row_buffer_set(char* p,int n) {
    p_row = p_next = p;
    n_row = n;
}

char* row_o::field_first_get() {
    p_next = p_row;
    return field_next_get();
}

char* row_o::field_next_get() {
    if ((p_row + n_row) <= p_next) {
        return 0;
    }
    char* p_field = p_next;
    p_next += ::strlen(p_field) + 1;
    return p_field;
}

int row_o::fields_count() {
    int n = 0;
    char* p_end = p_row + n_row;
    for (char* p = p_row; p < p_end; p += ::strlen(p) + 1) {
        ++n;
    }
    return n;
}
