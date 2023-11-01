/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
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

/******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/

#include "../asm_include/mm_gba_setup.inc"

#include "../asm_include/mp_defs.inc"
#include "../asm_include/mp_mas.inc"
#include "../asm_include/mp_mas_structs.inc"
#include "../asm_include/mp_macros.inc"
#include "../asm_include/mp_mixer_gba.inc"

/******************************************************************************
 *
 * Memory
 *
 ******************************************************************************/

	.BSS
	.ALIGN 2

/******************************************************************************
 * mm_vblank_function
 *
 * Pointer to a user function to be called during the vblank irq
 ******************************************************************************/
					.global mm_vblank_function
mm_vblank_function:	.space 4

/******************************************************************************
 *
 * Program
 *
 ******************************************************************************/

/******************************************************************************
 * mpp_call_*
 *
 * Functions to branch to a register
 ******************************************************************************/

.section ".iwram", "ax", %progbits
.thumb
.align 2

.global mpp_call_r7i, mpp_call_r2i, mpp_call_r1i

	.thumb_func
//-----------------------------------------------------------------------------
mpp_call_r7i: bx r7
//-----------------------------------------------------------------------------

	.thumb_func
//-----------------------------------------------------------------------------
mpp_call_r2i: bx r2
//-----------------------------------------------------------------------------

	.thumb_func
//-----------------------------------------------------------------------------
mpp_call_r1i: bx r1
//-----------------------------------------------------------------------------

	.BSS
	.ALIGN 2

/******************************************************************************
 * mp_solution
 *
 * Address of soundbank in memory/rom
 ******************************************************************************/
						.global mp_solution
mp_solution:	.space 4

	.TEXT
	.THUMB
	.ALIGN 2

/******************************************************************************
 * mmInit(system)
 *
 * Initialize maxmod
 ******************************************************************************/
						.global mmInit
						.thumb_func
mmInit:
	push	{lr}

	ldr	r2,=mp_solution
	mov	r1, #MM_GBA_SYSTEM_SOUNDBANK
	ldr	r1, [r0,r1]
	str	r1, [r2]

	ldr	r2,=mm_achannels
	ldr	r1, [r0,#MM_GBA_SYSTEM_ACTCH]
	str	r1, [r2]
	ldr	r1, [r0,#MM_GBA_SYSTEM_MODCH]
	str	r1, [r2,#4]
	ldr	r1, [r0,#MM_GBA_SYSTEM_MCH_COUNT]
	str	r1, [r2,#8]
	ldr	r1, [r0,#MM_GBA_SYSTEM_ACH_COUNT]
	str	r1, [r2,#12]

	bl	mmMixerInit	@ initialize software/hardware mixer

	ldr	r1,=mm_num_ach
	ldr	r1,[r1]
	mov	r0,#1
	lsl	r0, r1
	sub	r0,#1

	ldr	r1,=mm_ch_mask
	str	r0, [r1]

	ldr	r0,=0x400			//
	bl	mmSetModuleVolume
	ldr	r0,=0x400			//
	bl	mmSetJingleVolume
	ldr	r0,=0x400			//
	bl	mmSetEffectsVolume		//


	ldr	r0,=0x400
	bl	mmSetModuleTempo

	ldr	r0,=0x400
	bl	mmSetModulePitch

	bl	mmResetEffects

	ret0

/******************************************************************************
 * mmSetVBlankHandler
 *
 * Set function to be called during the vblank IRQ
 ******************************************************************************/
						.global mmSetVBlankHandler
						.thumb_func
mmSetVBlankHandler:

	ldr	r1,=mm_vblank_function
	str	r0, [r1]
	bx	lr

/******************************************************************************
 * mmFrame()
 *
 * Work routine, user _must_ call this every frame.
 ******************************************************************************/
						.global mmFrame
						.thumb_func
mmFrame:

	push	{lr}
	push	{r4-r7}

@ update effects

	ldr	r7,=mmUpdateEffects
	bl	_call_via_r7

@ update sub layer
@ sub layer has 60hz accuracy

	ldr	r7,=mppUpdateSub
	bl	_call_via_r7

@ update main layer and mix samples.
@ main layer is sample-accurate.

	ldr	r0,=mpp_channels	@ copy channels
	ldr	r1,=mm_pchannels
	ldr	r1,[r1]
	str	r1, [r0]
	ldr	r0,=mpp_nchannels	@ copy #channels
	ldr	r1,=mm_num_mch
	ldr	r1,[r1]
	strb	r1, [r0]
	ldr	r0,=mpp_clayer		@ layer=0 (main)
	mov	r1, #0
	strb	r1, [r0]

	ldr	r0,=mmLayerMain @mpp_layerA		@ copy layer pointer
	ldr	r1,=mpp_layerp
	str	r0, [r1]

	ldr	r4,=mm_mixlen
	ldr	r4,[r4]
					@ mixlen is divisible by 2

	ldrb	r1, [r0, #MPL_ISPLAYING]	@ check if main layer is active
	cmp	r1, #0
	beq	.mpf_no_mainlayer	@ skip processing if disabled (and just mix samples)

.mpf_mix_advr:

	ldr	r0,=mpp_layerp		@ get layer
	ldr	r0, [r0]

	mov	r1, #MPL_TICKRATE	@ get samples/tick
	ldrh	r5, [r0, r1]

	mov	r1, #MPL_SAMPCOUNT	@ get sample count
	ldrh	r6, [r0,r1]

	sub	r5, r6			@ calc tickrate-counter
	cmp	r5, #0
	bge	1f
	mov	r5, #0
1:	cmp	r5, r4			@ > mixlen?
	blt	.mpf_mix_adv		@ no, mix and process tick
	b	.mpf_mix		@ yes, mix the rest of samples

.mpf_mix_adv:

	mov	r1, #MPL_SAMPCOUNT	@ reset sample counter
	mov	r7, #0			@
	strh	r7, [r0,r1]		@
	sub	r4, r5			@ subtract from #samples to mix

	PROF_START

	mov	r0, r5
	ldr	r7,=mmMixerMix	@ mix samples
	bl	_call_via_r7

	PROF_END 0

	ldr	r7,=mppProcessTick
	bl	_call_via_r7

	b	.mpf_mix_advr	@ process more samples
.mpf_mix:

@ add samples remaining to SAMPCOUNT
@ and mix more samples

	mov	r1, #MPL_SAMPCOUNT
	add	r6, r4
	strh	r6, [r0, r1]
	mov	r0, r4
	PROF_START
	ldr	r1,=mmMixerMix
	bl	_call_via_r1
	PROF_END 0

	pop	{r4-r7}
	ret1			@ return to user

.mpf_no_mainlayer:

@ main layer isn't active,
@ mix full amount

	mov	r0, r4

	PROF_START
	ldr	r1,=mmMixerMix
	bl	_call_via_r1
	PROF_END 0

	pop	{r4-r7}
	ret1

.pool

//-----------------------------------------------------------------------------
.end
//-----------------------------------------------------------------------------

