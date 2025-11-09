#pragma once
#include <sys/time.h>

namespace base {

class timer_o {
public:
    struct timeval t1, t2;

public:
    timer_o() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    int elapsed() {
        return ((t2.tv_sec - t1.tv_sec) * 1000) + ((t2.tv_usec - t1.tv_usec) / 1000);
    }
    int split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};

}  // namespace base