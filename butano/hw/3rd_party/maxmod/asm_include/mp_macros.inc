/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                       MACROS TO MAKE LIFE EASY                           *
 *                                                                          *
 *         Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)         *
 *                                                                          *
 * Permission to use, copy, modify, and/or distribute this software for any *
 * purpose with or without fee is hereby granted, provided that the above   *
 * copyright notice and this permission notice appear in all copies.        *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES *
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF         *
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR  *
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   *
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN    *
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF  *
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.           *
 ****************************************************************************/

#ifdef SYS_NDS9

.macro ret0		@ arm9 pop {pc} has mode change
	pop	{pc}
.endm
.macro ret1		@ arm9 pop {pc} has mode change
	pop	{pc}
.endm
.macro ret2		@ arm9 pop {pc} has mode change
	pop	{pc}
.endm
.macro ret3		@ arm9 pop {pc} has mode change
	pop	{pc}
.endm

.macro jump7
	blx	r7
.endm


.macro jump2
	blx	r2
.endm

.macro jump1
	blx	r1
.endm

.macro fjump2 function
	ldr	r2,=function
	blx	r2
.endm

#else

.macro ret0		@ arm7 requires pop/bx combo
	pop	{r0}
	bx	r0
.endm
.macro ret1
	pop	{r1}
	bx	r1
.endm
.macro ret2
	pop	{r2}
	bx	r2
.endm
.macro ret3
	pop	{r3}
	bx	r3
.endm


.macro jump7
#ifdef USE_IWRAM
	bl	mpp_call_r7i
#else
	bl	mpp_call_r7
#endif
.endm

.macro jump2
#ifdef USE_IWRAM
	bl	mpp_call_r2i
#else
	bl	mpp_call_r2
#endif
.endm

.macro fjump2 function
	ldr	r2,=\function
	jump2
.endm

.macro jump1
#ifdef USE_IWRAM
	bl	mpp_call_r1i
#else
	bl	mpp_call_r1
#endif
.endm

#endif

.macro break9
9: b 9b
.endm

.macro breakp
mov	r11,r11
.endm

.macro GET_MIXCH reg
#ifdef SYS_NDS
	ldr	\reg,=mm_mix_channels
#endif
#ifdef SYS_GBA
	ldr	\reg,=mm_mixchannels
	ldr	\reg, [\reg]
#endif
.endm

//#define ENABLE_PROFILE

.macro PROF_START mode

#ifdef ENABLE_PROFILE

	push	{r0-r3}
	push	{lr}
	ldr	r1,=profile_start
	mov	r2, pc
	add	r2, #5
	mov	lr, r2
	bx	r1
	pop	{r0}
	mov	lr, r0
	pop	{r0-r3}
#endif	

.endm

.macro PROF_END mode

#ifdef ENABLE_PROFILE
	push	{r0-r3}
	push	{lr}
	mov	r0, #\mode
	ldr	r1,=profile_take
	mov	r2, pc
	add	r2, #5
	mov	lr, r2
	bx	r1
	pop	{r0}
	mov	lr, r0
	pop	{r0-r3}
	
#endif

.endm

.macro __SECTION_IWRAM
#ifdef SYS_GBA
	.section ".iwram", "ax", %progbits
#else
	.text
#endif
.endm

//------------------------------
.macro nocash_msg msg
//------------------------------
	mov	r12,r12
	b	9f
	.hword	0x6464
	.hword	0
	.string "\msg"
	.align 2
9:
.endm
//------------------------------
.macro nocash_reset_clks
//------------------------------
	nocash_msg %zeroclks%
.endm
//------------------------------
.macro nocash_print_clks
//------------------------------
	nocash_msg %lastclks%
.endm
//------------------------------
