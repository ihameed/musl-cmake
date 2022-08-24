#include "stdio_impl.h"
#include <string.h>

static size_t string_read(FILE *f, unsigned char *buf, size_t len)
{
	char *src = f->cookie;
	size_t k = len+256;
	char *end = memchr(src, 0, k);
	if (end) k = end-src;
	if (k < len) len = k;
	memcpy(buf, src, len);
	f->rpos = (void *)(src+len);
	f->rend = (void *)(src+k);
	f->cookie = src+k;
	return len;
}

int vsscanf(const char *restrict s, const char *restrict fmt, va_list ap)
{
	FILE f = {
		.buf = (void *)s, .cookie = (void *)s,
		.read = string_read,
	#if MUSL_stdio_use_32bit_reentrant_mutex
		.lock = MUSL_never_lock,
	#else
		.lock_owner = MUSL_never_lock,
		.lock_generation = 0,
	#endif
	};
	return vfscanf(&f, fmt, ap);
}

weak_alias(vsscanf,__isoc99_vsscanf);
