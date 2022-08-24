#include "stdio_impl.h"
#include "pthread_impl.h"

int __lockfile(FILE *f)
{
#if MUSL_stdio_use_32bit_reentrant_mutex
	int owner = f->lock, tid = MUSL_get_thread_label;
	if ((owner & ~MUSL_has_waiters) == tid)
		return 0;
	owner = a_cas(&f->lock, 0, tid);
	if (!owner) return 1;
	while ((owner = a_cas(&f->lock, 0, tid|MUSL_has_waiters))) {
		if ((owner & MUSL_has_waiters) ||
		    a_cas(&f->lock, owner, owner|MUSL_has_waiters)==owner)
			__futexwait(&f->lock, owner|MUSL_has_waiters, 1);
	}
	return 1;
#else
	int64_t const current_thread = (int64_t) __pthread_self();
	int64_t const lock_owner = _ma_load8(&f->lock_owner, __ATOMIC_RELAXED);
	if (lock_owner == current_thread) return 0;
	int64_t expected = 0;
	int64_t const waiter_tag = 1;
	for (;;) {
		uint32_t generation_snapshot = _ma_load4(&f->lock_generation, __ATOMIC_RELAXED);
		_Bool const acquire_lock_success = _ma_wcas8(&f->lock_owner, &expected, current_thread, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
		if (acquire_lock_success) return 1;
		_Bool const mark_wait_success = _ma_wcas8(&f->lock_owner, &expected, expected | waiter_tag, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
		if (mark_wait_success) {
			__futexwait(&f->lock_generation, generation_snapshot, 1);
		}
	}
#endif
}

void __unlockfile(FILE *f)
{
#if MUSL_stdio_use_32bit_reentrant_mutex
	if (a_swap(&f->lock, 0) & MUSL_has_waiters)
		__wake(&f->lock, 1, 1);
#else
	int64_t const old_owner = _ma_xchg8(&f->lock_owner, 0, __ATOMIC_RELEASE);
	int64_t const waiter_tag = 1;
	if ((old_owner & waiter_tag) != 0) {
		_ma_xadd(&f->lock_generation, 1, __ATOMIC_RELEASE);
		__wake(&f->lock_generation, 1, 1);
	}
#endif
}
