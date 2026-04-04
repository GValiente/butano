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
