@===============================================================================
@
@ Optimized memset for ARM7TDMI (GBA)
@
@ Provides:
@   __aeabi_memset     - set with arbitrary alignment
@   __aeabi_memset4    - set with 4-byte aligned pointer
@   __aeabi_memset8    - set with 8-byte aligned pointer
@   __aeabi_memclr     - clear with arbitrary alignment
@   __aeabi_memclr4    - clear with 4-byte aligned pointer
@   __aeabi_memclr8    - clear with 8-byte aligned pointer
@
@ C standard memset is in memset.cpp (separate TU for LTO inlining).
@
@ AEABI signature: __aeabi_memset(void *dest, size_t n, int c)
@ Note: parameter order differs from C memset (dest, c, n).
@ The byte value is in r2 (lowest 8 bits).
@
@ Handles alignment promotion: if dest is word-aligned (or can be aligned),
@ fills are promoted to word width. Uses stmia for bulk 32-byte stores.
@
@ Runs in IWRAM (ARM, 32-bit bus, 1-cycle access) for maximum throughput.
@
@===============================================================================
.syntax unified
    .arm
    .align 2
    .section .iwram._stdgba_memset, "ax", %progbits

@ =============================================================================
@ Clear entries: zero fill (c = 0)
@
@ Zero broadcast to all bytes is still zero, so memclr entries skip the
@ 5-cycle broadcast and jump directly to the fill path. This saves 2 cycles
@ on every memclr call (branch cost 3 vs broadcast cost 5).
@ =============================================================================
    .global __aeabi_memclr
    .type __aeabi_memclr, %function
__aeabi_memclr:
    mov     r2, #0
    b       .Lgeneric_broadcast_done    @ skip broadcast (strb/strh of 0 is fine)

    .global __aeabi_memclr8
    .type __aeabi_memclr8, %function
__aeabi_memclr8:
    .global __aeabi_memclr4
    .type __aeabi_memclr4, %function
__aeabi_memclr4:
    mov     r2, #0
    b       .Lfill_aligned              @ skip broadcast (0 is already valid word)

@ =============================================================================
@ Word-aligned entry: pointer is 4-byte (or 8-byte) aligned
@
@ Input:  r0 = dest (word-aligned), r1 = byte count, r2 = fill byte
@ Output: void
@ Clobbers: r0, r1, r2, r3, r12, r4-r8 (saved/restored for bulk path)
@
@ Broadcasts the byte to a 32-bit fill word, then uses stm for bulk fills.
@ Uses {r2-r8, r12} for 8-register stm -- only r4-r8 need push/pop (r2-r3
@ and r12 are caller-saved). This saves 2 cycles vs {r2-r9} which required
@ pushing/popping r9 too (same technique as memcpy.s).
@
@ Bulk loop: double-pumped (2x unroll) using subsge cascade trick
@ (same technique as memcpy).
@ =============================================================================
    .global __aeabi_memset8
    .type __aeabi_memset8, %function
__aeabi_memset8:
    .global __aeabi_memset4
    .type __aeabi_memset4, %function
__aeabi_memset4:
    @ Broadcast byte to word: 0xAB -> 0xABABABAB
    and     r2, r2, #0xFF
    orr     r2, r2, r2, lsl #8
    orr     r2, r2, r2, lsl #16

.Lfill_aligned:
    @ Internal entry point for generic path (r2 already broadcast to word)
    @ and for memclr4 (r2 = 0, which is a valid broadcast)
    cmp     r1, #32
    blt     .Lfill_words

    @ Spread fill word to all registers for stm
    mov     r3, r2
    push    {r4-r8}
    mov     r4, r2
    mov     r5, r2
    mov     r6, r2
    mov     r7, r2
    mov     r8, r2
    mov     r12, r2
    @ GE is set from cmp (r1 >= 32): first subsge will always execute
.Lbulk_fill:
    @ First 32-byte block (always executes on entry)
    subsge  r1, r1, #32
    stmiage r0!, {r2-r8, r12}
    @ Second 32-byte block (skipped if first went negative)
    subsge  r1, r1, #32
    stmiage r0!, {r2-r8, r12}
    bgt     .Lbulk_fill
    pop     {r4-r8}
    bxeq    lr              @ exact multiple of 64
    adds    r1, r1, #32
    bxeq    lr              @ exact multiple of 32

