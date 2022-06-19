/*
===============================================================================

 ABI:
    __aeabi_memset, __aeabi_memset4, __aeabi_memset8,
    __aeabi_memclr, __aeabi_memclr4, __aeabi_memclr8
 Standard:
    memset
 Support:
    __agbabi_wordset4

 Copyright (C) 2021-2022 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

    .arm
    .align 2

    .section .iwram.__aeabi_memset, "ax", %progbits
    .global __aeabi_memclr
__aeabi_memclr:
    mov     r2, #0
    b       .LskipShifts

    .global __aeabi_memset
__aeabi_memset:
    mov     r2, r2, lsl #24
    orr     r2, r2, r2, lsr #8
    orr     r2, r2, r2, lsr #16
    // Fallthrough

.LskipShifts:
    // Handle <= 2 byte set byte-by-byte
    cmp     r1, #2
    bgt     .LskipShortHead
    movs    r1, r1, lsl #31
    // Set byte and half
    strmib  r2, [r0], #1
    strcsb  r2, [r0], #1
    strcsb  r2, [r0]
    bx      lr

.LskipShortHead:
    // JoaoBapt carry & sign bit test
    rsb     r3, r0, #4
    movs    r3, r3, lsl #31
    // Set half and byte head
    strmib  r2, [r0], #1
    submi   r1, r1, #1
    strcsh  r2, [r0], #2
    subcs   r1, r1, #2
    b       __agbabi_wordset4

    .global __aeabi_memclr8
__aeabi_memclr8:
    .global __aeabi_memclr4
__aeabi_memclr4:
    mov     r2, #0
    b       __agbabi_wordset4

    .global __aeabi_memset8
__aeabi_memset8:
    .global __aeabi_memset4
__aeabi_memset4:
    mov     r2, r2, lsl #24
    orr     r2, r2, r2, lsr #8
    orr     r2, r2, r2, lsr #16
    // Fallthrough

    .global __agbabi_wordset4
__agbabi_wordset4:
    // Set 8 words
    movs    r12, r1, lsr #5
    beq     .Lskip32
    lsl     r3, r12, #5
    sub     r1, r1, r3
    push    {r4-r9}
    mov     r3, r2
    mov     r4, r2
    mov     r5, r2
    mov     r6, r2
    mov     r7, r2
    mov     r8, r2
    mov     r9, r2
.LsetWords8:
    stmia   r0!, {r2-r9}
    subs    r12, r12, #1
    bne     .LsetWords8
    pop     {r4-r9}
.Lskip32:

    // Set words
    movs    r12, r1, lsr #2
.LsetWords:
    subs    r12, r12, #1
    strhs   r2, [r0], #4
    bhs     .LsetWords

    // Set half and byte tail
    movs    r3, r1, lsl #31
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
