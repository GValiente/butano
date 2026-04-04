@===============================================================================
@
@ Optimized memcpy for ARM7TDMI (GBA)
@
@ Provides:
@   __aeabi_memcpy     - copy with arbitrary alignment
@   __aeabi_memcpy4    - copy with 4-byte aligned pointers
@   __aeabi_memcpy8    - copy with 8-byte aligned pointers
@
@ C standard memcpy is in memcpy.c (separate TU for LTO inlining).
@
@ Handles alignment promotion: if src and dst share the same alignment,
@ copies are promoted to halfword or word width. Uses ldmia/stmia for
@ bulk 32-byte transfers.
@
@ Runs in IWRAM (ARM, 32-bit bus, 1-cycle access) for maximum throughput.
@
@===============================================================================
.syntax unified
    .arm
    .align 2
    .section .iwram._stdgba_memcpy, "ax", %progbits
@ =============================================================================
@ Generic entry: arbitrary alignment
@
@ Input:  r0 = dst, r1 = src, r2 = byte count
@ Output: void (dst in r0 is clobbered)
@ Clobbers: r0, r1, r2, r3
@ =============================================================================
    .global __aeabi_memcpy
    .type __aeabi_memcpy, %function
__aeabi_memcpy:
    @ For very small copies (<=3 bytes) byte-by-byte is faster than
    @ alignment logic. At n=4 the alignment check + word path saves
    @ ~15 cycles over byte loop when pointers are word-aligned, which
    @ is the common case for GCC's implicit struct-copy memcpy calls.
    cmp     r2, #3
    ble     .Lcopy_bytes

    @ Test alignment compatibility of src and dst
    @ XOR low bits: bit 0 set -> byte-only, bit 1 set -> halfword-only
    eor     r3, r0, r1
    tst     r3, #1
    bne     .Lcopy_bytes            @ Unresolvable byte misalignment
    tst     r3, #2
    bne     .Lcopy_halves_entry     @ Can align to halfword but not word

    @ Both pointers share the same word alignment
    @ Fast path: if already word-aligned, skip alignment fixup entirely.
    @ tst(1)+beq(3)=4 vs the full joaobapt fixup's 8 cycles for aligned.
    tst     r0, #3
    beq     __aeabi_memcpy4

    @ Compute bytes needed to reach next word boundary: (-r0) & 3
    @ rsbs r3, r0, #4: low 2 bits of result = (-r0) & 3
    @ joaobapt trick: movs r3, r3, lsl #31 maps bit 0 -> N, bit 1 -> C
    rsbs    r3, r0, #4
    movs    r3, r3, lsl #31
    @ Copy 1 byte if bit 0 was set (mi)
    ldrbmi  r3, [r1], #1
    strbmi  r3, [r0], #1
    submi   r2, r2, #1
    @ Copy 2 bytes if bit 1 was set (cs)
    ldrhcs  r3, [r1], #2
    strhcs  r3, [r0], #2
    subcs   r2, r2, #2
    @ Fall through to word-aligned copy

@ =============================================================================
@ Word-aligned entry: pointers are 4-byte (or 8-byte) aligned
@
@ Input:  r0 = dst (word-aligned), r1 = src (word-aligned), r2 = byte count
@ Output: void
@ Clobbers: r0, r1, r2, r3, r12, r4-r9 (saved/restored for bulk path)
@
@ Uses {r3-r9, r12} for 8-register ldm/stm (ascending order: 3,4,5,6,7,8,
@ 9,12). r12 is caller-saved so only r4-r9 need push/pop -- saving 2 cycles
@ vs the old {r3-r10} which required pushing/popping r10 too.
@
@ Bulk loop: double-pumped (2x unroll) using subsge cascade trick.
@ When the first subtraction goes negative, the second 32-byte block is
@ automatically skipped via conditional execution. Saves ~6.5% cycles
@ on large copies (backgrounds, framebuffers).
@ =============================================================================
    .global __aeabi_memcpy8
    .type __aeabi_memcpy8, %function
