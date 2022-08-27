#pragma once

extern locale_t __musl_glibc_dummy_locale;

#define MUSL_GLIBC_TODO_crash_on_locale_access  (*({ __builtin_trap(); &__musl_glibc_dummy_locale; }))
#define CURRENT_LOCALE MUSL_GLIBC_TODO_crash_on_locale_access
#define C_LOCALE MUSL_GLIBC_TODO_crash_on_locale_access
#define UTF8_LOCALE MUSL_GLIBC_TODO_crash_on_locale_access

#define LCTRANS(msg, lc, loc) __lctrans(msg, NULL)
#define LCTRANS_CUR(msg) __lctrans_cur(msg)

hidden const char *__lctrans(const char *, const struct __locale_map *);
hidden const char *__lctrans_cur(const char *);
