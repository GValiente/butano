@===============================================================================
@
@ ABI:
@    __aeabi_memclr, __aeabi_memclr4, __aeabi_memclr8,
@    __aeabi_memset, __aeabi_memset4, __aeabi_memset8
@ Standard:
@    memset
@ Support:
@    __agbabi_wordset4, __agbabi_lwordset4, __agbabi_memset1
@
@ Copyright (C) 2021-2023 agbabi contributors
@ For conditions of distribution and use, see copyright notice in LICENSE.md
@
@===============================================================================

.syntax unified
#include "macros.inc"

    .arm
    .align 2

    .section .iwram.__aeabi_memclr, "ax", %progbits
    .global __aeabi_memclr
    .type __aeabi_memclr, %function
__aeabi_memclr:
    mov     r2, #0
    b       __aeabi_memset

    .global __aeabi_memclr8
    .type __aeabi_memclr8, %function
__aeabi_memclr8:
    .global __aeabi_memclr4
    .type __aeabi_memclr4, %function
__aeabi_memclr4:
    mov     r2, #0
    b       __agbabi_wordset4

    .section .iwram.__aeabi_memset, "ax", %progbits
    .global __aeabi_memset
    .type __aeabi_memset, %function
__aeabi_memset:
    @ < 8 bytes probably won't be aligned: go byte-by-byte
    cmp     r1, #8
    blt     __agbabi_memset1

    @ Copy head to align to next word
    rsb     r3, r0, #4
    joaobapt_test r3
    strbmi  r2, [r0], #1
    submi   r1, r1, #1
    strbcs  r2, [r0], #1
    strbcs  r2, [r0], #1
    subcs   r1, r1, #2

    .global __aeabi_memset8
    .type __aeabi_memset8, %function
__aeabi_memset8:
    .global __aeabi_memset4
    .type __aeabi_memset4, %function
__aeabi_memset4:
    lsl     r2, r2, #24
    orr     r2, r2, r2, lsr #8
    orr     r2, r2, r2, lsr #16

    .global __agbabi_wordset4
    .type __agbabi_wordset4, %function
__agbabi_wordset4:
    mov     r3, r2

    .global __agbabi_lwordset4
    .type __agbabi_lwordset4, %function
__agbabi_lwordset4:
    @ 16 words is roughly the threshold when lwordset is slower
    cmp     r1, #64
    blt     .Lset_2_words

    @ 8 word set
    push    {r4-r9}
    mov     r4, r2
    mov     r5, r3
    mov     r6, r2
    mov     r7, r3
    mov     r8, r2
    mov     r9, r3

.Lset_8_words:
    subs    r1, r1, #32
    stmiage r0!, {r2-r9}
    bgt     .Lset_8_words
    pop     {r4-r9}
    bxeq    lr

    @ Fixup remaining
    add     r1, r1, #32
.Lset_2_words:
    subs    r1, r1, #8
    stmiage r0!, {r2-r3}
    bgt     .Lset_2_words
    bxeq    lr

    @ Test for remaining word
    adds    r1, r1, #4
    strge   r2, [r0], #4
    bxeq    lr

    @ Set tail
    joaobapt_test r1
    strhcs  r2, [r0], #2
    strbmi  r2, [r0], #1
    bx      lr

    .section .iwram.__agbabi_memset1, "ax", %progbits
    .global __agbabi_memset1
    .type __agbabi_memset1, %function
__agbabi_memset1:
    subs    r1, r1, #1
    strbge  r2, [r0], #1
    bgt     __agbabi_memset1
    bx      lr

    .section .iwram.memset, "ax", %progbits
    .global memset
    .type memset, %function
memset:
    mov     r3, r1
    mov     r1, r2
    mov     r2, r3
    push    {r0, lr}
    bl      __aeabi_memset
    pop     {r0, lr}
    bx      lr
