@===============================================================================
@
@ 64-bit integer division for ARM7TDMI (GBA)
@
@ Provides:
@   __aeabi_uldivmod   - unsigned 64/64 -> quotient + remainder
@   __aeabi_ldivmod    - signed 64/64 -> quotient + remainder
@
@ Algorithm: Non-restoring division with computed jump into unrolled loop.
@ Adapted from agbabi (zlib license, github.com/felixjones/agbabi).
@ Original technique by JoaoBaptMG (github.com/JoaoBaptMG/gba-modern).
@
@ Runs in IWRAM (ARM, 32-bit bus, 1-cycle access) for maximum throughput.
@
@ Register optimisation: r12 (ip) is an AAPCS scratch register, so the
@ unsigned paths use it for preamble-only values that previously required
@ pushing/popping callee-saved registers. The signed wrapper pushes r12
@ once to protect its packed lr+flags before dispatching to unsigned code.
@
@ Layout: the 64/32 path (most common) is placed first so it is reached
@ by falling through from the entry check. The 64/64 path is placed after,
@ reached by a forward branch. This saves 3 cycles on the 64/32 entry
@ vs the old layout (which had a redundant cmp r3, #0 and a taken beq).
@
@ Signed layout: positive operands fall through, negative operands branch
@ to out-of-line negation code. Saves 7 cycles on the common both-positive
@ path (the two bge-taken branches + two orrlt instructions are eliminated).
@
@===============================================================================
.syntax unified
    .arm
    .align 2
    .section .iwram._stdgba_ldiv, "ax", %progbits
@ =============================================================================
@ Unsigned 64/64 division: __aeabi_uldivmod
@
@ Input:  r0:r1 = numerator (lo:hi), r2:r3 = denominator (lo:hi)
@ Output: r0:r1 = quotient (lo:hi), r2:r3 = remainder (lo:hi)
@ Clobbers: r12 (and r4 internally for 64/64, but saved/restored)
@
@ Entry dispatch: single cmp r3, #0 handles both div0-detection (when
@ combined with cmp r2) and the 64/32 vs 64/64 split.
@   - r3 != 0 -> denom is non-zero, take 64/64 path (bne, 3 cycles)
@   - r3 == 0 -> check r2 for div0, then fall through to 64/32 (4 cycles)
@ =============================================================================
    .global __aeabi_uldivmod
    .type __aeabi_uldivmod, %function
__aeabi_uldivmod:
    cmp     r3, #0
    bne     .Luldiv64_checked       @ denom_hi != 0 -> 64/64 (denom is non-zero)
    @ denom_hi == 0: check for div0
    cmp     r2, #0
    beq     __aeabi_ldiv0
@ =============================================================================
@ Unsigned 64/32 division (internal): denom_hi == 0, denom_lo != 0
@
@ r12 used for preamble-only values - no push/pop needed in any sub-path.
@ =============================================================================
.Luluidiv_checked:
    @ If numerator high word is also 0, bridge to 32-bit division
    cmp     r1, #0
    beq     .Lbridge_32
    @ If denom bit 31 is set, the non-restoring loop breaks because
    @ -denom wraps to a tiny positive value. Use the schoolbook path.
    cmp     r2, #0
    blt     .Luluidiv_schoolbook
    @ --- 64/32 non-restoring (denom < 2^31) ---
    @ r12 = shift counter (preamble-only, dead after computed jump)
    rsb     r3, r2, #0             @ r3 = -denom
    mov     r12, #28               @ initial guess for bit difference
    lsr     r2, r1, #4             @ r2 = num_hi >> 4
    @ Binary search for leading-bit difference (3 iterations)
    cmn     r3, r2, lsr #12
    subge   r12, r12, #16
    lsrge   r2, r2, #16
    cmn     r3, r2, lsr #4
    subge   r12, r12, #8
    lsrge   r2, r2, #8
    cmn     r3, r2
    subge   r12, r12, #4
    lsrge   r2, r2, #4
    @ Shift 64-bit numerator left by r12, then bump once
    lsl     r1, r1, r12
    rsb     r12, r12, #32
    orr     r1, r1, r0, lsr r12
    rsb     r12, r12, #32
    lsl     r0, r0, r12
    adds    r0, r0, r0             @ bump r0, carry feeds first iteration
    adcs    r1, r1, r1             @ bump r1 with carry
    @ Computed jump: 4 instructions per iteration = 16 bytes
    add     pc, pc, r12, lsl #4
    mov     r0, r0                 @ ARM7 pipeline nop
    @ 64 unrolled iterations of non-restoring 64/32 division
    .rept 64
    adcs    r2, r3, r2, lsl #1    @ remainder = remainder*2 + carry - denom
    subcc   r2, r2, r3            @ if borrow: restore (remainder += denom)
    adcs    r0, r0, r0            @ quotient_lo <<= 1, shift in ~borrow
    adcs    r1, r1, r1            @ quotient_hi <<= 1, shift in carry
    .endr
    mov     r3, #0                 @ remainder_hi = 0
    bx      lr
