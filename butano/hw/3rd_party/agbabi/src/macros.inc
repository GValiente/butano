@===============================================================================
@
@ ARM assembly support macros
@
@ Copyright (C) 2021-2023 agbabi contributors
@ For conditions of distribution and use, see copyright notice in LICENSE.md
@
@===============================================================================

@ Shift and test upper two bits, clobbering \reg
@ Use mi for first bit, cs for second bit
.macro joaobapt_test_lsl reg shift = #0
    movs    \reg, \reg, lsl \shift
.endm

@ Test lowest two bits, clobbering \reg
@ Use mi for low bit, cs for high bit
.macro joaobapt_test reg
    joaobapt_test_lsl \reg, #31
.endm

@ Test lowest two bits of \src, result stored in \dst
@ Use mi for low bit, cs for high bit
.macro joaobapt_test_into dst, src
    movs    \dst, \src, lsl #31
.endm

@ Branches depending on lowest two bits, clobbering \reg
@ b_mi = low bit case, b_cs = high bit case
.macro joaobapt_switch reg, b_mi, b_cs
    joaobapt_test \reg
    bmi     \b_mi
    bcs     \b_cs
.endm

@ Branches depending on alignment of \a and \b, clobbering \scratch
@ b_byte = off-by-byte case, b_half = off-by-half case
.macro align_switch a, b, scratch, b_byte, b_half
    eor     \scratch, \a, \b
    joaobapt_switch \scratch, \b_byte, \b_half
.endm
