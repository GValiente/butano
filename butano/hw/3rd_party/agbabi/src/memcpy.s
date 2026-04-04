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
    .arm
    .align 2

    .section .iwram.__agbabi_memcpy2, "ax", %progbits
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
