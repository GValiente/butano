@
@ Copyright (c) 2021 João Baptista de Paula e Silva jbaptistapsilva@yahoo.com.br
@ zlib License, see LICENSE file.
@

#include "../../include/bn_config_hbes.h"

#define SIZEOF_ENTRY 8

@ bn::hw::hblank_effects::_intr()
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global _ZN2bn2hw14hblank_effects5_intrEv
    .type _ZN2bn2hw14hblank_effects5_intrEv STT_FUNC
_ZN2bn2hw14hblank_effects5_intrEv:  @ That's the mangled name of bn::hw::hblank_effects::_intr()
    mov     r0, #0x4000000          @ build REG_VCOUNT
    ldrh    r0, [r0, #6]            @ load REG_VCOUNT into r0

    @ this implement if (vcount < 159) vcount++; else if (vcount > 226) vcount = 0; else return;
    cmp     r0, #226                @ if it's > 226, set it to -1
    movhi   r0, #-1                 @ now vcount = -1 or original vcount
    add     r0, r0, #1              @ add 1 to it (r0 = vcount+1 or 0)
    cmp     r0, #160                @ if vcount + 1 >= 160
    bxhs    lr                      @ bail out
    mov     r0, r0, lsl #1          @ now r0 = vcount*2

    @ bn::hw::hblank_effects::data, which is below
    ldr     r3, _ZN2bn2hw14hblank_effects4dataE             @ load data (entries*)
    ldr     r2, [r3], #4                                    @ load the 16-bit entries count
    add     r3, r3, r2, lsl #3                              @ offset the register
    adr     r1, .dataTransferEnd16                          @ pick up the label address
    add     r2, r2, r2, lsl #1                              @ multiply the count by 3
    sub     pc, r1, r2, lsl #2                              @ and set pc = label - count*12

    @ generate the instruction copying
    .rept   BN_CFG_HBES_MAX_ITEMS
    ldmdb   r3!, {r1, r2}           @ r1 = src (const uint16_t*), r2 = dest (volatile uint16_t*)
    ldrh    r1, [r1, r0]            @ r1 = src[vcount]
    strh    r1, [r2]                @ *dest = r1
    .endr
.dataTransferEnd16:

    @ now for the 32-bit data
    add     r3, r3, #(BN_CFG_HBES_MAX_ITEMS*SIZEOF_ENTRY)   @ offset the pointer
    ldr     r2, [r3], #4                            @ load the 32-bit count
    add     r3, r3, r2, lsl #3                      @ offset the register
    adr     r1, .dataTransferEnd32                  @ pick up the address
    add     r2, r2, r2, lsl #1                      @ multiply the count by 3
    sub     pc, r1, r2, lsl #2                      @ and set pc = label - count*12
    .rept   4
    ldmdb   r3!, {r1, r2}           @ r1 = src (const uint32_t*), r2 = dest (volatile uint32_t*)
    ldr     r1, [r1, r0, lsl #1]    @ r1 = src[vcount] (r0 = vcount*2, r0, lsl #1 = vcount*4)
    str     r1, [r2]                @ *dest = r1
    .endr
.dataTransferEnd32:

    bx      lr                                      @ we are done here, but tbh I really need to test this routine

    .global _ZN2bn2hw14hblank_effects4dataE
_ZN2bn2hw14hblank_effects4dataE:                    @ entries* bn::hw::hblank_effects::data
    .word   0
