#ifndef FEATURES_H
#define FEATURES_H

#include "../../include/features.h"

#define weak __attribute__((__weak__))
#define hidden __attribute__((__visibility__("hidden")))
#define weak_alias(old, new) \
	extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))

// XXXih: HACK: force hidden symbol visibility for symbols defined in assembly files
#include "../musl-cmake/src/glibc-compat/include/__glibc-compat-internal/musl-asm-symbol-hidden-visibility.h"

#if !defined(MUSL_cmake_glibc_compat)
    #define MUSL_use_libc_internals 1
#else
    #define MUSL_use_libc_internals 0
#endif

#endif
