
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global swi_LZ77UnCompWrite8bit
    .type swi_LZ77UnCompWrite8bit STT_FUNC
swi_LZ77UnCompWrite8bit:
    stmfd sp!, {r3 - r6}

    @ Read header word:
    @ bit0-3:  reserved
    @ bit4-7:  compressed type (1 for LZ77)
    @ bit8-31: size of compressed data
    ldr r2, [r0], #4
    lsrs r2, r2, #8
    @ ignore zero-length decompression requests
    beq .lz77_8bit_done

.lz77_8bit_loop:
    @ read encoder byte, shift to MSB for easier access.
    ldrb r3, [r0], #1
    orr r3, #0x01000000
.lz77_8bit_encoder_loop:
    tst r3, #0x80
    bne .lz77_8bit_copy_window
.lz77_8bit_copy_byte:
    @ copy byte from current source to current destination
    ldrb r4, [r0], #1
    strb r4, [r1], #1

    @ check if decompressed length has been reached.
    subs r2, #1
    beq .lz77_8bit_done

    @ read next encoder or process next block
    lsls r3, #1
    bcc .lz77_8bit_encoder_loop
    b .lz77_8bit_loop
.lz77_8bit_copy_window:
    @ read window tuple {displacement, size}
    ldrb r4, [r0], #1
    ldrb r5, [r0], #1

    @ r5 = window displacement
    orr r5, r5, r4, lsl #8
    bic r5, #0xF000
    add r5, #1

    @ r4 = window size
    lsr r4, #4
    add r4, #3
.lz77_8bit_copy_window_loop:
    @ copy byte from window to current destination
    ldrb r6, [r1, -r5]
    strb r6, [r1], #1

    @ check if decompressed length has been reached
    subs r2, #1
    beq .lz77_8bit_done

    @ check if window has been fully copied
    subs r4, #1
    bne .lz77_8bit_copy_window_loop

    @ read next encoder or process next block
    lsls r3, #1
    bcc .lz77_8bit_encoder_loop
    b .lz77_8bit_loop

.lz77_8bit_done:
    ldmfd sp!, {r3 - r6}
    bx lr


    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global swi_LZ77UnCompWrite16bit
    .type swi_LZ77UnCompWrite16bit STT_FUNC
swi_LZ77UnCompWrite16bit:
    stmfd sp!, {r3 - r7}

    @ Read header word:
    @ bit0-3:  reserved
    @ bit4-7:  compressed type (1 for LZ77)
    @ bit8-31: size of compressed data
    ldr r2, [r0], #4
    lsrs r2, r2, #8
    @ ignore zero-length decompression requests
    beq .lz77_16bit_done

.lz77_16bit_loop:
    @ read encoder byte, shift to MSB for easier access.
    ldrb r3, [r0], #1
    orr r3, #0x01000000
.lz77_16bit_encoder_loop:
    tst r3, #0x80
    bne .lz77_16bit_copy_window
.lz77_16bit_copy_byte:
    @ copy byte from current source to current destination
    ldrb r4, [r0], #1
    tst r1, #1
    moveq r7, r4
    orrne r7, r4, lsl #8
    strneh r7, [r1, #-1]
    add r1, #1

    @ check if decompressed length has been reached.
    subs r2, #1
    beq .lz77_16bit_done

    @ read next encoder or process next block
    lsls r3, #1
    bcc .lz77_16bit_encoder_loop
    b .lz77_16bit_loop
.lz77_16bit_copy_window:
    @ read window tuple {displacement, size}
    ldrb r4, [r0], #1
    ldrb r5, [r0], #1

    @ r5 = window displacement
    orr r5, r5, r4, lsl #8
    bic r5, #0xF000
    add r5, #1

    @ r4 = window size
    lsr r4, #4
    add r4, #3
.lz77_16bit_copy_window_loop:
    @ copy byte from window to current destination
    ldrb r6, [r1, -r5]
    tst r1, #1
    moveq r7, r6
    orrne r7, r6, lsl #8
    strneh r7, [r1, #-1]
    add r1, #1

    @ check if decompressed length has been reached
    subs r2, #1
    beq .lz77_16bit_done

    @ check if window has been fully copied
    subs r4, #1
    bne .lz77_16bit_copy_window_loop

    @ read next encoder or process next block
    lsls r3, #1
    bcc .lz77_16bit_encoder_loop
    b .lz77_16bit_loop

.lz77_16bit_done:
    ldmfd sp!, {r3 - r7}
    bx lr
