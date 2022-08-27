#define _GNU_SOURCE
#include <unistd.h>
#include "pthread_impl.h"

pid_t gettid(void)
{
#if MUSL_use_libc_internals
	return __pthread_self()->tid;
#else
	int ret = syscall(SYS_gettid);
	return ret;
#endif
}
