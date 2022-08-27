#pragma once

#if defined(__amd64__) // (
    __asm__(".symver dlopen, dlopen@GLIBC_2.2.5");
    __asm__(".symver dlclose, dlclose@GLIBC_2.2.5");
    __asm__(".symver dlsym, dlsym@GLIBC_2.2.5");
    __asm__(".symver dlmopen, dlmopen@GLIBC_2.3.4");
    __asm__(".symver dlvsym, dlvsym@GLIBC_2.2.5");
    __asm__(".symver dlerror, dlerror@GLIBC_2.2.5");
    __asm__(".symver dladdr, dladdr@GLIBC_2.2.5");
    __asm__(".symver dladdr1, dladdr1@GLIBC_2.3.3");
    __asm__(".symver dlinfo, dlinfo@GLIBC_2.3.3");
#elif defined(__i386__) // ) (
    __asm__(".symver dlopen, dlopen@GLIBC_2.0");
    __asm__(".symver dlclose, dlclose@GLIBC_2.0");
    __asm__(".symver dlsym, dlsym@GLIBC_2.0");
    __asm__(".symver dlmopen, dlmopen@GLIBC_2.3.4");
    __asm__(".symver dlvsym, dlvsym@GLIBC_2.1");
    __asm__(".symver dlerror, dlerror@GLIBC_2.0");
    __asm__(".symver dladdr, dladdr@GLIBC_2.0");
    __asm__(".symver dladdr1, dladdr1@GLIBC_2.3.3");
    __asm__(".symver dlinfo, dlinfo@GLIBC_2.3.3");
#elif defined(__aarch64__) // ) (
    __asm__(".symver dlopen, dlopen@GLIBC_2.17");
    __asm__(".symver dlclose, dlclose@GLIBC_2.17");
    __asm__(".symver dlsym, dlsym@GLIBC_2.17");
    __asm__(".symver dlmopen, dlmopen@GLIBC_2.17");
    __asm__(".symver dlvsym, dlvsym@GLIBC_2.17");
    __asm__(".symver dlerror, dlerror@GLIBC_2.17");
    __asm__(".symver dladdr, dladdr@GLIBC_2.17");
    __asm__(".symver dladdr1, dladdr1@GLIBC_2.17");
    __asm__(".symver dlinfo, dlinfo@GLIBC_2.17");
#elif defined(__arm__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // ) (
    __asm__(".symver dlopen, dlopen@GLIBC_2.4");
    __asm__(".symver dlclose, dlclose@GLIBC_2.4");
    __asm__(".symver dlsym, dlsym@GLIBC_2.4");
    __asm__(".symver dlmopen, dlmopen@GLIBC_2.4");
    __asm__(".symver dlvsym, dlvsym@GLIBC_2.4");
    __asm__(".symver dlerror, dlerror@GLIBC_2.4");
    __asm__(".symver dladdr, dladdr@GLIBC_2.4");
    __asm__(".symver dladdr1, dladdr1@GLIBC_2.4");
    __asm__(".symver dlinfo, dlinfo@GLIBC_2.4");
#elif defined(__arm__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ // ) (
    __asm__(".symver dlopen, dlopen@GLIBC_2.4");
    __asm__(".symver dlclose, dlclose@GLIBC_2.4");
    __asm__(".symver dlsym, dlsym@GLIBC_2.4");
    __asm__(".symver dlmopen, dlmopen@GLIBC_2.4");
    __asm__(".symver dlvsym, dlvsym@GLIBC_2.4");
    __asm__(".symver dlerror, dlerror@GLIBC_2.4");
    __asm__(".symver dladdr, dladdr@GLIBC_2.4");
    __asm__(".symver dladdr1, dladdr1@GLIBC_2.4");
    __asm__(".symver dlinfo, dlinfo@GLIBC_2.4");
#elif defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // ) (
    __asm__(".symver dlopen, dlopen@GLIBC_2.17");
    __asm__(".symver dlclose, dlclose@GLIBC_2.17");
    __asm__(".symver dlsym, dlsym@GLIBC_2.17");
    __asm__(".symver dlmopen, dlmopen@GLIBC_2.17");
    __asm__(".symver dlvsym, dlvsym@GLIBC_2.17");
    __asm__(".symver dlerror, dlerror@GLIBC_2.17");
    __asm__(".symver dladdr, dladdr@GLIBC_2.17");
    __asm__(".symver dladdr1, dladdr1@GLIBC_2.17");
    __asm__(".symver dlinfo, dlinfo@GLIBC_2.17");
#elif defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ // ) (
    __asm__(".symver dlopen, dlopen@GLIBC_2.3");
    __asm__(".symver dlclose, dlclose@GLIBC_2.3");
    __asm__(".symver dlsym, dlsym@GLIBC_2.3");
    __asm__(".symver dlmopen, dlmopen@GLIBC_2.3.4");
    __asm__(".symver dlvsym, dlvsym@GLIBC_2.3");
    __asm__(".symver dlerror, dlerror@GLIBC_2.3");
    __asm__(".symver dladdr, dladdr@GLIBC_2.3");
    __asm__(".symver dladdr1, dladdr1@GLIBC_2.3.3");
    __asm__(".symver dlinfo, dlinfo@GLIBC_2.3.3");
#elif defined(__powerpc__) && !defined(__powerpc64__) // ) (
    __asm__(".symver dlopen, dlopen@GLIBC_2.0");
    __asm__(".symver dlclose, dlclose@GLIBC_2.0");
    __asm__(".symver dlsym, dlsym@GLIBC_2.0");
    __asm__(".symver dlmopen, dlmopen@GLIBC_2.3.4");
    __asm__(".symver dlvsym, dlvsym@GLIBC_2.1");
    __asm__(".symver dlerror, dlerror@GLIBC_2.0");
    __asm__(".symver dladdr, dladdr@GLIBC_2.0");
    __asm__(".symver dladdr1, dladdr1@GLIBC_2.3.3");
    __asm__(".symver dlinfo, dlinfo@GLIBC_2.3.3");
#endif // )
