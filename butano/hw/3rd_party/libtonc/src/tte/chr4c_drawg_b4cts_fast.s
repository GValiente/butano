@
@ Col-major tile character renderer. 4->4bpp recolored, any size, transparent
@
@! \file chr4c_drawg_b4cts_fast.s
@! \author J Vijn
@! \date 20070725 - 20090801
@
@ === NOTES ===

#include "../../include/tonc_asminc.h"
#include "tte_types.s"

@ IWRAM_CODE void chr4c_drawg_b4cts_asm(int gid);
BEGIN_FUNC_ARM(chr4c_drawg_b4cts_fast, CSEC_IWRAM)
	stmfd	sp!, {r4-r11, lr}

	ldr		r5,=gp_tte_context
	ldr		r5, [r5]
	
	@ Preload dstBase (r4), dstPitch (ip), yx (r6), font (r7)
	ldmia	r5, {r4, ip}
	add		r3, r5, #TTC_cursorX
	ldmia	r3, {r6, r7}

	@ Get srcD (r1), width (r11), charH (r2)
	ldmia	r7, {r1, r3}			@ Load data, widths
	cmp		r3, #0
	ldrneb	r11, [r3, r0]			@ Var charW
	ldreqb	r11, [r7, #TF_charW]	@ Fixed charW
	ldrh	r3, [r7, #TF_cellS]
	mla		r1, r3, r0, r1			@ srcL
	ldrb	r2, [r7, #TF_charH]		@ charH
	ldrb	r10, [r7, #TF_cellH]	@ cellH PONDER: load later?

	@ Positional issues: dstD(r0), lsl(r8), lsr(r9), right(lr), cursorX 
	mov		r3, r6, lsr #16			@ y
	bic		r6, r6, r3, lsl #16		@ x

	add		r0, r4, r3, lsl #2		@ dstD= dstBase + y*4
	mov		r3, r6, lsr #3
	mla		r0, ip, r3, r0

	and		r6, r6, #7				@ x%7
	add		lr, r11, r6				@ right= width + x%8
	mov		r8, r6, lsl #2			@ lsl = x%8*4
	rsb		r9, r8, #32				@ lsr = 32-x%8*4			

	sub		r3, r10, r2				@ prep deltaS
	ldr		r6,=0x11111111
	ldrh	r7, [r5, #TTC_ink]
	ldrh	r10, [r5, #TTC_shadow]

	@ --- Reg-list for strip/render loop ---
	@ r0	dstL
	@ r1	srcL
	@ r2	scanline looper
	@ r3	raw
	@ r4	px / tmp
	@ r5	pxmask
	@ r6	bitmask
	@ r7	ink
	@ r8	left shift
	@ r9	right shift
	@ r10	shadow
	@ r11	charW
	@ ip	dstP
	@ lr	Right edge
	@ sp00	dstD			@# TODO: unnecessary; remove later
	@ sp04	charH
	@ sp08	deltaS = cellH-charH	 (delta srcL)

	cmp		r11, #8
	@ Prep for single-strip render
	suble	sp, sp, #12
	ble		.Lyloop4
	@ Prep for multi-strip render
	stmfd	sp!, {r0, r2, r3}			@ Store dstD, charH, deltaS
	b		.Lyloop4

	@ --- Strip loop ---
.Lsloop4:
		ldmia	sp, {r0,r2,r3}	@ Reload dstD, charH and deltaS
		add		r0, r0, ip		@ (Re)set dstD/dstL
		str		r0, [sp]
		add		r1, r1, r3, lsl #2
		sub		lr, lr, #8

		@ --- Render loop ---
.Lyloop4:
			@# Prep px and pxmask
			ldr		r3, [r1], #4
			and		r4, r6, r3				@ Ink mask
			and		r3, r6, r3, lsr #1		@ Shadow mask
			orrs	r5, r3, r4				@ Full mask
			beq		.Lnopx4	
			mul		r4, r7, r4				@ Apply ink (#fix for slowness?)
			mla		r4, r3, r10, r4			@ Apply shadow
			rsb		r5, r5, r5, lsl #4		@ Mask *= 0xF

			@ Render to left tile
			ldr		r3, [r0]
			bic		r3, r3, r5, lsl r8
			orr		r3, r3, r4, lsl r8
			str		r3, [r0]

			@ Render to right tile
			cmp		lr, #8
			ldrgt	r3, [r0, ip]
			bicgt	r3, r3, r5, lsr r9
			orrgt	r3, r3, r4, lsr r9
			strgt	r3, [r0, ip]
.Lnopx4:
			add		r0, r0, #4

			subs	r2, r2, #1
			bne		.Lyloop4

		@ Test for strip loop
		subs	r11, r11, #8
		bgt		.Lsloop4
	
	add		sp, sp, #12
	ldmfd	sp!, {r4-r11, lr}
	bx		lr
END_FUNC(chr4c_drawg_b4cts_fast)

@ EOF
