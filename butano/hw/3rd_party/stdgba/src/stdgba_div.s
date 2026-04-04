@===============================================================================
@
@ Fast 32-bit integer division for ARM7TDMI (GBA)
@
@ Provides:
@   __aeabi_uidiv      - unsigned 32/32 -> quotient
@   __aeabi_uidivmod   - unsigned 32/32 -> quotient + remainder
@   __aeabi_idiv       - signed 32/32 -> quotient
@   __aeabi_idivmod    - signed 32/32 -> quotient + remainder
@
@ Algorithm: Non-restoring division with computed jump into unrolled loop.
@ Based on the "Ultimate Division" technique described by Thomas Shermer
@ (chiark.greenend.org.uk/~theom/riscos/docs/ultimate/a252div.txt).
@ Adapted from agbabi (zlib license, github.com/felixjones/agbabi).
@
@ Runs in IWRAM (ARM, 32-bit bus, 1-cycle access) for maximum throughput.
@
@ Layout: signed entry is placed first so the common both-positive fast
@ path falls through to the shared unsigned body without a branch (saves 2
@ cycles vs bpl). The unsigned entry branches forward with bne (3 cycles),
@ which is 1 cycle faster than the old beq+b trampoline (4 cycles).
@
@===============================================================================
.syntax unified
    .arm
    .align 2
    .section .iwram._stdgba_div, "ax", %progbits
@ =============================================================================
@ Signed division: __aeabi_idivmod / __aeabi_idiv
@
@ Input:  r0 = numerator, r1 = denominator
@ Output: r0 = quotient, r1 = remainder
@ Clobbers: r2, r3, r12
@ =============================================================================
    .global __aeabi_idivmod
    .type __aeabi_idivmod, %function
__aeabi_idivmod:
    .global __aeabi_idiv
    .type __aeabi_idiv, %function
__aeabi_idiv:
    cmp     r1, #0
    beq     __aeabi_idiv0
    @ Fast path: both operands non-negative -> fall through to unsigned body
    @ orrs sets N if either r0 or r1 has bit 31 set (negative)
    orrs    r12, r0, r1
    bmi     .Lsigned_slow       @ 1 cycle (not taken) for common case
@ =============================================================================
@ Shared unsigned division body
@
@ Input:  r0 = numerator, r1 = denominator (known non-zero)
@ Output: r0 = quotient, r1 = remainder
@ Clobbers: r2, r3
@
@ The n<d check uses branchless conditional moves (3 cycles regardless of
@ outcome). When n >= d, the movlo instructions execute as 1-cycle NOPs,
@ and the bxlo is not taken (1 cycle), flowing into the main algorithm.
@ =============================================================================
.Lunsigned_checked:
    @ Quick out: numerator < denominator
    cmp     r0, r1
    blo     .Luidiv_num_lt_denom
.Lunsigned_body:
    @ Negate denominator early for binary search (cmn) and loop (adcs)
    rsb     r2, r1, #0          @ r2 = -denominator
    mov     r3, #28             @ initial guess for bit difference
    lsr     r1, r0, #4          @ r1 = numerator >> 4
    @ Binary search for leading-bit difference (3 iterations, 4-bit precision)
    @ cmn r2, X computes (-denom + X): N=0 (pl) iff X >= denom
    cmn     r2, r1, lsr #12
    subpl   r3, r3, #16
    lsrpl   r1, r1, #16
    cmn     r2, r1, lsr #4
    subpl   r3, r3, #8
    lsrpl   r1, r1, #8
    cmn     r2, r1
    subpl   r3, r3, #4
    lsrpl   r1, r1, #4
    @ Align numerator left (r2 already = -denominator)
    lsl     r0, r0, r3
    adds    r0, r0, r0          @ shift MSB into carry for first iteration
    @ Computed jump: skip (32 - iterations) x 12 bytes
    add     r3, r3, r3, lsl #1  @ r3 *= 3
    add     pc, pc, r3, lsl #2  @ pc += r3 * 12
    mov     r0, r0              @ required: ARM7 pipeline nop
    @ 32 unrolled iterations of non-restoring division
    .rept 32
    adcs    r1, r2, r1, lsl #1  @ remainder = remainder*2 + carry - denom
    sublo   r1, r1, r2          @ if borrow: restore (remainder += denom)
    adcs    r0, r0, r0          @ quotient <<= 1, shift in ~borrow
    .endr
    bx      lr
.Luidiv_num_lt_denom:
    @ numerator < denominator: quotient = 0, remainder = numerator
    mov     r1, r0
    mov     r0, #0
    bx      lr
@ Signed slow path: at least one operand negative
.Lsigned_slow:
    @ Save lr in r12, pack sign flags into upper bits
    @   bit 28 (CPSR V position) = numerator was negative
    @   bit 31 (CPSR N position) = denominator was negative
    mov     r12, lr
    cmp     r0, #0
    rsblt   r0, r0, #0          @ r0 = abs(numerator)
    orrlt   r12, #1 << 28
    cmp     r1, #0
    rsblt   r1, r1, #0          @ r1 = abs(denominator)
    orrlt   r12, #1 << 31
    @ Unsigned division of absolute values
    bl      .Lunsigned_checked
    @ Move packed sign bits into CPSR flags
    msr     cpsr_f, r12
    @ Negate quotient if exactly one operand was negative (N != V -> lt)
    rsblt   r0, r0, #0
    @ Negate remainder if numerator was negative (V=1 -> vs)
    rsbvs   r1, r1, #0
    @ Clear sign bits from saved lr and return
    bic     r12, #0xF << 28
    bx      r12
    .size __aeabi_idivmod, . - __aeabi_idivmod
@ =============================================================================
@ Unsigned division: __aeabi_uidivmod / __aeabi_uidiv
@
@ Falls through to the same shared body -- no trampoline branch.
@ The signed entry checks idiv0 + sign, this entry checks uidiv0 and
@ falls through directly.
@ =============================================================================
    .global __aeabi_uidivmod
    .type __aeabi_uidivmod, %function
__aeabi_uidivmod:
    .global __aeabi_uidiv
    .type __aeabi_uidiv, %function
__aeabi_uidiv:
    cmp     r1, #0
    bne     .Lunsigned_checked
    b       __aeabi_idiv0
    .size __aeabi_uidivmod, . - __aeabi_uidivmod
