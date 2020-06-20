@--------------------------------------------------------------------------------
@ lmul.s
@--------------------------------------------------------------------------------
@ Provides an implementation of long multiplication
@--------------------------------------------------------------------------------

@ r0:r1 and r2:r3 are factors
@ after it, r0:r1 has the product
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global __aeabi_lmul
    .type __aeabi_lmul STT_FUNC
__aeabi_lmul:
    mul     r3, r0, r3          @ r3 = Al*Bh
    mla     r3, r1, r2, r3      @ r3 = Al*Bh + Ah*Bl
    umull   r0, r1, r2, r0      @ r0:r1 = Al*Bl
    add     r1, r1, r3          @ r0:r1 = Al*Bl + (Al*Bh + Ah*Bl) << 32
    bx      lr                  @ return
