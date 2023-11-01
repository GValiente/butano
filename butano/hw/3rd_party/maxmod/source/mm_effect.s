/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                           Sound Effect System                            *
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

#include "../asm_include/mm_gba_setup.inc"

#include	"../asm_include/mp_defs.inc"
#include	"../asm_include/mp_mas.inc"
#include	"../asm_include/mp_mas_structs.inc"
#include	"../asm_include/mp_format_mas.inc"
#include	"../asm_include/mp_macros.inc"

#ifdef SYS_GBA
#include	"../asm_include/mp_mixer_gba.inc"
#endif

#ifdef SYS_NDS
#include	"../asm_include/mp_mixer_ds.inc"
#endif

/***********************************************************************
 *
 * Definitions
 *
 ***********************************************************************/

.struct 0					// mm_sound_effect
MM_SFX_SOURCE:	.space 4			// word: source
MM_SFX_RATE:	.space 2			// hword: rate
MM_SFX_HANDLE:	.space 2			// byte:  handle
MM_SFX_VOLUME:	.space 1			// byte:  volume
MM_SFX_PANNING:	.space 1			// byte:  panning
MM_SFX_SIZE:					// 8 bytes total

.equ	channelCount, 16
.equ	releaseLevel, 200

/***********************************************************************
 *
 * Memory
 *
 ***********************************************************************/

	.BSS
	.ALIGN 2
	.GLOBAL mm_sfx_bitmask, mm_sfx_clearmask

mm_sfx_mastervolume:	.space 4
mm_sfx_channels:	.space 2*channelCount
mm_sfx_bitmask:		.space 4
mm_sfx_clearmask:	.space 4

mm_sfx_counter:		.space 1

/***********************************************************************
 *
 * Program
 *
 ***********************************************************************/
 
	.TEXT
	.THUMB
	.ALIGN 2
	
/***********************************************************************
 * mmResetEffects
 ***********************************************************************/
						.global mmResetEffects
						.thumb_func
mmResetEffects:
	
	mov	r0, #0
	mov	r1, #channelCount
	ldr	r2,=mm_sfx_channels
	
1:	strh	r0, [r2]
	add	r2, #2
	sub	r1, #1
	bne	1b
	
	ldr	r2,=mm_sfx_bitmask
	str	r0, [r2]
	
	bx	lr
	
/***********************************************************************
 * mmGetFreeEffectChannel()
 *
 * Return index to free effect channel
 ***********************************************************************/
						.thumb_func
mmGetFreeEffectChannel:
	
	ldr	r0,=mm_sfx_bitmask		// r0 = bitmask
	ldr	r0, [r0]			//
	mov	r1, #1				// r1 = channel counter
	
.channel_search:				// shift out bits until we find a cleared one
	lsr	r0, #1				//
	bcc	.found_channel			//
	add	r1, #1				// r1 = index value
	b	.channel_search			//
	
.found_channel:
	
	cmp	r1, #channelCount+1		// if r1 == cc+1 then r1 = 0 (no handles avail.)
	bne	.found_valid_channel		//
	mov	r1, #0				//
.found_valid_channel:				//
	
	mov	r0, r1				// return value
	bx	lr				//

/***********************************************************************
 * mmEffect( id )
 *
 * Play sound effect with default parameters
 ***********************************************************************/
						.global mmEffect
						.thumb_func
mmEffect:
	push	{lr}
						// r0 = ssssssss
	mov	r1, #1				// r1 = hhhhrrrr
	lsl	r1, #10				//
	ldr	r2,=0x000080FF			// r2 = ----ppvv
	
	push	{r0-r2}				// mmEffectEx( sound )
	mov	r0, sp				// 
	bl	mmEffectEx			//
	add	sp, #12				//
	pop	{r3}				//
	bx	r3				//
	
/***********************************************************************
 * mmEffectEx( sound )
 *
 * Play sound effect with specified parameters
 ***********************************************************************/
						.global mmEffectEx
						.thumb_func
