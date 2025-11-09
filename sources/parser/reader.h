#pragma once

#include "base/string.h"
#include "parser/row.h"

namespace csv_parser {

class scanner_o;

class reader_o {
    int file;
    char* p_buffer;
    int n_min;
    int n_max;
    int n_size;

public:
    row_o row_access;

public:
    base::string_o s_filename;
    int i_error_last;

public:
    void buffer_allocate(int);
    bool file_open();
    bool file_scan(scanner_o&);
    void file_close();

protected:
    char* row_scan(char*, int);

public:
    static void files_scan(scanner_o&, int, int, char**);

public:
    reader_o(const char*);
    reader_o();
    ~reader_o();
};

}  // namespace csv_parser
