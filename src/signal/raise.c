#include <signal.h>
#include <stdint.h>
#include "syscall.h"
#include "pthread_impl.h"

int raise(int sig)
{
	sigset_t set;
	__block_app_sigs(&set);
#if MUSL_use_libc_internals
	int ret = syscall(SYS_tkill, __pthread_self()->tid, sig);
#else
	int tid = syscall(SYS_gettid);
	int ret = syscall(SYS_tkill, tid, sig);
#endif
	__restore_sigs(&set);
	return ret;
}