mmEffectEx:
	
	push	{r4-r6, lr}
	
	mov	r4, r0
	ldrh	r5, [r4, #MM_SFX_HANDLE]	// test if handle was given
	
	cmp	r5, #255
	bne	1f
	mov	r5, #0
	b	.got_handle
	
1:	cmp	r5, #0				//
	beq	.generate_new_handle		//
	
	lsl	r1, r5, #24			// check if channel is in use
	lsr	r1, #23				//
	sub	r1, #2				//
	ldr	r0,=mm_sfx_channels		//
	ldrb	r0, [r0, r1]			//
	cmp	r0, #0				//
	beq	.got_handle			// [valid handle otherwise]
	
	mov	r0, r5
	bl	mmEffectCancel			// attempt to stop active channel
	cmp	r0, #0				//
	bne	.got_handle			//
						// failure: generate new handle
.generate_new_handle:
	
	bl	mmGetFreeEffectChannel		// generate handle
	mov	r5, r0				//
	beq	.got_handle			//-(no available channels)
						//
	ldr	r0,=mm_sfx_counter		// (counter = ((counter+1) & 255))
	ldrb	r1, [r0]			//
	add	r1, #1				//
	strb	r1, [r0]			//
	lsl	r1, #8
	orr	r5, r1
	lsl	r5, #16
	lsr	r5, #16
	
.got_handle:
	
	ldr	r1,=mmAllocChannel		// allocate new channel
	bl	mpp_call_r1			//
	cmp	r0, #255			//
	bge	.no_available_channels		//
	
	mov	r6, r0
	
	cmp	r5, #0
	beq	1f 
	
	ldr	r1,=mm_sfx_channels		// register data
	sub	r2, r5, #1			// r3 = bit
	lsl	r2, #24				//
	lsr	r2, #24				//
	mov	r3, #1				//
	lsl	r3, r2				//
	lsl	r2, #1				//
	add	r1, r2				//
	add	r2, r0, #1			//
	strb	r2, [r1, #0]			//
	lsr	r2, r5, #8			//
	strb	r2, [r1, #1]			//
	
	ldr	r1,=mm_sfx_bitmask		// set bit
	ldr	r2, [r1]			//
	orr	r2, r3				//
	str	r2, [r1]			//
	
1:	

	ldr	r1,=mm_achannels		// setup channel
	ldr	r1, [r1]			//
	mov	r2, #MCA_SIZE			//
	mul	r2, r0				//
	add	r1, r2				//
						//
	mov	r2, #releaseLevel		//
	strb	r2, [r1, #MCA_FVOL]		//
	
	cmp	r5, #0				//
	bne	1f				//
	mov	r2, #ACHN_BACKGROUND		//
	b	2f				//
1:	mov	r2, #ACHN_CUSTOM		//
2:						//
	strb	r2, [r1, #MCA_TYPE]		//
	mov	r2, #MCAF_EFFECT		//
	strb	r2, [r1, #MCA_FLAGS]		//
	
	GET_MIXCH r1				// setup voice
	mov	r2, #MIXER_CHN_SIZE		//
	mul	r2, r0				//
	add	r3, r1, r2			//
	
#ifdef SYS_GBA

	ldr	r0,=mp_solution			// set sample data address
	ldr	r0, [r0]			//
	ldrh	r1, [r4, #MM_SFX_SOURCE]	//
	lsl	r1, #2				//
	add	r1, #12				//
	ldr	r1, [r0, r1]			//
	add	r1, r0				//
	ldrh	r2, [r1, #8+C_SAMPLEC_DFREQ]	//
	add	r1, #8+C_SAMPLE_DATA		//
	str	r1, [r3, #MIXER_CHN_SRC]	//
	
	ldrh	r0, [r4, #MM_SFX_RATE]		// set pitch to original * pitch
	mul	r2, r0				//
	lsr	r2, #10-2 			//
	str	r2, [r3, #MIXER_CHN_FREQ]	//
	
	mov	r1, #0				// reset read position
	str	r1, [r3, #MIXER_CHN_READ]	//
	
	ldrb	r0, [r4, #MM_SFX_VOLUME]	// set volume
	
	ldr	r1,=mm_sfx_mastervolume
	ldr	r1, [r1]
	mul	r0, r1
	lsr	r0, #10
	
	strb	r0, [r3, #MIXER_CHN_VOL]	//
	
	ldrb	r0, [r4, #MM_SFX_PANNING]	// set panning
	strb	r0, [r3, #MIXER_CHN_PAN]	//
	
#else
	ldr	r1, [r4, #MM_SFX_SOURCE]	// set sample address
	lsr	r0, r1, #16			// > 0x10000 = external sample
	bne	1f				//
	ldr	r0,=mmSampleBank		// ID# otherwise
	ldr	r0, [r0]			//
	lsl	r1, #2				//
	ldr	r1, [r0, r1]			//
	lsl	r1, #8				//
	lsr	r1, #8				//
	beq	.invalid_sample
	add	r1, #8				//
	ldr	r0,=0x2000000			//
	add	r1, r0				//
1:						//
	str	r1, [r3, #MIXER_CHN_SAMP]	//

	
	ldrh	r0, [r4, #MM_SFX_RATE]		// set pitch to original * pitch
	ldrh	r1, [r1, #C_SAMPLEC_DFREQ]	//
	mul	r1, r0				//
	lsr	r1, #10 			//
	strh	r1, [r3, #MIXER_CHN_FREQ]	//
	
	mov	r1, #0				// clear sample offset
	str	r1, [r3, #MIXER_CHN_READ]	//
	
	ldrb	r1, [r4, #MM_SFX_PANNING]	// set panning + startbit
	lsr	r1, #1				//
	add	r1, #0x80			//
	strb	r1, [r3, #MIXER_CHN_CNT]	//
	
	ldrb	r1, [r4, #MM_SFX_VOLUME]	// set volume
	ldr	r0,=mm_sfx_mastervolume
	ldr	r0, [r0]
	mul	r1, r0
//	lsr	r0, #10
	lsr	r2, r1, #2
//	lsl	r2, r1, #8

	strh	r2, [r3, #MIXER_CHN_VOL]	//
	
	b	.valid_sample
.invalid_sample:
	mov	r0, #0
	str	r0, [r3, #MIXER_CHN_SAMP]
	
.valid_sample:
	
#endif

	mov	r0, r5				// return handle
	pop	{r4-r6}				//
	ret1					//
	
.no_available_channels:	
	mov	r0, #0				// return bad
	pop	{r4-r6}				//
	ret1					//



/***********************************************************************
 * mme_get_channel_index
 *
 * Test handle and return mixing channel index
 ***********************************************************************/
						.thumb_func
mme_get_channel_index:

	lsl	r1, r0, #24			// mask and test channel#
	lsr	r1, #24-1			//
	cmp	r1, #0				//
	beq	.invalid_handle			//
	cmp	r1, #channelCount*2		//
	bgt	.invalid_handle			//
	
	ldr	r2,=mm_sfx_channels-2		// check if instances match
	ldrh	r3, [r2, r1]			//
	lsr	r1, r3, #8			//
	lsr	r2, r0, #8			//
	cmp	r1, r2				//
	bne	.invalid_handle			//
	
	lsl	r3, #24				// mask channel#
	lsr	r3, #24				//
	sub	r3, #1				//
	bx	lr				//
	
.invalid_handle:				// return invalid handle
	mov	r3, #0				//
	mvn	r3, r3				//
	bx	lr				//

/***********************************************************************
 * mmEffectActive( handle )
 *
 * Indicates if a sound effect is active or not
 ***********************************************************************/
						.global mmEffectActive
						.thumb_func
mmEffectActive:
	push	{lr}				//
	bl	mme_get_channel_index	//
	cmp	r3, #0					// if r3 >= 0, it is active
	bge	.active					//
	mov	r0, #0					// return false
	pop	{r3}					//
	bx	r3						//
	
.active:						// 
	mov	r0, #1					// return true
	pop	{r3}					//
	bx	r3						//
	
/***********************************************************************
 * mme_clear_channel(ch)
 *
 * Clear channel entry and bitmask
 * r3 preserved
 ***********************************************************************/
						.thumb_func
mme_clear_channel:
	mov	r1, #0
	ldr	r2,=mm_sfx_channels		// clear effect channel
	lsl	r0, #1				//
	strh	r1, [r2, r0]			//
	
	mov	r1, #1				// clear effect bitmask
	lsr	r0, #1				//
	lsl	r1, r0				//
	ldr	r2,=mm_sfx_bitmask		//
	ldr	r0, [r2, #4]
	orr	r0, r1
	str	r0, [r2, #4]
	ldr	r0, [r2]			//
	bic	r0, r1				//
	str	r0, [r2]			//
	bx	lr

/***********************************************************************
 * mmEffectVolume( handle, volume )
 *
 * Set effect volume
 *
 * volume 0..255
 ***********************************************************************/
						.global mmEffectVolume
						.thumb_func
mmEffectVolume:

	push	{r1, lr}
	
	bl	mme_get_channel_index
	pop	{r1}
	bmi	1f
	
	ldr	r0,=mm_sfx_mastervolume
	ldr	r0, [r0]
	mul	r1, r0
	
	#ifdef SYS_NDS
	
	lsr	r1, #2
	
	#endif
	
	#ifdef SYS_GBA
	
	lsr	r1, #10
	
	#endif
	
	mov	r0, r3
	
	bl	mmMixerSetVolume

1:	ret0

/***********************************************************************
 * mmEffectPanning( handle, panning )
 *
 * Set effect panning
 *
 * panning 0..255
 ***********************************************************************/ 
						.global mmEffectPanning
						.thumb_func
mmEffectPanning:

	push	{r1, lr}
	bl	mme_get_channel_index
	pop	{r1}
	bmi	1f
	
	mov	r0, r3
	bl	mmMixerSetPan
	
1:	ret0

/***********************************************************************
 * mmEffectRate( handle, rate )
 *
 * Set effect playback rate
 ***********************************************************************/
						.global mmEffectRate
						.thumb_func
mmEffectRate:
	
	push	{r1, lr}
	bl	mme_get_channel_index
	pop	{r1}
	bmi	1f
	
	mov	r0, r3
	bl	mmMixerSetFreq
	
1:	ret0

/***********************************************************************
 * mmEffectCancel( handle )
 *
 * Stop sound effect
 ***********************************************************************/
						.global mmEffectCancel
						.thumb_func
mmEffectCancel:
	
	push	{r0, lr}
	
	bl	mme_get_channel_index
	
	pop	{r0}
	
	bmi	1f
	
	mov	r1, #MCA_SIZE			// free achannel
	mul	r1, r3				//
	ldr	r2,=mm_achannels		//
	ldr	r2, [r2]			//
	add	r2, r1				//
	mov	r1, #ACHN_BACKGROUND		//
	strb	r1, [r2, #MCA_TYPE]		//
	mov	r1, #0				//
	strb	r1, [r2, #MCA_FVOL]		// clear volume for channel allocator
	
	lsl	r0, #24
	lsr	r0, #24
	sub	r0, #1
	bl	mme_clear_channel
	
	mov	r1, #0				// zero voice volume
	mov	r0, r3				//
	bl	mmMixerSetVolume		//
	
	mov	r0, #1
	ret1
1:	
	mov	r0, #0
	ret1

/***********************************************************************
 * mmEffectRelease( channel )
 *
 * Release sound effect (allow interruption)
 ***********************************************************************/
						.global mmEffectRelease
						.thumb_func
mmEffectRelease:

	push	{r0, lr}
	
	bl	mme_get_channel_index
	pop	{r0}
	
	bmi	1f
	
	mov	r1, #MCA_SIZE			// release achannel
	mul	r1, r3				//
	ldr	r2,=mm_achannels		//
	ldr	r2, [r2]			//
	add	r2, r1				//
	mov	r1, #ACHN_BACKGROUND		//
	strb	r1, [r2, #MCA_TYPE]		//
	
	lsl	r0, #24
	lsr	r0, #24
	sub	r0, #1
	bl	mme_clear_channel
	
1:	ret0

/***********************************************************************
 * mmEffectScaleRate( channel, factor )
 *
 * Scale sampling rate by 6.10 factor
 ***********************************************************************/
						.global mmEffectScaleRate
						.thumb_func
mmEffectScaleRate:
	
	push	{r1,lr}
	
	bl	mme_get_channel_index
	pop	{r1}
	
	bmi	1f
	
	mov	r0, r3
	bl	mmMixerMulFreq
	
1:	ret0

/***********************************************************************
 * mmSetEffectsVolume( volume )
 *
 * set master volume scale, 0->1024
 ***********************************************************************/
						.global mmSetEffectsVolume
						.thumb_func
mmSetEffectsVolume:

	lsr	r1, r0, #10
	beq	1f
	mov	r0, #1
	lsl	r0, #10
	
1:	ldr	r1,=mm_sfx_mastervolume
	str	r0, [r1]
	bx	lr
	
/***********************************************************************
 * mmEffectCancelAll()
 *
 * Stop all sound effects
 ***********************************************************************/
						.global mmEffectCancelAll
						.thumb_func
mmEffectCancelAll:

	push	{r4-r7,lr}
	
	ldr	r4,=mm_sfx_bitmask
	ldr	r4, [r4]
	ldr	r6,=mm_sfx_channels
	mov	r5, #0
	
	
	lsr	r4, #1
	bcc	.mmeca_next
.mmeca_process:

	ldrb	r7, [r6, r5]
	sub	r7, #1
	bmi	.mmeca_next
	
	mov	r0, r7
	mov	r1, #0
	bl	mmMixerSetVolume
	
	ldr	r0,=mm_achannels		// free achannel
	ldr	r0, [r0]			//
	mov	r1, #MCA_SIZE			//
	mul	r1, r7				//
	add	r0, r1				//
	mov	r1, #ACHN_BACKGROUND		//
	strb	r1, [r0, #MCA_TYPE]		//
	mov	r1, #0
	strb	r1, [r0, #MCA_FVOL]		//
	
.mmeca_next:
	add	r5, #2
	lsr	r4, #1
	bcs	.mmeca_process
	bne	.mmeca_next
	
	bl	mmResetEffects
	
	POP	{r4-r7}
	pop	{r3}
	bx	r3

/***********************************************************************
 * mmUpdateEffects()
 *
 * Update sound effects
 ***********************************************************************/
						.global mmUpdateEffects
						.thumb_func
mmUpdateEffects:
	
	push	{r4-r6,lr}
	
	ldr	r4,=mm_sfx_bitmask
	ldr	r4, [r4]
	ldr	r6,=mm_sfx_channels
	mov	r5, #0
	
	lsr	r4, #1
	bcc	.next_channel
.process_channel:

	ldrb	r0, [r6, r5]			// get channel index
	sub	r0, #1				//
	bmi	.next_channel			//
	
	GET_MIXCH r1
	
	mov	r2, #MIXER_CHN_SIZE		// get mixing channel pointer
	mul	r2, r0				//
	add	r1, r2				//
	
	#ifdef SYS_NDS				// test if channel is still active
						//
	ldr	r2, [r1, #MIXER_CHN_SAMP]	//
	lsl	r2, #8				//
	bne	.next_channel			//
						//
	#else					//
						//
	ldr	r2, [r1, #MIXER_CHN_SRC]	//
	asr	r2, #31				//
	beq	.next_channel			//
						//
	#endif					//
	
	ldr	r1,=mm_achannels		// free achannel
	ldr	r1, [r1]			//
	mov	r2, #MCA_SIZE			//
	mul	r2, r0				//
	add	r1, r2				//
	mov	r0, #0				//
	strb	r0, [r1, #MCA_TYPE]		//
	strb	r0, [r1, #MCA_FLAGS]		//
	strb	r0, [r6, r5]
	
.next_channel:
	add	r5, #2				// look for another set bit
	lsr	r4, #1				//
	bcs	.process_channel		//
	add	r5, #2				//
	lsr	r4, #1				//
	bcs	.process_channel		//
	bne	.next_channel			//
	
	mov	r4, #0
	mov	r5, #1
	lsl	r5, #32-channelCount
	ldr	r6,=mm_sfx_channels
	 
.build_new_bitmask:
	ldrb	r0, [r6]
	add	r6, #2
	cmp	r0, #0
	beq	1f
	orr	r4, r5
1:	lsl	r5, #1
	bne	.build_new_bitmask
	
	lsr	r4, #32-channelCount
	ldr	r0,=mm_sfx_bitmask
	ldr	r1, [r0]			// r1 = bits that change from 1->0
	mov	r2, r1
	eor	r1, r4
	and	r1, r2

	str	r4, [r0]
	ldr	r4, [r0, #4]
	orr	r4, r1
	str	r4, [r0, #4]			// write 1->0 mask
	
	pop	{r4-r6,pc}

.pool

.end
