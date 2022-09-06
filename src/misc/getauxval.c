#include <sys/auxv.h>
#include <errno.h>
#include "libc.h"

#if MUSL_use_libc_internals // (

unsigned long __getauxval(unsigned long item)
{
	size_t *auxv = libc.auxv;
	if (item == AT_SECURE) return libc.secure;
	for (; *auxv; auxv+=2)
		if (*auxv==item) return auxv[1];
	errno = ENOENT;
	return 0;
}

#else // ) (

#include "../../include/fcntl.h"
#include "../../include/unistd.h"

#define __USE_GNU
#include "../../musl-cmake/src/glibc-compat/include/dlfcn.h"
typedef unsigned long (* getauxval_proc_t) (unsigned long item);

static unsigned long
init_getauxval(unsigned long item);

// The maximum value defined for any of the AT_* constants in Linux as of
// 1c23f9e627a7b412978b4e852793c5e3c3efc555 is 51. And `AT_VECTOR_SIZE_BASE` is
// 20, while `AT_VECTOR_SIZE_ARCH` is no greater than 15. Any system that uses
// a newer kernel that launches executables with enough auxv entries to cause
// trailing entry truncation during the initialization of `procfs_auxv` is
// also highly likely going to use a newer glibc version that doesn't have a
// broken `getauxval`, so 64 entries (63 with a trailing pair of zeroes) should
// be more than sufficient.
#define _proc_auxv_max_entries 64

static getauxval_proc_t volatile getauxval_proc = &init_getauxval; // FIXME: use _Atomic
static uintptr_t procfs_auxv[_proc_auxv_max_entries] = { 0 };
static uintptr_t *procfs_auxv_data_dependency_ordering_ptr = 0; // FIXME: use _Atomic

static unsigned long
always_enoent(unsigned long item) {
	(void) item;
	errno = ENOENT;
	return 0;
}

static int
populate_auxv_from_procfs() {
	int ret = 1;
	int hdl = open("/proc/self/auxv", O_RDONLY);
	if (hdl == -1) {
		return 0;
	}
	size_t const max_sz = (_proc_auxv_max_entries - 1) * sizeof(uintptr_t);
	size_t offs = 0;
	uintptr_t local_auxv_buf[_proc_auxv_max_entries] = { 0 };
	for (;;) {
		ssize_t result = read(hdl, local_auxv_buf + offs, max_sz - offs);
		if (result == -1) {
			if (errno == EINTR) continue;
			ret = 0;
			goto error;
		} else if (result == 0) {
			if (offs % (2 * sizeof(uintptr_t)) != 0) {
				ret = 0;
				goto done;
			}
			for (int i = 0; i < _proc_auxv_max_entries; ++i) {
				procfs_auxv[i] = local_auxv_buf[i];
			}
			goto done;
		}
		offs += result;
	}
done:
	__asm__ __volatile__("" ::: "memory"); // compiler barrier
	procfs_auxv_data_dependency_ordering_ptr = procfs_auxv;
error:
	close(hdl);
	return ret;
}

static unsigned long
scan_procfs_auxv(unsigned long tag) {
	uintptr_t *auxv = procfs_auxv_data_dependency_ordering_ptr;
	for (; *auxv; auxv+=2)
		if (*auxv==tag) return auxv[1];
	errno = ENOENT;
	return 0;
}

/*
 * First, try loading and using `getauxval` from glibc by using
 * `dlsym(RTLD_DEFAULT, "getauxval")`. `getauxval` was added in glibc 2.16
 * (2012-06-30); it had a bug in which it failed to report `ENOENT` for
 * missing elf auxiliary vector "types". This bug was fixed in glibc 2.19
 * (2014-02-07).
 *
 * Second, try reading auxiliary vector values from `/proc/self/auxv`. The
 * contents of this file are copied to a local 64-entry buffer; it is
 * extremely unlikely that a system using glibc < 2.16 will also use a
 * kernel that has more than 64 auxv entries. It is possible for procfs to
 * not be mounted, or for some procfs entries to be unreadable (like on
 * e.g. Android [1], although Android is not a target platform for this
 * project).
 *
 * Finally, if the above doesn't work, fall back to a function that always
 * returns 0 and sets `ENOENT`.
 *
 * glibc exports a dynamic symbol named `_argv` that points to the original
 * argument vector. It would be possible to fetch the auxiliary vector by
 * getting the address of `_argv` via `dlsym(RTLD_DEFAULT, "_argv")` and
 * then scanning forward for the argv zero terminator and environment zero
 * terminator.  Unfortunately, `_argc` is not exported, and the memory
 * pointed to by `_argv` may be mutated arbitrarily; it's possible to read
 * `/proc/self/cmdline` to determine the size of the argument vector (and
 * `/proc/self/environ` to determine the size of the memory pointed to by
 * `__environ`), but if procfs is accessible and glibc is being used (e.g.
 * if the underlying os is normal Linux and not Android) then
 * `/proc/self/auxv` should also be readable.
 *
 * It'd also be possible to add before-main startup code that saves a copy
 * of the address of the auxiliary vector that is then read by an exported
 * (default visibility) getauxval-compatible function; in this case, an
 * executable using musl-glibc, and dlls built with musl-glibc loaded in a
 * musl-glibc executable, would have a reliable implementation of
 * `getauxval`. I may do this later.
 *
 * [1] https://android-review.googlesource.com/c/platform/ndk/+/51271/1/sources/android/cpufeatures/cpu-features.c
 */
static unsigned long
init_getauxval(unsigned long item) {
	// `getauxval` was introduced in glibc 2.16. glibc failed to report
	// ENOENT for missing AT_* "types" until 2.19. It's possible to eagerly
	// fall back to the procfs-based implementation for glibc 2.16 to 2.18,
	// but that would require parsing the output of `gnu_get_libc_version`
	// or doing something similarly unpleasant.
	void * const raw_proc = dlsym(RTLD_DEFAULT, "getauxval");
	if (raw_proc != 0) {
		getauxval_proc_t proc = (getauxval_proc_t) raw_proc;
		getauxval_proc = raw_proc;
		return proc(item);
	}
	int success = populate_auxv_from_procfs();
	if (success) {
		getauxval_proc = scan_procfs_auxv;
		return scan_procfs_auxv(item);
	}
	getauxval_proc = always_enoent;
	return always_enoent(item);
}

unsigned long __getauxval(unsigned long item)
{
	getauxval_proc_t proc = getauxval_proc;
	return proc(item);
}

#endif // )

weak_alias(__getauxval, getauxval);
