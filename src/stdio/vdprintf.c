#include "stdio_impl.h"

int vdprintf(int fd, const char *restrict fmt, va_list ap)
{
	FILE f = {
		.fd = fd, .lbf = EOF, .write = __stdio_write,
		.buf = (void *)fmt, .buf_size = 0,
	#if MUSL_stdio_use_32bit_reentrant_mutex
		.lock = MUSL_never_lock,
	#else
		.lock_owner = MUSL_never_lock,
		.lock_generation = 0
	#endif
	};
	return vfprintf(&f, fmt, ap);
}
