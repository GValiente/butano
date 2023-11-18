@===============================================================================
@
@ ABI:
@    __aeabi_lmul, __aeabi_llsl, __aeabi_llsr, __aeabi_lasr
@
@ Copyright (C) 2021-2023 agbabi contributors
@ For conditions of distribution and use, see copyright notice in LICENSE.md
@
@===============================================================================

.syntax unified

    .arm
    .align 2

    .section .iwram.__aeabi_lmul, "ax", %progbits
    .global __aeabi_lmul
    .type __aeabi_lmul, %function
__aeabi_lmul:
    mul     r3, r0, r3
    mla     r1, r2, r1, r3
    umull   r0, r3, r2, r0
    add     r1, r1, r3
    bx      lr

    .section .iwram.__aeabi_llsl, "ax", %progbits
    .global __aeabi_llsl
    .type __aeabi_llsl, %function
__aeabi_llsl:
    subs    r3, r2, #32
    rsb     r12, r2, #32
    lslmi   r1, r1, r2
    lslpl   r1, r0, r3
    orrmi   r1, r1, r0, lsr r12
    lsl     r0, r0, r2
    bx      lr

    .section .iwram.__aeabi_llsr, "ax", %progbits
    .global __aeabi_llsr
    .type __aeabi_llsr, %function
__aeabi_llsr:
    subs    r3, r2, #32
    rsb     r12, r2, #32
    lsrmi   r0, r0, r2
    lsrpl   r0, r1, r3
    orrmi   r0, r0, r1, lsl r12
    lsr     r1, r1, r2
    bx      lr

    .section .iwram.__aeabi_lasr, "ax", %progbits
    .global __aeabi_lasr
    .type __aeabi_lasr, %function
__aeabi_lasr:
    subs    r3, r2, #32
    rsb     r12, r2, #32
    lsrmi   r0, r0, r2
    asrpl   r0, r1, r3
    orrmi   r0, r0, r1, lsl r12
    asr     r1, r1, r2
    bx      lr
