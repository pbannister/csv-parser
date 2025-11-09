#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>

#undef ASSERT
#undef VERIFY

#include "base/trace.h"

#ifdef NDEBUG

#define ASSERT(X)
#define VERIFY(X)	((void)(X))
#define TRACE_FN(FN)
#define TRACE(X)

#else

#define ASSERT(X)	    assert(X)
#define VERIFY(X)	    assert(X)
#define TRACE_FN(FN)    base::trace_o _fn_(#FN)
#define TRACE(X)        base::trace_o::out(X)

#endif