@ -----------------------------------------------------------------------------
@ Schoolbook 64/32 for large denominators (denom bit 31 set)
@
@ r12 = q_hi (0 or 1). No push/pop needed.
@ -----------------------------------------------------------------------------
.Luluidiv_schoolbook:
    @ r0 = num_lo, r1 = num_hi, r2 = denom (>= 0x80000000)
    mov     r3, r2                 @ r3 = denom
    mov     r12, #0                @ r12 = q_hi (assume 0)
    @ Step 1: q_hi = (num_hi >= denom) ? 1 : 0
    cmp     r1, r2
    subhs   r1, r1, r2             @ rem = num_hi - denom
    movhs   r12, #1                @ q_hi = 1
    @ Step 2: q_lo = (rem : num_lo) / denom
    mov     r2, r1                 @ r2 = remainder
    @ Bump: shift MSB of num_lo into carry for first iteration
    adds    r0, r0, r0
    @ 32 iterations of restoring division
    .rept 32
    adcs    r2, r2, r2             @ rem = rem*2 + carry. C = bit32 overflow
    cmpcc   r2, r3                 @ if no overflow: compare rem vs denom
    subcs   r2, r2, r3             @ subtract if rem >= denom
    adcs    r0, r0, r0             @ shift quotient left, insert C. C = next num bit
    .endr
    @ r0 = q_lo, r2 = final remainder
    mov     r1, r12                @ r1 = q_hi
    mov     r3, #0                 @ remainder_hi = 0
    bx      lr
@ -----------------------------------------------------------------------------
@ Bridge to 32-bit: save lr in r12 instead of push/pop.
@ __aeabi_uidivmod uses only r0-r3, never touches r12.
@ -----------------------------------------------------------------------------
.Lbridge_32:
    mov     r1, r2                 @ r1 = denom
    mov     r12, lr                @ save lr in r12
    bl      __aeabi_uidivmod       @ r0 = quotient, r1 = remainder
    mov     r2, r1                 @ r2 = remainder_lo
    mov     r1, #0                 @ quotient_hi = 0
    mov     r3, #0                 @ remainder_hi = 0
    bx      r12                    @ return via r12
@ =============================================================================
@ 64/64 unsigned: denom_hi != 0, so quotient fits in 32 bits
@
@ Placed after 64/32 paths: reached by forward bne from entry (3 cycles).
@ r12 replaces r5 for the shift counter (preamble-only, dead after computed
@ jump). Only r4 needs push/pop for the negated denom_lo that lives across
@ the entire loop.
@ =============================================================================
.Luldiv64_checked:
    @ Quick out: numerator < denominator
    cmp     r1, r3
    cmpeq   r0, r2
    blo     .Luldiv64_zero
    push    {r4}
    @ Negate 64-bit denominator into r4:r1 (lo:hi)
    rsbs    r4, r2, #0              @ r4 = -denom_lo (sets carry)
    mov     r2, r1                  @ r2 = num_hi (save before r1 overwrite)
    rsc     r1, r3, #0             @ r1 = -denom_hi (uses carry from rsbs)
    @ Binary search for leading-bit difference (4 iterations)
    @ r12 = shift counter (preamble-only, dead after computed jump)
    mov     r12, #30
    lsr     r3, r2, #2
    cmn     r1, r3, lsr #14
    subge   r12, r12, #16
    lsrge   r3, r3, #16
    cmn     r1, r3, lsr #6
    subge   r12, r12, #8
    lsrge   r3, r3, #8
    cmn     r1, r3, lsr #2
    subge   r12, r12, #4
    lsrge   r3, r3, #4
    cmn     r1, r3
    subge   r12, r12, #2
    lsrge   r3, r3, #2
    @ Shift numerator train left by r12, then bump once
    lsl     r2, r2, r12
    rsb     r12, r12, #32
    orr     r2, r2, r0, lsr r12
    rsb     r12, r12, #32
    lsl     r0, r0, r12
    adds    r0, r0, r0              @ shift MSB into carry for first iteration
    @ Computed jump: 9 instructions per iteration = 36 bytes
    add     r12, r12, r12, lsl #3  @ r12 *= 9
    add     pc, pc, r12, lsl #2   @ pc += r12 * 36
    mov     r0, r0                  @ ARM7 pipeline nop
    @ 32 unrolled iterations of non-restoring 64/64 division
    .rept 32
    adcs    r2, r2, r2              @ remainder_lo = remainder_lo*2 + carry
    adc     r3, r3, r3              @ remainder_hi = remainder_hi*2 + carry
    adds    r2, r2, r4              @ trial subtract: remainder += neg_denom
    adcs    r3, r3, r1
    bcs     1f                      @ carry set = no borrow, subtraction OK
    subs    r2, r2, r4              @ restore: undo subtraction
    sbc     r3, r3, r1
    adds    r0, r0, #0              @ clear carry (quotient bit = 0)
