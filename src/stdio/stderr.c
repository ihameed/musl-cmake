#include "stdio_impl.h"

#undef stderr

static unsigned char buf[UNGET];
hidden FILE __stderr_FILE = {
	.buf = buf+UNGET,
	.buf_size = 0,
	.fd = 2,
	.flags = F_PERM | F_NORD,
	.lbf = -1,
	.write = __stdio_write,
	.seek = __stdio_seek,
	.close = __stdio_close,
#if MUSL_stdio_use_32bit_reentrant_mutex
	.lock = MUSL_never_lock,
#else
	.lock_owner = MUSL_never_lock,
	.lock_generation = 0
#endif
};
FILE *const stderr = &__stderr_FILE;
FILE *volatile __stderr_used = &__stderr_FILE;
