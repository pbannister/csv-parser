#include "parser/scanner.h"
#include "parser/row.h"

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

using namespace csv_parser;

void scanner_o::workers_close() {
    for (int i = 0; i < n_kids; ++i) {
        close(p_out[i]);
    }
    while (0 < n_kids--) {
        int status = 0;
        int pid = wait(&status);
        fprintf(stderr, "Worker #%d ended with status: %d\n", pid, status);
    }
}

scanner_o::~scanner_o() {
    workers_close();
}

bool scanner_o::open_to(int n, const char* sInterpreter, const char* sScript) {
    p_out = new int[n];
    for (int i = 0; i < n; ++i) {
        int f[2];
        VERIFY(0 == pipe(f));
        int pid = fork();
        if (0 == pid) {
            dup2(f[0], 0);
            close(f[1]);
            execl(sInterpreter, sInterpreter, sScript, (char*) 0);
            on_error(sInterpreter, errno);
            exit(2);
        } else {
            close(f[0]);
            p_out[i] = f[1];
            ++n_kids;
        }
    }
    return true;
}

void scanner_o::on_file(const char* s_filename) {
    fprintf(stderr, "Scanning: %s\n", s_filename);
    n_bytes_file = 0;
}

void scanner_o::on_row(row_o& row) {
    ++n_rows;
    n_bytes_file += row.row_length_get();
    ssize_t n_wrote_want = 0;
    ssize_t n_wrote_done = 0;
#if 0
    // For testing - dump fields read.
    char* pField = row.field_first_get();
    if (pField) {
        fputs(pField,stdout);
        while (pField = row.field_next_get()) {
            fputc(',',stdout);
            fputs(pField,stdout);
        }
        fputc('\n',stdout);
    }
#endif
    if (0 == n_kids) {
        return;  // for testing read performance
    }
    int f = p_out[n_rows % n_kids];
#if 0
    // Example of writing individual fields.
    for (char* pField = row.field_first_get(); pField; pField = row.field_next_get()) {
        write(f,pField,strlen(pField));
        write(f,"\037",1);  // Use ASCII US (unit separator) between fields.
    }
#endif
#if 1
    // Example of writing whole row buffer in one operation.
    char* p_row = row.row_buffer_get();
    int n_bytes_row = row.row_length_get();
    n_wrote_done += write(f, p_row, n_bytes_row);
    n_wrote_want += n_bytes_row;
#endif
    n_wrote_done += write(f, "\036", 1);  // Use ASCII RS (record separator) between rows.
    n_wrote_want += 1;
}

void scanner_o::on_EOF() {
    n_bytes_total += n_bytes_file;
}

void scanner_o::on_error(const char* s_filename, int iError) {
    const char* sError = strerror(iError);
    fprintf(stderr, "ERROR(%d) %s: %s\n", iError, s_filename, sError);
}
