#include "base/string.h"

#ifndef ASSERT
#include <assert.h>
#define ASSERT(X) assert(X)
#endif

using namespace base;

//
//  Out-of-line string methods and free list maintenance.
//

enum {
    STRING_BUFFER_SIZE  = 254,
    BOB_SENTINEL        = 0x66,
    EOB_SENTINEL        = 0x99
};

static void* g_pFreeList = 0;

#ifdef NDEBUG
#define CheckBuffer(p,n)
#else
inline static void buffer_check(const char* p_buffer,int cbBuffer = STRING_BUFFER_SIZE) 
{
	ASSERT(p_buffer);
    ASSERT(BOB_SENTINEL == (255&*(p_buffer-1)));
    ASSERT(EOB_SENTINEL == (255&*(p_buffer+cbBuffer)));
}
#endif

void string_o::recycle()
{
	//fprintf(stderr,"recycle 0x%08lx size %d\n",(long)sBuffer,cbBufferMax);
	buffer_check(s_buffer,n_buffer_max);
	char* p = s_buffer - 1;
    s_buffer = 0;
    n_buffer_max = 0;
    if (STRING_BUFFER_SIZE == n_buffer_max) {
    	// fprintf(stderr,"recycle 0x%08lx\n",(long)p);
        *((void**)p) = g_pFreeList;
        g_pFreeList = p;
    } else {
        delete p;
    }
}

void string_o::upsize_to(int n)
{
	if (n < STRING_BUFFER_SIZE) {
	    n_buffer_max = STRING_BUFFER_SIZE;
	    // Grab a buffer from the free list if present (the usual case).
	    if (g_pFreeList) {
	        s_buffer = (char*) g_pFreeList;
	        g_pFreeList = *((void**)g_pFreeList);
	    } else {
	        // Allocate a stock-sized buffer.
	        s_buffer = new char[2+STRING_BUFFER_SIZE];
	        *s_buffer++                      = (char) BOB_SENTINEL;
	        *(s_buffer+STRING_BUFFER_SIZE)   = (char) EOB_SENTINEL;
	    }
	    *s_buffer = 0;
	} else {
        // round up to a quanta allowing for NUL
		++n;
        n = ((n + STRING_BUFFER_SIZE) / STRING_BUFFER_SIZE) * STRING_BUFFER_SIZE;
        char* p = new char[n+2];
        p[0]    = (char) BOB_SENTINEL;
        p[1+n]  = (char) EOB_SENTINEL;
        ++p;
        ::strcpy(p,s_buffer);
        recycle();
        s_buffer = p;
        n_buffer_max = n;
	}
	//fprintf(stderr,"upsizeTo returns 0x%08lx size %d\n",(long)sBuffer,cbBufferMax);
}
