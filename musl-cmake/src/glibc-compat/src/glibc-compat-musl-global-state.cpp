#include "glibc-compat-musl-global-state.h"

#include "../../../../include/elf.h"
#include "../../../../include/fcntl.h"
#include "../../../../include/poll.h"
#include "../../../../src/internal/syscall.h"
#include "../../../../include/signal.h"
#include "../../../../include/locale.h"

extern "C" {
    locale_t __musl_glibc_dummy_locale;
    struct __musl_global_state __musl_global_state;
    char *__progname;
    char *__progname_full;
}

static size_t const AUX_CNT = 38;

extern "C"
void __attribute__((visibility("hidden"), __noinline__))
__init_musl_global_state(uintptr_t, int argc, char **argv) {
    size_t i;
    size_t *auxv;
    size_t aux[AUX_CNT] = { 0 };
    auto pn = argv[0];
    char **envp = argv+argc+1;
    for (i=0; envp[i]; i++);
    libc.auxv = auxv = reinterpret_cast<size_t *>(envp+i+1);
    for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT) aux[auxv[i]] = auxv[i+1];
    libc.page_size = aux[AT_PAGESZ];

    if (!pn) pn = reinterpret_cast<char *>(aux[AT_EXECFN]);
    if (!pn) pn = const_cast<char *>("");
    __progname = __progname_full = pn;
    for (i=0; pn[i]; i++) if (pn[i]=='/') __progname = pn+i+1;

    if (aux[AT_UID]==aux[AT_EUID] && aux[AT_GID]==aux[AT_EGID]
            && !aux[AT_SECURE]) return;

    struct pollfd pfd[3] = { {.fd=0}, {.fd=1}, {.fd=2} };
    #ifdef SYS_poll
        int r = __syscall(SYS_poll, pfd, 3, 0);
    #else
        auto zero_timespec = (struct timespec) { };
        int r = __syscall(SYS_ppoll, pfd, 3, &zero_timespec, 0, _NSIG/8);
    #endif
    if (r<0) __builtin_trap();
    for (i=0; i<3; i++) if (pfd[i].revents&POLLNVAL)
        if (__sys_open("/dev/null", O_RDWR)<0)
            __builtin_trap();
    libc.secure = 1;
}
