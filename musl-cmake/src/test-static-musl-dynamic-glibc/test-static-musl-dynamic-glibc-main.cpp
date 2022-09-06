#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>

void *
thread_proc(void *) {
    printf("hello from thread\n");
    pthread_cond_t pcond;
    pthread_cond_init(&pcond, nullptr);
    pthread_cond_destroy(&pcond);
    return nullptr;
}

struct dynamic_loader_global_data_t {
        int debug_mask;
        char const *platform;
        size_t platform_len;
        size_t page_size;
        size_t min_sig_stack_size;
        int inhibit_cache;
        int clock_tick;
        int verbose;
        int debug_fd;
        int lazy;
        int bind_not;
        int dynamic_weak;
        int fpu_control;
        uint64_t hwcap;
        uint64_t hwcap_mask;
        uintptr_t *auxv;
};

extern "C"
int
main(int, char **) {
    auto const ptr = static_cast<dynamic_loader_global_data_t const *>(dlsym(RTLD_DEFAULT, "_rtld_global_ro"));
    printf("got ptr = %p\n", ptr);
    printf("got auxv = %p\n", ptr->auxv);
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
