#pragma once

#include_next "features.h"

#if defined(_GNU_SOURCE) && !defined(__USE_GNU)
    #define __USE_GNU 1
#endif

#if defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE - 0) >= 700 && !defined(__USE_XOPEN2K)
    #define __USE_XOPEN2K 1
#endif

#if defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif
