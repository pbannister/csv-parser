#pragma once
#include <string.h>

namespace base {

//
//  Simple string class.
//

class string_o {
protected:
    char* s_buffer;
    int n_buffer_max;

public:
    string_o() {
        upsize_to(1);
    }
    string_o(const char* s) {
        upsize_to(::strlen(s));
        ::strcpy(s_buffer, s);
    }
    ~string_o() {
        recycle();
    }

protected:
    void recycle();
    void upsize_to(int);
    void size_to(int n) {
        if (n_buffer_max <= n) {
            upsize_to(n);
        }
    }

public:
    operator const char*() {
        return s_buffer;
    }
    char* buffer_get() {
        return s_buffer;
    }
    char* buffer_get(int n) {
        size_to(n);
        return s_buffer;
    }
    int buffer_size_get() {
        return n_buffer_max;
    }

public:
    void operator=(const char* s) {
        strcpy(s);
    }

public:
    void strcpy(const char* s) {
        size_to(::strlen(s));
        ::strcpy(s_buffer, s);
    }
    void strcpy(const char* s, int n) {
        size_to(n);
        ::strncpy(s_buffer, s, n);
        s_buffer[n] = 0;
    }
    void strcat(const char* s) {
        int n1 = ::strlen(s_buffer);
        int n2 = ::strlen(s);
        size_to(n1 + n2);
        ::strcpy(s_buffer + n1, s);
    }
    void strcat(const char* s, int n) {
        int n1 = ::strlen(s_buffer);
        size_to(n1 + n);
        ::strncpy(s_buffer + n1, s, n);
        s_buffer[n1 + n] = 0;
    }
};

}  // namespace base