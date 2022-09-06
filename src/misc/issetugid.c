#define _BSD_SOURCE
#include <unistd.h>
#include "libc.h"

#if MUSL_use_libc_internals
int issetugid(void)
{
	return libc.secure;
}
#endif
