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

#include "mp_defs.inc"
#include "mp_mas.inc"
#include "mp_mas_structs.inc"
#include "mp_macros.inc"

#ifdef SYS_GBA
#include "mp_mixer_gba.inc"
#endif

#ifdef SYS_NDS
#include "mp_mixer_ds.inc"
#endif

/******************************************************************************
 *
 * Memory
 *
 ******************************************************************************/

	.BSS
	.ALIGN

/******************************************************************************
 * mmCallback
 *
 * Function pointer to user event handler
 ******************************************************************************/
					.global mmCallback
mmCallback:	.space 4

/******************************************************************************
 * mmModuleCount
 *
 * Number of modules in soundbank
 ******************************************************************************/
					.global mmModuleCount
mmModuleCount:	.space 4

/******************************************************************************
 * mmModuleBank
 *
 * Address of module bank
 ******************************************************************************/
					.global mmModuleBank
mmModuleBank:	.space 4

/******************************************************************************
 * mmSampleBank
 *
 * Address of sample bank
 ******************************************************************************/
					.global mmSampleBank
mmSampleBank:	.space 4

/******************************************************************************
 * mm_ch_mask
 *
 * Bitmask to select which hardware/software channels can be used
 ******************************************************************************/
					.global mm_ch_mask
mm_ch_mask:	.space 4

/******************************************************************************
 * mm_vblank_function
 *
 * Pointer to a user function to be called during the vblank irq
 ******************************************************************************/
					.global mm_vblank_function
mm_vblank_function:	.space 4

/******************************************************************************
 * mm_rds_pchannels, mm_rds_achannels
 *
 * Memory for module/active channels for NDS system
 ******************************************************************************/
#ifdef SYS_NDS
mm_rds_pchannels:	.space MCH_SIZE*32
mm_rds_achannels:	.space MCA_SIZE*32
#endif

/******************************************************************************
 * mmInitialized
 *
 * Variable that will be 'true' if we are ready for playback
 ******************************************************************************/
					.global mmInitialized
mmInitialized:		.space 1




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

#ifdef SYS_GBA
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

#endif

//-----------------------------------------------------------------------------
	.TEXT
	.THUMB
	.ALIGN 2
//-----------------------------------------------------------------------------

.global mpp_call_r7, mpp_call_r1, mpp_call_r2, mpp_call_r3

	.thumb_func
@------------------------------------------------------------------------------
mpp_call_r7: bx r7
@------------------------------------------------------------------------------

	.thumb_func
@------------------------------------------------------------------------------
mpp_call_r1: bx r1
@------------------------------------------------------------------------------

	.thumb_func
@------------------------------------------------------------------------------
mpp_call_r2: bx r2
@------------------------------------------------------------------------------
	
	.thumb_func
@------------------------------------------------------------------------------
mpp_call_r3: bx r3
@------------------------------------------------------------------------------

/******************************************************************************
 * mmSetEventHandler
 *
 * Set function for handling playback events
 ******************************************************************************/
						.global mmSetEventHandler
						.thumb_func
mmSetEventHandler:

	ldr	r1,=mmCallback
	str	r0, [r1]
	bx	lr

#ifdef SYS_NDS
/******************************************************************************
 * mmLockChannels( mask )
 *
 * Lock audio channels to prevent the sequencer from using them.
 ******************************************************************************/
						.global mmLockChannels
						.thumb_func
mmLockChannels:
	push	{r4, r5, lr}
	ldr	r1,=mm_ch_mask			// clear bits
	ldr	r2, [r1]			//
	bic	r2, r0				//
	str	r2, [r1]			//
	
	mov	r4, r0
	mov	r5, #0
	
2:	lsr	r4, #1
	bcc	1f
	mov	r0, r5
	bl	StopActiveChannel
1:	add	r5, #1
	cmp	r4, #0
	bne	2b
	
	pop	{r4,r5}
	pop	{r3}
	bx	r3
	
/******************************************************************************
 * StopActiveChannel( index )
 *
 * Stop active channel
 ******************************************************************************/
						.thumb_func
