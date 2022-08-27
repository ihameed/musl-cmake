#include "../include/__stdlib-versions.h"
#include "../../../../include/stdlib.h"
#include "../../../../include/errno.h"
#include "glibc-compat-common.hpp"

extern "C" {

// XXXih: What's up with ld.bfd? If I place aligned_alloc and reallocarray in
// the same translation unit, compile with -ffunction-sections, and link with
// --gc-sections, pulling in aligned_alloc will pull in reallocarray and
// vice versa. This doesn't happen with ld.lld. I'm using multiple translation
// units as an (extremely ugly) workaround.
void *
aligned_alloc(__SIZE_TYPE__ const alignment, __SIZE_TYPE__ const size) {
    void *ret = nullptr;
    auto const err = posix_memalign(&ret, alignment, size);
    if (_glibc_compat_likely(err == 0)) {
        return ret;
    }
    errno = err;
    return nullptr;
}

}