.Lfill_words:
    @ Computed jump into unrolled word stores (same technique as memcpy)
    @
    @ r1 = remaining bytes. n_words = r1 >> 2 (0-7).
    @ Each str = 1 instruction = 4 bytes.
    @ Skip (7 - n_words) stores = (28 - (r1 & ~3)) bytes
    @
    @ Simplified word path: no beq early-exit for 0 words, no sub to extract
    @ tail bytes. The computed jump naturally skips all 7 str for 0 words,
    @ and the tail code reads original r1 directly. Saves 2 cycles (beq+sub)
    @ on 4-28 byte fills at the cost of 2 cycles on 0-3 byte fills through
    @ this path (rare: only post-bulk remainders with non-word-multiple sizes).
    bic     r3, r1, #3          @ r3 = n_bytes_words = r1 & ~3
    rsb     r3, r3, #28         @ r3 = (7 - n_words) * 4
    add     pc, pc, r3          @ pc += skip stores * 4
    mov     r0, r0              @ ARM7 pipeline nop
    str     r2, [r0], #4
    str     r2, [r0], #4
    str     r2, [r0], #4
    str     r2, [r0], #4
    str     r2, [r0], #4
    str     r2, [r0], #4
    str     r2, [r0], #4
    @ Fall through to tail

.Lfill_tail:
    @ Fill remaining 0-3 bytes using joaobapt bit test
    @ Low 2 bits of r1 encode the tail: bit 1 -> halfword (cs), bit 0 -> byte (mi)
    movs    r3, r1, lsl #31
    strhcs  r2, [r0], #2
    strbmi  r2, [r0]
    bx      lr

@ =============================================================================
@ Generic entry: arbitrary alignment
@
@ Input:  r0 = dest, r1 = byte count, r2 = fill byte
@ Output: void (dest in r0 is clobbered)
@ Clobbers: r0, r1, r2, r3
@
@ Tests for small fills (<=3 bytes) BEFORE broadcasting -- the byte loop
@ only uses strb which ignores the upper bits of r2. This saves 5 cycles
@ on the <= 3 byte path (skips the broadcast entirely).
@
@ For n > 3: broadcasts r2 to a full word, aligns dest, then enters the
@ word-aligned fill path.
@ =============================================================================
    .global __aeabi_memset
    .type __aeabi_memset, %function
__aeabi_memset:
    @ Check for small fills BEFORE broadcast to save 5 cycles on n <= 3.
    @ strb only uses the low 8 bits of r2, so broadcast isn't needed.
    cmp     r1, #3
    ble     .Lfill_bytes

    @ Broadcast byte to word -- needed for strh in alignment fixup
    and     r2, r2, #0xFF
    orr     r2, r2, r2, lsl #8
    orr     r2, r2, r2, lsl #16

.Lgeneric_broadcast_done:
    @ Entry point for memclr (r2 = 0, already a valid broadcast)

    @ Fast path: if dest is already word-aligned, skip alignment fixup.
    @ tst(1)+beq(3)=4 vs rsbs(1)+movs(1)+4 NOPs(4)+b(3)=9. Saves 5 cycles.
    tst     r0, #3
    beq     .Lfill_aligned

    @ Align dest to word boundary: compute bytes needed = (-r0) & 3
    @ Same joaobapt trick as memcpy
    rsbs    r3, r0, #4
    movs    r3, r3, lsl #31
    @ Fill 1 byte if bit 0 was set (mi)
    strbmi  r2, [r0], #1
    submi   r1, r1, #1
    @ Fill 2 bytes if bit 1 was set (cs)
    strhcs  r2, [r0], #2
    subcs   r1, r1, #2
    @ Now word-aligned, r2 already broadcast: skip to aligned fill
    b       .Lfill_aligned

@ Byte fill: small transfer (n <= 3)
.Lfill_bytes:
    subs    r1, r1, #1
    strbge  r2, [r0], #1
    bgt     .Lfill_bytes
    bx      lr
    .size __aeabi_memset, . - __aeabi_memset

    .section .iwram.memset, "ax", %progbits
    .global memset
    .type memset, %function
memset:
    mov     r3, r1
    mov     r1, r2
    mov     r2, r3
    push    {r0, lr}
    bl      __aeabi_memset
    pop     {r0, lr}
    bx      lr
