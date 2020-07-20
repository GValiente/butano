@
@ Tile character renderer. 1->4bpp recolored, any size, transparent
@
@! \file chr4r_drawg_b1cts_fast.s
@! \author J Vijn
@! \date 20070724 - 20070822
@
@ === NOTES ===

#include "tte_types.s"

@ IWRAM_CODE void chr4r_drawg_b1cts_asm(int gid);
	.section .iwram, "ax", %progbits
	.arm
	.align
	.global chr4r_drawg_b1cts_fast
chr4r_drawg_b1cts_fast:
	stmfd	sp!, {r4-r11, lr}

	ldr		ip,=gp_tte_context
	ldr		ip, [ip]
	
	@ Preload dstBase (r4), dstPitch (r5), yx (r6), 
	@	font (r7)
	ldmia	ip, {r4, r5}
	add		r3, ip, #TTC_cursorX
	ldmia	r3, {r6, r7}

	@ Get srcD (r1), width (r11), charH (r2)
	ldmia	r7, {r1, r3}		@ Load data, widths
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
	mov		r0, r3, lsr #3
	mla		r4, r5, r0, r4			@ dstD= dstBase+y/8*dstP
	and		r0, r3, #7				@ PONDER: use ror trick?
	add		r0, r4, r0, lsl #2		@ dstD += y%8*4.
#if 0	// Not worth it in the long run
	mov		r3, r6, lsr #3				@ dstL += x/8*8
	add		r0, r0, r3, lsl #5
	ands	lr, r6, #7					@ x%8
	mov		r8, lr, lsl #2				@ lsl = x%8*4
	addne	lr, lr, r11					@ split= (x%8) ? x%7+width : 0;
	rsb		r9, r8, #32					@ lsr = 32-x%8*4
#else
	mov		r6, r6, ror #3				@ x/8 (kinda)
	add		r0, r0, r6, lsl #5			@ dstL += x/8*8
	add		lr, r11, r6, lsr #(32-3)	@ right= width + x%8
	mov		r8, r6, lsr #(32-3-2)		@ lsl = x%8*4
	rsb		r9, r8, #32					@ lsr = 32-x%8*4
#endif
	ldr		r6,=0x01010101
	ldrh	r7, [ip, #TTC_ink]

	sub		ip, r5, #32				@ Fix dstP

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
	@ r10	dstD
	@ r11	charW
	@ ip	dstP
	@ lr	split indicator (right edge)
	@ sp00	charH
	@ sp04	deltaS = cellH-charH	 (delta srcL)

	cmp		r11, #8
	@ Prep for single-strip render
	suble	sp, sp, #8
	ble		.Lyloop
	@ Prep for multi-strip render
	sub		r3, r10, r2
	mov		r10, r0
	stmfd	sp!, {r2, r3}			@ Store charH, deltaS
	b		.Lyloop

	@ --- Strip loop ---
.Lsloop:
		ldmia	sp, {r2, r3}		@ Reload charH and deltaS
		add		r10, r10, #32		@ (Re)set dstD/dstL
		mov		r0, r10
		add		r1, r1, r3
		sub		lr, lr, #8

		@ --- Render loop ---
.Lyloop:
			@ Prep px and pxmask
			ldrb	r3, [r1], #1
			orrs	r3, r3, r3, lsl #12
			beq		.Lnopx				@ Skip if no pixels
			orr		r3, r3, r3, lsl #6
			and		r4, r3, r6, lsl #1
			and		r3, r3, r6
			orr		r3, r3, r4, lsl #3

			rsb		r5, r3, r3, lsl #4
			mul		r4, r3, r7		

			@ Render to left tile
			ldr		r3, [r0]
			bic		r3, r3, r5, lsl r8
			orr		r3, r3, r4, lsl r8
			str		r3, [r0]

			@ Render to right tile
			cmp		lr, #8
			ldrgt	r3, [r0, #32]
			bicgt	r3, r3, r5, lsr r9
			orrgt	r3, r3, r4, lsr r9
			strgt	r3, [r0, #32]
.Lnopx:
			add		r0, r0, #4
			@ Skip for new tile-row
			tst		r0, #31
			addeq	r0, r0, ip

			subs	r2, r2, #1
			bne		.Lyloop

		@ Test for strip loop
		subs	r11, r11, #8
		bgt		.Lsloop
	
	add		sp, sp, #8
	ldmfd	sp!, {r4-r11, lr}
	bx		lr
	.size chr4r_drawg_b1cts_fast, .-chr4r_drawg_b1cts_fast

@ EOF
