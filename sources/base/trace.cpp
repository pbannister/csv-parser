#include "base/trace.h"
#include <stdio.h>

using namespace base;

int trace_o::nPad = 1;
int trace_o::n_max = 64;
const char* trace_o::sPad = "                                                                ";
//                          1234567890123456789012345678901234567890123456789012345678901234

const char* trace_o::pad() {
	int i = n_max - ((2*nPad) % n_max);
	return sPad + i;
}
trace_o::trace_o(const char* s) :
	sName(s) {
	fprintf(stderr, "%s> %s\n", pad(), sName);
	++nPad;
}
trace_o::~trace_o() {
	--nPad;
	fprintf(stderr, "%s< %s\n", pad(), sName);
}
void trace_o::log(const char* s) {
	fprintf(stderr, "%s%s\n", pad(), s);
}
void trace_o::log(const char* s,const char* v) {
	fprintf(stderr, "%s%s: %s\n", pad(), s, v);
}
void trace_o::log(const char* s,int v) {
	fprintf(stderr, "%s%s: %d\n", pad(), s, v);
}
