#ifndef _STDIO_IMPL_H
#define _STDIO_IMPL_H

#include <stdio.h>
#include "syscall.h"

#define UNGET 8

#if MUSL_use_libc_internals
	#define MUSL_stdio_use_32bit_reentrant_mutex 1
	typedef __INT32_TYPE__ _stdio_owner_t;
	#define MUSL_get_thread_label ((_stdio_owner_t) (__pthread_self()->tid))
	#define MUSL_load_owner(f) ((_stdio_owner_t) ((f)->lock))
	#define MUSL_lockval_requires_lock_p(x) ((x) >= 0)
	#define MUSL_fake_thread_id (MAYBE_WAITERS - 1)
	#define MUSL_has_waiters MAYBE_WAITERS
	#define MUSL_never_lock -1
#else
	#if __INTPTR_WIDTH__ == 64
		#define MUSL_stdio_use_32bit_reentrant_mutex 0
		typedef __INT64_TYPE__ _stdio_owner_t;
		#define MUSL_get_thread_label ((__INT64_TYPE__) (__pthread_self()))
		#define MUSL_load_owner(f) ((_stdio_owner_t) (_ma_load8(&(f)->lock_owner, __ATOMIC_RELAXED)))
		#define MUSL_lockval_requires_lock_p(x) (x >= 0)
		#define MUSL_fake_thread_id 2
		#define MUSL_has_waiters 1
		#define MUSL_never_lock -1
	#elif __INTPTR_WIDTH__ == 32
		#define MUSL_stdio_use_32bit_reentrant_mutex 1
		typedef __UINT32_TYPE__ _stdio_owner_t;
		#define MUSL_get_thread_label ((__UINT32_TYPE__) __pthread_self())
		#define MUSL_load_owner(f) ((_stdio_owner_t) ((f)->lock))
		#define MUSL_lockval_requires_lock_p(x) (((x) & 0x1u) != 1)
		#define MUSL_fake_thread_id 0xfffffffcu
		#define MUSL_has_waiters 2
		#define MUSL_never_lock 1
	#else
		#error "Unsupported machine bit width."
	#endif
	#if defined(__clang__)
		#define _ma_load8(target, order) __c11_atomic_load(target, order)
		#define _ma_load4(target, order) __c11_atomic_load(target, order)
		#define _ma_wcas8(target, expected, desired, ok_order, fail_order) __c11_atomic_compare_exchange_weak(target, expected, desired, ok_order, fail_order)
		#define _ma_scas8(target, expected, desired, ok_order, fail_order) __c11_atomic_compare_exchange_strong(target, expected, desired, ok_order, fail_order)
		#define _ma_xchg8(target, val, order) __c11_atomic_exchange(target, val, order)
		#define _ma_xadd(target, addend, order) __c11_atomic_fetch_add(target, addend, order)
	#else
		#define _ma_load8(target, order) __atomic_load_8(target, order)
		#define _ma_load4(target, order) __atomic_load_4(target, order)
		#define _ma_wcas8(target, expected, desired, ok_order, fail_order) __atomic_compare_exchange_8(target, expected, desired, 1, ok_order, fail_order)
		#define _ma_scas8(target, expected, desired, ok_order, fail_order) __atomic_compare_exchange_8(target, expected, desired, 0, ok_order, fail_order)
		#define _ma_xchg8(target, val, order) __atomic_exchange_8(target, val, order)
		#define _ma_xadd(target, addend, order) __atomic_add_fetch(target, addend, order)
	#endif
#endif
#define MUSL_requires_lock_p(f) (MUSL_lockval_requires_lock_p(MUSL_load_owner(f)))

#define FFINALLOCK(f) (MUSL_requires_lock_p(f) ? __lockfile((f)) : 0)
#define FLOCK(f) int __need_unlock = (MUSL_requires_lock_p(f) ? __lockfile((f)) : 0)
#define FUNLOCK(f) do { if (__need_unlock) __unlockfile((f)); } while (0)

