@--------------------------------------------------------------------------------
@ sqrt32.s
@--------------------------------------------------------------------------------
@ Provides an implementation of a square root algorithm for 32-bits
@--------------------------------------------------------------------------------

@ Source code taken from http://www.pertinentdetail.org/sqrt
@ The actual function
@ u32 isqrt32(u32 x) // so x is in r0
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global isqrt32
    .type isqrt32 STT_FUNC
isqrt32:
    @ Registers: r0 = root, r2 = x, r3 = offset
    mov     r2, r0             @ move x to r2 first
isqrt32direct:                 @ shortcut used by isqrt64
    mov     r3, #3 << 30       @ initialize offset
    mov     r0, #1 << 30       @ initialize the first root

    @ short-circuit to special cases where some cycles can be shaved off
    movs    r1, r2, lsr #8     @ if the last three bytes are 0
    beq     .sqrt8             @ move to the 8-bit chunk
    movs    r1, r2, lsr #16    @ if the last two bytes are 0
    beq     .sqrt16            @ move to the 16-bit chunk
    movs    r1, r2, lsr #24    @ if the last byte is 0
    beq     .sqrt24            @ move to the 24-bit chunk

    @ now, 16 times, we will do the "simulation" loop
    cmp     r2, r0
    subhs   r2, r2, r0
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #2
    subhs   r2, r2, r0, ror #2
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #4
    subhs   r2, r2, r0, ror #4
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #6
    subhs   r2, r2, r0, ror #6
    adc     r0, r3, r0, lsl #1

.sqrt24:
    cmp     r2, r0, ror #8
    subhs   r2, r2, r0, ror #8
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #10
    subhs   r2, r2, r0, ror #10
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #12
    subhs   r2, r2, r0, ror #12
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #14
    subhs   r2, r2, r0, ror #14
    adc     r0, r3, r0, lsl #1

.sqrt16:
    cmp     r2, r0, ror #16
    subhs   r2, r2, r0, ror #16
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #18
    subhs   r2, r2, r0, ror #18
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #20
    subhs   r2, r2, r0, ror #20
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #22
    subhs   r2, r2, r0, ror #22
    adc     r0, r3, r0, lsl #1

.sqrt8:
    cmp     r2, r0, ror #24
    subhs   r2, r2, r0, ror #24
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #26
    subhs   r2, r2, r0, ror #26
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #28
    subhs   r2, r2, r0, ror #28
    adc     r0, r3, r0, lsl #1

    cmp     r2, r0, ror #30
    subhs   r2, r2, r0, ror #30
    adc     r0, r3, r0, lsl #1

    bic     r0, r0, #3 << 30
    bx      lr
