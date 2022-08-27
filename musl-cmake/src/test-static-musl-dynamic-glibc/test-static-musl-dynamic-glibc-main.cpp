#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

void *
thread_proc(void *) {
    printf("hello from thread\n");
    pthread_cond_t pcond;
    pthread_cond_init(&pcond, nullptr);
    pthread_cond_destroy(&pcond);
    return nullptr;
}

extern "C"
int
main(int, char **) {
    auto const dll = dlopen("/usr/lib/libc.so.6", RTLD_LOCAL);
    auto const raw_glibc_printf = dlsym(dll, "printf");
    auto const glibc_printf = reinterpret_cast<int (*) (char const *, ...)>(raw_glibc_printf);
    glibc_printf("glibc hober\n");
    printf("hober!\n");
    printf("glibc_printf = %p; musl printf = %p\n", glibc_printf, &printf);

    pthread_t pth { };
    pthread_create(&pth, nullptr, &thread_proc, nullptr);

    void *thr_ret = nullptr;
    pthread_join(pth, &thr_ret);
    return 0;
}
