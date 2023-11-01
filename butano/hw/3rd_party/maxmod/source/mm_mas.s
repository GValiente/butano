/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                            Module Processing                             *
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

#include "../asm_include/mp_format_mas.inc"
#include "../asm_include/mp_mas_structs.inc"
#include "../asm_include/mp_defs.inc"
#include "../asm_include/mp_macros.inc"

//-----------------------------------------------------------------------------
#ifdef SYS_GBA
//-----------------------------------------------------------------------------

#include "../asm_include/mp_mixer_gba.inc"
#include "../asm_include/swi_gba.inc"

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifdef SYS_NDS
//-----------------------------------------------------------------------------
#include "../asm_include/mp_mixer_ds.inc"
#include "../asm_include/swi_nds.inc"
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------


.equ	S3M_FREQ_DIVIDER	,57268224	// (s3m,xm,it)
.equ	MOD_FREQ_DIVIDER_PAL	,56750314	// (mod)
.equ	MOD_FREQ_DIVIDER_NTSC	,57272724	// (---)



/******************************************************************************
 *
 * Memory
 *
 ******************************************************************************/



	.BSS
	.ALIGN 2

/******************************************************************************
 * mmLayerMain
 *
 * Layer data for module playback.
 ******************************************************************************/
							.global mmLayerMain
mmLayerMain:		.space MPL_SIZE

/******************************************************************************
 * mmLayerSub
 *
 * Layer data for jingle playback.
 ******************************************************************************/
							.global mmLayerSub
mmLayerSub:		.space MPL_SIZE

/******************************************************************************
 * mpp_vars
 *
 * Holds intermediate data during the module processing.
 ******************************************************************************/
							.global mpp_vars
mpp_vars:		.space MPV_SIZE

	.ALIGN 2

/******************************************************************************
 * mpp_layerp
 *
 * Pointer to layer data during processing.
 ******************************************************************************/
							.global mpp_layerp
mpp_layerp:		.space 4

/******************************************************************************
 * mpp_channels
 *
 * Pointer to channel array during processing
 ******************************************************************************/
							.global mpp_channels
mpp_channels:		.space 4

/******************************************************************************
 * mpp_resolution
 *
 * Speed divider for DS timing.
 ******************************************************************************/
 
mpp_resolution:		.space 4

/******************************************************************************
 * mm_mastertempo
 *
 * Master tempo scaler.
 ******************************************************************************/
 
mm_mastertempo:		.space 4

/******************************************************************************
 * mm_masterpitch
 * 
 * Master pitch scaler.
 ******************************************************************************/
 
mm_masterpitch:		.space 4

/******************************************************************************
 * mpp_nchannels
 *
 * Number of channels in layer
 ******************************************************************************/
							.global mpp_nchannels
mpp_nchannels:		.space 1

/******************************************************************************
 * mpp_clayer
 *
 * Layer selection, 0 = main, 1 = sub
 ******************************************************************************/
							.global mpp_clayer
mpp_clayer:		.space 1

/******************************************************************************
 * mm_achannels, mm_pchannels, mm_num_mch, mm_num_ach, mm_schannels
 *
 * Channel data/sizes, don't move these around--see mmInit first
 ******************************************************************************/
							.ALIGN 2
							
.global mm_achannels, mm_pchannels, mm_num_mch, mm_num_ach, mm_schannels
						
mm_achannels:		.space 4
mm_pchannels:		.space 4
mm_num_mch:		.space 4
mm_num_ach:		.space 4
mm_schannels:		.space MP_SCHANNELS*MCH_SIZE



/******************************************************************************
 * 
 * Macros
 *
 ******************************************************************************/


/******************************************************************************
 * mpp_InstrumentPointer
 *
 * Calculate instrument address.
 * Requires r8 = layer
 * Returns in r0
 * Trashes r1, r2
 ******************************************************************************/
