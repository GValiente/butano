@
@ 8bpp bitmap glyph renderer. 1->8bpp recolored, 
@ any size, transparent
@
@! \file bmp8_drawg_b1cts_fast.s
@! \author J Vijn
@! \date 20070814 - 20070822
@
@ === NOTES ===

#include "../../include/tonc_asminc.h"
#include "tte_types.s"

@ IWRAM_CODE void bmp8_drawg_b1cts_fast(int gid);
BEGIN_FUNC_ARM(bmp8_drawg_b1cts_fast, CSEC_IWRAM)
	stmfd	sp!, {r4-r11}

	ldr		ip,=gp_tte_context
	ldr		ip, [ip]
	
	@ Preload dstBase (r4), dstPitch (r5), yx (r6), 
	@	font (r7)
	ldmia	ip, {r4, r5}
	add		r3, ip, #TTC_cursorX
	ldmia	r3, {r6, r7}

	@# Get srcD (r1), width (r8), charH (r9)
	ldmia	r7, {r1, r3}			@ Load data, widths
	cmp		r3, #0
	ldrneb	r8, [r3, r0]			@ Var charW
	ldreqb	r8, [r7, #TF_charW]		@ Fixed charW
	ldrh	r3, [r7, #TF_cellS]
	mla		r1, r3, r0, r1			@ srcL
	ldrb	r2, [r7, #TF_charH]		@ charH
	ldrb	r11, [r7, #TF_cellH]	@ cellH PONDER: load later?

	@# Positional issues: dstD(r0), cursorX, odd(r6)
	mov		r3, r6, lsr #16			@ y
	bic		r6, r6, r3, lsl #16		@ x
	mla		r0, r5, r3, r4
	mov		r3, r6, lsr #1
	add		r0, r0, r3, lsl #1		@ dstD= dstBase+(y*dstP+(x&~1))
	and		r6, r6, #1				@ odd

	@ Prep rest
	ldrh	r7, [ip, #TTC_ink]

	@ --- Reg-list for render loops ---
	@ r0	dstL
	@ r1	srcL
	@ r2	iy_rev
	@ r3	raw / tmp
	@ r4	px
	@ r5	dstP
	@ r6	odd
	@ r7	ink
	@ r8	charW
	@ r9	charH
	@ r10	dstD
	@ r11	deltaS
	@ ip	ix

	@ Prepare for strip loop
	cmp		r8, #8
	b	.Lyloop
	sub		r11, r11, r2	@ deltaS= cellH - charH
	mov		r10, r0			@ save dstD
	mov		r9, r2			@ save charH
	b	.Lyloop
	
	@ --- strip loop ---
.Lsloop:
		add		r0, r10, #16	@ dstL= dstD+16/2
		mov		r10, r0
		add		r1, r1, r11		@ srcL += deltaS
	
		@ --- iy-loop ---
		mov		r2, r9
.Lyloop:
			ldrb	r4, [r1], #1
			movs	r4, r4, lsl r6
			beq		.Lyend			@ No pixels: skip
			@ --- ix loop ---
			mov		ip, #0
.Lxloop:
				@ 2bit -> 2bytes and fill halfword
				movs	r3, r4, lsl #30		@ 'raw&3' and 'raw&2'
				beq		.Lnopx
				ldrh	r3, [r0, ip]
				andmi	r3, r3, #255
				orrmi	r3, r3, r7, lsl #8
				tst		r4, #1
				bicne	r3, r3, #255
				orrne	r3, r3, r7
				strh	r3, [r0, ip]
.Lnopx:
				add		ip, ip, #2			
				movs	r4, r4,	lsr #2
				bne		.Lxloop
.Lyend:
			@ Finish y-loop
			add		r0, r0, r5
			subs	r2, r2, #1	
			bne		.Lyloop

		@ Finish strip-loop
		sub		r8, r8, #8
		bgt		.Lsloop

	@ Final cleanup
	ldmfd	sp!, {r4-r11}
	bx		lr
END_FUNC(bmp8_drawg_b1cts_fast)

@ EOF
