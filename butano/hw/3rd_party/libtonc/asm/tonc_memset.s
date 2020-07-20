//
//  Alignment-safe and fast memset routines
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
@		memset32: ~5
@		memset16: ~8

	.file "tonc_memset.s"

#include "../include/tonc_asminc.h"

@ === void memset32(void *dst, u32 src, u32 wdn); =====================
/*! \fn void memset32(void *dst, u32 src, u32 wdn) IWRAM_CODE;
    \brief Fast-fill by words.
	\param dst Destination address.
	\param src Fill word (not address).
	\param wdn Number of words to fill.
	\note \a dst <b>must</b> be word aligned.
	\note \a r0 returns as \a dst + \a wdn.
*/
/* Reglist:
  r0, r1: dst, src
  r2: wdn, then wdn>>3
  r3-r10: data buffer
  r12: wdn&7
*/
BEGIN_FUNC_ARM(memset32, CSEC_IWRAM)
	and		r12, r2, #7
	movs	r2, r2, lsr #3
	beq		.Lres_set32
	push	{r4-r9}
	@ set 32byte chunks with 8fold xxmia
	mov		r3, r1
	mov		r4, r1
	mov		r5, r1
	mov		r6, r1
	mov		r7, r1
	mov		r8, r1
	mov		r9, r1
.Lmain_set32:
		stmia	r0!, {r1, r3-r9}
		subs	r2, r2, #1
		bhi		.Lmain_set32
	pop		{r4-r9}
	@ residual 0-7 words
.Lres_set32:
		subs	r12, r12, #1
		stmhsia	r0!, {r1}
		bhi		.Lres_set32
	bx	lr
END_FUNC(memset32)

@ === void memset16(void *dst, u16 src, u32 hwn); =====================
/*! \fn void memset16(void *dst, u16 src, u32 hwn);
    \brief Fill for halfwords.
	Uses <code>memset32()</code> if \a hwn>5
	\param dst Destination address.
	\param src Source halfword (not address).
	\param wdn Number of halfwords to fill.
	\note \a dst <b>must</b> be halfword aligned.
	\note \a r0 returns as \a dst + \a hwn.
*/
/* Reglist:
  r0, r1: dst, src
  r2, r4: wdn
  r3: tmp; and data buffer
*/
BEGIN_FUNC_THUMB(memset16, CSEC_TEXT)
	push	{r4, lr}
	@ under 6 hwords -> std set
	cmp		r2, #5
	bls		.Ltail_set16
	@ dst not word aligned: copy 1 hword and align
	lsl		r3, r0, #31
	bcc		.Lmain_set16
		strh	r1, [r0]
		add		r0, #2
		sub		r2, r2, #1
	@ Again, memset32 does the real work
.Lmain_set16:
	lsl		r4, r1, #16
	orr		r1, r4
	lsl		r4, r2, #31
	lsr		r2, r2, #1
	ldr		r3, =memset32
	bl		.Llong_bl
	@ NOTE: r0 is altered by memset32, but in exactly the right 
	@ way, so we can use is as is. r1 is now doubled though.
	lsr		r2, r4, #31
	beq		.Lend_set16
	lsr		r1, #16
.Ltail_set16:
	sub		r2, #1
	bcc		.Lend_set16		@ r2 was 0, bug out
	lsl		r2, r2, #1
.Lres_set16:
		strh	r1, [r0, r2]
		sub		r2, r2, #2
		bcs		.Lres_set16
.Lend_set16:
	pop		{r4}
	pop		{r3}
.Llong_bl:
	bx	r3
END_FUNC(memset16)
	

@ EOF
