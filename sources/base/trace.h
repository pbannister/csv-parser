#pragma once

namespace base {

class trace_o {
    const char* sName;
    static const char* sPad;
    static int nPad;
    static int n_max;
    static const char* pad();

public:
    trace_o(const char* s);
    ~trace_o();
    static void log(const char* s);
    static void log(const char* s, const char* v);
    static void log(const char* s, int v);
};

};  // namespace base
