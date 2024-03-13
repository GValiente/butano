@-------------------------------------------------------------
@ Original code by fleroviux, distributed under the MIT License:
@ Link: https://github.com/Cult-of-GBA/BIOS
@ Copyright 2020 - 2021 DenSinH fleroviux
@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
@-------------------------------------------------------------
@ Speed optimizations by Lorenzooone - 2024
@ 8bit version not included because the optimized 16bit version is faster.
@ Works fine even with 1 as the window offset (fixes LZ77 VRAM bug).
@-------------------------------------------------------------

 .arm
 .align
 .global swi_LZ77UnCompWrite16bit
 .global swi_LZ77UnCompWrite8bit
 .type swi_LZ77UnCompWrite16bit,function
 .type swi_LZ77UnCompWrite8bit,function
#ifdef __NDS__
 .section .itcm, "ax", %progbits
#else
 .section .iwram, "ax", %progbits
#endif

#define ALIGNMENT_FIX

swi_LZ77UnCompWrite16bit:
swi_LZ77UnCompWrite8bit:
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
    cmp r4, r2
    movgt r4, r2
    subs r2, r4
    subs r5, r1, r5
    @ About 50% of the time, the two addresses will be aligned. Abuse this...
    eor r6, r1, r5
    tst r6, #1
    bne .lz77_16bit_copy_window_not_aligned_check
    tst r1, #1
    beq .lz77_optimized_16bit_copy_window_loop
    ldrb r6, [r5], #1
    orr r7, r6, lsl #8
    #ifdef ALIGNMENT_FIX
    subs r1, #1
    strh r7, [r1], #2
    #else
    strh r7, [r1], #1
    #endif
    subs r4, #1
.lz77_optimized_16bit_copy_window_loop:
    ldrh r6, [r5], #2
    strh r6, [r1], #2
    subs r4, #2
    cmp r4, #1
    bgt .lz77_optimized_16bit_copy_window_loop
    bne .aligned_window_end_check
    @ copy byte from window to current destination
    ldrb r7, [r5]
    add r1, #1

.aligned_window_end_check:
    @ check if decompressed length has been reached
    cmp r2, #0
    beq .lz77_16bit_done

    @ read next encoder or process next block
    lsls r3, #1
    bcc .lz77_16bit_encoder_loop
    b .lz77_16bit_loop

.lz77_16bit_copy_window_not_aligned_check:
    subs r1, #1
    cmp r1, r5
    beq .set_previous_byte
    tst r5, #1
    beq .lz77_16bit_copy_window_not_aligned_loop
    ldrb r7, [r5], #1
    add r1, #1
    subs r4, #1
.lz77_16bit_copy_window_not_aligned_loop:
    ldrh r6, [r5], #2
    orr r7, r6, lsl #8
    strh r7, [r1], #2
    lsr r7, #16
    subs r4, #2
    cmp r4, #1
    bgt .lz77_16bit_copy_window_not_aligned_loop
    bne .window_end_check
    @ copy byte from window to current destination
    ldrb r6, [r5]
    orr r7, r6, lsl #8
    strh r7, [r1], #1

.window_end_check:
    add r1, #1
    @ check if decompressed length has been reached
    cmp r2, #0
    beq .lz77_16bit_done

    @ read next encoder or process next block
    lsls r3, #1
    bcc .lz77_16bit_encoder_loop
    b .lz77_16bit_loop

.set_previous_byte:
    tst r1, #1
    beq .got_byte_in_r7
    ldrb r7, [r1], #1
    movs r6, r7
    orr r7, r6, lsl #8
    b .set_loop
.got_byte_in_r7:
    movs r6, r7
    orr r7, r6, lsl #8
    strh r7, [r1], #2
    subs r4, #1
.set_loop:
    strh r7, [r1], #2
    subs r4, #2
    cmp r4, #1
    bgt .set_loop
    bne .set_previous_byte_end_check
    @ copy byte from window to current destination
    lsr r7, #8
    add r1, #1

.set_previous_byte_end_check:
    @ check if decompressed length has been reached
    cmp r2, #0
    beq .lz77_16bit_done

    @ read next encoder or process next block
    lsls r3, #1
    bcc .lz77_16bit_encoder_loop
    b .lz77_16bit_loop

.lz77_16bit_done:
    ldmfd sp!, {r3 - r7}
    bx lr
