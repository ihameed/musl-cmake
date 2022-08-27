function(_musl_cmake__glob_files dstvar group arch)
    file(GLOB asm-files "../../src/${group}/${arch}/*.s" "../../src/${group}/${arch}/*.S")
    file(GLOB raw-c-files "../../src/${group}/*.c")
    file(GLOB h-files "../../src/${group}/*.h")
    foreach(asm-file ${asm-files})
        get_filename_component(asm-filename "${asm-file}" NAME_WLE)
        set("asm-filename-${asm-filename}" "1")
    endforeach()
    foreach(c-file ${raw-c-files})
        get_filename_component(c-filename "${c-file}" NAME_WLE)
        if(NOT "${asm-filename-${c-filename}}")
            list(APPEND c-files "${c-file}")
        endif()
    endforeach()
    set("${dstvar}" ${asm-files} ${c-files} ${h-files} PARENT_SCOPE)
endfunction()

set(musl_cmake__src-groups
    "aio"
    "complex"
    "conf"
    "crypt"
    "ctype"
    "dirent"
    "env"
    "errno"
    "exit"
    "fcntl"
    "fenv"
    "include"
    "internal"
    "ipc"
    "ldso"
    "legacy"
    "linux"
    "locale"
    "malloc"
    "math"
    "misc"
    "mman"
    "mq"
    "multibyte"
    "network"
    "passwd"
    "prng"
    "process"
    "regex"
    "sched"
    "search"
    "select"
    "setjmp"
    "signal"
    "stat"
    "stdio"
    "stdlib"
    "string"
    "temp"
    "termios"
    "thread"
    "time"
    "unistd")

function(musl_cmake__generate_musl_file_lists name-prefix arch)
    foreach(group ${musl_cmake__src-groups})
        _musl_cmake__glob_files("files-${group}" "${group}" "${arch}")
    endforeach()

    file(GLOB files-crt "../../crt/${arch}/*.s" "../../crt/Scrt1.c")

    file(GLOB files-no-libc-internals-glibc-compat
        "../src/glibc-compat/src/*.cpp"
        "../src/glibc-compat/src/*.hpp"
        "../src/glibc-compat/src/*.h")

    set(depends-on-libc-internals
        ## linux
        "setgroups.c"

        ## unistd
        "setegid.c" "setreuid.c" "setresgid.c" "setresuid.c" "setuid.c"
        "setxid.c" "setregid.c" "setgid.c" "seteuid.c"

        ## internal
        "libc.c"

        ## errno
        "__errno_location.c"

        # vdso.c requires access to the elf auxiliary vector, which cannot be
        # reliably or easily accessed outside of the main executable's entry
        # point code. vdso.c is only used in musl to support sched_getcpu and
        # clock_gettime; both of these have had reasonable-ish glibc
        # implementations for over a decade. the most robust workaround i can
        # think of right now is:
        #
        # 1. use dlopen(RTLD_DEFAULT, "getauxval") to use glibc's reference/copy
        #    of the auxiliary vector if glibc-getauxval is available (and note
        #    that glibc < 2.19 doesn't set errno to ENOENT if an entry cannot be
        #    found), and if that doesn't work, then
        #
        # 2. open /proc/self/auxv and copy that to a large-enough fixed size
        #    storage blob. the PTRACE_MODE_READ_FSCREDS check trivially passes
        #    for threads in the same thread group, but proc might not be mounted
        #    and some rando linux setups (like android) will restrict access to
        #    /proc/self, or
        #
        # 3. scan to the end of `environ` to find the auxiliary vector. this
        #    will fall apart completely if setenv has ever been called.
        "vdso.c"

        ## process
        "fork.c" # requires running "atfork" callbacks
        )
    foreach(file ${depends-on-libc-internals})
        set("depends-on-libc-internals--file-${file}" "1")
    endforeach()
    foreach(group ${musl_cmake__src-groups})
        list(APPEND "musl-no-libc-internals-groups" "no-libc-internals-${group}")
        foreach(path ${files-${group}})
            get_filename_component(filename "${path}" NAME)
            if(NOT "${depends-on-libc-internals--file-${filename}}")
                list(APPEND "files-no-libc-internals-${group}" ${path})
            endif()
        endforeach()
    endforeach()

    foreach(group ${musl_cmake__src-groups} ${musl-no-libc-internals-groups} "crt" "no-libc-internals-glibc-compat")
        set("${name-prefix}-${group}" ${files-${group}} PARENT_SCOPE)
    endforeach()
endfunction()

set(musl_cmake__compile-definitions
    "$<$<COMPILE_LANGUAGE:CXX,C>:_XOPEN_SOURCE=700>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:CRT>")

set(musl_cmake__compile-options
    "$<$<COMPILE_LANGUAGE:C>:-std=c99>"
    "$<$<COMPILE_LANGUAGE:CXX>:-std=c++11>"
    "-Wa,--noexecstack"
    "-fomit-frame-pointer"
    "$<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>"
    "$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>"
    "$<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-fPIC>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-ffreestanding>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-nostdinc>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-fno-semantic-interposition>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-fstack-clash-protection>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-fvisibility=hidden>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-ffunction-sections>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-fdata-sections>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-fno-plt>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-fno-asynchronous-unwind-tables>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-fno-unwind-tables>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-Wno-shift-op-parentheses>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-Wno-string-plus-int>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-Wno-ignored-attributes>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-Wno-parentheses>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-Wno-return-local-addr>"
    "$<$<COMPILE_LANGUAGE:CXX,C>:-Wno-unknown-warning-option>"
    "-Wno-unused-command-line-argument")


function(musl_cmake__detect_architecture dst)
    set(cputype "${CMAKE_SYSTEM_PROCESSOR}")
    string(TOLOWER "${cputype}" cputype)
    set(known-archs "x86_64" "i386" "aarch64")
    set("aliases-x86_64" "x86_64" "x86-64" "amd64" "x64")
    set("aliases-i386" "i386" "i586" "i686" "x86")
    set("aliases-aarch64" "aarch64" "arm64")
    foreach(known-arch ${known-archs})
        foreach(alias ${aliases-${known-arch}})
            set("alias-for-${alias}" "${known-arch}")
        endforeach()
    endforeach()
    set(arch "${alias-for-${cputype}}")
    message(STATUS "musl_cmake: CMAKE_SYSTEM_PROCESSOR is \"${CMAKE_SYSTEM_PROCESSOR}\".")
    message(STATUS "musl_cmake: setting ${dst} to \"${arch}\".")
    set("${dst}" "${arch}" PARENT_SCOPE)
endfunction()

function(musl_cmake__linux_elf_program_interpreter_path dst arch)
    set("PT_INTERP-x86_64" "/lib64/ld-linux-x86-64.so.2")
    set("PT_INTERP-aarch64" "/lib/ld-linux-aarch64.so.1") # https://sourceware.org/bugzilla/show_bug.cgi?id=25129
    set("PT_INTERP" "${PT_INTERP-${arch}}")
    message(STATUS "musl_cmake: setting ${dst} to \"${PT_INTERP}\" for arch \"${arch}\"")
    set("${dst}" "${PT_INTERP}" PARENT_SCOPE)
endfunction()
