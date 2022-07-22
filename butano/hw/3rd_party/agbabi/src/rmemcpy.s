/*
===============================================================================

 Support:
    __agbabi_rmemcpy, __agbabi_rmemcpy1

 Copyright (C) 2021-2022 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

#include "macros.inc"

    .arm
    .align 2

    .section .iwram.__agbabi_rmemcpy, "ax", %progbits
    .global __agbabi_rmemcpy
__agbabi_rmemcpy:
    // >6-bytes is roughly the threshold when byte-by-byte copy is slower
    cmp     r2, #6
    ble     __agbabi_rmemcpy1

    align_switch r0, r1, r3, __agbabi_rmemcpy1, .Lcopy_halves

    // Check if end needs word aligning
    add     r3, r0, r2
    joaobapt_test r3

    // Copy byte tail to align
    submi   r2, r2, #1
    ldrmib  r3, [r1, r2]
    strmib  r3, [r0, r2]
    // r2 is now half aligned

    // Copy half tail to align
    subcs   r2, r2, #2
    ldrcsh  r3, [r1, r2]
    strcsh  r3, [r0, r2]
    // r2 is now word aligned

    cmp     r2, #32
    blt     .Lcopy_words

    // Word aligned, 32-byte copy
    push    {r0-r1, r4-r10, r12} // r12 for alignment
    add     r0, r0, r2
    add     r1, r1, r2
.Lloop_32:
    subs    r2, r2, #32
    ldmgedb r1!, {r3-r10}
    stmgedb r0!, {r3-r10}
    bgt     .Lloop_32
    pop     {r0-r1, r4-r10, r12} // r12 for alignment
    bxeq    lr

    // < 32 bytes remaining to be copied
    add     r2, r2, #32

.Lcopy_words:
    subs    r2, r2, #4
    ldrge   r3, [r1, r2]
    strge   r3, [r0, r2]
    bgt     .Lcopy_words
    bxeq    lr

    // Copy byte & half head
    joaobapt_test_into r3, r2
    // Copy half
    addcs   r2, r2, #2
    ldrcsh  r3, [r1, r2]
    strcsh  r3, [r0, r2]
    // Copy byte
    ldrmib  r3, [r1]
    strmib  r3, [r0]
    bx      lr

.Lcopy_halves:
    // Copy byte tail to align
    add     r3, r0, r2
    tst     r3, #1
    subne   r2, r2, #1
    ldrneb  r3, [r1, r2]
    strneb  r3, [r0, r2]
    // r2 is now half aligned

.Lloop_2:
    subs    r2, r2, #2
    ldrgeh  r3, [r1, r2]
    strgeh  r3, [r0, r2]
    bgt     .Lloop_2
    bxeq    lr

    // Copy byte head
    ldrb    r3, [r1]
    strb    r3, [r0]
    bx      lr

    .global __agbabi_rmemcpy1
__agbabi_rmemcpy1:
    subs    r2, r2, #1
    ldrgeb  r3, [r1, r2]
    strgeb  r3, [r0, r2]
    bgt     __agbabi_rmemcpy1
    bx      lr
