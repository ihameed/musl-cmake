#pragma once

#if defined(__amd64__) // (
    __asm__(".symver __h_errno_location, __h_errno_location@GLIBC_2.2.5");
#elif defined(__i386__) // ) (
    __asm__(".symver __h_errno_location, __h_errno_location@GLIBC_2.0");
#elif defined(__aarch64__) // ) (
    __asm__(".symver __h_errno_location, __h_errno_location@GLIBC_2.17");
#elif defined(__arm__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // ) (
    __asm__(".symver __h_errno_location, __h_errno_location@GLIBC_2.4");
#elif defined(__arm__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ // ) (
    __asm__(".symver __h_errno_location, __h_errno_location@GLIBC_2.4");
#elif defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // ) (
    __asm__(".symver __h_errno_location, __h_errno_location@GLIBC_2.17");
#elif defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ // ) (
    __asm__(".symver __h_errno_location, __h_errno_location@GLIBC_2.3");
#elif defined(__powerpc__) && !defined(__powerpc64__) // ) (
    __asm__(".symver __h_errno_location, __h_errno_location@GLIBC_2.0");
#endif // )
