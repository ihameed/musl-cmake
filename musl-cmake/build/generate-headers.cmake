# This script generates architecture-specific alltypes.h and syscall.h files
# using alltypes.h.in and syscall.h.in as input. The generated files are safe
# to check in to source control; be sure to re-run this script if the source
# files change. I am assuming that the source files will infrequently change
# and that application programmer convenience (i.e., loading a project and
# getting fully-functional cross-referencing without needing to first run
# a build) is more important than modifying either alltypes.h.in or syscall.h.in
# with minimal friction.
#
# Execute this script by running:
#
#     cmake -Dgenerate-headers-target-dir=path/to/outdir -P generate-headers.cmake

function(_musl_make_bits_alltypes_h arch target-dir)
    file(READ "../../arch/${arch}/bits/alltypes.h.in" arch-alltypes-h-in)
    file(READ "../../include/alltypes.h.in" alltypes-h-in)
    string(CONCAT alltypes-h "${arch-alltypes-h-in}" "${alltypes-h-in}")

    string(REGEX REPLACE "TYPEDEF ([^\n\r]*) ([^ \n\r\t]*);"
[=[#if defined(__NEED_\2) && !defined(__DEFINED_\2)
typedef \1 \2;
#define __DEFINED_\2
#endif
]=]
    alltypes-h "${alltypes-h}")

    string(REGEX REPLACE "STRUCT ([^ \n\r\t]*) ([^\n\r]*);"
[=[#if defined(__NEED_struct_\1) && !defined(__DEFINED_struct_\1)
struct \1 \2;
#define __DEFINED_struct_\1
#endif
]=]
    alltypes-h "${alltypes-h}")

    string(REGEX REPLACE "UNION ([^ \n\r\t]*) ([^\n\r]*);"
[=[#if defined(__NEED_union_\1) && !defined(__DEFINED_union_\1)
union \1 \2;
#define __DEFINED_union_\1
#endif
]=]
    alltypes-h "${alltypes-h}")

    file(MAKE_DIRECTORY "${target-dir}/${arch}/bits")
    file(WRITE "${target-dir}/${arch}/bits/alltypes.h" "${alltypes-h}")
endfunction()

function(_musl_make_bits_syscall_h arch target-dir)
    file(READ "../../arch/${arch}/bits/syscall.h.in" arch-syscall-h-in)
    string(REGEX REPLACE "__NR_" "SYS_" arch-syscall-h "${arch-syscall-h-in}")
    file(MAKE_DIRECTORY "${target-dir}/${arch}/bits")
    file(WRITE "${target-dir}/${arch}/bits/syscall.h" "${arch-syscall-h}")
endfunction()

function(_musl_make_version_h arch target-dir)
    file(MAKE_DIRECTORY "${target-dir}/${arch}")
    file(WRITE "${target-dir}/${arch}/version.h" [=[#define VERSION "0.0"]=])
endfunction()

set(_musl-architectures
    "aarch64" "arm" "i386" "m68k" "microblaze" "mips" "mips64" "mipsn32" "or1k"
    "powerpc" "powerpc64" "riscv64" "s390x" "sh" "x32" "x86_64")

function(_musl_generate_arch_headers target-dir)
    foreach(arch ${_musl-architectures})
        _musl_make_bits_alltypes_h("${arch}" "${target-dir}")
        _musl_make_bits_syscall_h("${arch}" "${target-dir}")
        _musl_make_version_h("${arch}" "${target-dir}")
    endforeach()
endfunction()

if(NOT DEFINED generate-headers-target-dir)
    message(FATAL_ERROR "Missing -Dgenerate-headers-target-dir.")
endif()

_musl_generate_arch_headers("${generate-headers-target-dir}")
