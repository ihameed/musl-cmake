#pragma once

#if defined(__cplusplus)
    extern "C" {
#endif

#define __NEED_size_t
#include <bits/alltypes.h>
#include <limits.h>

struct __locale_map; // XXXih

struct __locale_struct { // XXXih
	char __locale_struct_unused;
};

struct __musl_global_state {
    char mgs_dummy;
    size_t *auxv;
    size_t page_size;
    char secure;
};

#ifndef PAGE_SIZE
#define PAGE_SIZE libc.page_size
#endif

extern hidden struct __musl_global_state __musl_global_state;
#define libc __musl_global_state

extern char *__progname;
extern char *__progname_full;

#if defined(__cplusplus)
    } // extern "C"
#endif