1:
    adcs    r0, r0, r0              @ shift carry into quotient
    .endr
    @ r0 = quotient (32-bit), r2:r3 = remainder
    pop     {r4}
    mov     r1, #0                  @ quotient_hi = 0
    bx      lr
.Luldiv64_zero:
    @ numerator < denominator: quotient = 0, remainder = numerator
    mov     r2, r0
    mov     r3, r1
    mov     r0, #0
    mov     r1, #0
    bx      lr
    .size __aeabi_uldivmod, . - __aeabi_uldivmod
@ =============================================================================
@ Signed 64/64 division: __aeabi_ldivmod
@
@ Input:  r0:r1 = numerator (lo:hi), r2:r3 = denominator (lo:hi)
@ Output: r0:r1 = quotient (lo:hi), r2:r3 = remainder (lo:hi)
@ Clobbers: r12
@
@ Sign handling: positive operands fall through (1 cycle per blt not-taken),
@ negative operands branch to out-of-line negation (3 cycle blt + 3 cycle b
@ back). Saves 6 cycles on the common both-positive path vs the old inline
@ orrlt/bge approach which cost 5 cycles per operand when positive (the
@ orrlt costs 1 even when not executed, plus bge costs 3 when taken).
@
@ Dispatch: the cmp r3, #0 before blt .Ldenom_neg sets flags that are
@ reused by the adreq/beq dispatch below (push does not modify flags).
@ This eliminates the redundant cmp r3, #0 from the old code, saving 1 more
@ cycle.
@ =============================================================================
    .global __aeabi_ldivmod
    .type __aeabi_ldivmod, %function
__aeabi_ldivmod:
    @ Division by zero check
    cmp     r3, #0
    cmpeq   r2, #0
    beq     __aeabi_ldiv0
    @ Save lr in r12, pack sign flags into upper bits
    @   bit 28 (CPSR V position) = numerator was negative
    @   bit 31 (CPSR N position) = denominator was negative
    mov     r12, lr
    @ Make numerator positive (positive falls through, negative branches out)
    cmp     r1, #0
    blt     .Lnum_neg               @ 1 cycle (not taken) for common case
.Lafter_num:
    @ Make denominator positive and set flags for dispatch
    @ Z flag from this cmp is reused by adreq/beq below (push preserves flags)
    cmp     r3, #0
    blt     .Ldenom_neg             @ 1 cycle (not taken) for common case
.Lafter_denom:
    @ Save packed lr+flags: unsigned paths clobber r12
    push    {r12}
    @ Dispatch to unsigned path (div0 already checked, Z flag still valid)
    adreq   lr, .Lldiv_fixup
    beq     .Luluidiv_checked       @ r3 == 0 -> 64/32 path
    bl      .Luldiv64_checked       @ r3 != 0 -> 64/64 path
.Lldiv_fixup:
    @ Restore packed lr+flags
    pop     {r12}
    @ Move packed sign bits into CPSR flags
    msr     cpsr_f, r12
    @ Negate quotient if exactly one operand was negative (N != V -> lt)
    bge     .Lquot_pos
    rsbs    r0, r0, #0
    rsc     r1, r1, #0
    msr     cpsr_f, r12         @ restore flags
.Lquot_pos:
    @ Negate remainder if numerator was negative (V=1 -> vs)
    bvc     .Lrem_pos
    rsbs    r2, r2, #0
    rsc     r3, r3, #0
.Lrem_pos:
    @ Clear sign bits from saved lr and return
    bic     r12, #0xF << 28
    bx      r12
@ Out-of-line negation for signed operands (cold paths)
.Lnum_neg:
    orr     r12, r12, #1 << 28
    rsbs    r0, r0, #0
    rsc     r1, r1, #0
    b       .Lafter_num
.Ldenom_neg:
    orr     r12, r12, #1 << 31
    rsbs    r2, r2, #0
    rsc     r3, r3, #0
    cmp     r3, #0              @ re-set flags for dispatch (r3 may be 0 after negation)
    b       .Lafter_denom
    .size __aeabi_ldivmod, . - __aeabi_ldivmod
