@===============================================================================
@
@ 64-bit multiply and shift operations for ARM7TDMI (GBA)
@
@ Provides:
@   __aeabi_lmul   - 64-bit multiplication (truncated to 64 bits)
@   __aeabi_llsl   - 64-bit logical shift left
@   __aeabi_llsr   - 64-bit logical shift right
@   __aeabi_lasr   - 64-bit arithmetic shift right
@
@ These are required by the ARM EABI for 64-bit integer support. GCC emits
@ calls to these when operating on long long values.
@
@ Algorithm notes:
@   lmul uses 3 multiplies + 1 add, which is the algebraic minimum for
@   computing the low 64 bits of a 64x64 product.
@
@   The shift operations use the canonical ARM conditional-execution idiom
@   to handle both shift < 32 and shift >= 32 without branching.
@
@ Adapted from agbabi (zlib license, github.com/felixjones/agbabi).
@ Runs in IWRAM (ARM, 32-bit bus, 1-cycle access) for maximum throughput.
@
@===============================================================================
.syntax unified
    .arm
    .align 2
    .section .iwram._stdgba_lmul, "ax", %progbits

@ =============================================================================
@ 64-bit multiplication: __aeabi_lmul
@
@ Computes (r0:r1) = (r0:r1) * (r2:r3), truncated to 64 bits.
@
@ The full 128-bit product of a:b (where a = a_lo + a_hi*2^32) and
@ b = b_lo + b_hi*2^32 is:
@   a_lo*b_lo + (a_hi*b_lo + a_lo*b_hi)*2^32 + a_hi*b_hi*2^64
@ We only need the low 64 bits, so a_hi*b_hi is dropped entirely.
@
@ Input:  r0 = a_lo, r1 = a_hi, r2 = b_lo, r3 = b_hi
@ Output: r0 = result_lo, r1 = result_hi
@ Clobbers: r2, r3, r12
@ =============================================================================
    .global __aeabi_lmul
    .type __aeabi_lmul, %function
__aeabi_lmul:
    mul     r3, r0, r3          @ r3 = a_lo * b_hi (low 32 bits)
    mla     r1, r2, r1, r3      @ r1 = b_lo * a_hi + (a_lo * b_hi)
    umull   r0, r12, r2, r0     @ r0:r12 = a_lo * b_lo (full 64-bit)
    add     r1, r1, r12         @ r1 = cross terms + carry from low product
    bx      lr
    .size __aeabi_lmul, . - __aeabi_lmul

@ =============================================================================
@ 64-bit logical shift left: __aeabi_llsl
@
@ Computes (r0:r1) = (r0:r1) << r2
@
@ Two cases handled without branching via conditional execution:
@   shift < 32: hi = (hi << n) | (lo >> (32-n)), lo = lo << n
@   shift >= 32: hi = lo << (n-32), lo = 0
@
@ Input:  r0 = value_lo, r1 = value_hi, r2 = shift count
@ Output: r0 = result_lo, r1 = result_hi
@ Clobbers: r3, r12
@ =============================================================================
    .global __aeabi_llsl
    .type __aeabi_llsl, %function
__aeabi_llsl:
    subs    r3, r2, #32         @ r3 = n - 32 (sets N if n < 32)
    rsb     r12, r2, #32        @ r12 = 32 - n
    lslmi   r1, r1, r2         @ n < 32: hi <<= n
    lslpl   r1, r0, r3         @ n >= 32: hi = lo << (n-32)
    orrmi   r1, r1, r0, lsr r12 @ n < 32: hi |= lo >> (32-n)
    lsl     r0, r0, r2         @ lo <<= n (zeroes out if n >= 32)
    bx      lr
    .size __aeabi_llsl, . - __aeabi_llsl

@ =============================================================================
@ 64-bit logical shift right: __aeabi_llsr
@
@ Computes (r0:r1) = (unsigned)(r0:r1) >> r2
@
@ Two cases handled without branching via conditional execution:
@   shift < 32: lo = (lo >> n) | (hi << (32-n)), hi = hi >> n
@   shift >= 32: lo = hi >> (n-32), hi = 0
@
@ Input:  r0 = value_lo, r1 = value_hi, r2 = shift count
@ Output: r0 = result_lo, r1 = result_hi
@ Clobbers: r3, r12
@ =============================================================================
    .global __aeabi_llsr
    .type __aeabi_llsr, %function
__aeabi_llsr:
    subs    r3, r2, #32         @ r3 = n - 32 (sets N if n < 32)
    rsb     r12, r2, #32        @ r12 = 32 - n
    lsrmi   r0, r0, r2         @ n < 32: lo >>= n
    lsrpl   r0, r1, r3         @ n >= 32: lo = hi >> (n-32)
    orrmi   r0, r0, r1, lsl r12 @ n < 32: lo |= hi << (32-n)
    lsr     r1, r1, r2         @ hi >>= n (zeroes out if n >= 32)
    bx      lr
    .size __aeabi_llsr, . - __aeabi_llsr

@ =============================================================================
@ 64-bit arithmetic shift right: __aeabi_lasr
@
@ Computes (r0:r1) = (signed)(r0:r1) >> r2
@
@ Same as llsr but uses asr for the high word to preserve the sign bit.
@
@ Input:  r0 = value_lo, r1 = value_hi, r2 = shift count
@ Output: r0 = result_lo, r1 = result_hi
@ Clobbers: r3, r12
@ =============================================================================
    .global __aeabi_lasr
    .type __aeabi_lasr, %function
__aeabi_lasr:
    subs    r3, r2, #32         @ r3 = n - 32 (sets N if n < 32)
    rsb     r12, r2, #32        @ r12 = 32 - n
    lsrmi   r0, r0, r2         @ n < 32: lo >>= n (logical)
    asrpl   r0, r1, r3         @ n >= 32: lo = (signed)hi >> (n-32)
    orrmi   r0, r0, r1, lsl r12 @ n < 32: lo |= hi << (32-n)
    asr     r1, r1, r2         @ hi >>= n (arithmetic, preserves sign)
    bx      lr
    .size __aeabi_lasr, . - __aeabi_lasr
