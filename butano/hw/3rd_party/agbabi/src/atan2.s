/*
===============================================================================

 Support:
    __agbabi_atan2

 Taken from https://www.coranac.com/documents/arctangent/ by Jasper "cearn" Vijn
 Modified for libagbabi

===============================================================================
*/

#define BAM_BITS    ( 16 )

    .arm
    .align 2

    .section .iwram.__agbabi_atan2, "ax", %progbits
    .global __agbabi_atan2
__agbabi_atan2:
    subs    r3, r1, #0
    mov     r1, r0
    bne     .Latan2_octantify

    // Convert sign bit to pi
    lsr     r1, r0, #31
    lsl     r0, r1, #(BAM_BITS - 1)
    bx      lr

.Latan2_octantify:
    push    {r4, lr}

    // Fallthough flags from previous subs
    rsblt   r1, r0, #0 // x = -x
    rsblt   r3, r3, #0 // y = -y
    movlt   r2, #6
    movge   r2, #2

    movlt   r4, #4
    movge   r4, #0

    cmp     r1, #0
    rsble   r0, r1, #0 // t = -x
    movle   r1, r3 // x = y
    movle   r3, r0 // y = t

    movle   r4, r2

    cmp     r1, r3
    suble   r2, r3, r1 // t = y - x
    addle   r1, r1, r3 // x = x + y
    movle   r3, r2 // y = t

    addle   r4, r4, #1

    // t = y / x
    lsl     r0, r3, #15
    bl      __aeabi_idiv

    // t2 = ((-t) * t) >> 15
    rsb     r2, r0, #0
    mul     r2, r0, r2
    asr     r2, r2, #15

    add     r3, r2, r2, lsl #3
    rsb     r3, r2, r3, lsl #3
    asr     r3, r3, #0x000B
    add     r3, r3, #0x1000
    add     r3, r3, #0x0029

    mul     r3, r2, r3
    asr     r3, r3, #0x000F
    add     r3, r3, #0x1F00
    add     r3, r3, #0x000B

    mul     r3, r2, r3
    asr     r3, r3, #0x000F
    add     r3, r3, #0x3640
    add     r3, r3, #0x000C

    mul     r3, r2, r3
    asr     r3, r3, #0x000F
    add     r3, r3, #0xA200
    add     r3, r3, #0x00FC

    mul     r3, r0, r3
    asr     r3, r3, #15

    // octant *= pi/4
    lsl     r1, r4, #(BAM_BITS - 3)

    // +0.5 rounds to nearest integer
    add     r3, r3, #(1 << (17 - BAM_BITS))

    add     r0, r1, r3, asr #(18 - BAM_BITS)

    // Clear sign bit
    bic     r0, r0, #(1 << BAM_BITS)

    pop     {r4, lr}
    bx      lr
