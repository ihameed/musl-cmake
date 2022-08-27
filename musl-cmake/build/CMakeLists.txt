cmake_minimum_required(VERSION 3.15)
project("musl-cmake" LANGUAGES CXX ASM ASM_MASM C)

include("musl-cmake.cmake")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-nostdlib++" gnu-nostdlib-cpp)
check_cxx_compiler_flag("-fno-stack-protector" gnu-fno-stack-protector)

musl_cmake__detect_architecture("musl-arch")
musl_cmake__generate_musl_file_lists("musl-files" "${musl-arch}")
musl_cmake__linux_elf_program_interpreter_path("musl-program-interpreter" "${musl-arch}")

function(_def_lib_musl_full_static)
    set(target "lib-musl-full-static")
    add_library("${target}" STATIC)

    target_sources("${target}" PRIVATE ${musl-files-crt})
    foreach(group ${musl_cmake__src-groups})
        target_sources("${target}" PRIVATE ${musl-files-${group}})
    endforeach()
    target_include_directories("${target}" BEFORE PRIVATE
        "../src/generated-headers/${musl-arch}"
        "../../arch/${musl-arch}"
        "../../arch/generic"
        "../../src/include"
        "../../src/internal"
        "../../include"
        )
    target_compile_definitions("${target}" PRIVATE
        ${musl_cmake__compile-definitions})
    target_compile_options("${target}" PRIVATE
        ${musl_cmake__compile-options})
    set_source_files_properties("../../src/env/__libc_start_main.c" PROPERTIES COMPILE_FLAGS "-fno-stack-protector")
endfunction()
_def_lib_musl_full_static()

function(_def_lib_musl_partial_static)
    set(target "lib-musl-partial-static")
    add_library("${target}" STATIC)

    target_sources("${target}" PRIVATE
        #${musl-files-no-libc-internals-ctype} # locale??
        #${musl-files-no-libc-internals-multibyte} # locale???
        ${musl-files-no-libc-internals-errno}
        ${musl-files-no-libc-internals-fcntl}
        ${musl-files-no-libc-internals-internal}
        ${musl-files-no-libc-internals-math}
        ${musl-files-no-libc-internals-misc}
        ${musl-files-no-libc-internals-stat}
        ${musl-files-no-libc-internals-stdio}
        ${musl-files-no-libc-internals-string}
        ${musl-files-no-libc-internals-unistd}
        ${musl-files-no-libc-internals-glibc-compat}
        "../../src/locale/__lctrans.c" # musl's lctrans does not depend on musl internals.
    )
    target_include_directories("${target}" BEFORE PRIVATE
        "../src/glibc-compat/include"
        "../src/generated-headers/${musl-arch}"
        "../../arch/${musl-arch}"
        "../../arch/generic"
        "../../src/include"
        "../../src/internal"
        "../../include"
        )
    target_compile_definitions("${target}" PRIVATE
        ${musl_cmake__compile-definitions}
        "MUSL_cmake_glibc_compat=1")
    target_compile_options("${target}" PRIVATE
        ${musl_cmake__compile-options}
        )
    set_source_files_properties("../../src/env/__libc_start_main.c" PROPERTIES COMPILE_FLAGS "-fno-stack-protector")
endfunction()
_def_lib_musl_partial_static()

function(common_compile_options target)
    target_compile_options("${target}" PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
        $<$<COMPILE_LANGUAGE:C>:-std=c99>
        $<$<COMPILE_LANGUAGE:CXX,C>:-fPIE>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
        $<$<COMPILE_LANGUAGE:CXX,C>:-fno-semantic-interposition>
        $<$<COMPILE_LANGUAGE:CXX,C>:-fstack-clash-protection>
        $<$<COMPILE_LANGUAGE:CXX,C>:-fvisibility=hidden>
        $<$<COMPILE_LANGUAGE:CXX,C>:-ffunction-sections>
        $<$<COMPILE_LANGUAGE:CXX,C>:-fdata-sections>
        $<$<COMPILE_LANGUAGE:CXX,C>:-fno-plt>
        $<$<COMPILE_LANGUAGE:CXX,C>:-fno-asynchronous-unwind-tables>
        $<$<COMPILE_LANGUAGE:CXX,C>:-fno-unwind-tables>)