#define F_PERM 1
#define F_NORD 4
#define F_NOWR 8
#define F_EOF 16
#define F_ERR 32
#define F_SVB 64
#define F_APP 128

struct _IO_FILE {
	unsigned flags;
	unsigned char *rpos, *rend;
	int (*close)(FILE *);
	unsigned char *wend, *wpos;
	unsigned char *mustbezero_1;
	unsigned char *wbase;
	size_t (*read)(FILE *, unsigned char *, size_t);
	size_t (*write)(FILE *, const unsigned char *, size_t);
	off_t (*seek)(FILE *, off_t, int);
	unsigned char *buf;
	size_t buf_size;
	FILE *prev, *next;
	int fd;
	int pipe_pid;
	long lockcount;
	int mode;
#if MUSL_stdio_use_32bit_reentrant_mutex
	volatile int lock;
#else
	__INT64_TYPE__ _Atomic lock_owner;
	__UINT32_TYPE__ _Atomic lock_generation;
#endif
	int lbf;
	void *cookie;
	off_t off;
	char *getln_buf;
	void *mustbezero_2;
	unsigned char *shend;
	off_t shlim, shcnt;
	FILE *prev_locked, *next_locked;
	struct __locale_struct *locale;
};

extern hidden FILE *volatile __stdin_used;
extern hidden FILE *volatile __stdout_used;
extern hidden FILE *volatile __stderr_used;

hidden int __lockfile(FILE *);
hidden void __unlockfile(FILE *);

hidden size_t __stdio_read(FILE *, unsigned char *, size_t);
hidden size_t __stdio_write(FILE *, const unsigned char *, size_t);
hidden size_t __stdout_write(FILE *, const unsigned char *, size_t);
hidden off_t __stdio_seek(FILE *, off_t, int);
hidden int __stdio_close(FILE *);

hidden int __toread(FILE *);
hidden int __towrite(FILE *);

hidden void __stdio_exit(void);
hidden void __stdio_exit_needed(void);

// XXXih: TODO: Adjust this based on static/dynamic build configuration
// Removing this attribute prevents __overflow from appearing in the export
// list of position-independent executables (compiled with -fpie/-fPIE or
// -fpic/-fPIC) when statically linked with this copy of musl.
/*
#if defined(__PIC__) && (100*__GNUC__+__GNUC_MINOR__ >= 303)
__attribute__((visibility("protected")))
#endif
*/
int __overflow(FILE *, int), __uflow(FILE *);

hidden int __fseeko(FILE *, off_t, int);
hidden int __fseeko_unlocked(FILE *, off_t, int);
hidden off_t __ftello(FILE *);
hidden off_t __ftello_unlocked(FILE *);
hidden size_t __fwritex(const unsigned char *, size_t, FILE *);
hidden int __putc_unlocked(int, FILE *);

hidden FILE *__fdopen(int, const char *);
hidden int __fmodeflags(const char *);

hidden FILE *__ofl_add(FILE *f);
hidden FILE **__ofl_lock(void);
hidden void __ofl_unlock(void);

struct __pthread;
hidden void __register_locked_file(FILE *, struct __pthread *);
hidden void __unlist_locked_file(FILE *);
hidden void __do_orphaned_stdio_locks(void);

#define MAYBE_WAITERS 0x40000000

hidden void __getopt_msg(const char *, const char *, const char *, size_t);

#define feof(f) ((f)->flags & F_EOF)
#define ferror(f) ((f)->flags & F_ERR)

#define getc_unlocked(f) \
	( ((f)->rpos != (f)->rend) ? *(f)->rpos++ : __uflow((f)) )

#define putc_unlocked(c, f) \
	( (((unsigned char)(c)!=(f)->lbf && (f)->wpos!=(f)->wend)) \
	? *(f)->wpos++ = (unsigned char)(c) \
	: __overflow((f),(unsigned char)(c)) )

/* Caller-allocated FILE * operations */
hidden FILE *__fopen_rb_ca(const char *, FILE *, unsigned char *, size_t);
hidden int __fclose_ca(FILE *);

#endif
