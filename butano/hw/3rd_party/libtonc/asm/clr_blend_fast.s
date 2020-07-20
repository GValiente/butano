//
//	Color blend with 33 alpha levels
//
//! \file tonc_memcpy.s
//! \author J Vijn
//! \date 20071130 - 20090801

#include "../include/tonc_asminc.h"

#ifndef CLR_ROUND
#define CLR_ROUND 1
#endif

/*
void clr_blend_fast(COLOR *srca, COLOR *srcb, COLOR *dst, 
	int nclrs, u32 alpha) IWRAM_CODE;
*/
//!	Blends color arrays \a srca and \a srcb into \a dst.
/*!	\param srca	Source array A.
	\param srcb	Source array B
	\param dst	Destination array.
	\param nclrs	Number of colors.
	\param alpha	Blend weight (range: 0-32).
	\note	u32 version, 2 clrs/loop. Loop: 18i/32c, Barrel shifter FTW.
	\note	Properly rounds the blending. If you don't want that, remove 
			the references to lr.
*/
BEGIN_FUNC_ARM(clr_blend_fast, CSEC_IWRAM)
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
.Lbld_fast_loop:
		ldr		r8, [r0], #4			@ a= *pa++
		ldr		r9, [r1], #4			@ b= *pb++
		@ --- -g-|b-r
		and		r4, r6, r8, lsl #5		@ x/32: (-g-|b-r)
		and		r5, r7, r9				@ y: -g-|b-r
		sub		r5, r5, r4, lsr #5		@ z: y-x
		mla		r4, r5, r12, r4			@ z: (y-x)*w + x*32
#if(CLR_ROUND==1)
		add		r4, r4, lr, lsl #4		@ round
#endif
		and		r10, r7, r4, lsr #5		@ blend(-g-|b-r)			
		@ --- b-r|-g- (rotated by 16 for great awesome)
		and		r4, r6, r8, ror #11		@ x/32: -g-|b-r (ror16)
		and		r5, r7, r9, ror #16		@ y: -g-|b-r (ror16)
		sub		r5, r5, r4, lsr #5		@ z: y-x
		mla		r4, r5, r12, r4			@ z: (y-x)*w + x*32
#if(CLR_ROUND==1)
		add		r4, r4, lr, lsl #4		@ round
#endif
		and		r4, r7, r4, lsr #5		@ blend(-g-|b-r (ror16))
		@ --- mix -g-|b-r and b-r|-g-
		orr		r10, r10, r4, ror #16
		@ --- write blended, loop
		str		r10, [r2], #4			@ *dst++= c
		subs	r3, r3, #1
		bgt		.Lbld_fast_loop		
	ldmfd	sp!, {r4-r10, lr}
	bx		lr
END_FUNC(clr_blend_fast)


@ EOF
