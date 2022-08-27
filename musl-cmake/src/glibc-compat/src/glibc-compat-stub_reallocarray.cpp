#include "../include/__stdlib-versions.h"
#include "../../../../include/stdlib.h"
#include "../../../../include/errno.h"
#include "glibc-compat-common.hpp"

extern "C" {

void *
reallocarray(void * const ptr, __SIZE_TYPE__ const nmemb, __SIZE_TYPE__ const size) {
    size_t result;
    auto const overflow = __builtin_mul_overflow(nmemb, size, &result);
    if (_glibc_compat_unlikely(overflow)) {
        errno = ENOMEM;
        return 0;
    }
    return realloc(ptr, result);
}

}
