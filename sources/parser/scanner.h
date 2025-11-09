#pragma once

namespace csv_parser {

class row_o;

class scanner_o {
public:
    int n_rows;
    long long n_bytes_file;
    double n_bytes_total;

protected:
    int n_kids;
    int* p_out;

public:
    bool open_to(int, const char*, const char* s = 0);
    void on_file(const char*);
    void on_row(row_o&);
    void on_EOF();
    void on_error(const char*, int);
    void workers_close();

public:
    scanner_o() : n_rows(0), n_bytes_file(0), n_bytes_total(0), n_kids(0), p_out(0) {
    }
    ~scanner_o();
};

}  // namespace csv_parser