StopActiveChannel:
	push	{r4}
	
	GET_MIXCH r1				// stop mixing channel
	mov	r2, #MIXER_CHN_SIZE		//
	mul	r2, r0				//
	add	r1, r2				//
						//
						//
	#ifdef SYS_GBA				//
						//
	mov	r2, #0				//
	sub	r2, #1				//
	str	r2, [r1, #MIXER_CHN_SRC]	//
						//
	#endif					//
						//
	#ifdef SYS_NDS				//
						//
	mov	r2, #0				//
	str	r2, [r1, #MIXER_CHN_SAMP]	//
	strh	r2, [r1, #MIXER_CHN_CVOL]	//
	strh	r2, [r1, #MIXER_CHN_VOL]	//
						//
	#endif					//
	
	ldr	r1,=0x4000400			// stop hardware channel
	lsl	r2, r0, #4			//
	mov	r3, #0				//
	str	r3, [r1, r2]			//
	
	ldr	r1,=mm_achannels		// disable achn
	ldr	r1, [r1]			//
	mov	r2, #MCA_SIZE			//
	mul	r2, r0				//
	add	r1, r2				//
	mov	r2, #0				//
	ldrb	r4, [r1, #MCA_FLAGS]		//
	strb	r2, [r1, #MCA_FLAGS]		//
	strb	r2, [r1, #MCA_TYPE]		//
	
	lsr	r1, r4, #8
	bcs	.iseffect
	
	lsr	r4, #7
	bcs	.issub
	
	ldr	r1,=mm_pchannels		// stop hooked pchannel
	ldr	r1, [r1]			//
	ldr	r2,=mm_num_mch			//
	ldr	r2, [r2]			//
						//
2:	ldrb	r3, [r1, #MCH_ALLOC]		//
	cmp	r3, r0				//
	bne	1f				//
	mov	r3, #255			//
	strb	r3, [r1, #MCH_ALLOC]		//
	b	.iseffect			//
1:	sub	r2, #1				//
	bne	2b				//
	
	b	.iseffect			//
	
.issub:
	// stop sub pchannel
	ldr	r1,=mm_schannels
	mov	r2, #4
	
2:	ldrb	r3, [r1, #MCH_ALLOC]		//
	cmp	r3, r0				//
	bne	1f				//
	mov	r3, #255			//
	strb	r3, [r1, #MCH_ALLOC]		//
	b	.iseffect			//
1:	sub	r2, #1				//
	bne	2b				//
	
.iseffect:

	// hope it works out for effects...
	
	pop	{r4}
	bx	lr
	

/******************************************************************************
 * mmUnlockChannels( mask )
 *
 * Unlock audio channels so they can be used by the sequencer.
 ******************************************************************************/
						.global mmUnlockChannels
						.thumb_func
mmUnlockChannels:

#ifdef SYS_NDS
	ldr	r1,=mm_mixing_mode		// can NOT unlock channels in mode b
	ldrb	r1, [r1]			//
	cmp	r1, #1				//
	beq	1f				//
#endif
	
	ldr	r1,=mm_ch_mask
	ldr	r2, [r1]
	orr	r2, r0
	str	r2, [r1]
1:	bx	lr
#endif



/******************************************************************************
 *
 * GBA
 *
 ******************************************************************************/




//-----------------------------------------------------------------------------
#ifdef SYS_GBA
//-----------------------------------------------------------------------------

	.BSS
	.ALIGN

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
#endif
//-----------------------------------------------------------------------------



/******************************************************************************
 *
 * NDS
 *
 ******************************************************************************/



//-----------------------------------------------------------------------------
#ifdef SYS_NDS
//-----------------------------------------------------------------------------

	.TEXT
	.THUMB
	.ALIGN

/******************************************************************************
 * mmSuspendIRQ_t
 *
 * Function to disable interrupts via the status register
 ******************************************************************************/
						.global mmSuspendIRQ_t
						.thumb_func
mmSuspendIRQ_t:
	ldr	r0,=1f
	bx	r0

.arm
.align
1:	mrs	r0, cpsr
	and	r1, r0, #0x80
	orr	r0, #0x80
	msr	cpsr, r0
	str	r1, previous_irq_state
	bx	lr
.thumb

/******************************************************************************
 * mmRestoreIRQ_t
 *
 * Function to enable interrupts via the status register
 ******************************************************************************/	
						.global	mmRestoreIRQ_t
						.thumb_func
mmRestoreIRQ_t:
	ldr	r0,=1f
	bx	r0

.arm
.align
1:	mrs	r0, cpsr
	ldr	r1, previous_irq_state
	bic	r0, #0x80
	orr	r0, r1
	msr	cpsr, r0
	bx	lr
	
.thumb

previous_irq_state:
	.space	4

	.thumb_func
/******************************************************************************
 * mmIsInitialized()
 *
 * Returns true if the system is ready for playback
 ******************************************************************************/
						.global mmIsInitialized
						.thumb_func
mmIsInitialized:
	ldr	r0,=mmInitialized
	ldrb	r0, [r0]
	bx	lr
	
/******************************************************************************
 * mmInit7()
 *
 * Initialize system
 ******************************************************************************/
						.global mmInit7
						.thumb_func
mmInit7:
	push	{lr}
	mov	r0, #0x08
	ldr	r1,=mmFrame
	bl	irqSet
	
	mov	r0, #0x08
	bl	irqEnable
	
	ldr	r0,=0x400			// set volumes
	bl	mmSetModuleVolume		//
	ldr	r0,=0x400			//
	bl	mmSetJingleVolume		//
	ldr	r0,=0x400			//
	bl	mmSetEffectsVolume		//
	
	ldr	r0,=mmInitialized		// set initialized flag
	mov	r1, #42				//
	strb	r1, [r0]			//
	
	ldr	r0,=0xFFFF			// select all hardware channels
	bl	mmUnlockChannels		//
	
	ldr	r0,=mm_achannels		// setup channel addresses 
	ldr	r1,=mm_rds_achannels		//
	str	r1, [r0]			//
	ldr	r1,=mm_rds_pchannels		//
	str	r1, [r0,#4]			//
	mov	r1, #32				// 32 channels
	str	r1, [r0,#8]			//
	str	r1, [r0,#12]			//
	
	ldr	r0,=0x400
	bl	mmSetModuleTempo
	
	ldr	r0,=0x400
	bl	mmSetModulePitch
	
	bl	mmResetEffects
	
	bl	mmMixerInit			// setup mixer
	
	ldr	r0,=mmEventForwarder		// forward events
	bl	mmSetEventHandler
	
	ldr	r0,=mmInitialized		// set initialized flag
	mov	r1, #42				//
	strb	r1, [r0]			//
	
.exit_r3:
	pop	{r3}
	bx	r3

/******************************************************************************
 * mmInstall( channel )
 *
 * Install ARM7 system
 ******************************************************************************/
						.global mmInstall
						.thumb_func
mmInstall:
	push	{lr}
	
	ldr	r1,=mmInitialized		// not initialized until we get soundbank data
	mov	r2, #0				//
	strb	r2, [r1]			//
	
	bl	mmSetupComms			// setup communication
	
	b	.exit_r3

/******************************************************************************
 * mmEventForwarder( msg, param )
 *
 * Forward event to arm9
 ******************************************************************************/
						.thumb_func
mmEventForwarder:
	
	push	{lr}
	lsl	r1, #8
	orr	r0, r1
	mov	r1, #1
	lsl	r1, #20
	orr	r0, r1
	bl	mmARM9msg
	pop	{pc}
	
/******************************************************************************
 * mmGetSoundBank( n_songs, bank )
 *
 * Load sound bank address
 ******************************************************************************/
						.global mmGetSoundBank
						.thumb_func
mmGetSoundBank:
	ldr	r2,=mmModuleCount		// save data
	stmia	r2!, {r0,r1}			//
	
	lsl	r0, #2				// also sample bank address
	add	r1, r0				//	
	stmia	r2!, {r1}			//
	
//------------------------------------------------
// initialize system
//------------------------------------------------

	b	mmInit7

/******************************************************************************
 * mmFrame()
 *
 * Routine function
 ******************************************************************************/
						.global mmFrame
						.thumb_func
mmFrame:
	
	push	{lr}
	
	ldr	r0,=mmInitialized	// catch not-initialized
	ldrb	r0, [r0]		//
	cmp	r0, #42			//
	bne	1f			//

	bl	mmMixerPre		// <-- critical timing
	
	ldr	r0,=0x4000208		// enable irq
	mov	r1, #1			//
	strh	r1, [r0]		//
	
	bl	mmUpdateEffects		// update sound effects
	bl	mmPulse			// update module playback
	bl	mmMixerMix		// update audio
	
	
	bl	mmSendUpdateToARM9
	
1:	bl	mmProcessComms		// process communications
	
	ret1
	
.pool

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
.end
//-----------------------------------------------------------------------------

