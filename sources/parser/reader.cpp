#include "parser/reader.h"
#include "parser/scanner.h"
#include "parser/row.h"
#include "parser/character_map.h"

#include "common.h"

#include <fcntl.h>
#include <unistd.h>

using namespace csv_parser;

static character_map_o g_map;

reader_o::reader_o(const char* fn) :
    file(-1), p_buffer(0), n_min(0), n_max(0), n_size(0), s_filename(fn), i_error_last(0) {
    buffer_allocate(1 * 1024 * 1024);
}

reader_o::reader_o() :
    file(0), p_buffer(0), n_min(0), n_max(0), n_size(0), s_filename("STDIN"), i_error_last(0) {
    buffer_allocate(1 * 1024 * 1024);
}

reader_o::~reader_o() {
    if (0 < file) {
        file_close();
    }
}

void reader_o::buffer_allocate(int cb) {
    p_buffer = new char[cb + 1];
    n_max = cb;
    n_min = n_max >> 4; // Default maximum row size is 1/16th of the buffer size.
    n_size = 0;
}

bool reader_o::file_open() {
    TRACE_FN(reader_o::file_open);
    file = open(s_filename,O_LARGEFILE);
    if (file < 0) {
        i_error_last = errno;
        perror(s_filename);
        return false;
    }
    return true;
}

// Since the normalized row will always be the same size or smaller,
// we can re-write the buffer in-place.
char* reader_o::row_scan(char* p_base,int /*n_left*/) {
    char* p_map = g_map.map_get();
    char* p_in1 = p_base;
    char* p_in2 = p_base;
    char* p_out = p_base;
    char c_quote = '"';
    bool in_quote = false;
    for (;;) {
        char c = *p_in2++;
        char v = p_map[255 & c];
        if (TO_SELF == v) {
            *p_out++ = c;
        } else if (TO_DELIMITER == v) {
            if (in_quote) {
                *p_out++ = c;
            } else {
                p_in1 = p_in2;
                *p_out++ = 0;
            }
        } else if (TO_QUOTE == v) {
            if ((p_in1 + 1) == p_in2) {
                in_quote = true;
                c_quote = c;
            } else if (!in_quote) {
                *p_out++ = c;
            } else if (c_quote == *p_in2) {
                *p_out++ = c;
                ++p_in2;
            } else {
                in_quote = false;
                *p_out++ = 0;
            }
        } else if (TO_EOL == v) {
            // LF ends a row, unless inside a quoted field.
            if (in_quote) {
                *p_out++ = c;
            } else {
                p_in1 = p_in2;
                *p_out++ = 0;
                break;
            }
        } else if (TO_SKIP == v) {
            // Drop character.
        } else if (TO_SPACE == v) {
            // Convert all other control characters to a single space.
            *p_out++ = ' ';
        }
    }
    row_access.row_buffer_set(p_base,(int) (p_out - p_base));
    return p_in2;
}

bool reader_o::file_scan(scanner_o& scanner) {
    TRACE_FN(reader_o::file_scan);
    for (;;) {
        int n_wanted = n_max - n_size;
        n_wanted &= ~(0xFFFF); // read in 64KB quanta
        int n_read = read(file,p_buffer + n_size,n_wanted);
        if (n_read <= 0) {
            break;
        }
        n_size += n_read;
        p_buffer[n_size] = '\n'; // sentinel
        char* p_base = p_buffer;
        char* p_EOB = p_buffer + n_size;
        for (;;) {
            int n_left = n_size - (int) (p_base - p_buffer);
            if (n_left < n_min) {
                memmove(p_buffer,p_base,n_left);
                n_size = n_left;
                break;
            }
            p_base = row_scan(p_base,n_left);
            scanner.on_row(row_access);
            if (p_EOB <= p_base) {
                n_size = 0;
                break;
            }
        }
    }
    char* p_base = p_buffer;
    char* p_EOB = p_buffer + n_size;
    while (p_base < p_EOB) {
        int n_left = n_size - (int) (p_base - p_buffer);
        p_base = row_scan(p_base,n_left);
        scanner.on_row(row_access);
    }
    scanner.on_EOF();
    return true;
}

void reader_o::file_close() {
    TRACE_FN(reader_o::file_close);
    if (0 < file) {
        close(file);
        file = -1;
    }
}

void reader_o::files_scan(scanner_o& scanner,int i_base,int ac,char** av) {
    TRACE_FN(reader_o::files_scan);
    for (int i = i_base; i < ac; ++i) {
        const char* s_filename = av[i];
        reader_o reader(s_filename);
        scanner.on_file(s_filename);
        if (!reader.file_open()) {
            scanner.on_error(s_filename,reader.i_error_last);
            return;
        }
        bool bOK = reader.file_scan(scanner);
        if (!bOK) {
            fprintf(stderr,"FAIL  '%s'\n",s_filename);
        }
        reader.file_close();
        fprintf(stderr,"Done with: %s\n",s_filename);
        fflush(0);
    }
}
