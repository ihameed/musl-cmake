#include "stdio_impl.h"
#include "pthread_impl.h"

#ifdef __GNUC__
__attribute__((__noinline__))
#endif
static int locking_putc(int c, FILE *f)
{
#if MUSL_stdio_use_32bit_reentrant_mutex
	if (a_cas(&f->lock, 0, MUSL_fake_thread_id)) __lockfile(f);
	c = putc_unlocked(c, f);
	if (a_swap(&f->lock, 0) & MUSL_has_waiters)
		__wake(&f->lock, 1, 1);
	return c;
#else
	int64_t expected = 0;
	if (!_ma_wcas8(&f->lock_owner, &expected, 2, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE)) __lockfile(f);
	c = putc_unlocked(c, f);
	if (_ma_xchg8(&f->lock_owner, 0, __ATOMIC_RELEASE) & MAYBE_WAITERS) {
		_ma_xadd(&f->lock_generation, 1, __ATOMIC_RELEASE);
		__wake(&f->lock_generation, 1, 1);
	}
	return c;
#endif
}

static inline int do_putc(int c, FILE *f)
{
#if MUSL_stdio_use_32bit_reentrant_mutex
	int l = f->lock;
	if (!MUSL_lockval_requires_lock_p(l) || l && (l & ~MUSL_has_waiters) == MUSL_get_thread_label)
		return putc_unlocked(c, f);
	return locking_putc(c, f);
#else
	int64_t owner = _ma_load8(&f->lock_owner, __ATOMIC_ACQUIRE);
	if (!MUSL_lockval_requires_lock_p(owner) || owner && (owner & ~MUSL_has_waiters) == MUSL_get_thread_label)
		return putc_unlocked(c, f);
	return locking_putc(c, f);
#endif
}
