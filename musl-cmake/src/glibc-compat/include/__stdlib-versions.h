#pragma once

#if defined(__amd64__) // (
    __asm__(".symver posix_memalign, posix_memalign@GLIBC_2.2.5");
    __asm__(".symver malloc, malloc@GLIBC_2.2.5");
    __asm__(".symver calloc, calloc@GLIBC_2.2.5");
    __asm__(".symver realloc, realloc@GLIBC_2.2.5");
    __asm__(".symver free, free@GLIBC_2.2.5");
    __asm__(".symver valloc, valloc@GLIBC_2.2.5");
    __asm__(".symver memalign, memalign@GLIBC_2.2.5");
    __asm__(".symver malloc_usable_size, malloc_usable_size@GLIBC_2.2.5");
#elif defined(__i386__) // ) (
    __asm__(".symver posix_memalign, posix_memalign@GLIBC_2.2");
    __asm__(".symver malloc, malloc@GLIBC_2.0");
    __asm__(".symver calloc, calloc@GLIBC_2.0");
    __asm__(".symver realloc, realloc@GLIBC_2.0");
    __asm__(".symver free, free@GLIBC_2.0");
    __asm__(".symver valloc, valloc@GLIBC_2.0");
    __asm__(".symver memalign, memalign@GLIBC_2.0");
    __asm__(".symver malloc_usable_size, malloc_usable_size@GLIBC_2.0");
#elif defined(__aarch64__) // ) (
    __asm__(".symver posix_memalign, posix_memalign@GLIBC_2.17");
    __asm__(".symver malloc, malloc@GLIBC_2.17");
    __asm__(".symver calloc, calloc@GLIBC_2.17");
    __asm__(".symver realloc, realloc@GLIBC_2.17");
    __asm__(".symver free, free@GLIBC_2.17");
    __asm__(".symver valloc, valloc@GLIBC_2.17");
    __asm__(".symver memalign, memalign@GLIBC_2.17");
    __asm__(".symver malloc_usable_size, malloc_usable_size@GLIBC_2.17");
#elif defined(__arm__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // ) (
    __asm__(".symver posix_memalign, posix_memalign@GLIBC_2.4");
    __asm__(".symver malloc, malloc@GLIBC_2.4");
    __asm__(".symver calloc, calloc@GLIBC_2.4");
    __asm__(".symver realloc, realloc@GLIBC_2.4");
    __asm__(".symver free, free@GLIBC_2.4");
    __asm__(".symver valloc, valloc@GLIBC_2.4");
    __asm__(".symver memalign, memalign@GLIBC_2.4");
    __asm__(".symver malloc_usable_size, malloc_usable_size@GLIBC_2.4");
#elif defined(__arm__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ // ) (
    __asm__(".symver posix_memalign, posix_memalign@GLIBC_2.4");
    __asm__(".symver malloc, malloc@GLIBC_2.4");
    __asm__(".symver calloc, calloc@GLIBC_2.4");
    __asm__(".symver realloc, realloc@GLIBC_2.4");
    __asm__(".symver free, free@GLIBC_2.4");
    __asm__(".symver valloc, valloc@GLIBC_2.4");
    __asm__(".symver memalign, memalign@GLIBC_2.4");
    __asm__(".symver malloc_usable_size, malloc_usable_size@GLIBC_2.4");
#elif defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // ) (
    __asm__(".symver posix_memalign, posix_memalign@GLIBC_2.17");
    __asm__(".symver malloc, malloc@GLIBC_2.17");
    __asm__(".symver calloc, calloc@GLIBC_2.17");
    __asm__(".symver realloc, realloc@GLIBC_2.17");
    __asm__(".symver free, free@GLIBC_2.17");
    __asm__(".symver valloc, valloc@GLIBC_2.17");
    __asm__(".symver memalign, memalign@GLIBC_2.17");
    __asm__(".symver malloc_usable_size, malloc_usable_size@GLIBC_2.17");
#elif defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ // ) (
    __asm__(".symver posix_memalign, posix_memalign@GLIBC_2.3");
    __asm__(".symver malloc, malloc@GLIBC_2.3");
    __asm__(".symver calloc, calloc@GLIBC_2.3");
    __asm__(".symver realloc, realloc@GLIBC_2.3");
    __asm__(".symver free, free@GLIBC_2.3");
    __asm__(".symver valloc, valloc@GLIBC_2.3");
    __asm__(".symver memalign, memalign@GLIBC_2.3");
    __asm__(".symver malloc_usable_size, malloc_usable_size@GLIBC_2.3");
#elif defined(__powerpc__) && !defined(__powerpc64__) // ) (
    __asm__(".symver posix_memalign, posix_memalign@GLIBC_2.2");
    __asm__(".symver malloc, malloc@GLIBC_2.0");
    __asm__(".symver calloc, calloc@GLIBC_2.0");
    __asm__(".symver realloc, realloc@GLIBC_2.0");
    __asm__(".symver free, free@GLIBC_2.0");
    __asm__(".symver valloc, valloc@GLIBC_2.0");
    __asm__(".symver memalign, memalign@GLIBC_2.0");
    __asm__(".symver malloc_usable_size, malloc_usable_size@GLIBC_2.0");
#endif // )