.macro mpp_InstrumentPointer
	mov	r1, r8	
	ldr	r2,[r1,#MPL_SONGADR]	
	ldr	r1,[r1,#MPL_INSTTABLE]	
	lsl	r0, #2			
	ldr	r0,[r1,r0]		
	add	r0, r2
.endm

/******************************************************************************
 * mpp_SamplePointer
 *
 * Calculate sample address.
 * Requires r8 = layer
 * Returns in r0
 * Trashes r1, r2
 ******************************************************************************/
.macro mpp_SamplePointer	
	mov	r1, r8		
	ldr	r2,[r1,#MPL_SONGADR]	
	ldr	r1,[r1,#MPL_SAMPTABLE]	
	lsl	r0, #2			
	ldr	r0,[r1,r0]		
	add	r0, r2
.endm



/******************************************************************************
 *
 * Program
 *
 ******************************************************************************/



	.TEXT
	.ALIGN 2

/******************************************************************************
 * mpp_resetchannels(...)
 *
 * Reset channel data, and any active channels linked to the layer.
 * Requires r5 = layer, r6 = channels, r7 = #channels
 ******************************************************************************/
							.thumb_func
mpp_resetchannels:

	push	{r4-r6}

	mov	r0, r6					// clear channel data to 0
	mov	r1, #MCH_SIZE/4				//
	mul	r1, r7					//
	mov	r2, #0					//
1:	stmia	r0!, {r2}				//
	sub	r1, #1					//
	bne	1b					//
	
	mov	r0, r6					// reset channel indexes
	sub	r0, #MCH_SIZE-MCH_ALLOC			//
	mov	r1, #MCH_SIZE				//
	mul	r1, r7					//
	mov	r2, #255				//
1:	strb	r2, [r0, r1]				//
	sub	r1, #MCH_SIZE				//
	bne	1b					//

	GET_MIXCH r4					// reset active channels linked to this layer
	ldr	r6,=mpp_clayer				//
	ldrb	r6, [r6]				//

#ifdef SYS_GBA						// disabled status differs between systems
	ldr	r5,=1<<31				//
#endif							//
#ifdef SYS_NDS						//
	mov	r5, #0					//
#endif							//

	ldr	r0,=mm_achannels			// r0 = achannels
	ldr	r0, [r0]				//
	ldr	r1,=mm_num_ach				// r1 = #achannels
	ldr	r1, [r1]				//
	mov	r2, #0					// r2 = 0 (for clearing)
	
.mpic_loop3:
	ldrb	r3, [r0, #MCA_FLAGS]			// test if layer matches
	lsr	r3, #6					//
	cmp	r3, r6					//
	bne	.mpic_l3_skip				//

	mov	r3, #MCA_SIZE-4				// clear achannel data to zero
.mpic_loop4:						//
	str	r2, [r0, r3]				//
	sub	r3, r3, #4				//
	bpl	.mpic_loop4				//

	str	r5, [r4]				// disable mixer channel
	
.mpic_l3_skip:
	
	add	r0, #MCA_SIZE				// increment stuff and loop
	add	r4, #MIXER_CHN_SIZE			//
							//
	sub	r1, #1					//
	bne	.mpic_loop3				//
	
	pop	{r4-r6}
	bx	lr

#ifdef SYS_NDS

/******************************************************************************
 * mm_reset_channels()
 *
 * Reset all channels.
 ******************************************************************************/
						.global mm_reset_channels
						.thumb_func
mm_reset_channels:

	push	{lr}
	
	ldr	r0,=mm_achannels		// clear active channel data
	ldr	r0, [r0]			//
	mov	r1, #0				//
	ldr	r2,=MCA_SIZE*32/4		//
						//
1:	stmia	r0!, {r1}			//
	sub	r2, #1				//
	bne	1b				//
	
	ldr	r0,=mm_pchannels		// reset channel allocation
	ldr	r0, [r0]			//
	mov	r1, #255			//
	mov	r2, #32				//
						//
1:	strb	r1, [r0, #MCH_ALLOC]		//
	add	r0, #MCH_SIZE			//
	sub	r2, #1				//
	bne	1b				//
	
	ldr	r0,=mm_schannels		// reset channel allocation
	mov	r2, #4				//
						//
1:	strb	r1, [r0, #MCH_ALLOC]		//
	add	r0, #MCH_SIZE			//
	sub	r2, #1				//
	bne	1b				//
	
	bl	mmResetEffects
	
	pop	{r3}
	bx	r3
	
#endif

/******************************************************************************
 * mpp_suspend()
 *
 * Suspend main module and associated channels.
 ******************************************************************************/
						.thumb_func
mpp_suspend:

	push	{r4,lr}
	ldr	r0,=mm_achannels
	ldr	r0, [r0]
	GET_MIXCH r1
	ldr	r2,=mm_num_ach
	ldr	r2, [r2]
	mov	r4, #0
.mpps_loop:
	ldrb	r3, [r0, #MCA_FLAGS]
	lsr	r3, #6
	bne	.mpps_next
#ifdef SYS_GBA 
	str	r4, [r1, #MIXER_CHN_FREQ]
#else
	strh	r4, [r1, #MIXER_CHN_FREQ]
	strh	r4, [r1, #MIXER_CHN_VOL]
#endif

.mpps_next:
	add	r0, #MCA_SIZE
	add	r1, #MIXER_CHN_SIZE
	sub	r2, #1
	bne	.mpps_loop
	
	pop	{r4, pc}

/******************************************************************************
 *
 * NDS System
 *
 ******************************************************************************/

//-----------------------------------------------------------------------------
#ifdef SYS_NDS
//-----------------------------------------------------------------------------

/******************************************************************************
 * mmStart( module_ID, mode )
 *
 * Start module playback.
 *
 * module_ID : Index of module.
 * mode : Playback mode.
 ******************************************************************************/
							.global mmStart
							.thumb_func
mmStart:
	
	mov	r2, #0
.mpps_backdoor:
	lsl	r0, #2
	ldr	r3,=mmModuleBank
	ldr	r3, [r3]
	ldr	r0, [r3, r0]
	
	cmp	r0, #0
	beq	1f
	add	r0, #8
	b	mmPlayModule
1:	bx	lr

/******************************************************************************
 * mmJingle( module_ID )
 *
 * Play module as jingle.
 *
 * module_ID : Index of module
 ******************************************************************************/
							.global mmJingle
							.thumb_func
mmJingle:
	
	mov	r2, #1
	mov	r1, #MPP_PLAY_ONCE
	b	.mpps_backdoor
	
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

/******************************************************************************
 *
 * GBA System
 *
 ******************************************************************************/
 
//-----------------------------------------------------------------------------
#ifdef SYS_GBA
//-----------------------------------------------------------------------------

/******************************************************************************
 * mmStart( module_ID, mode )
 *
 * Start module playback
 *
 * module_ID : id of module
 * mode : mode of playback
 ******************************************************************************/
							.global mmStart
							.thumb_func
mmStart:

	mov	r2, #0
.mpps_backdoor:
	push	{r2}
	ldr	r2,=mp_solution		@ resolve song address
	ldr	r2, [r2]
	ldrh	r3, [r2, #0]
	lsl	r3, #2
	add	r3, #12
	add	r3, r2
	lsl	r0, #2
	add	r0, r3
	ldr	r0, [r0]
	add	r0, r2
	
	pop	{r2}
	
	add	r0, #8

	b	mmPlayModule
1:	bx	lr

/******************************************************************************
 * mmJingle( module_ID )
 *
 * Start jingle playback
 *
 * module_ID : index of module
 ******************************************************************************/
							.global mmJingle
							.thumb_func
mmJingle:

	mov	r2, #1
	mov	r1, #MPP_PLAY_ONCE
	b	.mpps_backdoor

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

/******************************************************************************
 * mmPlayModule( address, mode, layer )
 *
 * Start playing module.
 ******************************************************************************/
							.global mmPlayModule
							.thumb_func
mmPlayModule:
	
	push	{lr}
	push	{r4-r7}
	
	ldr	r3,=mpp_clayer
	strb	r2, [r3]
	
	cmp	r2, #0
	bne	1f
	ldr	r5,=mmLayerMain
	@ldr	r6,=mpp_pchannels
	ldr	r6,=mm_pchannels
	ldr	r6,[r6]
	@mov	r7, #MP_MCHANNELS
	ldr	r7,=mm_num_mch
	ldr	r7,[r7]
	b	2f
1:	ldr	r5,=mmLayerSub
	ldr	r6,=mm_schannels
	mov	r7, #MP_SCHANNELS
2:
//	push	{r2}
	
	mov	r2, #MPL_MODE
	strb	r1, [r5, r2]

	mov	r4, r0
	str	r4, [r5, #MPL_SONGADR]
	
	bl	mpp_resetchannels
	
	ldrb	r3, [r4, #C_MAS_INSTN]
	ldrb	r2, [r4, #C_MAS_SAMPN]
	lsl	r3, #2
	lsl	r2, #2
	
	mov	r0, r4
	add	r0, #255
	add	r0, #C_MAS_TABLES-255
	
	str	r0, [r5, #MPL_INSTTABLE]	@ setup instrument table
	add	r0, r3
	str	r0, [r5, #MPL_SAMPTABLE]	@ setup sample table
	add	r0, r2
	str	r0, [r5, #MPL_PATTTABLE]	@ setup pattern table

	mov	r0, #0		@ set pattern to 0
	
	@ldr	r1,=mpp_setposition
	@bl	mpp_call_r1
	bl	mpp_setposition
	
	ldrb	r0, [r4, #C_MAS_TEMPO]	@ load initial tempo
	
	bl	mpp_setbpm
	
	ldrb	r0, [r4, #C_MAS_GV]	@ load initial global volume
	

	strb	r0, [r5, #MPL_GV]

	ldrb	r0, [r4, #C_MAS_FLAGS]	@ read song flags
	strb	r0, [r5, #MPL_FLAGS]	@ save
	
	lsl	r0, #32-2
	lsr	r0, #32-2+1
	strb	r0, [r5, #MPL_OLDEFFECTS]

	ldrb	r0, [r4, #C_MAS_SPEED]	@ speed
	strb	r0, [r5, #MPL_SPEED]		@ and set
	
	mov	r0, #1				// mpp_playing=true
	strb	r0, [r5, #MPL_ISPLAYING]
	
	mov	r1, #MPL_VALID			// set valid flag
	strb	r0, [r5, r1]			//

	bl	mpp_resetvars
	
	@ setup channel volumes
	mov	r0, r6
	add	r0, #MCH_CVOLUME
	mov	r3, r7
	add	r4, #C_MAS_CHANVOL
.cvol_setup:
	ldrb	r1, [r4]
	strb	r1, [r0]
	add	r0, #MCH_SIZE
	add	r4, #1
	sub	r3, #1
	bne	.cvol_setup
	
	add	r4, #32
	sub	r4, r7
	
	mov	r0, r6
	add	r0, #MCH_PANNING
	mov	r3, r7
.cpan_setup:
	ldrb	r1, [r4]
	strb	r1, [r0]
	add	r0, #MCH_SIZE
	add	r4, #1
	sub	r3, #1
	bne	.cpan_setup

//	pop	{r2}		@ <-- FIX.2 WHY WAS THIS PRESERVED
	
	pop	{r4-r7}
	pop	{r0}		@ return
	
	bx	r0		@
.pool

/******************************************************************************
 * mmPause()
 *
 * Pause module playback.
 ******************************************************************************/
							.global mmPause
							.thumb_func
mmPause:
	
	push	{lr}
	push	{r5}
	
	ldr	r5,=mmLayerMain
	
	mov	r0, #MPL_VALID
	ldrb	r0, [r5, r0]
	cmp	r0, #0
	beq	1f
	
	mov	r0, #0
	strb	r0, [r5, #MPL_ISPLAYING]
	
	bl	mpp_suspend
	
1:	pop	{r5}
	ret0

/******************************************************************************
 * mmResume()
 *
 * Resume module playback.
 ******************************************************************************/
							.global mmResume
							.thumb_func
mmResume:

	ldr	r1,=mmLayerMain
	mov	r0, #MPL_VALID
	ldrb	r0, [r1, r0]
	cmp	r0, #0
	beq	1f
	
	mov	r0, #1
	strb	r0, [r1, #MPL_ISPLAYING]
1:	bx	lr

/******************************************************************************
 * mmActive()
 *
 * Returns true if module is playing.
 ******************************************************************************/
							.global mmActive
							.thumb_func
mmActive:

	ldr	r0,=mmLayerMain
	ldrb	r0, [r0, #MPL_ISPLAYING]
	bx	lr

/******************************************************************************
 * mmActiveSub()
 *
 * Returns true if a jingle is playing.
 ******************************************************************************/
							.global mmActiveSub
							.thumb_func
mmActiveSub:
	
	ldr	r0,=mmLayerSub
	ldrb	r0, [r0, #MPL_ISPLAYING]
	bx	lr

/******************************************************************************
 * mmSetModuleVolume( volume )
 *
 * Set master module volume.
 *
 * volume : 0->1024
 ******************************************************************************/
							.global mmSetModuleVolume
							.thumb_func
mmSetModuleVolume:

@ clamp volume 0->1024
	lsr	r1, r0, #10
	beq	1f
	mov	r0, #1
	lsl	r0, #10

@ set volume
1:	ldr	r1,=mmLayerMain
	mov	r2, #MPL_VOLUME
	strh	r0, [r1, r2]
	bx	lr

/******************************************************************************
 * mmSetJingleVolume( volume )
 *
 * Set master jingle volume.
 *
 * volume : 0->1024
 ******************************************************************************/
							.global mmSetJingleVolume
							.thumb_func
mmSetJingleVolume:

@ clamp volume 0->1024
	lsr	r1, r0, #10
	beq	1f
	mov	r0, #1
	lsl	r0, #10

@ set volume
1:	ldr	r1,=mmLayerSub @mpp_layerB
	mov	r2, #MPL_VOLUME
	strh	r0, [r1, r2]
	bx	lr

/******************************************************************************
 * mppStop() [[internal function]]
 *
 * Stop module playback.
 ******************************************************************************/
							.thumb_func
mppStop:

	push	{lr}

	ldr	r0,=mpp_clayer
	ldrb	r0, [r0]
	
	cmp	r0, #0
	beq	1f
	ldr	r5,=mmLayerSub
	ldr	r6,=mm_schannels
	mov	r7, #MP_SCHANNELS
	b	2f
1:	ldr	r5,=mmLayerMain
	@ldr	r6,=mpp_pchannels
	ldr	r6,=mm_pchannels
	ldr	r6,[r6]
	@mov	r7, #MP_MCHANNELS
	ldr	r7,=mm_num_mch
	ldr	r7,[r7]
2:
	
	mov	r0, #0
	strb	r0, [r5, #MPL_ISPLAYING]
	
	mov	r1, #MPL_VALID
	strb	r0, [r5, r1]
	
	bl	mpp_resetchannels
	pop	{r0}
	bx	r0

/******************************************************************************
 * mmGetPositionTick()
 *
 * Get current number of elapsed ticks in the row being played.
 ******************************************************************************/
							.global mmGetPositionTick
							.thumb_func
mmGetPositionTick:

	ldr	r0,=mmLayerMain
	ldrb	r0, [r0, #MPL_TICK]
	bx	lr

/******************************************************************************
 * mmGetPositionRow()
 *
 * Get current row being played.
 ******************************************************************************/
							.global mmGetPositionRow
							.thumb_func
mmGetPositionRow:

	ldr	r0,=mmLayerMain
	ldrb	r0, [r0, #MPL_ROW]
	bx	lr

/******************************************************************************
 * mmGetPosition()
 *
 * Get current pattern order being played.
 ******************************************************************************/
							.global mmGetPosition
							.thumb_func
mmGetPosition:
	
	ldr	r1,=mmLayerMain
	ldrb	r0, [r1, #MPL_POSITION]
	bx	lr
	
/******************************************************************************
 * mmPosition( position )
 *
 * Set playback position
 ******************************************************************************/
							.global mmPosition
							.thumb_func
mmPosition:
	
	push	{r4-r7,lr}
	ldr	r5,=mmLayerMain
	ldr	r6,=mm_pchannels
	ldr	r6,[r6]
	ldr	r7,=mm_num_mch
	ldr	r7, [r7]
	
	push	{r0}
//	bl	mpp_resetchannels
	
	pop	{r0}
	bl	mpp_setposition

	pop	{r4-r7}
	pop	{r3}
	bx	r3
	
/******************************************************************************
 * mmSetModuleTempo( tempo )
 *
 * Set master tempo
 *
 * tempo : x.10 fixed point tempo, 0.5->2.0
 ******************************************************************************/
							.global mmSetModuleTempo
							.thumb_func
mmSetModuleTempo:
	
	push	{r5,lr}
	
	mov	r1, #1				// clamp value: 512->2048
	lsl	r1, #11				//
	cmp	r0, r1				//
	ble	1f				//
	mov	r0, r1				//
1:	mov	r1, #1				//
	lsl	r1, #9				//
	cmp	r0, r1				//
	bge	1f				//
	mov	r0, r1				//
1:
	
	ldr	r1,=mm_mastertempo
	str	r0, [r1]
	
	ldr	r5,=mmLayerMain
	ldr	r0,=mpp_clayer
	mov	r1, #0
	strb	r1, [r0]
	
	ldrb	r0, [r5, #MPL_BPM]
	cmp	r0, #0
	beq	1f
	bl	mpp_setbpm
	
1:	pop	{r5}
	pop	{r3}
	bx	r3
	
/******************************************************************************
 * mmSetModulePitch( pitch )
 *
 * Set master pitch
 *
 * pitch : x.10 fixed point value, range = 0.5->2.0
 ******************************************************************************/
							.global mmSetModulePitch
							.thumb_func
mmSetModulePitch:
	push	{r5,lr}
	
	mov	r1, #1				// clamp value: 512->2048
	lsl	r1, #11				//
	cmp	r0, r1				//
	ble	1f				//
	mov	r0, r1				//
1:	mov	r1, #1				//
	lsl	r1, #9				//
	cmp	r0, r1				//
	bge	1f				//
	mov	r0, r1				//
1:
	
	ldr	r1,=mm_masterpitch
	str	r0, [r1]
	
1:	pop	{r5}
	pop	{r3}
	bx	r3
	
.pool

//-----------------------------------------------------------------------------
#ifdef SYS_NDS7
//-----------------------------------------------------------------------------

/******************************************************************************
 * mmSetResolution( divider )
 *
 * Set update resolution
 ******************************************************************************/
							.global mmSetResolution
							.thumb_func
mmSetResolution:

	push	{r5, lr}
	
	ldr	r1,=mpp_resolution
	str	r0, [r1]
	
	ldr	r5,=mmLayerMain
	
	ldr	r0,=mpp_clayer
	mov	r1, #0
	strb	r1, [r0]
	
	ldrb	r0, [r5, #MPL_BPM]
	cmp	r0, #0
	beq	1f
	bl	mpp_setbpm
1:	ldr	r5,=mmLayerSub

	ldr	r0,=mpp_clayer
	mov	r1, #1
	strb	r1, [r0]
	
	ldrb	r0, [r5, #MPL_BPM]
	cmp	r0, #0
	beq	1f
	bl	mpp_setbpm
1:	pop	{r5}
	pop	{r3}
	bx	r3

#endif

/******************************************************************************
 * mmStop()
 *
 * Stop module playback.
 ******************************************************************************/
							.global mmStop
							.thumb_func
mmStop:

	push	{r4-r7,lr}
	ldr	r1,=mpp_clayer
	mov	r0, #0
	strb	r0, [r1]
	bl	mppStop
	pop	{r4-r7}
	ret3

/******************************************************************************
 * mpp_resetvars()
 *
 * Reset pattern variables
 * Input r5 = layer
 ******************************************************************************/
							.thumb_func
mpp_resetvars:

	mov	r0, #255
	strb	r0, [r5, #MPL_PATTJUMP]
	mov	r0, #0
	strb	r0, [r5, #MPL_PATTJUMP_ROW]
	bx	lr

/******************************************************************************
 * mpp_setbpm( bpm )
 *
 * Set BPM. bpm = 32..255
 * Input r5 = layer
 ******************************************************************************/
							.thumb_func
mpp_setbpm:
	
	strb	r0, [r5, #MPL_BPM]
	
#ifdef SYS_GBA

	ldr	r1,=mpp_clayer
	ldrb	r1, [r1]
	cmp	r1, #0
	bne	1f
	
	ldr	r1,=mm_mastertempo	// multiply by master tempo
	ldr	r1, [r1]		//
	mul	r1, r0			//
	lsr	r1, #10			//
	
	ldr	r0,=mm_bpmdv		@ samples per tick ~= mixfreq / (bpm/2.5) ~= mixfreq*2.5/bpm
	ldr	r0,[r0]
	
	swi	SWI_DIVIDE		@ SWI 07h, divide r1/r0
	lsr	r0, #1			@ multiple of two
	lsl	r0, #1			@ ---------------
	mov	r1, #MPL_TICKRATE
	strh	r0, [r5, r1]		@ 
	mov	r1, #MPL_SAMPCOUNT
//	mov	r0, #0
//	strh	r0, [r5, r1]
	bx	lr			@ return
	
1:	@ SUB LAYER, time using vsync (rate = bpm/2.5 / 59.7)
	
	lsl	r0, #15
	mov	r1, #149
	swi	SWI_DIVIDE
	mov	r1, #MPL_TICKRATE
	strh	r0, [r5, r1]
	bx	lr
	
#endif

#ifdef SYS_NDS

@ vsync = ~59.8261 HZ (says GBATEK)
@ divider = hz * 2.5 * 64

	ldr	r1,=mpp_clayer
	ldrb	r1, [r1]
	cmp	r1, #0
	bne	1f
	ldr	r1,=mm_mastertempo	// multiply by master tempo
	ldr	r1, [r1]		//
	mul	r0, r1			//
//	lsr	r1, #10			//

	lsl	r0, #16+6-10
	b	2f
1:
	lsl	r0, #16+6
2:
	@ using 60hz vsync for timing
//	lsl	r0, #16+6
	ldr	r1,=mpp_resolution
	ldr	r1, [r1]
	swi	SWI_DIVIDE
	lsr	r0, #1
	mov	r1, #MPL_TICKRATE
	strh	r0, [r5, r1]
	bx	lr
	
#endif

.pool

/******************************************************************************
 * mpp_setposition( position )
 *
 * Set sequence position.
 * Input r5 = layer
 ******************************************************************************/
							.thumb_func
mpp_setposition:
	
	push	{lr}
	
mpp_setpositionA:
	
	strb	r0, [r5, #MPL_POSITION]
	
	ldr	r1, [r5, #MPL_SONGADR]
	mov	r3, r1
	add	r1, #C_MAS_ORDER	@ get sequence entry
	ldrb	r1, [r1, r0]		@
	
	cmp	r1, #254
	bne	.mpsp_skippatt
	add	r0, #1
	b	mpp_setpositionA
.mpsp_skippatt:
	
	cmp	r1, #255
	bne	.mpsp_endsong
	
	@ END OF SONG!!! WOOPHEE!!!!
	

	mov	r0, #MPL_MODE @mpp_playmode
	ldrb	r0, [r5, r0]
	
	cmp	r0, #MPP_PLAY_ONCE
	bge	1f
@	@ its looping:
	b	3f

1:	@ its playing once:

	bl	mppStop
	mov	r0, #MPCB_SONGFINISHED
	ldr	r2,=mmCallback
	ldr	r2,[r2]
	cmp	r2, #0
	beq	3f
	ldr	r1,=mpp_clayer
	ldrb	r1, [r1]
	bl	mpp_call_r2
	
3:	
	
	ldrb	r0, [r5, #MPL_ISPLAYING]
	cmp	r0, #0
	bne	1f

	pop	{pc}

1:	
	ldr	r0, [r5, #MPL_SONGADR]	@ set position to 'restart'
	ldrb	r0, [r0, #C_MAS_REP]
	b	mpp_setpositionA
.mpsp_endsong:
	
	mov	r0, r1
	
	ldr	r1, [r5, #MPL_PATTTABLE]
	lsl	r0, #2
	
	@ r1 = pattern address( in table )
	
	ldr	r1, [r1, r0]
	add	r1, r3		@ add song address
	
	@ r1 = pattern address
	
	ldrb	r2, [r1]		@ set pattern size
	strb	r2, [r5, #MPL_NROWS]	@
	
	mov	r2, #0			@ reset tick/row
	strh	r2, [r5, #MPL_TICK]
	strb	r2, [r5, #MPL_FPATTDELAY]
	strb	r2, [r5, #MPL_PATTDELAY]

	mov	r0, #MPL_PATTREAD
	add	r1, #1
	str	r1, [r5, r0]		@ store pattern data address

	mov	r0, #MPL_PLOOP_ADR		@ reset pattern loop
	str	r1, [r5, r0]
	mov	r0, #0
	strb	r0, [r5, #MPL_PLOOP_ROW]
	strb	r0, [r5, #MPL_PLOOP_TIMES]
	
	pop	{pc}

//-----------------------------------------------------------------------------
#ifdef SYS_NDS
//-----------------------------------------------------------------------------

/******************************************************************************
 * mppUpdateLayer( layer )
 *
 * Update module layer
 ******************************************************************************/
							.thumb_func
mppUpdateLayer:

	push	{lr}
	ldr	r1,=mpp_layerp
	str	r0, [r1]
	mov	r1, #MPL_TICKRATE
	ldrh	r1, [r0, r1]	
	mov	r2, #MPL_TICKFRAC
	ldrh	r3, [r0, r2]
	lsl	r1, #1
	add	r3, r1
	strh	r3, [r0, r2]
	lsr	r3, #16
	beq	1f
2:	push	{r3}
	
	bl	mppProcessTick
	pop	{r3}
	sub	r3, #1
	bne	2b
1:	pop	{pc}

/******************************************************************************
 * mmPulse()
 *
 * NDS Work Routine
 ******************************************************************************/
						.global mmPulse
						.thumb_func
mmPulse:
		
	push	{lr}
	
	ldr	r0,=mpp_channels		// update main layer
	ldr	r1,=mm_pchannels		//
	ldr	r1,[r1]				//
	str	r1, [r0]			//
	ldr	r0,=mpp_nchannels		//
	ldr	r1,=mm_num_mch			//
	ldr	r1,[r1]				//
	strb	r1, [r0]			//
	ldr	r0,=mpp_clayer			//
	mov	r1, #0				//
	strb	r1, [r0]			//
						//
	ldr	r0,=mmLayerMain			//
	bl	mppUpdateLayer			//

	ldr	r0,=mpp_channels		// update sub layer
	ldr	r1,=mm_schannels		//
	str	r1, [r0]			//
	ldr	r0,=mpp_nchannels		//
	mov	r1, #MP_SCHANNELS		//
	strb	r1, [r0]			//
	ldr	r0,=mpp_clayer			//
	mov	r1, #1				//
	strb	r1, [r0]			//
						//
	ldr	r0,=mmLayerSub			//
	bl	mppUpdateLayer			//
	
	pop	{pc}
	
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifdef SYS_GBA
//-----------------------------------------------------------------------------

/******************************************************************************
 * mppUpdateSub()
 *
 * Update sub-module/jingle, this is bad for some reason...
 ******************************************************************************/
						.global mppUpdateSub
						.thumb_func
mppUpdateSub:
	
	ldr	r0,=mmLayerSub
	ldrb	r0, [r0, #MPL_ISPLAYING]
	cmp	r0, #0
	bne	.mppus_update
	bx	lr
.mppus_update:
	
	ldr	r0,=mpp_channels
	ldr	r1,=mm_schannels
	str	r1, [r0]
	ldr	r0,=mpp_nchannels
	mov	r1, #MP_SCHANNELS
	strb	r1, [r0]
	ldr	r0,=mpp_clayer
	mov	r1, #1
	strb	r1, [r0]
	
	push	{lr}
	ldr	r0,=mmLayerSub
	ldr	r1,=mpp_layerp
	str	r0, [r1]
	
	mov	r1, #MPL_TICKRATE
	ldrh	r1, [r0, r1]
	mov	r2, #MPL_TICKFRAC
	ldrh	r3, [r0, r2]
	lsl	r1, #1
	add	r3, r1
	strh	r3, [r0, r2]
	lsr	r3, #16
	beq	1f
2:	push	{r3}
	ldr	r1,=mppProcessTick
	bl	mpp_call_r1
	pop	{r3}
	sub	r3, #1
	bgt	2b
1:	pop	{pc}
	
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

.pool

//-----------------------------------------------------------------------------
// IWRAM CODE
//-----------------------------------------------------------------------------
#ifdef USE_IWRAM
	.SECTION ".iwram", "ax", %progbits
	.ALIGN 2
#endif
//-----------------------------------------------------------------------------

/******************************************************************************
 * mppProcessTick()
 *
 * Process module tick.
 ******************************************************************************/
.global mppProcessTick				@@@@@@@@@@@@@@@@@@
.thumb_func					@@@@     @@@     @@
						@@@  @@  @@@@@@  @@
mppProcessTick:					@@      @@@ @    @@
						@@  @@@@@@@    @@@
						@@@@@@@@@@@@@@@@@

	add	r0,pc,#0			// switch to ARM to preserve regs
	bx	r0				//
.arm						//
	stmfd	sp!, {lr}			//
	stmfd	sp!, {r4-r10}			//
	
	add	r0,pc,#1			// switch back to THUMB
	bx	r0
.thumb

	ldr	r0,=mpp_layerp			// test if module is playing
	ldr	r0, [r0]			//
	mov	r8, r0				//
	ldrb	r1, [r0, #MPL_ISPLAYING]	//
	cmp	r1, #0				//
	bne	1f				//
	ldr	r1,=.mppt_exit			//
	mov	pc,r1				//
1:

@---------------------------------------------------
@ read pattern data
@---------------------------------------------------

	ldrb	r1, [r0, #MPL_PATTDELAY]
	cmp	r1, #0
	bne	.mpp_pt_skippatternread
	ldrb	r0, [r0, #MPL_TICK]
	cmp	r0, #0
	bne	.mpp_pt_skippatternread
	
	PROF_START
	fjump2 mmReadPattern
	//bl	mpp_ReadPattern
	PROF_END 4

@-----------------------------------------	
.mpp_pt_skippatternread:
	
	mov	r0, r8
	mov	r4, #MPL_MCH_UPDATE
	ldr	r4, [r0, r4]
	
@---------------------------------------------------
@ loop through module channels
@---------------------------------------------------
	
	ldr	r7,=mpp_channels
	ldr	r7, [r7]
	mov	r0, #0
	mov	r10, r0	@ use r10 as channel counter
	mov	r0, r8
	ldrb	r0, [r0, #MPL_TICK]
	cmp	r0, #0
	bne	pchannel_loop_other
	
@---------------------------------------------------
pchannel_loop_first:
@---------------------------------------------------	
	lsr	r4, #1
	bcc	pchannel_empty
	fjump2	mmUpdateChannel_T0
pchannel_empty:
	mov	r0, #1
	add	r10, r0
	add	r7, #MCH_SIZE
	cmp	r4, #0
	bne	pchannel_loop_first
	b	pchannel_loop_finished
	
@---------------------------------------------------
pchannel_loop_other:
@---------------------------------------------------
	lsr	r4, #1
	bcc	pchannel_empty2
	#ifdef FOO_UC
	bl	mpp_Update_Channel
	#else
	fjump2	mmUpdateChannel_TN
	#endif
	
pchannel_empty2:
	mov	r0, #1
	add	r10, r0
	add	r7, #MCH_SIZE
	cmp	r4, #0
	bne	pchannel_loop_other
	
pchannel_loop_finished:
	
@---------------------------------------------------
@ loop through active channels
@---------------------------------------------------
	PROF_START
	ldr	r6,=mm_achannels
	ldr	r6, [r6]
	@ldr	r6,=mpp_achannels
	mov	r4, #0
	
@---------------------------------------------------	
.mpp_pt_achn_loop:
@---------------------------------------------------

	ldrb	r0, [r6, #MCA_TYPE]
	cmp	r0, #ACHN_DISABLED
	beq	.mpp_pt_achn_disabled
	ldr	r0,=mpp_clayer
	ldrb	r0, [r0]
	ldrb	r1, [r6, #MCA_FLAGS]
	lsr	r1, #6
	cmp	r1, r0
	bne	.mpp_pt_achn_next
	
	ldr	r1,=mpp_vars
	ldrb	r0, [r6, #MCA_VOLUME]
	strb	r0, [r1, #MPV_AFVOL]
	mov	r0, #0
	strh	r0, [r1, #MPV_PANPLUS]
	
	ldr	r5, [r6, #MCA_PERIOD]
	
	bl	mpp_Update_ACHN
	
	b	.mpp_pt_achn_next
	
@---------------------------------------------------	
.mpp_pt_achn_disabled:
@	mov	r0, r4
@	bl	mp_Mixer_StopChannel
	
@---------------------------------------------------
.mpp_pt_achn_next:
@---------------------------------------------------

	ldrb	r0, [r6, #MCA_FLAGS]
	mov	r1, #MCAF_UPDATED
	bic	r0, r1
	strb	r0, [r6, #MCA_FLAGS]
	add	r6, #MCA_SIZE
	add	r4, #1
	ldr	r0,=mm_num_ach
	ldr	r0,[r0]
	
	cmp	r4, r0
@	cmp	r4, #MP_NCHANNELS
	bne	.mpp_pt_achn_loop
@---------------------------------------------------
	PROF_END 6
	
	
	
	ldr	r1,=mppProcessTick_incframe
	mov	pc,r1

.pool

//-----------------------------------------------------------------------------
// TEXT Code
//-----------------------------------------------------------------------------
	.TEXT
	.THUMB
	.ALIGN 2
//-----------------------------------------------------------------------------

/******************************************************************************
 * mppProcessTick_incframe [[internal]]
 ******************************************************************************/
						.thumb_func
mppProcessTick_incframe:

@---------------------------------------------------
@ update tick/row/position
@---------------------------------------------------
	
	mov	r5, r8				@ get tick#
	ldrb	r1, [r5, #MPL_TICK]		@ ..
	add	r1, #1				@ increment
	
	ldrb	r2, [r5, #MPL_SPEED]		@ compare with speed
	cmp	r1, r2				@ ..
	blt	.mppt_continuerow		@ if less than, continue this row
	
	ldrb	r2, [r5, #MPL_FPATTDELAY]
	cmp	r2, #0
	beq	.mppt_nofpd
	sub	r2, #1
	strb	r2, [r5, #MPL_FPATTDELAY]
	b	.mppt_continuerow
.mppt_nofpd:
	
	mov	r1, #0				@  .. otherwise clear tick count
	b	.mppt_nextrow			@  and advance to next row
	
.mppt_continuerow:				@ continue current row:
	strb	r1, [r5, #MPL_TICK]		@ save tick#
	b	.mppt_exit			@ exit
	
.mppt_nextrow:						@ advance row
	
	ldrb	r2, [r5, #MPL_PATTDELAY]
	cmp	r2, #0
	beq	.mppt_nopd
	sub	r2, #1
	strb	r2, [r5, #MPL_PATTDELAY]
	beq	.mppt_nopd
	b	.mppt_continuerow
.mppt_nopd:
	
	strb	r1, [r5, #MPL_TICK]			@ save tick# (0)
	ldrb	r1, [r5, #MPL_PATTJUMP]
	cmp	r1, #255
	beq	.mppt_no_pj
	
	mov	r2, #255
	strb	r2, [r5, #MPL_PATTJUMP]
	mov	r0, r1
	bl	mpp_setposition
	
	ldrb	r1, [r5, #MPL_PATTJUMP_ROW]
	cmp	r1, #0
	beq	.mppt_pj_no_offset
	mov	r2, #0
	strb	r2, [r5, #MPL_PATTJUMP_ROW]
	bl	mpph_FastForward
.mppt_pj_no_offset:
	
	b	.mppt_exit
	
.mppt_no_pj:
	
	mov	r3, #MPL_PLOOP_JUMP
	ldrb	r1, [r5, r3]
	cmp	r1, #0
	beq	.mppt_no_ploop
	mov	r1, #0
	strb	r1, [r5, r3]
	ldrb	r1, [r5, #MPL_PLOOP_ROW]
	strb	r1, [r5, #MPL_ROW]
	mov	r3, #MPL_PLOOP_ADR
	ldr	r1, [r5, r3]

	mov	r3, #MPL_PATTREAD
	str	r1, [r5, r3]
	b	.mppt_exit
.mppt_no_ploop:
	ldrb	r1, [r5, #MPL_ROW]			@ ..
	add	r1, #1					@ increment
	ldrb	r2, [r5, #MPL_NROWS]			@ 
	add	r2, #1
	cmp	r1, r2					@ check with #rows for pattern
	bne	.mppt_continuepattern			@ if !=, then continue playing this pattern
	
.mppt_nextposition:					@ advance position
	
	ldrb	r0, [r5, #MPL_POSITION]			@ increment position
	add	r0, #1
	
	bl	mpp_setposition
	b	.mppt_exit
	
.mppt_continuepattern:
	
	strb	r1, [r5, #MPL_ROW]			@ save row count
	
.thumb_func
.mppt_exit:
	
	@ switch to arm
	ldr	r0,=.mppt_exita
	bx	r0
.arm
.align 2
.mppt_exita:

	ldmfd	sp!, {r4-r10}
	ldmfd	sp!, {r0}
	bx	r0
.thumb

.pool

//-----------------------------------------------------------------------------
// IWRAM CODE
//-----------------------------------------------------------------------------
#ifdef USE_IWRAM
	.SECTION ".iwram", "ax", %progbits
	.ALIGN 2
#endif
//-----------------------------------------------------------------------------

/******************************************************************************
 * mpp_Channel_NewNote()
 *
 * Process new note.
 * Input r7 = pchannel address
 ******************************************************************************/
						.global mpp_Channel_NewNote
						.thumb_func
mpp_Channel_NewNote:
	
	@ r7 = pchannel address
	push	{r4,lr}

	ldrb	r0, [r7, #MCH_INST]		@ get instrument#
	sub	r0, #1
	bcc	.mppt_skipnna

	bl	mpp_Channel_GetACHN
	cmp	r6, #0
	beq	.mppt_alloc_channel	
	
	ldrb	r0, [r7, #MCH_INST]		@ get instrument#
	sub	r0, #1
	
	mpp_InstrumentPointer
	
	ldrb	r1, [r7, #MCH_BFLAGS]		@ fetch NNA
	lsr	r1, #6
	
	beq	.mppt_NNA_CUT			@ skip if zero
	
	ldrb	r1, [r0, #C_MASI_DCT]		@ otherwise check duplicate check type
	lsl	r1, #32-2
	lsr	r1, #32-2
	lsl	r1, #1				@ jump to mppt_DCT_TABLE[dct]
	add	r1, pc
	mov	pc, r1

.mppt_DCT_TABLE:
b	.mppt_DCNA
b	.mppt_DCT_NOTE
b	.mppt_DCT_SAMP
b	.mppt_DCT_INST
.mppt_DCT_NOTE:					@ DCT note ---------------------
	ldrb	r1, [r7, #MCH_PNOTE]		@ get pattern note
	lsl	r1, #1				@ translate to real note
	add	r1, #C_MASI_MAP			@ with note/sample map
	ldrb	r1, [r0, r1]			@ r1 = real note
	ldrb	r2, [r7, #MCH_NOTE]		@ compare with last note
	cmp	r1, r2				@
	beq	.mppt_DCA			@ equal? perform DCA
	b	.mppt_DCNA			@ otherwise skip
	
.mppt_DCT_SAMP:					@ DCT sample -------------------
	
	// **WARNING: code not functional with new instrument table
	ldrb	r1, [r7, #MCH_PNOTE]		@ get pattern note
	lsl	r1, #1				@ translate to sample#
	add	r1, #C_MASI_MAP+1		@ with note/sample map
	ldrb	r1, [r0, r1]			@ r1 = sample#
	ldrb	r2, [r6, #MCA_SAMPLE]		@ compare with achn's sample
	cmp	r1, r2				@
	beq	.mppt_DCA			@ equal? perform DCA
	b	.mppt_DCNA			@ otherwise skip
	
.mppt_DCT_INST:					@ DCT instrument ---------------
	ldrb	r1, [r7, #MCH_INST]		@ load instrument
	ldrb	r2, [r6, #MCA_INST]		@ compare with active inst
	cmp	r1, r2				@
	bne	.mppt_DCNA			@ not equal? skip DCA
	
.mppt_DCA:					@ DUPLICATE CHECK ACTION -------
	ldrb	r1, [r0, #C_MASI_DCA]		@ read type
	cmp	r1, #IT_DCA_CUT			@ cut?
	beq	.mppt_NNA_CUT			@   branch
	cmp	r1, #IT_DCA_OFF			@ note-off?
	beq	.mppt_NNA_OFF			@   branch
	b	.mppt_NNA_FADE			@ note-fade otherwise
	
.mppt_DCNA:
	
	ldrb	r1, [r7, #MCH_BFLAGS]
	lsr	r1, #6				@ get NNA
	lsl	r1, #1				@ and jump to
	add	r1, pc				@ NNA_TABLE[NNA]
	mov	pc, r1
.mppt_NNA_TABLE:
b	.mppt_NNA_CUT
b	.mppt_NNA_CONTINUE
b	.mppt_NNA_OFF
b	.mppt_NNA_FADE
	
@---------------------------------------------------------------------------------
.mppt_NNA_CUT:
@---------------------------------------------------------------------------------
	
	#ifdef SYS_NDS				// nds supports volume ramping
	
	ldrb	r1, [r6, #MCA_TYPE]
	cmp	r1, #0
	BEQ	.mppt_samechannel
	mov	r1, #ACHN_BACKGROUND
	strb	r1, [r6, #MCA_TYPE]
	mov	r1, #0
	strb	r1, [r6, #MCA_VOLUME]
	b	.mppt_NNA_FINISHED
	
	#else
	b	.mppt_samechannel
	#endif
	
@---------------------------------------------------------------------------------
.mppt_NNA_CONTINUE:
@---------------------------------------------------------------------------------
	
	mov	r1, #ACHN_BACKGROUND	@ use different channel
	strb	r1, [r6, #MCA_TYPE]	@ set active channel to "background"
	b	.mppt_NNA_FINISHED	@ finished

@---------------------------------------------------------------------------------
.mppt_NNA_OFF:
@---------------------------------------------------------------------------------

	ldrb	r1, [r6, #MCA_FLAGS]	@ clear KEYON in flags byte
	mov	r2, #MCAF_KEYON
	bic	r1, r2

	strb	r1, [r6, #MCA_FLAGS]
	mov	r1, #ACHN_BACKGROUND	@ set type to "background"
	strb	r1, [r6, #MCA_TYPE]
	b	.mppt_NNA_FINISHED	@ finished

@---------------------------------------------------------------------------------
.mppt_NNA_FADE:
@---------------------------------------------------------------------------------

	ldrb	r1, [r6, #MCA_FLAGS]	@ set NOTE FADE in flags byte
	mov	r2, #MCAF_FADE
	orr	r1, r2
	strb	r1, [r6, #MCA_FLAGS]
	mov	r1, #ACHN_BACKGROUND	@ set type to "background"
	strb	r1, [r6, #MCA_TYPE]	@
	
.mppt_NNA_FINISHED:
	
.mppt_alloc_channel:

	mov	r4, r6

	ldr	r1,=mmAllocChannel
	jump1				@ find new active channel
	strb	r0, [r7, #MCH_ALLOC]	@ save number
	
	#ifdef SYS_NDS
	
	cmp	r4, #0
	beq	.mppt_samechannel

	mov	r1, #MCA_SIZE		@ copy data from previous channel
	mul	r0, r1			@ (for volume ramping wierdness)
	ldr	r1,=mm_achannels	@
	ldr	r1,[r1]			@
	add	r0, r1			@
					@
	mov	r2, #MCA_SIZE/4
1:	ldmia	r4!, {r1}
	stmia	r0!, {r1}
	sub	r2, #1
	bne	1b
	
/*	ldr	r1, [r4, #MCA_FADE]	@
	str	r1, [r0, #MCA_FADE]	@
	ldr	r1, [r4, #MCA_ENVC_PAN]	@
	str	r1, [r0, #MCA_ENVC_PAN]	@
	ldr	r1, [r4, #MCA_AVIB_DEP]	@
	str	r1, [r0, #MCA_AVIB_DEP]	@
	ldrb	r1, [r4, #MCA_FLAGS]	@
	strb	r1, [r0, #MCA_FLAGS]	@
	ldrb	r1, [r4, #MCA_VOLUME]
	strb	r1, [r0, #MCA_VOLUME]
	ldrb	r1, [r4, #MCA_PANNING]
	strb	r1, [r0, #MCA_PANNING]
	ldrb	r1, [r4, #MCA_SAMPLE]
	strb	r1, [r0, #MCA_SAMPLE]
	ldrb	r1, [r4, #MCA_INST]
	strb	r1, [r0, #MCA_INST]*/
	
	#endif
	
.mppt_samechannel:
	
.mppt_skipnna:
	pop	{r4}
	pop	{r3}
	bx	r3
	
.pool

.align 2
.thumb_func
@------------------------------------------------------------------------------------------------------
mpp_Channel_GetACHN:
@------------------------------------------------------------------------------------------------------
	
	@ gets the active channel pointer
	@ and stores in r6
	@ gives 0 if N/A
	
	ldrb	r0, [r7, #MCH_ALLOC]
	cmp	r0, #255
	bge	1f
	ldr	r6,=mm_achannels
	ldr	r6,[r6]
	mov	r1, #MCA_SIZE
	mul	r0, r1
	add	r6, r0
	bx	lr
	
1:	mov	r6, #0
	bx	lr
.pool

.align 2
.thumb_func
@----------------------------------------------------------------------------------------------------
mpp_Update_ACHN:
@----------------------------------------------------------------------------------------------------
		
@ r5 = affected period
@ r6 = achannel address

	push	{lr}			@ enter subroutine
	
@ check updated flag & exit if already updated
	
	ldrb	r0, [r6, #MCA_FLAGS]
	mov	r1, #MCAF_UPDATED
	tst	r0, r1
	beq	.mpp_achn_update
	pop	{pc}

@--------------------------------------------

	.global mpp_Update_ACHN_notest
	.thumb_func
@----------------------------------------------------------------------------------------------------
mpp_Update_ACHN_notest:
@----------------------------------------------------------------------------------------------------
	push	{lr}
	
.mpp_achn_update:
	
@------------------------------------------------------------------------
@ Envelope Processing
@------------------------------------------------------------------------
	
	ldrb	r0, [r6, #MCA_INST]
	sub	r0, #1
	bCS	1f
	b	.mppt_achn_noinst
1:	mpp_InstrumentPointer

@ get envelope flags
	
	mov	r1, r0
	ldrb	r2, [r1, #C_MASI_ENVFLAGS]
	add	r1, #C_MASI_ENVELOPES
	
	lsr	r2, #1				@ shift out volume envelope bit
	bcc	.mppt_no_volenv

	ldrb	r3, [r6, #MCA_FLAGS]
	lsr	r3, #6
	
	bcs	.mppt_achn_ve_enabled
	ldrb	r0, [r1]
	add	r1, r0
	b	.mppt_no_volenv
.mppt_achn_ve_enabled:
	
	push	{r1, r2}
	
	ldrh	r0, [r6, #MCA_ENVC_VOL]
	mov	r2, r1
	ldrb	r1, [r6, #MCA_ENVN_VOL]
	bl	mpph_ProcessEnvelope
	strb	r1, [r6, #MCA_ENVN_VOL]
	strh	r0, [r6, #MCA_ENVC_VOL]
	mov	r1, r3
	
	cmp	r2, #1
	bne	.mpph_volenv_notend
	ldrb	r0, [r6, #MCA_FLAGS]
	
	mov	r3, r8				@ stupid xm doesn't fade out at envelope end
	ldrb	r3, [r3, #MPL_FLAGS]
	lsr	r3, #C_FLAGS_XS
	
	mov	r3, #MCAF_ENVEND
	bcs	1f
	mov	r3, #MCAF_ENVEND+MCAF_FADE
1:
	orr	r0, r3
	strb	r0, [r6, #MCA_FLAGS]
.mpph_volenv_notend:

	cmp	r2, #1
	blt	.mpph_volenv_normal
	
	@ check keyon and turn on fade...
	ldrb	r0, [r6, #MCA_FLAGS]
	mov	r2, #MCAF_KEYON
	tst	r0, r2
	bne	.mpph_volenv_normal
.mpph_volenv_notefade:
	mov	r2, #MCAF_FADE
	orr	r0, r2
	strb	r0, [r6, #MCA_FLAGS]
	
.mpph_volenv_normal:
	
	ldr	r0,=mpp_vars
	ldrb	r2, [r0, #MPV_AFVOL]
	mul	r2, r1
	lsr	r2, #6+6
	strb	r2, [r0, #MPV_AFVOL]
	pop	{r1, r2}
	ldrb	r0, [r1]
	add	r1, r0
	b	.mppt_has_volenv
.mppt_no_volenv:
	
	ldrb	r0, [r6, #MCA_FLAGS]
	mov	r3, #MCAF_ENVEND
	orr	r0, r3
	mov	r3, #MCAF_KEYON
	tst	r0, r3
	bne	.mppt_has_volenv
	mov	r3, #MCAF_FADE
	orr	r0, r3
	strb	r0, [r6, #MCA_FLAGS]
	
	mov	r0, r8		@ check XM MODE and cut note
	ldrb	r0, [r0, #MPL_FLAGS]
	lsr	r0, #C_FLAGS_XS
	bcc	.mppt_has_volenv
	mov	r0, #0
	strh	r0, [r6, #MCA_FADE]
.mppt_has_volenv:
	
	lsr	r2, #1
	bcc	.mppt_no_panenv
	push	{r1, r2}
	ldrh	r0, [r6, #MCA_ENVC_PAN]
	mov	r2, r1
	ldrb	r1, [r6, #MCA_ENVN_PAN]
	bl	mpph_ProcessEnvelope
	strb	r1, [r6, #MCA_ENVN_PAN]
	strh	r0, [r6, #MCA_ENVC_PAN]
	mov	r1, r3

	ldr	r0,=mpp_vars
	mov	r3, #MPV_PANPLUS
	ldrsh	r2, [r0,r3]
	lsr	r1, #4
	sub	r1, #128
	add	r2, r1
	strh	r2, [r0,r3]
	pop	{r1, r2}
.mppt_no_panenv:
	
	lsr	r2, #1
	bcc	.mppt_no_pitchenv
	ldrb	r0, [r1, #C_MASIE_FILTER]
	cmp	r0, #0
	bne	.mppt_no_pitchenv
	push	{r1, r2}
	ldrh	r0, [r6, #MCA_ENVC_PIC]
	mov	r2, r1
	ldrb	r1, [r6, #MCA_ENVN_PIC]
	bl	mpph_ProcessEnvelope
	strb	r1, [r6, #MCA_ENVN_PIC]
	strh	r0, [r6, #MCA_ENVC_PIC]
	mov	r1, r3
	lsr	r1, #3
	sub	r1, #255
	mov	r0, r5
	sub	r1, #1
	bmi	.mppt_pitchenv_minus
#ifdef USE_IWRAM
	ldr	r2,=mpph_LinearPitchSlide_Up
	jump2
#else
	bl	mpph_LinearPitchSlide_Up
#endif
	b	.mppt_pitchenv_plus
.mppt_pitchenv_minus:
	neg	r1, r1
#ifdef USE_IWRAM
	ldr	r2,=mpph_LinearPitchSlide_Down
	jump2
#else
	bl	mpph_LinearPitchSlide_Down
#endif
.mppt_pitchenv_plus:
	mov	r5, r0
	pop	{r1, r2}
.mppt_no_pitchenv:
	
	ldrb	r0, [r6, #MCA_FLAGS]
	mov	r1, #MCAF_FADE
	tst	r0, r1
	beq	.mppt_achn_nofade
	ldrb	r0, [r6, #MCA_INST]
	sub	r0, #1
	
	mpp_InstrumentPointer
	ldrb	r0, [r0, #C_MASI_FADE]

	ldrh	r1, [r6, #MCA_FADE]

	sub	r1, r0
	bcs	.mppt_achn_fadeout_clip
	mov	r1, #0
.mppt_achn_fadeout_clip:
	strh	r1, [r6, #MCA_FADE]

.mppt_achn_nofade:
	
.mppt_achn_keyon:
	

@----------------------------------------------------------------------------------
@ *** PROCESS AUTO VIBRATO
@----------------------------------------------------------------------------------
	
	ldrb	r0, [r6, #MCA_SAMPLE]
	sub	r0, #1
	bcc	.mppt_achn_nostart	@ no sample!!
	
	@bl	mpp_SamplePointer
	mpp_SamplePointer
	ldrh	r1, [r0, #C_MASS_VIR]	@ get av-rate
	cmp	r1, #0			@ 0?
	beq	.mppt_av_disabled	@  if 0 then its disabled
	ldrh	r2, [r6, #MCA_AVIB_DEP]	@ get depth counter
	add	r2, r1			@ add rate
	lsr	r1, r2, #15		@ check for 15-bit overflow
	beq	.mppt_av_depclip	@ ..
	
	ldr	r2,=32768		@ and clamp to 32768
.mppt_av_depclip:			
	strh	r2, [r6, #MCA_AVIB_DEP]	@ save depth counter
	ldrb	r1, [r0, #C_MASS_VID]	@ get av-depth
	mul	r1, r2			@ multiply

	ldrb	r3, [r6, #MCA_AVIB_POS]	@ get table position
	ldrb	r2, [r0, #C_MASS_VIS]	@ get av-speed
	add	r3, r2			@ add to position
	lsl	r3, #32-8		@ wrap position to 0->255
	lsr	r3, #32-8		@ ..
	strb	r3, [r6, #MCA_AVIB_POS]		@ save position
	ldr	r2,=mpp_TABLE_FineSineData	@ get table pointer
	ldrsb	r2, [r2, r3]			@ load table value at position
	mul	r2, r1				@ multiply with depth
	asr	r2, #23				@ shift value
	bmi	.mppt_av_minus			@ and perform slide...
.mppt_av_plus:					@ --slide up
	mov	r1, r2				@ r1 = slide value
	mov	r0, r5				@ r0 = frequency
#ifdef USE_IWRAM
	fjump2	mpph_PitchSlide_Up
#else
	bl	mpph_PitchSlide_Up		@ pitch slide
#endif
	b	.mppt_av_finished		@
.mppt_av_minus:					@ --slide down
	neg	r1, r2				@ r1 = slide value
	mov	r0, r5				@ r0 = frequency
#ifdef USE_IWRAM
	ldr	r2,=mpph_PitchSlide_Down
	jump2
#else
	bl	mpph_PitchSlide_Down		@ pitch slide
#endif

.mppt_av_finished:
	mov	r5, r0				@ affect frequency
.mppt_av_disabled:
	
@---------------------------------------------------------------------------------
	
.mppt_achn_noinst:
	
	push	{r4}
	mov	r0, #MIXER_CHN_SIZE
	mul	r4, r0
	@ldr	r0,=mp_channels
@	ldr	r0,=mm_mixchannels
@	ldr	r0,[r0]
	GET_MIXCH r0
	add	r4, r0
	
	@ *** UPDATE MIXING INFORMATION
	
	ldrb	r0, [r6, #MCA_FLAGS]		@ read flags
	mov	r1, #MCAF_START			@ test start bit
	tst	r0, r1				@ ..
	beq	.mppt_achn_nostart		@
.mppt_achn_start:				@ START NOTE
	bic	r0, r1				@ clear bit
	strb	r0, [r6, #MCA_FLAGS]		@ save flags
	ldrb	r0, [r6, #MCA_SAMPLE]		@ get sample #

	sub	r0, #1				@ ..
	bcc	.mppt_achn_nostart		@ quit if invalid
	@bl	mpp_SamplePointer		@ get sample address
	mpp_SamplePointer
	
	ldrh	r3, [r0, #C_MASS_MSLID]
	
	add	r1,r3,#1			@ msl id == 0xFFFF?
	lsr	r1, #17
	
	bcc	.mppt_achn_msl_sample 

.mppt_achn_direct_sample:			@ then sample follows
	
	add	r0, #12
	
//------------------------------------------------
#ifdef SYS_GBA
//	ldr	r1, [r0,#C_SAMPLE_LEN]	@ setup mixer (GBA)
//	lsl	r1, #MP_SAMPFRAC
//	str	r1, [r4,#MIXER_CHN_LEN]
//	ldr	r1, [r0,#C_SAMPLE_LOOP]
//	str	r1, [r4,#MIXER_CHN_LOOP]
	add	r0, #C_SAMPLE_DATA
	str	r0, [r4,#MIXER_CHN_SRC]
	
#else
//-------------------------------------------

	ldr	r1,=0x2000000
	sub	r0, r1
	str	r0, [r4, #MIXER_CHN_SAMP]
	ldrb	r1, [r4, #MIXER_CHN_CNT]
	mov	r0, #MIXER_CF_START
	orr	r1, r0
	strb	r1, [r4, #MIXER_CHN_CNT]
	
#endif
//-------------------
	
	b	.mppt_achn_gotsample
.mppt_achn_msl_sample:				@ otherwise get from solution

	#ifdef SYS_GBA

	ldr	r2,=mp_solution
	ldr	r2, [r2]
	mov	r1, r2
	add	r1, #12
	lsl	r3, #2
	ldr	r1, [r1, r3]
	add	r1, #8
	add	r0, r1, r2
	
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ NOTICE, USE LDM HERE
	
//	ldr	r1, [r0,#C_SAMPLE_LEN]	@ setup mixer (GBA)
//	lsl	r1, #MP_SAMPFRAC
//	str	r1, [r4,#MIXER_CHN_LEN]
//	ldr	r1, [r0,#C_SAMPLE_LOOP]
//	str	r1, [r4,#MIXER_CHN_LOOP]
	add	r0, #C_SAMPLE_DATA
	str	r0, [r4,#MIXER_CHN_SRC]

	#endif

	#ifdef SYS_NDS
			
	ldr	r2,=mmSampleBank	@ get samplebank pointer
	ldr	r2, [r2]
	lsl	r3, #2		@ add msl_id *4
	ldr	r1, [r2, r3]
	
	lsl	r1, #8		@ mask out counter value
	lsr	r1, #8
	add	r1, #8

	str	r1, [r4,#MIXER_CHN_SAMP]
	ldrb	r1, [r4,#MIXER_CHN_CNT]		// read control		**CNT was cleared, no need to read it
	mov	r0, #MIXER_CF_START		// set start bit
	orr	r1, r0
	strb	r1, [r4,#MIXER_CHN_CNT]		// save control

	#endif
	
.mppt_achn_gotsample:
	ldr	r1,=mpp_vars
	ldrb	r1, [r1, #MPV_SAMPOFF]
#ifdef SYS_GBA
	lsl	r1, #MP_SAMPFRAC+8
	str	r1, [r4, #MIXER_CHN_READ]
#else
	str	r1, [r4, #MIXER_CHN_READ]
#endif
	
.mppt_achn_nostart:
	@ set pitch
	
	ldrb	r0, [r6, #MCA_SAMPLE]		@ get sample#
	sub	r0, #1				@ ..
	bcc	.mppt_achn_setvolumeA
	@bl	mpp_SamplePointer		@ quit if invalid
	mpp_SamplePointer
	push	{r0}
	
	mov	r1, r8
	ldrb	r1, [r1, #MPL_FLAGS]
	lsr	r1, #C_FLAGS_SS
	bcc	.mppt_achn_amigafreqs
.mppt_achn_linearfreqs:
	
	ldrh	r0, [r0, #C_MASS_FREQ]		@ get C5SPEED
	LSL	R0, #2
	lsr	r1, r5, #8			@ do some stuff...
	mul	r1, r0				@ ... period * freq?
	
	lsr	r1, #8
	
	ldr	r0,=mpp_clayer
	ldrb	r0, [r0]
	cmp	r0, #0
	bne	1f
	ldr	r0,=mm_masterpitch
	ldr	r0, [r0]
	mul	r1, r0
	lsr	r1, #10
1:

	#ifdef SYS_GBA
	
	
//	ldr	r0,=mm_freqscalar
//	ldr	r0, [r0]
	ldr	r0,=(4096*65536)/15768
	mul	r0, r1
	lsr	r0, #16
	str	r0, [r4, #MIXER_CHN_FREQ]
	
	#else

	ldr	r0,=MIXER_SCALE
	mul	r0, r1
	lsr	r0, #16+1
	strh	r0, [r4, #MIXER_CHN_FREQ]
	
	//strh	r1, [r4, #MIXER_CHN_FREQ]
	#endif
	
	b	.mppt_achn_setvolume
	
.mppt_achn_amigafreqs:
	ldr	r0,=MOD_FREQ_DIVIDER_PAL
	movs	r1, r5
	
	beq	.mppt_achn_setvolume		@ 0 is a bad period
	swi	SWI_DIVIDE
	
	ldr	r1,=mpp_clayer
	ldrb	r1, [r1]
	cmp	r1, #0
	bne	1f
	ldr	r1,=mm_masterpitch
	ldr	r1, [r1]
	mul	r0, r1
	lsr	r0, #10
1:

	#ifdef SYS_GBA

//	ldr	r1,=mm_freqscalar
//	ldr	r1,[r1]
	ldr	r1,=(4096*65536)/15768
	mul	r0, r1
	lsr	r0, #16

	str	r0, [r4, #MIXER_CHN_FREQ]
	#else
//	mov	r1, r0
//	ldr	r0,=16756991			@ calculate ds mixer timing
//	swi	SWI_DIVIDE
//	neg	r0,r0
	//lsr	r0, #5
	ldr	r1,=MIXER_SCALE
	mul	r0, r1
	lsr	r0, #16+1
	strh	r0, [r4, #MIXER_CHN_FREQ]
	#endif

@----------------------------------------------------------------------------------------------------
.mppt_achn_setvolume:
@----------------------------------------------------------------------------------------------------
	
	@ set volume
	
	pop	{r0}
						@ <-- stepped oct 28, 3:27pm 
	ldrb	r3, [r0, #C_MASS_GV]		@ SV, 6-bit
	ldrb	r0, [r6, #MCA_INST]
	sub	r0, #1
	bcs	1f
	
.thumb_func
.mppt_achn_setvolumeA:	
	mov	r1, #0
	b	.mppt_achn_badinstrument
1:	
	mpp_InstrumentPointer
	ldrb	r0, [r0, #C_MASI_GVOL]	@ IV, 7-bit
	mul	r3, r0
	
	ldr	r1,=mpp_vars
	ldrb	r0, [r1, #MPV_AFVOL]	@ ((CV*VOL)/32*VEV/64) 7-bit
	
	mul	r3, r0
	
	mov	r1, r8			@ get global vollume
	ldrb	r0, [r1, #MPL_FLAGS]
	lsr	r0, #4
	ldrb	r0, [r1, #MPL_GV]	@ ..		     7-bit
	
	bcc	1f
	lsl	r0, #1			@ xm mode global volume is only 0->64, shift to 0->128
	
1:	mul	r3, r0			@ multiply
	
	lsr	r3, #10
	
	ldrh	r0, [r6, #MCA_FADE]
	
	
	mul	r3, r0
	
	lsr	r3, r3, #10
	
	mov	r0, r8
	mov	r1, #MPL_VOLUME
	ldrh	r0, [r0, r1]
	mul	r3, r0
	
//------------------------------------------------
#ifdef SYS_NDS
	lsr	r1, r3, #19-3-5 ///#19-3	(new 16-bit levels!)
	ldr	r3,=65535  //2047
	cmp	r1, r3				@ clip values over 255
	blt	1f
	mov	r1, r3
1:
.mppt_achn_badinstrument:
//	lsr	r3, r1, #3		(new 16-bit levels!)
	lsr	r3, r1, #8		
	strb	r3, [r6, #MCA_FVOL]

#else

	lsr	r1, r3, #19
	cmp	r1, #255			@ clip values over 255
	blt	1f
	mov	r1, #255
1:
.mppt_achn_badinstrument:
	strb	r1, [r6, #MCA_FVOL]

#endif

	cmp	r1, #0
	bne	.mppt_achn_audible
	
	#ifdef SYS_NDS			// nds has volume ramping!
	
	ldrb	r3, [r6, #MCA_TYPE]
	cmp	r3, #ACHN_BACKGROUND
	bne	1f
	ldrb	r3, [r6, #MCA_VOLUME]
	cmp	r3, #0
	bne	1f
	ldrh	r3, [r4, #MIXER_CHN_CVOL]
	cmp	r3, #0
	beq	.mppt_achn_not_audible
	#endif
	
1:	ldrb	r3, [r6, #MCA_FLAGS]
	mov	r2, #MCAF_ENVEND
	tst	r3, r2
	beq	.mppt_achn_audible
	mov	r2, #MCAF_KEYON
	tst	r3, r2
	bne	.mppt_achn_audible
	
//	#ifdef SYS_NDS
//	ldrh	r3, [r4, #MIXER_CHN_CVOL]	// nds has volume ramping!!
//	cmp	r3, #0
//	bne	.mppt_achn_audible
//	#endif

.mppt_achn_not_audible:
	@ STOP CHANNEL

#ifdef SYS_GBA
	ldr	r0,=1<<31
	str	r0,[r4,#MIXER_CHN_SRC]	@ stop mixer channel
#else
	mov	r0,#0
	str	r0,[r4,#MIXER_CHN_SAMP]	@ stop mixer channel
#endif

	ldrb	r3, [r6, #MCA_TYPE]
	cmp	r3, #ACHN_FOREGROUND
	bne	.mpp_achn_noparent
	ldrb	r1, [r6, #MCA_PARENT]
	mov	r3, #MCH_SIZE
	mul	r1, r3
	ldr	r0,=mpp_channels
	ldr	r0, [r0]
	add	r0, r1
	mov	r1, #255
	strb	r1, [r0, #MCH_ALLOC]
.mpp_achn_noparent:
	
	mov	r1, #ACHN_DISABLED
	strb	r1, [r6, #MCA_TYPE]
	b	.mpp_achn_updated
.mppt_achn_audible:
	
#ifdef SYS_NDS
	
	strh	r1, [r4, #MIXER_CHN_VOL]
	
#else
	
	strb	r1, [r4, #MIXER_CHN_VOL]
	
#endif
	
#ifdef SYS_GBA						// check if mixer channel has ended

	ldr	r0, [r4, #MIXER_CHN_SRC]
	asr	r0, #31
	beq	1f

#else

	ldr	r0, [r4, #MIXER_CHN_SAMP]
	lsl	r0, #8
	bne	1f
	
#endif

	ldrb	r3, [r6, #MCA_TYPE]
	cmp	r3, #ACHN_FOREGROUND
	bne	2f

	ldrb	r1, [r6, #MCA_PARENT]			// stop channel if channel ended
	mov	r3, #MCH_SIZE
	mul	r1, r3
	ldr	r0,=mpp_channels
	ldr	r0, [r0]
	add	r0, r1
	mov	r1, #255
	strb	r1, [r0, #MCH_ALLOC]
2:

#ifdef SYS_GBA
	ldr	r0,=1<<31
	str	r0,[r4,#MIXER_CHN_SRC]	@ stop mixer channel
#else
	mov	r0,#0
	str	r0,[r4,#MIXER_CHN_SAMP]	@ stop mixer channel
#endif

	mov	r1, #ACHN_DISABLED
	strb	r1, [r6, #MCA_TYPE]
	b	.mpp_achn_updated

	@ set panning
1:	ldr	r1,=mpp_vars
	mov	r3, #MPV_PANPLUS
	ldrsh	r0, [r1,r3]
	ldrb	r1, [r6, #MCA_PANNING]
	
	add	r1, r0

	cmp	r1, #0
	bge	.mpp_achn_clippan1
	mov	r1, #0
.mpp_achn_clippan1:
	cmp	r1, #255
	ble	.mpp_achn_clippan2
	mov	r1, #255
.mpp_achn_clippan2:

	#ifdef SYS_NDS
	lsr	r1, #1
	ldrb	r0, [r4, #MIXER_CHN_CNT]
	lsr	r0, #7
	lsl	r0, #7
	orr	r0, r1
	strb	r0, [r4, #MIXER_CHN_CNT]
	#endif
	
	#ifdef SYS_GBA
	strb	r1, [r4, #MIXER_CHN_PAN]
	#endif
	
	
.mpp_achn_updated:
	pop	{r4}
	pop	{r0}
	bx	r0
	//pop	{pc}				@ exit
.pool

.align 2
.thumb_func
@-------------------------------------------------------------------------
mpph_ProcessEnvelope:			@ params={count,node,address}
@-------------------------------------------------------------------------

@ processes the envelope at <address>
@ returns:
@ r0=count
@ r1=node
@ r2=exit_code
@ r3=value*64
	
	push	{r4,r5}

@ get node and base
	lsl	r4, r1, #2
	add	r4, #C_MASIE_NODES
	add	r4, r2
	
	ldrh	r3, [r4, #2]
	lsl	r3, #32-7
	lsr	r3, #32-7-6

@ check for zero count
	
	cmp	r0, #0
	bne	.mpph_pe_between
.mpph_pe_new:

@ process envelope loop
	
	ldrb	r5, [r2, #C_MASIE_LEND]
	cmp	r1, r5
	bne	1f
	ldrb	r1, [r2, #C_MASIE_LSTART]
	mov	r2, #2
	b	.mpph_pe_exit

@ process envelope sustain loop

1:	ldrb	r5, [r6, #MCA_FLAGS]
	lsr	r5, #1	@ locked
	bcc	1f
	ldrb	r5, [r2, #C_MASIE_SEND]
	cmp	r1, r5
	bne	1f
	ldrb	r1, [r2, #C_MASIE_SSTART]
	mov	r2, #0
	b	.mpph_pe_exit

@ check for end

1:	ldrb	r5, [r2, #C_MASIE_NODEC]
	sub	r5, #1
	cmp	r1, r5
	bne	.mpph_count
	mov	r2, #2
	b	.mpph_pe_exit

.mpph_pe_between:

@                            delta * count 
@ formula : y = base*2^6 + -----------------
@                                 2^3
	mov	r5, #0
	ldrsh	r5, [r4,r5]
	mul	r5, r0
	asr	r5, #3
	add	r3, r5
	
.mpph_count:

@ increment count and check if == read count

	add	r0, #1
	ldrh	r5, [r4, #2]
	lsr	r5, #7
	cmp	r0, r5
	bne	.mpph_pe_exit

@ increment node and reset counter

	mov	r0, #0
	add	r1, #1

.mpph_pe_exit:
	
	pop	{r4,r5}
	bx	lr
.pool

/*
.align 2
.thumb_func
@--------------------------------------------------------------------------------------------
mpp_Alloc_Channel:
@--------------------------------------------------------------------------------------------
	
	@ find a channel to use
	@ returns invalid channel if none available
	push	{r4,r5}				@ preserve reg(s)
	ldr	r5,=mm_ch_mask
	ldr	r5, [r5]

	@ldr	r1,=mpp_achannels		@ load pointer
	ldr	r1,=mm_achannels
	ldr	r1,[r1]
	mov	r0, #0				@ load counter
	mov	r2, #255			@ r2 = MAXVOL+1 (highest)
	add	r2, #1
	mov	r3, #255			@ r3 = 255 (none found)
	b	.mppac_start

.mppac_next:
	add	r1, #MCA_SIZE		@ change pointer
	add	r0, #1			@ count
.mppac_start:
	lsr	r5, #1
	bcs	.mppac_check
	bne	.mppac_next
.mppac_end:
	mov	r0, r3			@ if no disabled channels are found, use lowest volume channel
.mppac_found:

	pop	{r4,r5}
	bx	lr
	
.mppac_check:
	ldrb	r4, [r1, #MCA_TYPE]	@ check active channel type
	cmp	r4, #ACHN_DISABLED	@ disabled?
	beq	.mppac_found		@ if so, use this channel
	cmp	r4, #ACHN_BACKGROUND	@ check if its a background channel
	bne	.mppac_next
	
	ldrb	r4, [r1, #MCA_FVOL]	@ compare volumes
	cmp	r4, r2			@
	bge	.mppac_next
	mov	r3, r0			@ save channel#
	mov	r2, r4			@ and volume
	b	.mppac_next
*/

.pool

@........................................................................................

.thumb_func
mpp_PatternPointer:
	mov	r1, r8
	ldr	r2,[r1,#MPL_SONGADR]
	ldr	r1,[r1,#MPL_PATTTABLE]
	lsl	r0, #2
	ldr	r0,[r1,r0]
	add	r0, r2
	bx	lr
.pool
#ifdef FOO_UC
.align 2
.thumb_func
mpp_GetPeriod:
	@ r0 = note
	@ r2 = tuning
	@ CLOBBERS R1,R3
	@ RETURN
	@ r0 = IT/S3M PERIOD

	mov	r1, r8
	ldrb	r1, [r1, #MPL_FLAGS]
	lsr	r1, #C_FLAGS_SS
	bcs	.mpp_gp_linear
.mpp_gp_amiga:
	
	mov	r3, r0
	ldr	r0,=note_table_mod
	ldrb	r1, [r0, r3]
	sub	r0, #3*10
	lsr	r3, #2
	ldrb	r0, [r0, r3]
	
	@ r0 = octave
	@ r1 = note
	lsl	r1, #1
	ldr	r3,=ST3_FREQTABLE
	ldrh	r1, [r3, r1]
	
	ldr	r3,=133808
	mul	r1, r3
	lsr	r1, r0
	
	lsr	r0, r1, #3
	mov	r1, r2
	
	swi	SWI_DIVIDE
	lsl	r0, #3
	
	bx	lr

.mpp_gp_linear:
	ldr	r1,=IT_PitchTable
	lsl	r0, #2
	ldr	r0, [r1, r0]
	
	bx	lr

#endif

@=============================================================================
@                                EFFECT MEMORY
@=============================================================================

.text

mpp_effect_memmap_xm:
.byte	0,0,0,0,2,3,4,0,0,5,0,6,7,0,0,8,9,10,11,0,0,0,0,12,0,0,0,0,0,0,13
@	/,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q, R, S,T,U,V,W, X,Y,Z,0,1,2,3

.equ	MPP_XM_VFX_MEM_VS,	12  @ $ud
.equ	MPP_XM_VFX_MEM_FVS,	13  @ $ud
.equ	MPP_XM_VFX_MEM_GLIS,	14  @ $0x
.equ	MPP_XM_VFX_MEM_PANSL,	7   @ $lr

mpp_effect_memmap_it:
.byte	0,0,0,0,2,3,3,0,0,4,5,2,2,0,6,7,8,9,10,11,12,0,0,13,0,14,0
@	/,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R, S, T, U,V,W, X,Y, Z
mpp_veffect_memmap_it:

.equ	MPP_IT_VFX_MEM,		14
.equ	MPP_GLIS_MEM,		0
.equ	MPP_IT_PORTAMEM,	2


@ 0 means custom behavior, or disabled

.align 2
.thumb_func
@-----------------------------------------------------------------------------
mpp_Channel_ExchangeMemory:
@-----------------------------------------------------------------------------
	
	@ r0 = effect#
	@ r1 = param

@ check flags for XM mode
	
	mov	r2, r8
	ldrb	r2, [r2, #MPL_FLAGS]
	lsr	r2, #C_FLAGS_XS
	bcs	1f

@ IT effects 
	
	ldr	r2,=mpp_effect_memmap_it
	b	2f
	
1:
@ XM effects

	ldr	r2,=mpp_effect_memmap_xm
2:	ldrb	r2, [r2, r0]
	sub	r2, #1
	bcc	3f
	
@ if param=0 then load memory value, otherwise save param to memory
	
	add	r2, #MCH_MEMORY
	cmp	r1, #0
	bne	1f
	ldrb	r1, [r7, r2]
	strb	r1, [r7, #MCH_PARAM]
1:	strb	r1, [r7, r2]
	
3:	bx	lr

@----------------------------------------------------------
mpp_Channel_ExchangeGxx:
@----------------------------------------------------------

	@ r1 = param



/******************************************************************************
 *
 * Volume Commands
 *
 ******************************************************************************/



/******************************************************************************
 * mpp_Process_VolumeCommand()
 *
 * Process volume command
 ******************************************************************************/
					.global mpp_Process_VolumeCommand
					.thumb_func
mpp_Process_VolumeCommand:
	
	mov	r0, r8
	ldrb	r2, [r0, #MPL_TICK]
	ldr	r0, [r0, #MPL_SONGADR]
	ldrb	r0, [r0, #C_MAS_FLAGS]
	lsr	r0, #4
	ldrb	r0, [r7, #MCH_VOLCMD]
	bcc	.mppuv_it
	b	.mppuv_xm
.mppuv_it:

@ determine which command to use 

	cmp	r0, #64
	ble	.mppuv_setvol
	cmp	r0, #84
	ble	.mppuv_fvol
	cmp	r0, #104
	ble	.mppuv_volslide
	cmp	r0, #124
	ble	.mppuv_porta
	cmp	r0, #192
	ble	.mppuv_panning
	cmp	r0, #202
	ble	.mppuv_glissando
	cmp	r0, #212
	ble	.mppuv_vibrato
.mppuv_exit1:
	bx	lr

.align 2
@-----------------------------------------------------------------------------------
.mppuv_setvol:					@ SET VOLUME
@-----------------------------------------------------------------------------------

@ sets volume on tick0

	cmp	r2, #0
	bne	.mppuv_setvol_exit
	strb	r0, [r7, #MCH_VOLUME]
.mppuv_setvol_exit:
	bx	lr				@ exit

.align 2
@-----------------------------------------------------------------------------------
.mppuv_fvol:					@ FINE VOLUME SLIDE UP/DOWN
@-----------------------------------------------------------------------------------

	cmp	r2, #0				@ only slide on tick0
	bne	.mppuv_exit1			@ ..
	ldrb	r1, [r7, #MCH_VOLUME]		@ load channel volume
	mov	r2, #MCH_MEMORY+MPP_IT_VFX_MEM
	cmp	r0, #75				@ check slide direction
	bge	.mppuv_fvoldown			@ jump to slide down if value is 75+ (75-84 is slide down)
.mppuv_fvolup:					@ ------ slide up ----------
	sub	r0, #65				@ 65->74 , 0->9
.mppuv_volup:					@ ** entry for volume slide up
	bne	1f				@ is value 0?
	ldrb	r0, [r7, r2]			@  then fetch value from memory
1:	strb	r0, [r7, r2]			@ save value
.mppuv_volupA:
	add	r1, r0				@ add to volume
	cmp	r1, #64				@ clamp to 0->64
	blt	.mppuv_fvol_exit		@ ..
	mov	r1, #64				@ ..
	b	.mppuv_fvol_exit		@ ..
.mppuv_fvoldown:				@ ------ slide down --------
	sub	r0, #75				@ 75->84 , 0->9
.mppuv_voldown:					@ ** entry for volume slide down
	bne	1f				@ is value 0?
	ldrb	r0, [r7, r2]			@  then fetch value from memory
1:	strb	r0, [r7, r2]			@ save value
.mppuv_voldownA:
	sub	r1, r0				@ subtract from volume
	bcs	.mppuv_fvol_exit		@ check overflow and clamp
	mov	r1, #0				@ ..
.mppuv_fvol_exit:				@ ..
	strb	r1, [r7, #MCH_VOLUME]		@ store volume
.mppuv_exit2:
	bx	lr				@ exit function
	
.align 2
@----------------------------------------------------------------------------------
.mppuv_volslide:				@ VOLUME SLIDE UP/DOWN
@----------------------------------------------------------------------------------

	cmp	r2, #0				@ only slide on other ticks
	beq	.mppuv_exit1			@ ..
	ldrb	r1, [r7, #MCH_VOLUME]		@ get volume
	cmp	r0, #95				@ check slide direction
	bge	.mppuv_vs_down			
.mppuv_vs_up:					@ slide up...
	mov	r2, #MCH_MEMORY+MPP_IT_VFX_MEM
	sub	r0, #85				@ 85->94 , 0->9
	b	.mppuv_volup			@ branch to function (use fvol code)

.mppuv_vs_down:					@ slide down...
	mov	r2, #MCH_MEMORY+MPP_IT_VFX_MEM
	sub	r0, #95				@ 95->104 , 0->9
	b	.mppuv_voldown			@ branch to function (use fvol code)

.align 2
@---------------------------------------------------------------------------------------
.mppuv_porta:					@ PORTAMENTO UP/DOWN
@---------------------------------------------------------------------------------------

	cmp	r2, #0				@ only slide on other ticks
	beq	.mppuv_exit2
	
	push	{lr}				@ save return address
	mov	r1, r0				@ get period value
	mov	r0, #MCH_PERIOD
	ldr	r0, [r7, r0]
	
	cmp	r1, #115			@ check slide direction
	bge	.mppuv_porta_up
.mppuv_porta_down:
	sub	r1, #105			@ map value 0->9
	lsl	r1, #2				@ volume command slides are
	bne	1f
	ldrb	r1, [r7, #MCH_MEMORY+MPP_IT_PORTAMEM]
1:	strb	r1, [r7, #MCH_MEMORY+MPP_IT_PORTAMEM]
	bl	mpph_PitchSlide_Down		@ equal to normal slides *4
	b	.mppuv_porta_set
	
.mppuv_porta_up:
	sub	r1, #115			@ slide up...
	lsl	r1, #2
	bne	1f
	ldrb	r1, [r7, #MCH_MEMORY+MPP_IT_PORTAMEM]
1:	strb	r1, [r7, #MCH_MEMORY+MPP_IT_PORTAMEM]
	bl	mpph_PitchSlide_Up

.mppuv_porta_set:
	mov	r2, #MCH_PERIOD			@ store new period
	ldr	r1, [r7, r2]
	str	r0, [r7, r2]
	sub	r0, r1				@ and edit temp period
	add	r5, r0
	
	pop	{r0}
	bx	r0
//	pop	{pc}				@ exit

.align 2
@---------------------------------------------------------------------------------------
.mppuv_panning:					@ SET PANNING
@---------------------------------------------------------------------------------------

	cmp	r2, #0				@ only set on tick 0
	bne	.mppuv_exit1			@ ..
	sub	r0, #128			@ map to 0->64
	lsl	r0, #2
	cmp	r0, #255
	blt	.mppuv_p_store
	mov	r0, #255
.mppuv_p_store:
	strb	r0, [r7, #MCH_PANNING]		@ save to active channel
.mppuv_p_exit:
	bx	lr				@ exit

.align 2
@---------------------------------------------------------------------------------------
.mppuv_glissando:				@ GLISSANDO
@---------------------------------------------------------------------------------------

	cmp	r2, #0
	beq	.mppuv_p_exit

	sub	r0, #193
	ldr	r1,=vcmd_glissando_table
	ldrb	r0, [r1, r0]

	mov	r1, r8
	ldrb	r1, [r1, #MPL_FLAGS]
	lsr	r1, #C_FLAGS_GS
	bcs	2f
@ single gxx

	cmp	r0, #0
	beq	1f
	ldrb	r0, [r7, #MCH_MEMORY+MPP_GLIS_MEM]
1:	strb	r0, [r7, #MCH_MEMORY+MPP_GLIS_MEM]
	b	.mppe_glis_ot

2: @ shared gxx

	cmp	r0, #0
	beq	1f
	ldrb	r0, [r7, #MCH_MEMORY+MPP_IT_PORTAMEM]
1:	strb	r0, [r7, #MCH_MEMORY+MPP_IT_PORTAMEM]
	strb	r0, [r7, #MCH_MEMORY+MPP_GLIS_MEM]
	
	b	.mppe_glis_ot
.pool

vcmd_glissando_table:
.byte	0,1,4,8,16,32,64,96,128,255

.align 2
@---------------------------------------------------------------------------------------
.mppuv_vibrato:					@ VIBRATO (SPEED)
@---------------------------------------------------------------------------------------
	
	@ vibrato... sets speed
	cmp	r2, #0
	beq	.mppuv_vib_exit
	sub	r0, #203
	beq	1f
	lsl	r0, #2
	strb	r0, [r7, #MCH_VIBSPD]
1:	b	mppe_DoVibrato
.mppuv_vib_exit:
	bx	lr



/******************************************************************************
 *
 * XM Volume Commands
 *
 ******************************************************************************/



.align 2
.thumb_func
@---------------------------------------------------------------------------------------
.mppuv_xm:
@---------------------------------------------------------------------------------------

@ determine command type 
	
	cmp	r0, #0		@ 0 = none
	beq	.mppuv_exit4
	cmp	r0, #0x50
	ble	.mppuv_xm_setvol
	cmp	r0, #0x80
	blt	.mppuv_xm_volslide
	cmp	r0, #0xA0
	blt	.mppuv_xm_fvolslide
	cmp	r0, #0xC0
	blt	.mppuv_xm_vibrato
	cmp	r0, #0xD0
	blt	.mppuv_xm_panning
	cmp	r0, #0xF0
	blt	.mppuv_xm_panslide
	b	.mppuv_xm_toneporta

.align 2
.thumb_func
@----------------------------------------------------------------------------------------
.mppuv_xm_setvol:					@ Set Volume
@----------------------------------------------------------------------------------------

	cmp	R2, #0
	bne	.mppuv_exit4
	sub	r0, #0x10
	strb	r0, [r7, #MCH_VOLUME]
.mppuv_exit4:
	bx	lr

.align 2
.thumb_func
@----------------------------------------------------------------------------------------
.mppuv_xm_volslide:					@ Volume Slide
@----------------------------------------------------------------------------------------

	cmp	r2, #0
	beq	.mppuv_exit2
	ldrb	r1, [r7, #MCH_VOLUME]
	mov	r3, #MCH_MEMORY+MPP_XM_VFX_MEM_VS
	ldrb	r2, [r7, r3]
	cmp	r0, #0x70
	bge	.mppuv_xm_volslide_up
	sub	r0, #0x60
.mppuv_xm_volslide_dn_check:
	bne	1f
	mov	r0, r2
	lsl	r0, #32-4
	lsr	r0, #32-4
	b	2f
1:	lsr	r2, #4
	lsl	r2, #4
	orr	r2, r0
	strb	r2, [r7, r3]	
2:	b	.mppuv_voldownA

.mppuv_xm_volslide_up:
	sub	r0, #0x70
.mppuv_xm_volslide_up_check:
	bne	1f
	mov	r0, r2
	lsr	r0, #4
	b	2f
1:	lsl	r2, #32-4
	lsr	r2, #32-4
	lsl	r0, #4
	orr	r2, r0
	lsr	r0, #4
	strb	r2, [r7, r3]
2:	b	.mppuv_volupA

.align 2
.thumb_func
@----------------------------------------------------------------------------------------
.mppuv_xm_fvolslide:					@ Fine Volume Slide
@----------------------------------------------------------------------------------------

	cmp	r2, #0
	bne	.mppuv_exit4
	ldrb	r1, [r7, #MCH_VOLUME]
	mov	r3, #MCH_MEMORY+MPP_XM_VFX_MEM_FVS
	ldrb	r2, [r7, r3]
	cmp	r0, #0x90
	bge	.mppuv_xm_fvolslide_up
	sub	r0, #0x80
	b	.mppuv_xm_volslide_dn_check
.mppuv_xm_fvolslide_up:
	sub	r0, #0x90
	b	.mppuv_xm_volslide_up_check

.align 2
.thumb_func
@----------------------------------------------------------------------------------------
.mppuv_xm_vibrato:					@ Vibrato
@----------------------------------------------------------------------------------------
	
	@ xm vibrato
	@ sets speed or depth
	
	cmp	r2, #0
	beq	.mppuv_xm_vibexit
	cmp	r0, #0xB0
	bge	.mppuv_xm_vibdepth

.mppuv_xm_vibspd:
	sub	r0, #0xA0
	lsl	r0, #2
	beq	1f
	strb	r0, [r7, #MCH_VIBSPD]
1:	b	mppe_DoVibrato
	
.mppuv_xm_vibdepth:
	sub	r0, #0xB0
	lsl	r0, #3
	beq	1f
	strb	r0, [r7, #MCH_VIBDEP]
1:	b	mppe_DoVibrato
	
.mppuv_xm_vibexit:
	bx	lr

.align 2
.thumb_func
@----------------------------------------------------------------------------------------
.mppuv_xm_panning:					@ Panning
@----------------------------------------------------------------------------------------
	
	cmp	r2, #0
	bne	.mppuv_exit3
	sub	r0, #0xC0
	lsl	r0, #4
	cmp	r0, #240
	beq	.mppuv_xm_panhack
	strb	r0, [r7, #MCH_PANNING]
	bx	lr
.mppuv_xm_panhack:
	mov	r0, #255
	strb	r0, [r7, #MCH_PANNING]
.mppuv_exit3:
	bx	lr
	
.align 2
.thumb_func
@----------------------------------------------------------------------------------------
.mppuv_xm_panslide:					@ Panning Slide
@----------------------------------------------------------------------------------------
	
	cmp	r2, #0
	beq	.mppuv_exit3
	ldrb	r2, [r7, #MCH_PANNING]
	ldrb	r3, [r7, #MCH_MEMORY + MPP_XM_VFX_MEM_PANSL]
	cmp	r0, #0xE0
	bge	.mppuv_xm_panslide_right
	sub	r0, #0xD0
	bne	1f
	lsr	r0, r3, #4
	b	2f
1:	lsl	r3, #32-4
	lsr	r3, #32-4
	lsl	r0, #4
	orr	r3, r0
	lsr	r0, #4
	strb	r3, [r7, #MCH_MEMORY + MPP_XM_VFX_MEM_PANSL]

2:	lsl	r0, #2
	sub	r2, r0
	bcs	.mppuv_xm_panslide_set
	mov	r2, #0
	b	.mppuv_xm_panslide_set
.mppuv_xm_panslide_right:
	sub	r0, #0xE0
	bne	1f
	lsl	r0, r3, #32-4
	lsr	r0, #32-4
	b	2f
1:	lsr	r3, #4
	lsl	r3, #4
	orr	r3, r0
	strb	r3, [r7, #MCH_MEMORY + MPP_XM_VFX_MEM_PANSL]
2:	lsl	r0, #2
	add	r2, r0
	cmp	r2, #255
	blt	.mppuv_xm_panslide_set
	mov	r2, #255
.mppuv_xm_panslide_set:
	strb	r2, [r7, #MCH_PANNING]
	bx	lr

.align 2
.thumb_func
@-------------------------------------------------------------------------------------
.mppuv_xm_toneporta:					@ Glissando
@-------------------------------------------------------------------------------------

	@ glissando...
	@ on nonzero ticks, do a regular glissando slide at speed * 16
	cmp	r2, #0
	beq	1f
	
	sub	r0, #0xF0
	lsl	r0, #4
	beq	2f
	mov	r1, #MCH_MEMORY+MPP_XM_VFX_MEM_GLIS
	strb	r0, [r7, r1]
2:	ldrb	r0, [r7, r1]
	mov	r1, r0
	
	b	.mppe_glis_backdoor
	
1:	bx	lr



/******************************************************************************
 *
 * Module Effects
 *
 ******************************************************************************/



/******************************************************************************
 * mpp_ProcessEffect()
 *
 * Process pattern effect.
 ******************************************************************************/
						.global mpp_Process_Effect
						.thumb_func
mpp_Process_Effect:

	push	{lr}
	ldrb	r0, [r7, #MCH_EFFECT]	@ get effect#
	ldrb	r1, [r7, #MCH_PARAM]	@ r1 = param
	bl	mpp_Channel_ExchangeMemory

	lsl	r0, #1

	pop	{r2}
	mov	lr, r2

	mov	r2, r8
	ldrb	r2, [r2, #MPL_TICK]	@ r2 = tick#

	cmp	r2, #0			@ Z flag = tick0 :)
	add	r0, pc
	mov	pc, r0
	
	b	mppe_todo
	b	mppe_SetSpeed
	b	mppe_PositionJump
	b	mppe_PatternBreak
	b	mppe_VolumeSlide
	b	mppe_Portamento
	b	mppe_Portamento
	b	mppe_Glissando
	b	mppe_Vibrato
	b	mppe_todo
	b	mppe_Arpeggio
	b	mppe_VibratoVolume
	b	mppe_PortaVolume
	b	mppe_ChannelVolume
	b	mppe_ChannelVolumeSlide
	b	mppe_SampleOffset
	b	mppe_todo
	b	mppe_Retrigger
	b	mppe_Tremolo		@ tremolo
	b	mppe_Extended
	b	mppe_SetTempo
	b	mppe_FineVibrato
	b	mppe_SetGlobalVolume
	b	mppe_GlobalVolumeSlide
	b	mppe_SetPanning
	b	mppe_Panbrello
	b	mppe_ZXX
	b	mppe_SetVolume
	b	mppe_KeyOff
	b	mppe_EnvelopePos
	b	mppe_OldTremor
.pool

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_SetSpeed:				@ EFFECT Axy: SET SPEED
@---------------------------------------------------------------------------------

	bne	.mppe_ss_exit		@ dont set on nonzero ticks
	cmp	r1, #0
	beq	.mppe_ss_exit
	mov	r0, r8
	
	strb	r1, [r0, #MPL_SPEED]
.mppe_ss_exit:
.thumb_func
mppe_todo:
	bx	lr			@ exit

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_PositionJump:			@ EFFECT Bxy: SET POSITION
@---------------------------------------------------------------------------------

	bne	.mppe_pj_exit		@ skip nonzero ticks
	mov	r0, r8
	strb	r1, [r0, #MPL_PATTJUMP]
.mppe_pj_exit:
	bx	lr			@ exit
.pool

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_PatternBreak:				@ EFFECT Cxy: PATTERN BREAK
@---------------------------------------------------------------------------------
	bne	.mppe_pb_exit			@ skip nonzero ticks
	mov	r0, r8				@ get variables
	strb	r1, [r0, #MPL_PATTJUMP_ROW]	@ save param to row value
	ldrb	r1, [r0, #MPL_PATTJUMP]		@ check if pattjump=empty
	cmp	r1, #255			@ 255=empty
	bne	.mppe_pb_exit			@ ...
	ldrb	r1, [r0, #MPL_POSITION]		@ if empty, set pattjump=position+1
	add	r1, #1
	strb	r1, [r0, #MPL_PATTJUMP]
.mppe_pb_exit:
	bx	lr				@ finished
.pool

.align 2
.thumb_func
@------------------------------------------------------------------------------------------
mppe_VolumeSlide:				@ EFFECT Dxy: VOLUME SLIDE
@------------------------------------------------------------------------------------------

	push	{lr}
	ldrb	r0, [r7, #MCH_VOLUME]		@ load volume
	
	bl	mpph_VolumeSlide64
	
	strb	r0, [r7, #MCH_VOLUME]		@ save volume
.mppe_vs_zt:
	pop	{r0}
	bx	r0
//	pop	{pc}				@ exit

.align 2
.thumb_func
@----------------------------------------------------------------------------------
mppe_Portamento:				@ EFFECT Exy/Fxy: Portamento
@----------------------------------------------------------------------------------

	push	{lr}
	
.mppe_pd_ot:
	mov	r3, #0
	mov	r0, r1
	lsr	r0, #4				@ test for Ex param (Extra fine slide)
	cmp	r0, #0xE			@ ..
.mppe_pd_checkE:				@ ..
	bne	.mppe_pd_checkF			@ ..
	cmp	r2, #0				@ Extra fine slide: only slide on tick0
	bne	.mppe_pd_exit			@ ..
	lsl	r1, #32-4			@ mask out slide value
	lsr	r1, #32-4			@ ..
	mov	r3, #1
	b	.mppe_pd_otherslide		@ skip the *4 multiplication
.mppe_pd_checkF:				@ ------------------------------------
	cmp	r0, #0xF			@ test for Fx param (Fine slide)
	bne	.mppe_pd_regslide		@ ..
	cmp	r2, #0				@ Fine slide: only slide on tick0
	bne	.mppe_pd_exit			@ ..
	lsl	r1, #32-4			@ mask out slide value
	lsr	r1, #32-4			@ ..
	b	.mppe_pd_otherslide
.mppe_pd_regslide:
	cmp	r2, #0
	beq	.mppe_pd_exit
.mppe_pd_otherslide:
	
	ldrb	r0, [r7, #MCH_EFFECT]		@ check slide direction
	mov	r2, #MCH_PERIOD
	cmp	r0, #5				@ .. (5 = portamento down)
	ldr	r0, [r7, r2]			@ get period
	
	bne	.mppe_pd_slideup		@ branch to function
.mppe_pd_slidedown:				@ -------SLIDE DOWN-------
	
	cmp	r3, #0
	bne	.mppe_pd_fineslidedown
	bl	mpph_PitchSlide_Down
	
	b	.mppe_pd_store			@ store & exit

.mppe_pd_fineslidedown:
	bl	mpph_FinePitchSlide_Down
	b	.mppe_pd_store
	
.mppe_pd_slideup:				@ ---------SLIDE UP---------
	
	cmp	r3, #0
	bne	.mppe_pd_fineslideup
	bl	mpph_PitchSlide_Up
	b	.mppe_pd_store
.mppe_pd_fineslideup:
	bl	mpph_FinePitchSlide_Up

.mppe_pd_store:
	mov	r2, #MCH_PERIOD
	ldr	r1, [r7, #MCH_PERIOD]
	str	r0, [r7, #MCH_PERIOD]
	sub	r0, r1
	add	r5, r0
.mppe_pd_exit:
	pop	{r0}
	bx	r0				@ exit

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_Glissando:					@ EFFECT Gxy: Glissando
@---------------------------------------------------------------------------------

	bne	.mppe_glis_ot
	
	mov	r0, r8
	ldrb	r0, [r0, #MPL_FLAGS]
	lsr	r0, #C_FLAGS_GS
	bcc	2f
	@ gxx is shared, IT MODE ONLY!!
	cmp	r1, #0
	bne	3f
	ldrb	r1, [r7, #MCH_MEMORY+MPP_IT_PORTAMEM]
	strb	r1, [r7, #MCH_PARAM]
3:	strb	r1, [r7, #MCH_MEMORY+MPP_IT_PORTAMEM]
	strb	r1, [r7, #MCH_MEMORY+MPP_GLIS_MEM]	@ for simplification later
	b	.mppe_glis_ot
	
2:	@ gxx is separate
	cmp	r1, #0
	bne	3f
	
	ldrb	r1, [r7, #MCH_MEMORY+MPP_GLIS_MEM]
	strb	r1, [r7, #MCH_PARAM]
3:	strb	r1, [r7, #MCH_MEMORY+MPP_GLIS_MEM]

	bx	lr
//	b	.mppe_glis_exit
.mppe_glis_ot:
	
	push	{lr}					// save return address

	ldrb	r1, [r7, #MCH_MEMORY+MPP_GLIS_MEM]

.mppe_glis_backdoor:

	push	{r1}
	
	cmp	r6, #0					// exit if no active channel
	bne	1f					//
	pop	{r1,r3}					//
	bx	r3					//
1:							//
	
	ldrb	r0, [r6, #MCA_SAMPLE]			// get target period
	sub	r0, #1					//
	mpp_SamplePointer				//
	ldrh	r1, [r0, #C_MASS_FREQ]			//
	LSL	R1, #2					//
	ldrb	r2, [r7, #MCH_NOTE]			//
	ldr	r3,=mmGetPeriod				//
	bl	mpp_call_r3				//
	
	pop	{r1}					// r1 = parameter
	push	{r0}					// 
	
	mov	r3, r0					// r3 = target period
	mov	r2, #MCH_PERIOD				// r0 = current period
	ldr	r0, [r7, r2]				//
	mov	r2, r8					// test S flag
	ldrb	r2, [r2, #MPL_FLAGS]			//
	lsr	r2, #C_FLAGS_SS				//
	bCC	.mppe_glis_amiga
	cmp	r0, r3
	blt	.mppe_glis_slideup
	bgt	.mppe_glis_slidedown
.mppe_glis_noslide:
	pop	{r0}
	pop	{r3}
	bx	r3

.mppe_glis_slideup:
	bl	mpph_PitchSlide_Up
	pop	{r1}
	
	cmp	r0, r1
	blt	.mppe_glis_store
	mov	r0, r1
	b	.mppe_glis_store
.mppe_glis_slidedown:
	bl	mpph_PitchSlide_Down
	pop	{r1}
	cmp	r0, r1
	bgt	.mppe_glis_store
	mov	r0, r1
.mppe_glis_store:
	
	mov	r2, #MCH_PERIOD
	ldr	r1, [r7, r2] @#MCA_PERIOD]
	str	r0, [r7, r2] @#MCA_PERIOD]
	sub	r0, r1
	add	r5, r0
	
.mppe_glis_exit:
	pop	{r3}
	bx	r3


	//bx	lr

.mppe_glis_amiga:

	cmp	r0, r3
	blt	.mppe_glis_amiga_up
	bgt	.mppe_glis_amiga_down
	pop	{r0}
	pop	{r3}
	bx	r3

.mppe_glis_amiga_down:
	bl	mpph_PitchSlide_Up
	pop	{r1}
	cmp	r0, r1
	bgt	.mppe_glis_store
	mov	r0, r1
	b	.mppe_glis_store
.mppe_glis_amiga_up:
	bl	mpph_PitchSlide_Down
	pop	{r1}
	cmp	r0, r1
	blt	.mppe_glis_store
	mov	r0, r1
	b	.mppe_glis_store

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_Vibrato:					@ EFFECT Hxy: Vibrato
@---------------------------------------------------------------------------------

	bne	.mppe_v_ot
	lsr	r0, r1, #4			@ if (x != 0) {
	beq	.mppe_v_nospd			@   speed = 4*x;
	lsl	r0, #2				@   ..
	strb	r0, [r7, #MCH_VIBSPD]		@   ..
.mppe_v_nospd:
	
	lsl	r0, r1, #32-4		 	@ if (y != 0) {
	beq	.mppe_v_nodep			@ ..
	lsr	r0, #32-6			@   depth = y * 4;
	mov	r1, r8
	ldrb	r1, [r1, #MPL_OLDEFFECTS]	@   if(OldEffects)
	lsl	r0, r1				@      depth <<= 1;
	strb	r0, [r7, #MCH_VIBDEP]		@
	b	mppe_DoVibrato
.mppe_v_nodep:
	BX	LR

.align 2
.thumb_func
@-------------------------------------------------------
mppe_DoVibrato:
@-------------------------------------------------------

.mppe_v_ot:
	mov	r0, r8
	ldrb	r1, [r0, #MPL_TICK]
	ldrb	r0, [r0, #MPL_OLDEFFECTS]
	
	cmp	r0, #0
	beq	.mppe_dv_update
	cmp	r1, #0
	bne	.mppe_dv_update
	
	push	{lr}
	ldrb	r1, [r7, #MCH_VIBPOS]
	b	.mppe_dv_notupdate
.mppe_dv_update:
	push	{lr}
	ldrb	r0, [r7, #MCH_VIBSPD]
	ldrb	r1, [r7, #MCH_VIBPOS]
	add	r1, r0
	lsl	r1, #32-8
	lsr	r1, #32-8
	strb	r1, [r7, #MCH_VIBPOS]
.mppe_dv_notupdate:
	ldr	r2,=mpp_TABLE_FineSineData
	ldrsb	r1, [r2, r1]
	ldrb	r0, [r7, #MCH_VIBDEP]
	mul	r1, r0
	asr	r1, #8
	mov	r0, r5
	cmp	r1, #0
	blt	.mppe_dv_negative
	bl	mpph_PitchSlide_Up
	b	.mppe_dv_store
.mppe_dv_negative:
	neg	r1, r1
	bl	mpph_PitchSlide_Down
.mppe_dv_store:
	mov	r5, r0
	pop	{r0}
	bx	r0
//	pop	{pc}		@ return THUMB
.pool

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_Tremor:					@ EFFECT Ixy: Tremor
@---------------------------------------------------------------------------------
	bx	lr

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_Arpeggio:					@ EFFECT Jxy: Arpeggio
@---------------------------------------------------------------------------------

	bne	.mppe_arp_ot

	mov	r0, #0
	strb	r0, [r7, #MCH_FXMEM]
.mppe_arp_ot:
	cmp	r6, #0
	beq	1f
	ldrb	r0, [r7, #MCH_FXMEM]
//	ldrb	r3, [r6, #MCA_SAMPLE]	?		???	
	cmp	r0, #1
	bgt	.mppe_arp_2
	beq	.mppe_arp_1
.mppe_arp_0:
	mov	r0, #1
	strb	r0, [r7, #MCH_FXMEM]
	@ do nothing! :)
1:	bx	lr
	
.mppe_arp_1:
	
	mov	r0, #2			@ set next tick to '2'
	strb	r0, [r7, #MCH_FXMEM]	@ save...
	mov	r0, r5
	lsr	r1, #4			@ mask out high nibble of param
.mppe_arp_others:
	mov	r2, r5
	cmp	r1, #12			@ see if its >= 12
	blt	.mppea1_12		@ ..
	add	r2, r5			@  add period if so... (octave higher)
.mppea1_12:				@  ..
	lsl	r1, #4			@ *16*hword
	
	mov	r0, r5
	push	{lr}
	bl	mpph_LinearPitchSlide_Up

	mov	r5, r0
	pop	{r0}
	bx	r0
//	pop	{pc}
	
.mppe_arp_2:
	mov	r0, #0
	strb	r0, [r7, #MCH_FXMEM]
	mov	r0, r5
	lsl	r1, #32-4
	lsr	r1, #32-4
	b	.mppe_arp_others

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_VibratoVolume:			@ EFFECT Kxy: Vibrato+Volume Slide
@---------------------------------------------------------------------------------

	push	{lr}
	push	{r1,r2}
	bl	mppe_DoVibrato
	pop	{r1,r2}
	cmp	r2, #0
	bl	mppe_VolumeSlide
	pop	{r0}
	bx	r0
//	pop	{pc}

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_PortaVolume:			@ EFFECT Lxy: Portamento+Volume Slide
@---------------------------------------------------------------------------------

	push	{lr}
	push	{r1,r2}
	ldrb	r1, [r7, #MCH_MEMORY+MPP_GLIS_MEM]
	bl	mppe_Glissando
	pop	{r1, r2}
	cmp	r2, #0
	bl	mppe_VolumeSlide
	pop	{r0}
	bx	r0
//	pop	{pc}

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_ChannelVolume:				@ EFFECT Mxy: Set Channel Volume
@---------------------------------------------------------------------------------

	bne	.mppe_cv_exit			@ quite simple...
	cmp	r1, #0x40
	bgt	.mppe_cv_exit			@ ignore command if parameter is > 0x40
	strb	r1, [r7, #MCH_CVOLUME]
.mppe_cv_exit:
	bx	lr
	
.align 2
.thumb_func
@------------------------------------------------------------------------------------
mppe_ChannelVolumeSlide:			@ EFFECT Nxy: Channel Volume Slide
@------------------------------------------------------------------------------------

	push	{lr}

	ldrb	r0, [r7, #MCH_CVOLUME]		@ load volume
	bl	mpph_VolumeSlide64

	strb	r0, [r7, #MCH_CVOLUME]		@ save volume
	pop	{r0}
	bx	r0
//	pop	{pc}				@ exit

.align 2
.thumb_func
@----------------------------------------------------------------------------------
mppe_SampleOffset:				@ EFFECT Oxy Sample Offset
@----------------------------------------------------------------------------------

	bne	.mppe_so_exit			@ skip on other ticks

	ldr	r0,=mpp_vars
	strb	r1, [r0, #MPV_SAMPOFF]		@ set offset
.mppe_so_exit:
	bx	lr
.pool

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_PanningSlide:				@ EFFECT Pxy Panning Slide
@---------------------------------------------------------------------------------

	push	{lr}

	mov	r0, #255
	push	{r0}
	ldrb	r0, [r7, #MCH_PANNING]		@ load panning
	
	bl	mpph_VolumeSlide

	strb	r0, [r7, #MCH_PANNING]		@ save panning
	pop	{r0}
	bx	r0
//	pop	{pc}				@ exit

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_Retrigger:					@ EFFECT Qxy Retrigger Note
@---------------------------------------------------------------------------------

	ldrb	r0, [r7, #MCH_FXMEM]
	cmp	r0, #0
	bne	.mppe_retrig_refillN
.mppe_retrig_refill:
	lsl	r0, r1, #32-4
	lsr	r0, #32-4
	add	r0, #1
.mppe_retrig_exitcount:
	strb	r0, [r7, #MCH_FXMEM]
	bx	lr
.mppe_retrig_refillN:
	sub	r0, #1
	cmp	r0, #1
	bne	.mppe_retrig_exitcount
.mppe_retrig_fire:
	
	ldrb	r2, [r7, #MCH_VOLUME]
	lsr	r0, r1, #4
	beq	.mppe_retrig_v_change0
	cmp	r0, #5
	ble	.mppe_retrig_v_change_sub
	cmp	r0, #6
	beq	.mppe_retrig_v_change_23
	cmp	r0, #7
	beq	.mppe_retrig_v_change_12
	cmp	r0, #8
	beq	.mppe_retrig_v_change0
	cmp	r0, #0xD
	ble	.mppe_retrig_v_change_add
	cmp	r0, #0xE
	beq	.mppe_retrig_v_change_32
	cmp	r0, #0xF
	beq	.mppe_retrig_v_change_21
.mppe_retrig_v_change_21:
	lsl	r2, #1
	cmp	r2, #64
	blt	.mppe_retrig_finish
	mov	r2, #64
.mppe_retrig_v_change0:
	b	.mppe_retrig_finish
.mppe_retrig_v_change_sub:
	sub	r0, #1
	mov	r3, #1
	lsl	r3, r0
	sub	r2, r3
	bcs	.mppe_retrig_finish
	mov	r2, #0
	b	.mppe_retrig_finish
.mppe_retrig_v_change_23:
	mov	r0, #171
	mul	r2, r0
	lsr	r2, #8
	b	.mppe_retrig_finish
.mppe_retrig_v_change_12:
	lsr	r2, #1
	b	.mppe_retrig_finish
.mppe_retrig_v_change_add:
	sub	r0, #9
	mov	r3, #1
	lsl	r3, r0
	add	r2, r3
	cmp	r2, #64
	blt	.mppe_retrig_finish
	mov	r2, #64
	b	.mppe_retrig_finish
.mppe_retrig_v_change_32:
	mov	r0, #192
	mul	r2, r0
	lsr	r2, #7
.mppe_retrig_finish:
	strb	r2, [r7, #MCH_VOLUME]
	cmp	r6, #0
	beq	.mppe_retrig_refill
	ldrb	r0, [r6, #MCA_FLAGS]
	mov	r2, #MCAF_START
	orr	r0, r2
	strb	r0, [r6, #MCA_FLAGS]
	b	.mppe_retrig_refill

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_Tremolo:					@ EFFECT Rxy: Tremolo
@---------------------------------------------------------------------------------

@ r1 = param
@ Z = tick0

	beq	.mppe_trem_zt			@ skip this part on tick0
.mppe_trem_ot:
	@ X = speed, Y = depth
	ldrb	r0, [r7, #MCH_FXMEM]		@ get sine position
	lsr	r3, r1, #4			@ mask out SPEED
	lsl	r3, #2				@ speed*4 to compensate for larger sine table
	add	r0, r3				@ add to position
	strb	r0, [r7, #MCH_FXMEM]		@ save (value & 255)
.mppe_trem_zt:
	ldrb	r0, [r7, #MCH_FXMEM]		@ get sine position
	ldr	r3,=mpp_TABLE_FineSineData	@ load sine table value
	ldrsb	r0, [r3, r0]
	lsl	r1, #32-4			@ mask out DEPTH
	lsr	r1, #32-4
	mul	r0, r1				@ SINE*DEPTH / 64
	asr	r0, #6
	mov	r1, r8
	ldrb	r1, [r1, #MPL_FLAGS]
	lsr	r1, #C_FLAGS_XS
	bcs	1f
	asr	r0, #1
1:	ldr	r1,=mpp_vars			@ set volume addition variable
	strb	r0, [r1, #MPV_VOLPLUS]
	bx	lr
.pool

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_Extended:				@ EFFECT Sxy: Extended Effects
@---------------------------------------------------------------------------------
	
	lsr	r0, r1, #4
	lsl	r0, #1
	cmp	r2, #0
	add	r0, pc
	mov	pc, r0
	
	@ branch table...
	b	mppex_XM_FVolSlideUp	@ S0x
	b	mppex_XM_FVolSlideDown	@ S1x
	b	mppex_OldRetrig		@ S2x
	b	mppex_VibForm		@ S3x
	b	mppex_TremForm		@ S4x
	b	mppex_PanbForm		@ S5x
	b	mppex_FPattDelay	@ S6x
	b	mppex_InstControl	@ S7x
	b	mppex_SetPanning	@ S8x
	b	mppex_SoundControl	@ S9x
	b	mppex_HighOffset	@ SAx
	b	mppex_PatternLoop	@ SBx
	b	mppex_NoteCut		@ SCx
	b	mppex_NoteDelay		@ SDx
	b	mppex_PatternDelay	@ SEy
	b	mppex_SongMessage	@ SFx

.mppe_ex_quit:

@-------------------------------------------
.thumb_func
mppex_Unused:
	bx	lr

@-------------------------------------------

.thumb_func
mppex_XM_FVolSlideUp:
	bne	2f
	ldrb	r0, [r7, #MCH_VOLUME]
	lsl	r1, #32-4
	lsr	r1, #32-4
	add	r0, r1
	cmp	r0, #64
	blt	1f
	mov	r0, #64
1:	strb	r0, [r7, #MCH_VOLUME]
2:	bx	lr

.thumb_func
mppex_XM_FVolSlideDown:
	bne	2f
	ldrb	r0, [r7, #MCH_VOLUME]
	lsl	r1, #32-4
	lsr	r1, #32-4
	sub	r0, r1
	bcs	1f
	mov	r0, #0
1:	strb	r0, [r7, #MCH_VOLUME]
2:	bx	lr

@-------------------------------------------
.thumb_func
mppex_OldRetrig:
	bne	1f
	lsl	r1, #32-4
	lsr	r1, #32-4
	strb	r1, [r7, #MCH_FXMEM]
	bx	lr
	
1:	ldrb	r0, [r7, #MCH_FXMEM]
	sub	r0, #1

	bne	1f
	
	lsl	r0, r1, #32-4
	lsr	r0, #32-4
	strb	r0, [r7, #MCH_FXMEM]

	cmp	r6, #0
	beq	1f
	ldrb	r1, [r6, #MCA_FLAGS]
	mov	r2, #MCAF_START
	
	orr	r1, r2
	strb	r1, [r6, #MCA_FLAGS]
	
1:	strb	r0, [r7, #MCH_FXMEM]
	bx	lr

@-------------------------------------------
.thumb_func
mppex_VibForm:
	bx	lr

@-------------------------------------------
.thumb_func
mppex_TremForm:
	bx	lr

@-------------------------------------------
.thumb_func
mppex_PanbForm:
	bx	lr

@-------------------------------------------
.thumb_func
mppex_FPattDelay:
	bne	.mppex_fpd_exit
	lsl	r1, #32-4
	lsr	r1, #32-4
	mov	r0, r8
	strb	r1, [r0, #MPL_FPATTDELAY]
.mppex_fpd_exit:
	bx	lr

@-------------------------------------------
.thumb_func
mppex_InstControl:
	bne	.mppex_ic_exit
	lsl	r1, #32-4
	lsr	r1, #32-4
	cmp	r1, #2
	ble	.mppex_ic_pastnotes
	cmp	r1, #6
	ble	.mppex_ic_nna
	cmp	r1, #8
	ble	.mppex_ic_envelope
	bx	lr
.mppex_ic_pastnotes:
	@ todo...
	bx	lr
.mppex_ic_nna:
	@ overwrite NNA
	sub	r1, #3
	ldrb	r2, [r7, #MCH_BFLAGS]
	lsl	r2, #32-6
	lsr	r2, #32-6
	lsl	r1, #6
	orr	r2, r1
	strb	r2, [r7, #MCH_BFLAGS]
	bx	lr
.mppex_ic_envelope:
	cmp	r6, #0
	beq	.mppex_ic_exit
	ldrb	r2, [r6, #MCA_FLAGS]
	mov	r0, #32
	bic	r2, r0
	sub	r1, #7
	lsl	r1, #5
	orr	r2, r1
	strb	r2, [r6, #MCA_FLAGS]

.mppex_ic_exit:
	bx	lr

@-------------------------------------------
.thumb_func
mppex_SetPanning:
	lsl	r1, #4
	strb	r1, [r7, #MCH_PANNING]
	bx	lr

@-------------------------------------------
.thumb_func
mppex_SoundControl:
	cmp	r1, #0x91
	beq	.mppex_sc_surround
	bx	lr
.mppex_sc_surround:
	@ set surround
	bx	lr

@-------------------------------------------
.thumb_func
mppex_HighOffset:
	@ todo...
	bx	lr

@-------------------------------------------
.thumb_func
mppex_PatternLoop:
	bne	.mppex_pl_exit				@ dont update on nonzero ticks
	mov	r2, r8

	lsl	r1, #32-4				@ mask low nibble of parameter
	lsr	r1, #32-4				@ ...
	bne	.mppex_pl_not0				@ is zero?
	
	ldrb	r1, [r2, #MPL_ROW]			@   ...
	strb	r1, [r2, #MPL_PLOOP_ROW]		@    ...
	ldr	r1,=mpp_vars				@    ....
	ldr	r1, [r1, #MPV_PATTREAD_P]		@   .. ...
	mov	r3, #MPL_PLOOP_ADR
	str	r1, [r2, r3]				@  ...  ...
	bx	lr					@ ...    ...
.mppex_pl_not0:						@ otherwise...
	ldrb	r0, [r2, #MPL_PLOOP_TIMES]		@   get pattern loop counter
	cmp	r0, #0					@   zero?
	bne	.mppex_pl_active			@   if not then its already active
	strb	r1, [r2, #MPL_PLOOP_TIMES]		@     zero: save parameter to counter
	b	.mppex_pl_exit_enable			@     exit & enable jump
.mppex_pl_active:					@  nonzero:
	sub	r0, #1					@    decrement counter
	strb	r0, [r2, #MPL_PLOOP_TIMES]		@    save
	beq	.mppex_pl_exit				@    enable jump if not 0
.mppex_pl_exit_enable:
	mov	r0, #1					@    enable jump
	mov	r3, #MPL_PLOOP_JUMP
	strb	r0, [r2, r3]				@    ..
.mppex_pl_exit:						@    exit
	bx	lr					@    ....
.pool

@-------------------------------------------
.thumb_func
mppex_NoteCut:
	lsl	r1, #32-4			@ mask parameter
	lsr	r1, #32-4			@ ..
	cmp	r1, r2				@ compare with tick#
	bne	.mppex_nc_exit			@ if equal:
	mov	r0, #0				@   cut volume
	strb	r0, [r7, #MCH_VOLUME]		@   ..
.mppex_nc_exit:					@ exit
	bx	lr				@ ..

@-------------------------------------------
.thumb_func
mppex_NoteDelay:
	
	mov	r0, r8
	ldrb	r2, [r0, #MPL_TICK]
	lsl	r1, #32-4
	lsr	r1, #32-4
	cmp	r2, r1
	bge	1f
	ldr	r0,=mpp_vars
	strb	r1, [r0, #MPV_NOTEDELAY]
1:	bx	lr

@-------------------------------------------
.thumb_func
mppex_PatternDelay:
	bne	.mppex_pd_quit			@ update on tick0
	lsl	r1, #32-4			@ mask parameter
	lsr	r1, #32-4			@ ..
	mov	r0, r8
	ldrb	r2, [r0, #MPL_PATTDELAY]	@ get patterndelay
	cmp	r2, #0				@ only update if it's 0
	bne	.mppex_pd_quit			@ ..
	add	r1, #1				@ set to param+1
	strb	r1, [r0, #MPL_PATTDELAY]	@ ..
.mppex_pd_quit:					@ exit
	bx	lr				@ ..

@-------------------------------------------
.thumb_func
mppex_SongMessage:

	bne	.mppex_pd_quit			@ update on tick0
	push	{lr}				@ save return address
	lsl	r1, #32-4			@ mask parameter
	lsr	r1, #32-4			@ ..
	ldr	r2,=mmCallback
	ldr	r2, [r2]
	cmp	r2, #0
	beq	1f
	mov	r0, #MPCB_SONGMESSAGE
	
	bl	mpp_call_r2
	@jump2
1:	//pop	{pc}
	pop	{r0}
	bx	r0
.pool

.align 2
.thumb_func
@----------------------------------------------------------------------------------------
mppe_SetTempo:					@ EFFECT Txy: Set Tempo / Tempo Slide
@----------------------------------------------------------------------------------------

	@ 0x  = slide down
	@ 1x  = slide up
	@ 2x+ = set
	
	// BUGGED???
	// not using setbpm for slides???

	cmp	r1, #0x20
	bge	.mppe_st_set
	cmp	r2, #0
	beq	.mppe_st_exit

	mov	r0, r8
	ldrb	r2, [r0, #MPL_BPM]

	cmp	r1, #0x10
	bge	.mppe_st_slideup
.mppe_st_slidedown:
	sub	r2, r1
	cmp	r2, #32
	bge	.mppe_st_save
	mov	r2, #32
.mppe_st_save:
	mov	r0, r2
	b	.mppe_st_set2
.mppe_st_slideup:
	
	lsl	r1, #32-4
	lsr	r1, #32-4
	
	add	r2, r1
	cmp	r2, #255
	blt	.mppe_st_save
	mov	r2, #255
	b	.mppe_st_save
.mppe_st_set:
	cmp	r2, #0
	bne	.mppe_st_exit
	mov	r0, r1
.mppe_st_set2:
	push	{r5,lr}
	mov	r5, r8
	ldr	r1,=mpp_setbpm
	bl	mpp_call_r1
	@jump1
	pop	{r5}
	pop	{r3}
	bx	r3
.mppe_st_exit:
	bx	lr
.pool

.align 2
.thumb_func
@----------------------------------------------------------------------------------------
mppe_FineVibrato:				@ EFFECT Uxy: Fine Vibrato
@----------------------------------------------------------------------------------------

	bne	.mppe_fv_ot
	lsr	r0, r1, #4
	beq	.mppe_fv_nospd
	lsl	r0, #2
	strb	r0, [r7, #MCH_VIBSPD]
.mppe_fv_nospd:
	
	lsl	r0, r1, #32-4
	beq	.mppe_fv_nodep
//	lsr	r0, #32			heh...
	lsr	r0, #32-4
	mov	r1, r8
	ldrb	r1, [r1, #MPL_OLDEFFECTS]
	lsl	r0, r1
	strb	r0, [r7, #MCH_VIBDEP]
.mppe_fv_nodep:

.mppe_fv_ot:
	b	mppe_DoVibrato
.pool

.align 2
.thumb_func
@------------------------------------------------------------------------------------
mppe_SetGlobalVolume:			@ EFFECT Vxy: Set Global Volume
@------------------------------------------------------------------------------------

	bne	.mppe_sgv_exit		@ on tick0:
	mov	r0, r8
	ldrb	r2, [r0, #MPL_FLAGS]
	mov	r3, #(1<<(C_FLAGS_XS-1))+(1<<(C_FLAGS_LS-1))
	tst	r2, r3
	beq	1f
	mov	r2, #0x40
	b	2f
1:	mov	r2, #0x80
2:
	cmp	r1, r2
	blt	1f
	mov	r1, r2
1:	strb	r1, [r0, #MPL_GV]	@ save param to global volume
.mppe_sgv_exit:
	bx	lr
.pool


.align 2
.thumb_func
@----------------------------------------------------------------------------------
mppe_GlobalVolumeSlide:				@ EFFECT Wxy: Global Volume Slide
@----------------------------------------------------------------------------------

	push	{lr}

.mppe_gvs_ot:
	mov	r0, r8
	ldrb	r0, [r0, #MPL_FLAGS]
	lsr	r0, #C_FLAGS_XS
	bcs	1f
	
	mov	r0, #128
	b	2f

1:	mov	r0, #64

2:	push	{r0}
	mov	r0, r8
	ldrb	r0, [r0, #MPL_GV]		@ load global volume
	
	bl	mpph_VolumeSlide		@ slide..	
	
	mov	r1, r8
	strb	r0, [r1, #MPL_GV]		@ save global volume
	pop	{r0}
	bx	r0
//	pop	{pc}				@ exit
.pool


.align 2
.thumb_func
@---------------------------------------------------------------------------------------
mppe_SetPanning:				@ EFFECT Xxy: Set Panning
@---------------------------------------------------------------------------------------

	bne	.mppe_sp_exit			@ on tick0:
	strb	r1, [r7, #MCH_PANNING]		@ set panning=param
.mppe_sp_exit:
	bx	lr
.pool

.align 2
.thumb_func
@---------------------------------------------------------------------------------------
mppe_Panbrello:					@ EFFECT Yxy: Panbrello
@---------------------------------------------------------------------------------------

	@ todo
	bx	lr

.align 2
.thumb_func
@---------------------------------------------------------------------------------
mppe_ZXX:				@ EFFECT Zxy: Set Filter
@---------------------------------------------------------------------------------

@	ZXX IS NOT SUPPORTED
	bx	lr			@     exit
.pool

@=======================================================================================
@                                     OLD EFFECTS
@=======================================================================================

.align 2
.thumb_func
@-----------------------------------------------------------------------------------
mppe_SetVolume:				@ EFFECT 0xx: Set Volume
@-----------------------------------------------------------------------------------

	bne	.mppe_sv_exit		@ on tick0:
	strb	r1, [r7, #MCH_VOLUME]	@ set volume=param
.mppe_sv_exit:
	bx	lr

.align 2
.thumb_func
@-----------------------------------------------------------------------------------
mppe_KeyOff:				@ EFFECT 1xx: Key Off
@-----------------------------------------------------------------------------------
	cmp	r1, r2			@ if tick=param:
	bne	.mppe_ko_exit		@ 
	cmp	r6, #0
	beq	.mppe_ko_exit
	ldrb	r0, [r6, #MCA_FLAGS]	@   clear keyon from flags
	mov	r1, #MCAF_KEYON		@
	bic	r0, r1			@
	strb	r0, [r6, #MCA_FLAGS]	@
.mppe_ko_exit:
	bx	lr			@ finished
.pool

.align 2
.thumb_func
@-----------------------------------------------------------------------------------
mppe_EnvelopePos:			@ EFFECT 1xx: Envelope Position
@-----------------------------------------------------------------------------------
	bne	.mppe_ep_ot		@ on tick0:
	cmp	r6, #0
	beq	.mppe_ep_ot

	@ - NOT SUPPORTED ANYMORE -

@	strh	r1, [r6, #MCA_ENVP_VOL]	@ set volume envelope position
@	strh	r1, [r6, #MCA_ENVP_PAN]	@ set panning envelope positin
					@ pitch envelope wasn't invented yet
.mppe_ep_ot:
	bx	lr			@ finished

.align 2
.thumb_func
@-----------------------------------------------------------------------------------
mppe_OldTremor:				@ EFFECT 3xy: Old Tremor
@-----------------------------------------------------------------------------------
	
	bne	.mppe_ot_ot
	bx	lr
.mppe_ot_ot:
	ldrb	r0, [r7, #MCH_FXMEM]
	cmp	r0, #0
	bne	.mppe_ot_old
.mppe_ot_new:
	ldrb	r0, [r7, #MCH_BFLAGS+1]
	mov	r3, #0b110
	eor	r0, r3
	strb	r0, [r7, #MCH_BFLAGS+1]
	lsr	r0, #3
	bcc	.mppe_ot_low
	lsr	r1, #4
	add	r1, #1
	strb	r1, [r7, #MCH_FXMEM]
	b	.mppe_ot_apply
.mppe_ot_low:
	lsl	r1, #32-4
	lsr	r1, #32-4
	add	r1, #1
	strb	r1, [r7, #MCH_FXMEM]
	b	.mppe_ot_apply
	
.mppe_ot_old:
	sub	r0, #1
	strb	r0, [r7, #MCH_FXMEM]

.mppe_ot_apply:
	ldrb	r2, [r7, #MCH_BFLAGS+1]
	
	lsr	r2, #3
	bcs	.mppe_ot_cut
	mov	r1, #-64&255 
	ldr	r2,=mpp_vars
	strb	r1, [r2, #MPV_VOLPLUS]
.mppe_ot_cut:
	bx	lr

@===============================================================================
@                                    FUNCTIONS!!
@===============================================================================

.align 2
.thumb_func
@--------------------------------------------------------------------------
mpph_PitchSlide_Down:				@ Linear/Amiga slide down
@--------------------------------------------------------------------------

	@ r0 = period
	@ r1 = slide value (/4)
	mov	r2, r8
	ldrb	r2, [r2, #MPL_FLAGS]
	lsr	r2, #C_FLAGS_SS
	bcc	.mpph_psd_amiga
	b	.mpph_psd

.thumb_func
@--------------------------------------------------------------------------
mpph_LinearPitchSlide_Down:			@ Linear slide down
@--------------------------------------------------------------------------

	mov	r2, r8
	ldrb	r2, [r2, #MPL_FLAGS]
	lsr	r2, #C_FLAGS_SS
	bcc	.mpph_psu
.mpph_psd:
	ldr	r2,=mpp_TABLE_LinearSlideDownTable
.mpph_psd_fine:
	lsl	r1, #1
	ldrh	r1, [r2, r1]
	lsr	r0, #5
	mul	r0, r1
	lsr	r0, #16 -5
.mpph_psd_clip:
	cmp	r0, #0
	bge	.mpph_psd_clipdone
	mov	r0, #0
.mpph_psd_clipdone:
	bx	lr
.mpph_psd_amiga:
	lsl	r1, #4
.mpph_psd_amiga_fine:
	add	r0, r1
	lsr	r1, r0, #16+5
	beq	.mpph_psd_clipdone
	mov	r0, #1
	lsl	r0, #16+5
	b	.mpph_psd_clip

.align 2
.thumb_func
@--------------------------------------------------------------------------
mpph_PitchSlide_Up:			@ Linear/Amiga slide up
@--------------------------------------------------------------------------

	@ r0 = period
	@ r1 = slide value (/4)

	mov	r2, r8
	ldrb	r2, [r2, #MPL_FLAGS]
	lsr	r2, #C_FLAGS_SS
	bcc	.mpph_psu_amiga
	b	.mpph_psu
.thumb_func
mpph_LinearPitchSlide_Up:		@ Linear slide up
	mov	r2, r8
	ldrb	r2, [r2, #MPL_FLAGS]
	lsr	r2, #C_FLAGS_SS
	bcc	.mpph_psd
.mpph_psu:
	ldr	r2,=mpp_TABLE_LinearSlideUpTable
	mov	r3, r0
	cmp	r1, #192
	blt	.mpph_psu_notdouble
	add	r3, r3
.mpph_psu_notdouble:
.mpph_psu_fine:
	lsl	r1, #1
	ldrh	r1, [r2, r1]
	lsr	r0, #5
	mul	r0, r1
	lsr	r0, #16-5
	add	r0, r3
.mpph_psu_clip:
	mov	r1, r0
	lsr	r1, #16+5
	beq	.mpph_psu_clipped
	mov	r0, #1
	lsl	r0, #16+5
.mpph_psu_clipped:
	bx	lr
.mpph_psu_amiga:
	lsl	r1, #4
.mpph_psu_amiga_fine:
	sub	r0, r1
	bcs	.mpph_psu_clipped
	mov	r0, #0
	bx	lr

.align 2
.thumb_func
@---------------------------------------------------------------------------
mpph_FinePitchSlide_Up:
@---------------------------------------------------------------------------

	@ r0 = period
	@ r1 = slide value (0-15)
	mov	r2, r8
	ldrb	r2, [r2, #MPL_FLAGS]
	lsr	r2, #C_FLAGS_SS
	bcc	.mpph_fpsu_amiga
	ldr	r2,=mpp_TABLE_FineLinearSlideUpTable
	mov	r3, r0
	b	.mpph_psu_fine
.mpph_fpsu_amiga:
	lsl	r1, #2
	b	.mpph_psu_amiga_fine

.align 2
.thumb_func
@-----------------------------------------------------------------------------------
mpph_FinePitchSlide_Down:
@-----------------------------------------------------------------------------------

	@ r0 = period
	@ r1 = slide value (0-15)
	
	mov	r2, r8
	ldrb	r2, [r2, #MPL_FLAGS]
	lsr	r2, #C_FLAGS_SS
	bcc	.mpph_fpsd_amiga
	ldr	r2,=mpp_TABLE_FineLinearSlideDownTable
	b	.mpph_psd_fine
.mpph_fpsd_amiga:
	lsl	r1, #2
	b	.mpph_psd_amiga_fine
.pool

@-----------------------------------------------------------------------------------

.text
.align 2
.thumb_func
@-----------------------------------------------------------------------------------
mpph_FastForward:
@-----------------------------------------------------------------------------------

	@ r1 = #rows to skip
	cmp	r1, #0
	bne	.mpph_ff
.mpph_ff_exitf:
	bx	lr
.mpph_ff:
	
	mov	r0, r8
	ldrb	r2, [r0, #MPL_NROWS]
	add	r2, #1
	cmp	r1, r2
	bge	.mpph_ff_exitf
	strb	r1, [r0, #MPL_ROW]
	push	{r7,lr}
	ldr	r7,=mmReadPattern //mpp_ReadPattern
.mpph_ff_loop:
	push	{r1,r7}
	bl	mpp_call_r7
	pop	{r1,r7}
	sub	r1, #1
	bne	.mpph_ff_loop

	pop	{r7}
	pop	{r0}
	bx	r0
//	pop	{pc}
.pool

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@          IWRAM CODE          @
@                              @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
.thumb_func
@-----------------------------------------------------------------------------
mpph_VolumeSlide64:
@-----------------------------------------------------------------------------

	mov	r3, #64
	push	{r3}

.thumb_func
@-----------------------------------------------------------------------------
mpph_VolumeSlide:
@-----------------------------------------------------------------------------

	@ r0 = volume
	@ r1 = paramter
	@ r2 = tick#
	@ stack:1 = max volume
	
	mov	r3, r8
	ldrb	r3, [r3, #MPL_FLAGS]
	lsr	r3, #C_FLAGS_XS
	bcs	.mpph_vs_XM
	
	cmp	r1, #0x0F			@  is value 15?
	bne	.mpph_vs_hack1			@    then only slide on tick0
	b	.mpph_vs_fsub
.mpph_vs_hack1:
	cmp	r1, #0xF0			@  is value 15?
	bne	.mpph_vs_hack2			@    then only slide on tick0
	cmp	r2, #0				@    ..
	bne	.mpph_vs_exit			@    ..
	b	.mpph_vs_fadd
.mpph_vs_hack2:

	mov	r3, r1				@ test for Dx0
	lsl	r3, #32-4			@ ..
	bne	.mpph_vs_next1			@ ..
.mpph_vs_add:					@ Dx0:			(used for DxF too)
	cmp	r2, #0
	beq	.mpph_vs_exit
.mpph_vs_fadd:
	lsr	r1, #4				@  fix value
	add	r0, r1				@  add to volume
	pop	{r1}
	cmp	r0, r1				@  clip values past 64
	blt	.mpph_vs_exit2			@  ..
	mov	r0, r1				@  ..
	b	.mpph_vs_exit2			@  ..
.mpph_vs_next1:					@---------------------
	mov	r3, r1				@ test for D0x
	lsr	r3, #4				@ ..
	bne	.mpph_vs_next2			@ ..
.mpph_vs_sub:					@ D0x:
	cmp	r2, #0
	beq	.mpph_vs_exit
.mpph_vs_fsub:
	lsl	r1, #32-4			@  mask value
	lsr	r1, #32-4			@  ..
	
	sub	r0, r1				@  subtract from volume
	bcs	.mpph_vs_exit			@  clip values under 0
	mov	r0, #0				@  ..
	b	.mpph_vs_exit			@  ..
.mpph_vs_next2:					@---------------------
	cmp	r2, #0				@ fine slides now... only slide on tick0
	bne	.mpph_vs_exit			@ ..
	
	mov	r3, r1				@ test for DxF
	lsl	r3, #32-4			@ ..
	lsr	r3, #32-4
	cmp	r3, #0x0F			@ ..
	beq	.mpph_vs_fadd			@ branch
	
	mov	r3, r1				@ test for DFx
	lsr	r3, #4				@ ..
	cmp	r3, #0x0F			@ ..
	beq	.mpph_vs_fsub			@ branch
.mpph_vs_exit:

	pop	{r1}

.mpph_vs_exit2:
	bx	lr				@ exit if all fail
	
.mpph_vs_XM:
	cmp	r2, #0
	beq	.mpph_vs_exit
	
	lsr	r3, r1, #4
	lsl	r1, #32-4
	lsr	r1, #32-4
	sub	r3, r1
	add	r0, r3
	pop	{r1}
	cmp	r0, r1
	blt	.mpph_vsxm_testlow
	mov	r0, r1
.mpph_vsxm_testlow:
	cmp	r0, #0
	bgt	.mpph_vs_exit2
	mov	r0, #0
	bx	lr
.pool

@==========================================================================================
@                                          TABLES
@==========================================================================================

.TEXT

.align 2
/******************************************************************************
 * ST3_FREQTABLE
 *
 * LUT for amiga periods.
 ******************************************************************************/
						.global ST3_FREQTABLE
ST3_FREQTABLE:

.hword	1712*8, 1616*8, 1524*8, 1440*8, 1356*8, 1280*8, 1208*8, 1140*8, 1076*8, 1016*8, 960*8, 907*8	@ MORE ACCURACY SCALARS

@middle octave is 4.
@	
@			 133808 * ( period(NOTE) >> octave )
@	note_st3period = --------------------------------------------
@	 		 middle_c_finetunevalue(INSTRUMENT)
@

.align 2	
@-------------------------------------------------------------------------------------------
mpp_TABLE_LinearSlideUpTable:			@ value = 2^(val/192), 16.16 fixed
@-------------------------------------------------------------------------------------------

.hword	0,     237,   475,   714,   953   @ 0->4		@ ADD 1.0
.hword	1194,  1435,  1677,  1920,  2164  @ 5->9
.hword	2409,  2655,  2902,  3149,  3397  @ 10->14
.hword	3647,  3897,  4148,  4400,  4653  @ 15->19
.hword	4907,  5157,  5417,  5674,  5932  @ 20->24
.hword	6190,  6449,  6710,  6971,  7233  @ 25->29
.hword	7496,  7761,  8026,  8292,  8559  @ 30->34
.hword	8027,  9096,  9366,  9636,  9908  @ 35->39
.hword	10181, 10455, 10730, 11006, 11283 @ 40->44
.hword	11560, 11839, 12119, 12400, 12682 @ 45->49
.hword	12965, 13249, 13533, 13819, 14106 @ 50->54
.hword	14394, 14684, 14974, 15265, 15557 @ 55->59
.hword	15850, 16145, 16440, 16737, 17034 @ 60->64
.hword	17333, 17633, 17933, 18235, 18538 @ 65->69
.hword	18842, 19147, 19454, 19761, 20070 @ 70->74
.hword	20379, 20690, 21002, 21315, 21629 @ 75->79
.hword	21944, 22260, 22578, 22897, 23216 @ 80->84
.hword	23537, 23860, 24183, 24507, 24833 @ 85->89
.hword	25160, 25488, 25817, 26148, 26479 @ 90->94
.hword	26812, 27146, 27481, 27818, 28155 @ 95->99
.hword	28494, 28834, 29175, 29518, 29862 @ 100->104
.hword	30207, 30553, 30900, 31248, 31599 @ 105->109
.hword	31951, 32303, 32657, 33012, 33369 @ 110->114
.hword	33726, 34085, 34446, 34807, 35170 @ 115->119
.hword	35534, 35900, 36267, 36635, 37004 @ 120->124
.hword	37375, 37747, 38121, 38496, 38872 @ 125->129
.hword	39250, 39629, 40009, 40391, 40774 @ 130->134
.hword	41158, 41544, 41932, 42320, 42710 @ 135->139
.hword	43102, 43495, 43889, 44285, 44682 @ 140->144
.hword	45081, 45481, 45882, 46285, 46690 @ 145->149
.hword	47095, 47503, 47917, 48322, 48734 @ 150->154
.hword	49147, 49562, 49978, 50396, 50815 @ 155->159
.hword	51236, 51658, 52082, 52507, 52934 @ 160->164
.hword	53363, 53793, 54224, 54658, 55092 @ 165->169
.hword	55529, 55966, 56406, 56847, 57289 @ 170->174
.hword	57734, 58179, 58627, 59076, 59527 @ 175->179
.hword	59979, 60433, 60889, 61346, 61805 @ 180->184
.hword	62265, 62727, 63191, 63657, 64124 @ 185->189
.hword	64593, 65064, 0,     474,   950   @ 190->194		@ ADD 2.0 w/ 192+
.hword	1427,  1906,  2387,  2870,  3355  @ 195->199
.hword	3841,  4327,  4818,  5310,  5803  @ 200->204
.hword	6298,  6795,  7294,  7794,  8296  @ 205->209
.hword	8800,  9306,  9814,  10323, 10835 @ 210->214
.hword	11348, 11863, 12380, 12899, 13419 @ 215->219
.hword	13942, 14467, 14993, 15521, 16051 @ 220->224
.hword	16583, 17117, 17653, 18191, 18731 @ 225->229
.hword	19273, 19817, 20362, 20910, 21460 @ 230->234
.hword	22011, 22565, 23121, 23678, 24238 @ 235->239
.hword	24800, 25363, 25929, 25497, 27067 @ 240->244
.hword	27639, 28213, 28789, 29367, 29947 @ 245->249
.hword	30530, 31114, 31701, 32289, 32880 @ 250->254
.hword	33473, 34068                      @ 255->256

.align 2
@-------------------------------------------------------------------------------------
mpp_TABLE_LinearSlideDownTable:			@ value = 2^(-val/192), 16.16 fixed
@-------------------------------------------------------------------------------------

.hword	65535, 65300, 65065, 64830, 64596, 64364, 64132, 63901 @ 0->7
.hword	63670, 63441, 63212, 62984, 62757, 62531, 62306, 62081 @ 8->15
.hword	61858, 61635, 61413, 61191, 60971, 60751, 60532, 60314 @ 16->23
.hword	60097, 59880, 59664, 59449, 59235, 59022, 58809, 58597 @ 24->31
.hword	58386, 58176, 57966, 57757, 57549, 57341, 57135, 56929 @ 32->39
.hword	56724, 56519, 56316, 56113, 55911, 55709, 55508, 55308 @ 40->47
.hword	55109, 54910, 54713, 54515, 54319, 54123, 53928, 53734 @ 48->55
.hword	53540, 53347, 53155, 52963, 52773, 52582, 52393, 52204 @ 56->63
.hword	52016, 51829, 51642, 51456, 51270, 51085, 50901, 50718 @ 64->71
.hword	50535, 50353, 50172, 49991, 49811, 49631, 49452, 49274 @ 72->79
.hword	49097, 48920, 48743, 48568, 48393, 48128, 48044, 47871 @ 80->87
.hword	47699, 47527, 47356, 47185, 47015, 46846, 46677, 46509 @ 88->95
.hword	46341, 46174, 46008, 45842, 45677, 45512, 45348, 45185 @ 96->103
.hword	45022, 44859, 44698, 44537, 44376, 44216, 44057, 43898 @104->111
.hword	43740, 43582, 43425, 43269, 43113, 42958, 42803, 42649 @112->119
.hword	42495, 42342, 42189, 42037, 41886, 41735, 41584, 41434 @120->127
.hword	41285, 41136, 40988, 40840, 40639, 40566, 40400, 40253 @128->135
.hword	40110, 39965, 39821, 39678, 39535, 39392, 39250, 39109 @136->143
.hword	38968, 38828, 38688, 38548, 38409, 38271, 38133, 37996 @144->151
.hword	37859, 37722, 37586, 37451, 37316, 37181, 37047, 36914 @152->159
.hword	36781, 36648, 36516, 36385, 36254, 36123, 35993, 35863 @160->167
.hword	35734, 35605, 35477, 35349, 35221, 35095, 34968, 34842 @168->175
.hword	34716, 34591, 34467, 34343, 34219, 34095, 33973, 33850 @176->183
.hword	33728, 33607, 33486, 33365, 33245, 33125, 33005, 32887 @184->191
.hword	32768, 32650, 32532, 32415, 32298, 32182, 32066, 31950 @192->199
.hword	31835, 31720, 31606, 31492, 31379, 31266, 31153, 31041 @200->207
.hword	30929, 30817, 30706, 30596, 30485, 30376, 30226, 30157 @208->215
.hword	30048, 29940, 29832, 29725, 29618, 29511, 29405, 29299 @216->223
.hword	29193, 29088, 28983, 28879, 28774, 28671, 28567, 28464 @224->231
.hword	28362, 28260, 28158, 28056, 27955, 27855, 27754, 27654 @232->239
.hword	27554, 27455, 27356, 27258, 27159, 27062, 26964, 26867 @240->247
.hword	26770, 26674, 26577, 26482, 26386, 26291, 26196, 26102 @248->255
.hword	26008                                                  @ 256

/******************************************************************************
 * IT_PitchTable
 *
 * LUT for linear periods.
 ******************************************************************************/
						.global IT_PitchTable
						.align 2
IT_PitchTable:

.hword	2048, 0,   2170, 0,   2299, 0,   2435, 0,   2580, 0,   2734, 0 @ C-0
.hword	2896, 0,   3069, 0,   3251, 0,   3444, 0,   3649, 0,   3866, 0 @>B-0

.hword	4096, 0,   4340, 0,   4598, 0,   4871, 0,   5161, 0,   5468, 0 @ C-1
.hword	5793, 0,   6137, 0,   6502, 0,   6889, 0,   7298, 0,   7732, 0 @>B-1

.hword	8192, 0,   8679, 0,   9195, 0,   9742, 0,   10321, 0,  10935, 0 @ octave 2
.hword	11585, 0,  12274, 0,  13004, 0,  13777, 0,  14596, 0,  15464, 0

.hword	16384, 0,  17358, 0,  18390, 0,  19484, 0,  20643, 0,  21870, 0 @ octave 3
.hword	23170, 0,  24548, 0,  26008, 0,  27554, 0,  29193, 0,  30929, 0

.hword	32768, 0,  34716, 0,  36781, 0,  38968, 0,  41285, 0,  43740, 0 @ octave 4
.hword	46341, 0,  49097, 0,  52016, 0,  55109, 0,  58386, 0,  61858, 0

.hword	0, 1,      3897, 1,   8026, 1,   12400, 1,  17034, 1,  21944, 1 @ octave 5
.hword	27146, 1,  32657, 1,  38496, 1,  44682, 1,  51236, 1,  58179, 1

.hword	0, 2,      7794, 2,   16051, 2,  24800, 2,  34068, 2,  43888, 2 @ octave 6
.hword	54292, 2,  65314, 2,  11456, 3,  23828, 3,  36936, 3,  50823, 3

.hword	0, 4,      15588, 4,  32103, 4,  49600, 4,  2601, 5,   22240, 5 @ octave 7
.hword	43048, 5,  65092, 5,  22912, 6,  47656, 6,  8336, 7,   36110, 7

.hword	0, 8,      31176, 8,  64205, 8,  33663, 9,  5201, 10,  44481, 10 @ octave 8
.hword	20559, 11, 64648, 11, 45823, 12, 29776, 13, 16671, 14, 6684, 15

.hword	0, 16,     62352, 16, 62875, 17, 1790,  19, 10403, 20, 23425, 21 @ octave 9
.hword	41118, 22, 63761, 23, 26111, 25, 59552, 26, 33342, 28, 13368, 30

.align 2
@-------------------------------------------------------------------------------------
mpp_TABLE_FineLinearSlideUpTable:
@-------------------------------------------------------------------------------------

.hword	0,      59,     118,    178,    237    @ 0->4		ADD 1x
.hword	296,    356,    415,    475,    535    @ 5->9
.hword	594,    654,    714,    773,    833    @ 10->14
.hword	893                                    @ 15

.align 2
@-------------------------------------------------------------------------------------
mpp_TABLE_FineLinearSlideDownTable:
@-------------------------------------------------------------------------------------

.hword	65535, 65477, 65418, 65359, 65300, 65241, 65182, 65359 @ 0->7
.hword	65065, 65006, 64947, 64888, 64830, 64772, 64713, 64645 @ 8->15

@-------------------------------------------------------------------------------------
mpp_TABLE_FineSineData:
@-------------------------------------------------------------------------------------

.byte	  0,  2,  3,  5,  6,  8,  9, 11, 12, 14, 16, 17, 19, 20, 22, 23
.byte	 24, 26, 27, 29, 30, 32, 33, 34, 36, 37, 38, 39, 41, 42, 43, 44
.byte	 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 56, 57, 58, 59
.byte	 59, 60, 60, 61, 61, 62, 62, 62, 63, 63, 63, 64, 64, 64, 64, 64
.byte	 64, 64, 64, 64, 64, 64, 63, 63, 63, 62, 62, 62, 61, 61, 60, 60
.byte	 59, 59, 58, 57, 56, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46
.byte	 45, 44, 43, 42, 41, 39, 38, 37, 36, 34, 33, 32, 30, 29, 27, 26
.byte	 24, 23, 22, 20, 19, 17, 16, 14, 12, 11,  9,  8,  6,  5,  3,  2
.byte	  0, -2, -3, -5, -6, -8, -9,-11,-12,-14,-16,-17,-19,-20,-22,-23
.byte	-24,-26,-27,-29,-30,-32,-33,-34,-36,-37,-38,-39,-41,-42,-43,-44
.byte	-45,-46,-47,-48,-49,-50,-51,-52,-53,-54,-55,-56,-56,-57,-58,-59
.byte	-59,-60,-60,-61,-61,-62,-62,-62,-63,-63,-63,-64,-64,-64,-64,-64
.byte	-64,-64,-64,-64,-64,-64,-63,-63,-63,-62,-62,-62,-61,-61,-60,-60
.byte	-59,-59,-58,-57,-56,-56,-55,-54,-53,-52,-51,-50,-49,-48,-47,-46
.byte	-45,-44,-43,-42,-41,-39,-38,-37,-36,-34,-33,-32,-30,-29,-27,-26
.byte	-24,-23,-22,-20,-19,-17,-16,-14,-12,-11, -9, -8, -6, -5, -3, -2

.end