__aeabi_memcpy8:
    .global __aeabi_memcpy4
    .type __aeabi_memcpy4, %function
__aeabi_memcpy4:
    cmp     r2, #32
    blt     .Lcopy_words

    push    {r4-r9}
    @ GE is set from cmp (r2 >= 32): first subsge will always execute
.Lbulk:
    @ First 32-byte block (always executes on entry)
    subsge  r2, r2, #32
    ldmiage r1!, {r3-r9, r12}
    stmiage r0!, {r3-r9, r12}
    @ Second 32-byte block (skipped if first went negative)
    subsge  r2, r2, #32
    ldmiage r1!, {r3-r9, r12}
    stmiage r0!, {r3-r9, r12}
    bgt     .Lbulk
    pop     {r4-r9}
    bxeq    lr              @ exact multiple of 64
    adds    r2, r2, #32
    bxeq    lr              @ exact multiple of 32

.Lcopy_words:
    @ Computed jump into unrolled word copies (div.s technique)
    @ Avoids 10-cycle/word branch loop; costs 5 + 6*n_words instead
    @
    @ r2 = remaining bytes. n_words = r2 >> 2 (0-7).
    @ No bics/beq/sub guard: the computed jump naturally skips all 7 pairs
    @ when r2 < 4 (0 words), and the joaobapt tail only reads r2's low 2
    @ bits which are unchanged by the word copies. Saves 2 cycles vs the
    @ old bics+beq+sub approach. Same technique as memset word path.
    bic     r3, r2, #3          @ r3 = n_bytes_words = r2 & ~3
    rsb     r3, r3, #28         @ r3 = (7 - n_words) * 4
    add     pc, pc, r3, lsl #1  @ pc += skip_pairs * 8
    mov     r0, r0              @ ARM7 pipeline nop
    ldr     r3, [r1], #4
    str     r3, [r0], #4
    ldr     r3, [r1], #4
    str     r3, [r0], #4
    ldr     r3, [r1], #4
    str     r3, [r0], #4
    ldr     r3, [r1], #4
    str     r3, [r0], #4
    ldr     r3, [r1], #4
    str     r3, [r0], #4
    ldr     r3, [r1], #4
    str     r3, [r0], #4
    ldr     r3, [r1], #4
    str     r3, [r0], #4
    @ Fall through to tail

.Lcopy_tail:
    @ Copy remaining 0-3 bytes using joaobapt bit test
    @ Low 2 bits of r2 encode the tail: bit 1 -> halfword (cs), bit 0 -> byte (mi)
    movs    r3, r2, lsl #31
    ldrhcs  r3, [r1], #2
    strhcs  r3, [r0], #2
    ldrbmi  r3, [r1]
    strbmi  r3, [r0]
    bx      lr

@ Halfword copy: src and dst are halfword-compatible but not word-compatible
.Lcopy_halves_entry:
    @ Align to halfword boundary if needed
    tst     r0, #1
    ldrbne  r3, [r1], #1
    strbne  r3, [r0], #1
    subne   r2, r2, #1
.Lcopy_halves:
    subs    r2, r2, #2
    ldrhge  r3, [r1], #2
    strhge  r3, [r0], #2
    bgt     .Lcopy_halves
    bxeq    lr
    @ One trailing byte
    adds    r2, r2, #2
    ldrbne  r3, [r1]
    strbne  r3, [r0]
    bx      lr

@ Byte copy: unresolvable alignment or small transfer
.Lcopy_bytes:
    subs    r2, r2, #1
    ldrbge  r3, [r1], #1
    strbge  r3, [r0], #1
    bgt     .Lcopy_bytes
    bx      lr
    .size __aeabi_memcpy, . - __aeabi_memcpy

    .section .iwram.memcpy, "ax", %progbits
    .global memcpy
    .type memcpy, %function
memcpy:
    push    {r0, lr}
    bl      __aeabi_memcpy
    pop     {r0, lr}
    bx      lr
