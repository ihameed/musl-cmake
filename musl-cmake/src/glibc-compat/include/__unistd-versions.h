#pragma once

#if defined(__amd64__) // (
    __asm__(".symver setegid, setegid@GLIBC_2.2.5");
    __asm__(".symver setreuid, setreuid@GLIBC_2.2.5");
    __asm__(".symver setresgid, setresgid@GLIBC_2.2.5");
    __asm__(".symver setresuid, setresuid@GLIBC_2.2.5");
    __asm__(".symver setuid, setuid@GLIBC_2.2.5");
    __asm__(".symver setregid, setregid@GLIBC_2.2.5");
    __asm__(".symver setgid, setgid@GLIBC_2.2.5");
    __asm__(".symver seteuid, seteuid@GLIBC_2.2.5");
    __asm__(".symver setgroups, setgroups@GLIBC_2.2.5");
#elif defined(__i386__) // ) (
    __asm__(".symver setegid, setegid@GLIBC_2.0");
    __asm__(".symver setreuid, setreuid@GLIBC_2.0");
    __asm__(".symver setresgid, setresgid@GLIBC_2.0");
    __asm__(".symver setresuid, setresuid@GLIBC_2.0");
    __asm__(".symver setuid, setuid@GLIBC_2.0");
    __asm__(".symver setregid, setregid@GLIBC_2.0");
    __asm__(".symver setgid, setgid@GLIBC_2.0");
    __asm__(".symver seteuid, seteuid@GLIBC_2.0");
    __asm__(".symver setgroups, setgroups@GLIBC_2.0");
#elif defined(__aarch64__) // ) (
    __asm__(".symver setegid, setegid@GLIBC_2.17");
    __asm__(".symver setreuid, setreuid@GLIBC_2.17");
    __asm__(".symver setresgid, setresgid@GLIBC_2.17");
    __asm__(".symver setresuid, setresuid@GLIBC_2.17");
    __asm__(".symver setuid, setuid@GLIBC_2.17");
    __asm__(".symver setregid, setregid@GLIBC_2.17");
    __asm__(".symver setgid, setgid@GLIBC_2.17");
    __asm__(".symver seteuid, seteuid@GLIBC_2.17");
    __asm__(".symver setgroups, setgroups@GLIBC_2.17");
#elif defined(__arm__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // ) (
    __asm__(".symver setegid, setegid@GLIBC_2.4");
    __asm__(".symver setreuid, setreuid@GLIBC_2.4");
    __asm__(".symver setresgid, setresgid@GLIBC_2.4");
    __asm__(".symver setresuid, setresuid@GLIBC_2.4");
    __asm__(".symver setuid, setuid@GLIBC_2.4");
    __asm__(".symver setregid, setregid@GLIBC_2.4");
    __asm__(".symver setgid, setgid@GLIBC_2.4");
    __asm__(".symver seteuid, seteuid@GLIBC_2.4");
    __asm__(".symver setgroups, setgroups@GLIBC_2.4");
#elif defined(__arm__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ // ) (
    __asm__(".symver setegid, setegid@GLIBC_2.4");
    __asm__(".symver setreuid, setreuid@GLIBC_2.4");
    __asm__(".symver setresgid, setresgid@GLIBC_2.4");
    __asm__(".symver setresuid, setresuid@GLIBC_2.4");
    __asm__(".symver setuid, setuid@GLIBC_2.4");
    __asm__(".symver setregid, setregid@GLIBC_2.4");
    __asm__(".symver setgid, setgid@GLIBC_2.4");
    __asm__(".symver seteuid, seteuid@GLIBC_2.4");
    __asm__(".symver setgroups, setgroups@GLIBC_2.4");
#elif defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ // ) (
    __asm__(".symver setegid, setegid@GLIBC_2.17");
    __asm__(".symver setreuid, setreuid@GLIBC_2.17");
    __asm__(".symver setresgid, setresgid@GLIBC_2.17");
    __asm__(".symver setresuid, setresuid@GLIBC_2.17");
    __asm__(".symver setuid, setuid@GLIBC_2.17");
    __asm__(".symver setregid, setregid@GLIBC_2.17");
    __asm__(".symver setgid, setgid@GLIBC_2.17");
    __asm__(".symver seteuid, seteuid@GLIBC_2.17");
    __asm__(".symver setgroups, setgroups@GLIBC_2.17");
#elif defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ // ) (
    __asm__(".symver setegid, setegid@GLIBC_2.3");
    __asm__(".symver setreuid, setreuid@GLIBC_2.3");
    __asm__(".symver setresgid, setresgid@GLIBC_2.3");
    __asm__(".symver setresuid, setresuid@GLIBC_2.3");
    __asm__(".symver setuid, setuid@GLIBC_2.3");
    __asm__(".symver setregid, setregid@GLIBC_2.3");
    __asm__(".symver setgid, setgid@GLIBC_2.3");
    __asm__(".symver seteuid, seteuid@GLIBC_2.3");
    __asm__(".symver setgroups, setgroups@GLIBC_2.3");
#elif defined(__powerpc__) && !defined(__powerpc64__) // ) (
    __asm__(".symver setegid, setegid@GLIBC_2.0");
    __asm__(".symver setreuid, setreuid@GLIBC_2.0");
    __asm__(".symver setresgid, setresgid@GLIBC_2.0");
    __asm__(".symver setresuid, setresuid@GLIBC_2.0");
    __asm__(".symver setuid, setuid@GLIBC_2.0");
    __asm__(".symver setregid, setregid@GLIBC_2.0");
    __asm__(".symver setgid, setgid@GLIBC_2.0");
    __asm__(".symver seteuid, seteuid@GLIBC_2.0");
    __asm__(".symver setgroups, setgroups@GLIBC_2.0");
#endif // )
