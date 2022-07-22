/*
===============================================================================

 ABI:
    __aeabi_memset, __aeabi_memset4, __aeabi_memset8,
    __aeabi_memclr, __aeabi_memclr4, __aeabi_memclr8
 Standard:
    memset
 Support:
    __agbabi_wordset4, __agbabi_lwordset4

 Copyright (C) 2021-2022 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

#include "macros.inc"

    .arm
    .align 2

    .section .iwram.__aeabi_memclr, "ax", %progbits
    .global __aeabi_memclr
__aeabi_memclr:
    mov     r2, #0
    b       .Lwordset

    .global __aeabi_memclr8
__aeabi_memclr8:
    .global __aeabi_memclr4
__aeabi_memclr4:
    mov     r2, #0
    b       __agbabi_wordset4

    .section .iwram.__aeabi_memset, "ax", %progbits
    .global __aeabi_memset
__aeabi_memset:
    mov     r2, r2, lsl #24
    orr     r2, r2, r2, lsr #8
    orr     r2, r2, r2, lsr #16

.Lwordset:
    // Handle < 4 bytes in byte-by-byte tail
    cmp     r1, #4
    blt     .Lset_tail3

    // Check if address needs word aligning
    rsbs     r3, r0, #4
    joaobapt_test r3
    strmib  r2, [r0], #1
    submi   r1, r1, #1
    strcsh  r2, [r0], #2
    subcs   r1, r1, #2

    .global __agbabi_wordset4
__agbabi_wordset4:
    mov     r3, r2

    .global __agbabi_lwordset4
__agbabi_lwordset4:
    // >=64-bytes is roughly the threshold when 8-byte copy is slower
    cmp     r1, #64
    blt     .Lloop_8

    // Load up registers
    push    {r4-r9}
    mov     r4, r2
    mov     r5, r3
    mov     r6, r2
    mov     r7, r3
    mov     r8, r2
    mov     r9, r3

.Lloop_32:
    subs    r1, r1, #32
    stmgeia r0!, {r2-r9}
    bgt     .Lloop_32
    pop     {r4-r9}
    bxeq    lr
    add     r1, r1, #32

.Lloop_8:
    subs    r1, r1, #8
    stmgeia r0!, {r2-r3}
    bgt     .Lloop_8
    bxeq    lr

    // Copy word tail
    adds    r1, r1, #4
    strge   r2, [r0], #4
    bxeq    lr

.Lset_tail3:
    joaobapt_test r1
    strcsh  r2, [r0], #2
    strmib  r2, [r0]
    bx      lr

    .section .iwram.memset, "ax", %progbits
    .global memset
memset:
    mov     r3, r1
    mov     r1, r2
    mov     r2, r3
    push    {r0, lr}
    bl      __aeabi_memset
    pop     {r0, lr}
    bx      lr
