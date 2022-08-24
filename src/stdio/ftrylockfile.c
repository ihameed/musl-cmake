#include "stdio_impl.h"
#include "pthread_impl.h"
#include <limits.h>

#if MUSL_use_libc_internals
void __do_orphaned_stdio_locks()
{
	FILE *f;
	for (f=__pthread_self()->stdio_locks; f; f=f->next_locked)
		a_store(&f->lock, 0x40000000);
}

void __unlist_locked_file(FILE *f)
{
	if (f->lockcount) {
		if (f->next_locked) f->next_locked->prev_locked = f->prev_locked;
		if (f->prev_locked) f->prev_locked->next_locked = f->next_locked;
		else __pthread_self()->stdio_locks = f->next_locked;
	}
}

void __register_locked_file(FILE *f, pthread_t self)
{
	f->lockcount = 1;
	f->prev_locked = 0;
	f->next_locked = self->stdio_locks;
	if (f->next_locked) f->next_locked->prev_locked = f;
	self->stdio_locks = f;
}
#else
void __do_orphaned_stdio_locks()
{
}

void __unlist_locked_file(FILE *f)
{
    (void) f;
}
void __register_locked_file(FILE *f, pthread_t self)
{
    (void) f;
    (void) self;
}
#endif

int ftrylockfile(FILE *f)
{
#if MUSL_use_libc_internals
	pthread_t self = __pthread_self();
	int tid = self->tid;
#else
	pthread_t self = 0;
	_stdio_owner_t tid = MUSL_get_thread_label;
#endif
	_stdio_owner_t owner = MUSL_load_owner(f);
	if ((owner & ~MAYBE_WAITERS) == tid) {
		if (f->lockcount == LONG_MAX)
			return -1;
		f->lockcount++;
		return 0;
	}
#if MUSL_stdio_use_32bit_reentrant_mutex
	if (!MUSL_lockval_requires_lock_p(owner)) f->lock = owner = 0;
	if (owner || a_cas(&f->lock, 0, tid))
		return -1;
#else
	if (!MUSL_lockval_requires_lock_p(owner)) f->lock_owner = owner = 0;
	int64_t expected = 0;
	if (owner || !_ma_scas8(&f->lock_owner, &expected, tid, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE))
		return -1;
#endif
	__register_locked_file(f, self);
	return 0;
}
