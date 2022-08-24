#include "stdio_impl.h"
#include <fcntl.h>
#include <string.h>

FILE *__fopen_rb_ca(const char *filename, FILE *f, unsigned char *buf, size_t len)
{
	memset(f, 0, sizeof *f);

	f->fd = sys_open(filename, O_RDONLY|O_CLOEXEC);
	if (f->fd < 0) return 0;
	__syscall(SYS_fcntl, f->fd, F_SETFD, FD_CLOEXEC);

	f->flags = F_NOWR | F_PERM;
	f->buf = buf + UNGET;
	f->buf_size = len - UNGET;
	f->read = __stdio_read;
	f->seek = __stdio_seek;
	f->close = __stdio_close;
#if MUSL_stdio_use_32bit_reentrant_mutex
	f->lock = -1;
#else
	f->lock_owner = -1;
	f->lock_generation = 0;
#endif

	return f;
}
