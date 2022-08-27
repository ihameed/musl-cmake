#pragma once

#define _glibc_compat_likely(x)     __builtin_expect((x),1)
#define _glibc_compat_unlikely(x)     __builtin_expect((x),0)
