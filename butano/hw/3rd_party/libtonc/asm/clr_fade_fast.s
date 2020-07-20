//
//	Color fade with 33 alpha levels
//
//! \file tonc_memcpy.s
//! \author J Vijn
//! \date 20071130 - 20071130

#include "../include/tonc_asminc.h"

#ifndef CLR_ROUND
#define CLR_ROUND 1
#endif

/*
void clr_fade_fast(COLOR *src, COLOR clr, COLOR *dst, 
	int nclrs, u32 alpha) IWRAM_CODE;
*/
//!	Fades color arrays \a srca to \a clr into \a dst.
/*!	\param src	Source array.
*	\param clr	Final color (at alpha=32).
*	\param dst	Destination array.
*	\param nclrs	Number of colors.
*	\param alpha	Blend weight (range: 0-32).
*	\note u32 version, 2 clrs/loop. Loop: 18i/32c, Barrel shifter FTW.
*/
	.section .iwram,"ax", %progbits
	.align 2
	.arm
	.global clr_fade_fast
clr_fade_fast:
	movs	r3, r3, lsr #1			@ adjust nclrs for u32 run
	bxeq	lr						@ quit on nclrs=0
	ldr		r12, [sp]				@ get alpha from stack
	stmfd	sp!, {r4-r10, lr}
#if(CLR_ROUND==1)
	ldr		lr, =0x00200401			@ -1-|1-1
	rsb		r7, lr, lr, lsl #5		@ MASKLO: -g-|b-r
#else
	ldr		r7, =0x03E07C1F			@ MASKLO: -g-|b-r
#endif
	mov		r6, r7, lsl #5			@ MASKHI: g-|b-r-

	@ Precalc y1 and y2
	orr		r1, r1, r1, lsl #16
	and		r9, r7, r1, ror #16		@ precalc: y2= -g-|b-r (ror16)
	and		r1, r7, r1				@ precalc: y1= -g-|b-r
.Lfade_fast_loop:
		ldr		r8, [r0], #4			@ a= *pa++
		@ --- -g-|b-r
		and		r4, r6, r8, lsl #5		@ x/32: (-g-|b-r)
		sub		r5, r1, r4, lsr #5		@ z: y1-x
		mla		r4, r5, r12, r4			@ z: (y1-x)*w + x*32
#if(CLR_ROUND==1)
		add		r4, r4, lr, lsl #4		@ round
#endif
		and		r10, r7, r4, lsr #5		@ blend(-g-|b-r)			
		@ --- b-r|-g- (rotated by 16 for great awesome)
		and		r4, r6, r8, ror #11		@ x/32: -g-|b-r (ror16)
		sub		r5, r9, r4, lsr #5		@ z: y2-x
		mla		r4, r5, r12, r4			@ z: (y2-x)*w + x*32
#if(CLR_ROUND==1)
		add		r4, r4, lr, lsl #4		@ round
#endif
		and		r4, r7, r4, lsr #5		@ blend(-g-|b-r (ror16))
		@ --- mix -g-|b-r and b-r|-g-
		orr		r10, r10, r4, ror #16
		@ --- write faded, loop
		str		r10, [r2], #4			@ *dst++= c
		subs	r3, r3, #1
		bgt		.Lfade_fast_loop		
	ldmfd	sp!, {r4-r10, lr}
	bx		lr

@ EOF
