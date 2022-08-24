#include "stdio_impl.h"

#undef stdout

static unsigned char buf[BUFSIZ+UNGET];
hidden FILE __stdout_FILE = {
	.buf = buf+UNGET,
	.buf_size = sizeof buf-UNGET,
	.fd = 1,
	.flags = F_PERM | F_NORD,
	.lbf = '\n',
	.write = __stdout_write,
	.seek = __stdio_seek,
	.close = __stdio_close,
#if MUSL_stdio_use_32bit_reentrant_mutex
	.lock = MUSL_never_lock,
#else
	.lock_owner = MUSL_never_lock,
	.lock_generation = 0
#endif
};
FILE *const stdout = &__stdout_FILE;
FILE *volatile __stdout_used = &__stdout_FILE;
