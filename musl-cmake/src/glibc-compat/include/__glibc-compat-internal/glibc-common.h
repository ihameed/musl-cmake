#pragma once

#if !defined(__BEGIN_DECLS)
    #if defined(__cplusplus)
        #define __BEGIN_DECLS extern "C" {
    #else
        #define __BEGIN_DECLS
    #endif
#endif

#if !defined(__END_DECLS)
    #if defined(__cplusplus)
        #define __END_DECLS }
    #else
        #define __END_DECLS
    #endif
#endif

#if !defined(__THROW)
    #define __THROW __attribute__ ((__nothrow__, __leaf__))
#endif

#if !defined(__THROWNL)
    #define __THROWNL __attribute__ ((__nothrow__))
#endif

#if !defined(__nonnull)
    #define __nonnull(params) __attribute__ ((__nonnull__ params))
#endif

#if !defined(__attribute_deprecated__)
    #define __attribute_deprecated__ __attribute__ ((__deprecated__))
#endif

#if !defined(__attribute_const__)
    #define __attribute_const__ __attribute__ ((__const__))
#endif
