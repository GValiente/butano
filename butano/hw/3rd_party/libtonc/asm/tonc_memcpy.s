//
//  Alignment-safe and fast memcpy routines
//
//! \file tonc_memcpy.s
//! \author J Vijn
//! \date 20060508 - 20090801
//
// === NOTES ===
@ * 20050924: Lower overhead for all; reduced i-count for u16 loops.
@ * These are 16/32bit memset and memcpy. The 32bit versions are in 
@   iwram for maximum effect and pretty much do what CpuFastSet does, 
@   except that it'll work for non multiples of 8 words too. Speed 
@   is as good as CpuFastSet, but with a little less overhead.
@ * The 16bit versions call the 32bit ones if possible and/or desirable. 
@   They are thumb/ROM functions but did them in asm anyway because 
@   GCC goes haywire with the use of registers resulting in a much
@   higher overhead (i.e., detrimental for low counts)
@ * Crossover with inline while(nn--) loops (not for(ii++), which are
@   much slower):
@		memcpy32: ~4
@		memcpy16: ~8

#include "../include/tonc_asminc.h"

	.file "tonc_memcpy.s"

@ === void memcpy32(void *dst, const void *src, u32 wdn); =============
/*! \fn void memcpy32(void *dst, const void *src, u32 wdn) CODE_IN_IWRAM;
    \brief Fast-copy by words.
	\param dst Destination address.
	\param src Source address.
	\param wdn Number of words.
	\note \a src and \a dst <b>must</b> be word aligned.
	\note \a r0 and \a r1 return as \a dst + \a wdn and \a src + \a wdn.
*/
/* Reglist:
  r0, r1: dst, src
  r2: wdn, then wdn>>3
  r3-r10: data buffer
  r12: wdn&7
*/
BEGIN_FUNC_ARM(memcpy32, CSEC_IWRAM)
	and		r12, r2, #7
	movs	r2, r2, lsr #3
	beq		.Lres_cpy32
	push	{r4-r10}
	@ copy 32byte chunks with 8fold xxmia
.Lmain_cpy32:
		ldmia	r1!, {r3-r10}	
		stmia	r0!, {r3-r10}
		subs	r2, r2, #1
		bhi		.Lmain_cpy32
	pop		{r4-r10}
	@ and the residual 0-7 words
.Lres_cpy32:
		subs	r12, r12, #1
		ldmcsia	r1!, {r3}
		stmcsia	r0!, {r3}
		bhi		.Lres_cpy32
	bx	lr
END_FUNC(memcpy32)

@ === void memcpy16(void *dst, const void *src, u32 hwn); =============
/*! \fn void memcpy16(void *dst, const void *src, u32 hwn);
    \brief Copy for halfwords.
	Uses <code>memcpy32()</code> if \a hwn>6 and 
	  \a src and \a dst are aligned equally.
	\param dst Destination address.
	\param src Source address.
	\param wdn Number of halfwords to fill.
	\note \a dst and \a src <b>must</b> be halfword aligned.
	\note \a r0 and \a r1 return as \a dst + \a hwn and \a src + \a hwn.
*/
/* Reglist:
  r0, r1: dst, src
  r2, r4: wdn
  r3: tmp; and data buffer
*/

BEGIN_FUNC_THUMB(memcpy16, CSEC_TEXT)
	push	{r4, lr}
	@ under 5 hwords -> std cpy
	cmp		r2, #5
	bls		.Ltail_cpy16
	@ unreconcilable alignment -> std cpy
	@ if (dst^src)&2 -> alignment impossible
	mov		r3, r0
	eor		r3, r1
	lsl		r3, r3, #31		@ (dst^src), bit 1 into carry
	bcs		.Ltail_cpy16	@ (dst^src)&2 : must copy by halfword
	@ src and dst have same alignment -> word align
	lsl		r3, r0, #31
	bcc		.Lmain_cpy16	@ ~src&2 : already word aligned
	@ aligning is necessary: copy 1 hword and align
		ldrh	r3, [r1]
		strh	r3, [r0]
		add		r0, #2
		add		r1, #2
		sub		r2, r2, #1
	@ right, and for the REAL work, we're gonna use memcpy32
.Lmain_cpy16:
	lsl		r4, r2, #31
	lsr		r2, r2, #1
	ldr		r3, =memcpy32
	bl		.Llong_bl
	@ NOTE: r0,r1 are altered by memcpy32, but in exactly the right 
	@ way, so we can use them as is.
	lsr		r2, r4, #31
	beq		.Lend_cpy16
.Ltail_cpy16:
	sub		r2, #1
	bcc		.Lend_cpy16		@ r2 was 0, bug out
	lsl		r2, r2, #1
.Lres_cpy16:
		ldrh	r3, [r1, r2]
		strh	r3, [r0, r2]
		sub		r2, r2, #2
		bcs		.Lres_cpy16
.Lend_cpy16:
	pop		{r4}
	pop		{r3}
.Llong_bl:
	bx	r3
END_FUNC(memcpy16)

@ EOF
