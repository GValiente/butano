/*
===============================================================================

 ABI:
    __aeabi_memcpy, __aeabi_memcpy4, __aeabi_memcpy8
 Standard:
    memcpy
 Support:
    __agbabi_memcpy2

 Copyright (C) 2021-2022 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

    .arm
    .align 2

    .section .iwram.__aeabi_memcpy, "ax", %progbits
    .global __aeabi_memcpy
__aeabi_memcpy:
    // Check pointer alignment
    eor     r3, r1, r0
    // JoaoBapt carry & sign bit test
    movs    r3, r3, lsl #31
    bmi     .Lcopy1
    bcs     .Lcopy2

.Lcopy4:
    // Handle <= 2 byte copies byte-by-byte
    cmp     r2, #2
    ble     .Lcopy1

    // Copy half and byte head
    rsb     r3, r0, #4
    movs    r3, r3, lsl #31
    ldrmib  r3, [r1], #1
    strmib  r3, [r0], #1
    submi   r2, r2, #1
    ldrcsh  r3, [r1], #2
    strcsh  r3, [r0], #2
    subcs   r2, r2, #2
    // Fallthrough

    .global __aeabi_memcpy8
__aeabi_memcpy8:
    .global __aeabi_memcpy4
__aeabi_memcpy4:
    // Copy 8 words
    movs    r12, r2, lsr #5
    beq     .Lskip32
    lsl     r3, r12, #5
    sub     r2, r2, r3
    push    {r4-r10}
.LcopyWords8:
    ldmia   r1!, {r3-r10}
    stmia   r0!, {r3-r10}
    subs    r12, r12, #1
    bne     .LcopyWords8
    pop     {r4-r10}
.Lskip32:

    // Copy words
    movs    r12, r2, lsr #2
.LcopyWords:
    subs    r12, r12, #1
    ldrhs   r3, [r1], #4
    strhs   r3, [r0], #4
    bhs     .LcopyWords

    // Copy half and byte tail
    movs    r3, r2, lsl #31
    ldrcsh  r3, [r1], #2
    strcsh  r3, [r0], #2
    ldrmib  r3, [r1]
    strmib  r3, [r0]
    bx      lr

.Lcopy2:
    // Copy byte head
    tst     r0, #1
    ldrneb  r3, [r1], #1
    strneb  r3, [r0], #1
    subne   r2, r2, #1
    // Fallthrough

    .global __agbabi_memcpy2
__agbabi_memcpy2:
    // Copy halves
    movs    r12, r2, lsr #1
.LcopyHalves:
    subs    r12, r12, #1
    ldrhsh  r3, [r1], #2
    strhsh  r3, [r0], #2
    bhs     .LcopyHalves

    // Copy byte tail
    tst     r2, #1
    ldrneb  r3, [r1]
    strneb  r3, [r0]
    bx      lr

.Lcopy1:
    subs    r2, r2, #1
    ldrhsb  r3, [r1], #1
    strhsb  r3, [r0], #1
    bhs     .Lcopy1
    bx      lr

    .section .iwram.memcpy, "ax", %progbits
    .global memcpy
memcpy:
    push    {r0, lr}
    bl      __aeabi_memcpy
    pop     {r0, lr}
    bx      lr