endfunction()

function(common_link_options target)
    target_link_options("${target}" PRIVATE
        "-static-libgcc"
        $<$<BOOL:${gnu-nostdlib-cpp}>:-nostdlib++>

        "-Wl,--gc-sections"

        # Only added DT_NEEDED dependencies for libraries that are
        # actually used by the output binary.
        "-Wl,--as-needed"

        # Mark pages modified by the dynamic loader during relocation as
        # read-only after the loader has finished relocation.
        "-Wl,-z,relro"

        # Disable lazy symbol resolution.
        "-Wl,-z,now"

        # In a .so, if a .so-local definition for a global symbol exists,
        # use it instead of using a call through the PLT or GOT or
        # whatever. This mirrors the behavior of linkers and DLLs on
        # Windows. Note that it's possible for multiple different .sos to
        # include definitions of symbols that are assumed to be
        # whole-program-global, like RTTI information, which is used in
        # the implementation of exceptions. But this doesn't matter to
        # me, and shouldn't matter for any well-designed project: the very
        # idea of a "stable C++ ABI" is silly, and letting exceptions or
        # whatever implicitly-generated uncontrollable C++ crap leak across
        # .so/DLL boundaries is a bad idea.
        "-Wl,-Bsymbolic"
        "-Wl,-Bsymbolic-functions"
        "-Wl,--no-undefined"
        "-Wl,--dynamic-linker,${musl-program-interpreter}"

        "-Wl,--build-id=0x0101010101010101")
endfunction()

function(external_debug_info target)
    add_custom_command(TARGET "${target}" POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --only-keep-debug $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.dbg
        COMMAND ${CMAKE_OBJCOPY} --strip-all $<TARGET_FILE:${target}>
        COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink $<TARGET_FILE:${target}>.dbg $<TARGET_FILE:${target}>)
    set_property(TARGET "${target}" APPEND PROPERTY ADDITIONAL_CLEAN_FILES $<TARGET_FILE:${target}>.dbg)
endfunction()

function(_def_test_full_static_link)
    set(target "test-full-static-link")
    add_executable("${target}")
    target_sources("${target}" PRIVATE "../src/test-full-static-link/test-full-static-link-main.cpp")
    target_link_libraries("${target}" PRIVATE "lib-musl-full-static")
    target_include_directories("${target}" SYSTEM BEFORE PRIVATE
        "../src/generated-headers/${musl-arch}"
        "../../arch/${musl-arch}"
        "../../arch/generic"
        "../../include")
    target_compile_options("${target}" PRIVATE
        $<$<COMPILE_LANGUAGE:CXX,C>:-nostdinc>
        $<$<COMPILE_LANGUAGE:CXX,C>:-ffreestanding>)
    common_compile_options("${target}")
    target_link_options("${target}" PRIVATE
        "-e_start"
        "-nostartfiles"
        "-nolibc")
    common_link_options("${target}")
    external_debug_info("${target}")
endfunction()
_def_test_full_static_link()

function(_def_test_static_musl_dynamic_glibc)
    set(target "test-static-musl-dynamic-glibc")
    add_executable("${target}")
    target_sources("${target}" PRIVATE
        "../src/test-static-musl-dynamic-glibc/test-static-musl-dynamic-glibc-main.cpp"
        "../src/glibc-compat/src/start.S"
        )
    target_link_libraries("${target}" PRIVATE "lib-musl-partial-static")
    target_include_directories("${target}" SYSTEM BEFORE PRIVATE
        "../src/glibc-compat/include"
        "../src/generated-headers/${musl-arch}"
        "../../arch/${musl-arch}"
        "../../arch/generic"
        "../../include")
    common_compile_options("${target}")
    target_link_options("${target}" PRIVATE
        "-e__glibc_compat_start"
        "-pthread"
        "-ldl")
    common_link_options("${target}")
    external_debug_info("${target}")
endfunction()
_def_test_static_musl_dynamic_glibc()
