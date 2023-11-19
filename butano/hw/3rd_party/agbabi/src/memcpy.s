@===============================================================================
@
@ ABI:
@    __aeabi_memcpy, __aeabi_memcpy4, __aeabi_memcpy8
@ Standard:
@    memcpy
@ Support:
@    __agbabi_memcpy2, __agbabi_memcpy1
@
@ Copyright (C) 2021-2023 agbabi contributors
@ For conditions of distribution and use, see copyright notice in LICENSE.md
@
@===============================================================================

.syntax unified
#include "macros.inc"

    .arm
    .align 2

    .section .iwram.__aeabi_memcpy, "ax", %progbits
    .global __aeabi_memcpy
    .type __aeabi_memcpy, %function
__aeabi_memcpy:
    @ >6-bytes is roughly the threshold when byte-by-byte copy is slower
    cmp     r2, #6
    ble     __agbabi_memcpy1

    align_switch r0, r1, r3, __agbabi_memcpy1, .Lcopy_halves

    @ Check if r0 (or r1) needs word aligning
    rsbs    r3, r0, #4
    joaobapt_test r3

    @ Copy byte head to align
    ldrbmi  r3, [r1], #1
    strbmi  r3, [r0], #1
    submi   r2, r2, #1
    @ r0, r1 are now half aligned

    @ Copy half head to align
    ldrhcs  r3, [r1], #2
    strhcs  r3, [r0], #2
    subcs   r2, r2, #2
    @ r0, r1 are now word aligned

    .global __aeabi_memcpy8
    .type __aeabi_memcpy8, %function
__aeabi_memcpy8:
    .global __aeabi_memcpy4
    .type __aeabi_memcpy4, %function
__aeabi_memcpy4:
    cmp     r2, #32
    blt     .Lcopy_words

    @ Word aligned, 32-byte copy
    push    {r4-r10}
.Lloop_32:
    subs    r2, r2, #32
    ldmiage r1!, {r3-r10}
    stmiage r0!, {r3-r10}
    bgt     .Lloop_32
    pop     {r4-r10}
    bxeq    lr

    @ < 32 bytes remaining to be copied
    add     r2, r2, #32

.Lcopy_words:
    cmp     r2, #4
    blt     .Lcopy_halves
.Lloop_4:
    subs    r2, r2, #4
    ldrge   r3, [r1], #4
    strge   r3, [r0], #4
    bgt     .Lloop_4
    bxeq    lr

    @ Copy byte & half tail
    @ This test still works when r2 is negative
    joaobapt_test r2
    @ Copy half
    ldrhcs  r3, [r1], #2
    strhcs  r3, [r0], #2
    @ Copy byte
    ldrbmi  r3, [r1]
    strbmi  r3, [r0]
    bx      lr

.Lcopy_halves:
    @ Copy byte head to align
    tst     r0, #1
    ldrbne  r3, [r1], #1
    strbne  r3, [r0], #1
    subne   r2, r2, #1
    @ r0, r1 are now half aligned

    .global __agbabi_memcpy2
    .type __agbabi_memcpy2, %function
__agbabi_memcpy2:
    subs    r2, r2, #2
    ldrhge  r3, [r1], #2
    strhge  r3, [r0], #2
    bgt     __agbabi_memcpy2
    bxeq    lr

    @ Copy byte tail
    adds    r2, r2, #2
    ldrbne  r3, [r1]
    strbne  r3, [r0]
    bx      lr

    .section .iwram.__agbabi_memcpy1, "ax", %progbits
    .global __agbabi_memcpy1
    .type __agbabi_memcpy1, %function
__agbabi_memcpy1:
    subs    r2, r2, #1
    ldrbge  r3, [r1], #1
    strbge  r3, [r0], #1
    bgt     __agbabi_memcpy1
    bx      lr

    .section .iwram.memcpy, "ax", %progbits
    .global memcpy
    .type memcpy, %function
memcpy:
    push    {r0, lr}
    bl      __aeabi_memcpy
    pop     {r0, lr}
    bx      lr
