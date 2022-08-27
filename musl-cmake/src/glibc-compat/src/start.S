/*
The contents of this file are derived from the glibc sources. The source
files in question have the following license:

   Copyright (C) 1995-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file with other
   programs, and to distribute those programs without any restriction
   coming from the use of this file. (The GNU Lesser General Public
   License restrictions do apply in other respects; for example, they
   cover modification of the file, and distribution when not linked
   into another program.)

   Note that people who make modified versions of this file are not
   obligated to grant this special exception for their modified
   versions; it is their choice whether to do so. The GNU Lesser
   General Public License gives permission to release a modified
   version without this exception; this exception also makes it
   possible to release a modified version which carries forward this
   exception.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.

The aforementioned special exception is granted in this modified version.
*/

#define MUSL_support_main_in_dll 0

#if defined(__amd64__)
/* ==========================================================================
 * amd64 (glibc/sysdeps/x86_64/start.S) (
 * ========================================================================== */
.symver __libc_start_main, __libc_start_main@GLIBC_2.2.5
.hidden __glibc_compat_start
.globl __glibc_compat_start
.type __glibc_compat_start,@function
.align 1<<4
__glibc_compat_start:
    // __libc_start_main(fptr main, int argc, char *argv, fptr init, fptr fini, fptr stack_end);
    // main : rdi
    // argc : rsi
    // argv : rdx
    // init : rcx
    // fini : r8
    // rtld_fini : r9
    // stack_end : push to stack
    .cfi_startproc
    .cfi_undefined rip
    xorl %ebp, %ebp // clear frame pointer to tag this as an entry frame with no predecessor
    mov %rdx, %r12 // rtld_fini; preserve in a c-call-preserved register
#ifdef __ILP32__
    mov (%rsp), %r13d // argc; preserve in a c-call-preserved register
    add $4, %esp // realign stack
#else
    popq %r13 // argc; preserve in a c-call-preserved register
#endif
    mov %rsp, %r14 // argv; preserve in a c-call-preserved register
    and $~15, %rsp // align stack to 16 bytes

    pushq %rax // push 8 bytes of arbitrary data to maintain 16-byte alignment after pushing stack_end
    pushq %rsp // stack_end

#if !MUSL_support_main_in_dll
    // This won't work if main, __libc_csu_init, and __libc_csu_fini are
    // defined in a shared object. Use rip-relative offsets instead of absolute
    // addresses to maintain position-independence.
    leaq main(%rip), %rdi // main
    leaq __libc_csu_init(%rip), %rcx // init
    leaq __libc_csu_fini(%rip), %r8 // fini
#else
    mov main@GOTPCREL(%rip), %rdi // main
    mov __libc_csu_init@GOTPCREL(%rip), %rcx // init
    mov __libc_csu_fini@GOTPCREL(%rip), %r8 // fini
#endif
    mov %r13, %rsi // argc
    mov %r14, %rdx // argv
    mov %r12, %r9 // rtld_fini

    // I don't intend to ever support static linking with musl-glibc, because
    // static binaries would be better made with vanilla musl or without
    // any libc cruft at all.
    //
    // The dependency on the global offset table (GOT) is benign; it'll always
    // be present in any reasonable musl-glibc configuration.
    //
    // I also don't use lazy binding, so use an indirect jump to the global
    // offset slot instead of calling through the procedure linkage table
    // (PLT), whose entire purpose is to lazily fill in GOT slots.
    call *__libc_start_main@GOTPCREL(%rip)

    hlt
    .cfi_endproc
.size __glibc_compat_start,.-__glibc_compat_start
/* ==========================================================================
 * )
 * ========================================================================== */
#elif defined(__aarch64__)
/* ==========================================================================
 * aarch64 (glibc/sysdeps/aarch64/start.S) (
 * ========================================================================== */
.symver __libc_start_main, __libc_start_main@GLIBC_2.17
.hidden __glibc_compat_start
.globl __glibc_compat_start
.type __glibc_compat_start,%function
.p2align 6
__glibc_compat_start:
    // __libc_start_main(fptr main, int argc, char *argv, fptr init, fptr fini, fptr stack_end);
    // main : x0
    // argc : x1
    // argv : x2
    // init : x3
    // fini : x4
    // rtld_fini : x5
    // stack_end : x6
    .cfi_startproc;
    .cfi_undefined x30
    mov x29, #0 // clear frame pointer register
    mov x30, #0 // clear link register

    mov x21, x0 // rtld_fini; preserve in a c-call-preserved register
    mov x22, sp // stack_end; preserve in a c-call-preserved register

    ldr x19, [sp, #0] // argc; preserve in a c-call-preserved register
    add x20, sp, #(1<<3) // argv; preserve in a c-call-preserved register

#if !MUSL_support_main_in_dll
    // This won't work if main, __libc_csu_init, and __libc_csu_fini are
    // defined in a shared object.
    adr x5, __main_offset
    ldr x1, __main_offset
    add x0, x5, x1

    ldr x1, __libc_csu_init_offset
    add x3, x5, x1

    ldr x1, __libc_csu_fini_offset
    add x4, x5, x1
#else
    adrp x0, :got:main             // main
    ldr  x0, [x0, #:got_lo12:main] // main

    adrp x3, :got:__libc_csu_init             // init
    ldr  x3, [x0, #:got_lo12:__libc_csu_init] // init

    adrp x4, :got:__libc_csu_fini             // fini
    ldr  x4, [x0, #:got_lo12:__libc_csu_fini] // fini
#endif
    mov x1, x19 // argc
    mov x2, x20 // argv
    mov x5, x21 // rtld_fini
    mov x6, x22 // stack_end

    bl __libc_start_main

    brk #0x3e8
#if !MUSL_support_main_in_dll
    // These are here as a low-effort way to get relative offsets located
    // within 1MiB of the ldr instructions doing pc-relative loads above, with
    // a relative base address also within 1MiB of the pc-relative adr
    // instruction above.
.balign 8
__main_offset:
    .dword main - __main_offset
.balign 8
__libc_csu_init_offset:
    .dword __libc_csu_init - __main_offset
.balign 8
__libc_csu_fini_offset:
    .dword __libc_csu_fini - __main_offset
#endif
    .cfi_endproc
.size __glibc_compat_start,.-__glibc_compat_start
/* ==========================================================================
 * )
 * ========================================================================== */
#endif
