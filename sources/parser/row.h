#pragma once

namespace csv_parser {

class row_o {
protected:
    char* p_row;
    int n_row;
    char* p_next;

public:
    row_o() :
        p_row(0), n_row(0), p_next(0) {
    }

public:
    char* row_buffer_get() {
        return p_row;
    }
    int row_length_get() {
        return n_row;
    }

public:
    void row_buffer_set(char*, int);
    char* field_first_get();
    char* field_next_get();
    int fields_count();
};

}  // namespace csv_parser