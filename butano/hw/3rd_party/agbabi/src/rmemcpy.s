/*
===============================================================================

 Support:
    __agbabi_rmemcpy, __agbabi_rmemcpy2, __agbabi_rmemcpy4

 Copyright (C) 2021-2022 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

    .arm
    .align 2

    .section .iwram.__agbabi_rmemcpy, "ax", %progbits
    .global __agbabi_rmemcpy
__agbabi_rmemcpy:
    // Adjust pointers
    add     r0, r0, r2
    add     r1, r1, r2

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

    // Copy byte and half tail
    movs    r3, r0, lsl #31
    ldrmib  r3, [r1, #-1]!
    strmib  r3, [r0, #-1]!
    submi   r2, r2, #1
    ldrcsh  r3, [r1, #-2]!
    strcsh  r3, [r0, #-2]!
    subcs   r2, r2, #2

    b       .LskipAdjust4

    .global __agbabi_rmemcpy4
__agbabi_rmemcpy4:
    // Adjust pointers
    add     r0, r0, r2
    add     r1, r1, r2

.LskipAdjust4:
    // Copy 8 words
    movs    r12, r2, lsr #5
    beq     .Lskip32
    lsl     r3, r12, #5
    sub     r2, r2, r3
    push    {r4-r10}
.LcopyWords8:
    ldmdb   r1!, {r3-r10}
    stmdb   r0!, {r3-r10}
    subs    r12, r12, #1
    bne     .LcopyWords8
    pop     {r4-r10}
.Lskip32:

    // Copy words
    movs    r12, r2, lsr #2
.LcopyWords:
    subs    r12, r12, #1
    ldrhs   r3, [r1, #-4]!
    strhs   r3, [r0, #-4]!
    bhs     .LcopyWords

    // Copy half and byte head
    // JoaoBapt carry & sign bit test
    movs    r3, r2, lsl #31
    ldrcsh  r3, [r1, #-2]!
    strcsh  r3, [r0, #-2]!
    ldrmib  r3, [r1, #-1]
    strmib  r3, [r0, #-1]
    bx      lr

.Lcopy2:
    // Copy byte tail
    tst     r0, #1
    cmpne   r2, #0
    ldrneb  r3, [r1, #-1]!
    strneb  r3, [r0, #-1]!
    subne   r2, r2, #1

    b       .LskipAdjust2

    .global __agbabi_rmemcpy2
__agbabi_rmemcpy2:
    // Adjust pointers
    add     r0, r0, r2
    add     r1, r1, r2

.LskipAdjust2:
    // Copy halves
    movs    r12, r2, lsr #1
.LcopyHalves:
    subs    r12, r12, #1
    ldrhsh  r3, [r1, #-2]!
    strhsh  r3, [r0, #-2]!
    bhs     .LcopyHalves

    // Copy byte head
    tst     r2, #1
    ldrneb  r3, [r1, #-1]
    strneb  r3, [r0, #-1]
    bx      lr

.Lcopy1:
    subs    r2, r2, #1
    ldrhsb  r3, [r1, #-1]!
    strhsb  r3, [r0, #-1]!
    bhs     .Lcopy1
    bx      lr
