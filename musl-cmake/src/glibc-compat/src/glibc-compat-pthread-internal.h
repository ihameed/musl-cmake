#pragma once

#define pthread __pthread

struct pthread {
	/* Part 1 -- these fields may be external or
	 * internal (accessed via asm) ABI. Do not change. */
	struct pthread *__musl_pthread__self;
#ifndef TLS_ABOVE_TP
	uintptr_t *__musl_pthread__dtv;
#endif
	void *__musl_pthread___nonabi_prev;
	void *__musl_pthread___nonabi_next;
	uintptr_t __musl_pthread__sysinfo;
#ifndef TLS_ABOVE_TP
#ifdef CANARY_PAD
	uintptr_t __musl_pthread__canary_pad;
#endif
	uintptr_t __musl_pthread__canary;
#endif
};

#ifndef TP_OFFSET
#define TP_OFFSET 0
#endif

#ifndef DTP_OFFSET
#define DTP_OFFSET 0
#endif

#ifdef TLS_ABOVE_TP
#define TP_ADJ(p) ((char *)(p) + sizeof(struct pthread) + TP_OFFSET)
#define __pthread_self() ((struct __pthread *)(__get_tp() - sizeof(struct __pthread) - TP_OFFSET))
#else
#define TP_ADJ(p) (p)
#define __pthread_self() ((struct __pthread*)__get_tp())
#endif

hidden int __clone(int (*)(void *), void *, int, void *, ...); // musl-glibc: from glibc

static inline void __wake(volatile void *addr, int cnt, int priv)
{
	if (priv) priv = FUTEX_PRIVATE;
	if (cnt<0) cnt = INT_MAX;
	__syscall(SYS_futex, addr, FUTEX_WAKE|priv, cnt) != -ENOSYS ||
	__syscall(SYS_futex, addr, FUTEX_WAKE, cnt);
}
static inline void __futexwait(volatile void *addr, int val, int priv)
{
	if (priv) priv = FUTEX_PRIVATE;
	__syscall(SYS_futex, addr, FUTEX_WAIT|priv, val, 0) != -ENOSYS ||
	__syscall(SYS_futex, addr, FUTEX_WAIT, val, 0);
}
