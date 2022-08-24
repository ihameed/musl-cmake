#include "stdio_impl.h"

#undef stdin

static unsigned char buf[BUFSIZ+UNGET];
hidden FILE __stdin_FILE = {
	.buf = buf+UNGET,
	.buf_size = sizeof buf-UNGET,
	.fd = 0,
	.flags = F_PERM | F_NOWR,
	.read = __stdio_read,
	.seek = __stdio_seek,
	.close = __stdio_close,
#if MUSL_stdio_use_32bit_reentrant_mutex
	.lock = MUSL_never_lock,
#else
	.lock_owner = MUSL_never_lock,
	.lock_generation = 0
#endif
};
FILE *const stdin = &__stdin_FILE;
FILE *volatile __stdin_used = &__stdin_FILE;
