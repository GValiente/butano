/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                            GBA Audio System                              *
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

@ DEFINITIONS

#include "../asm_include/mp_macros.inc"
#include "../asm_include/mp_format_mas.inc"
#include "../asm_include/mp_defs.inc"

@ timer freq = 2^24 / mixfreq
@ mixlen ~= mixfreq * 0.01673
@ recommended mixing frequencies: 5734,7884,10512,13379,15768,18157,21024,26758,31536,36314,40137,42048
@ other mixing frequencies may cause clicks
@ mixlen must be divisible by 2

@==================================================================
@                          GLOBAL SYMBOLS
@==================================================================


	.global	mmVBlank
	.type	mmVBlank STT_FUNC
	.global	mmMixerMix
	.type	mmMixerMix STT_FUNC

	.global	mmMixerSetSource
	.type	mmMixerSetSource STT_FUNC
	.global	mmMixerSetRead
	.type	mmMixerSetRead STT_FUNC
	.global	mmMixerSetFreq
	.type	mmMixerSetFreq STT_FUNC
	.global	mmMixerStopChannel
	.type	mmMixerStopChannel STT_FUNC
	.global	mmMixerInit
	.type	mmMixerInit STT_FUNC
	.global mmMixerChannelActive
	.type	mmMixerChannelActive STT_FUNC
	.global	mmMixerMulFreq
	.type	mmMixerMulFreq STT_FUNC
	.global	mmMixerSetVolume
	.type	mmMixerSetVolume STT_FUNC
	.global	mmMixerSetPan
	.type	mmMixerSetPan STT_FUNC

//	.global	mm_freqscalar
	.global mm_mixlen
	.global mm_bpmdv
	.global	mp_mix_seg
	.global	mp_writepos
	.global	mm_mixchannels

@===============================================
@ more definitions
@===============================================

.EQU	REG_SOUNDCNT_L,	0x4000080
.EQU	REG_SOUNDCNT_H,	0x4000082
.EQU	REG_SOUNDCNT_X,	0x4000084

.EQU	REG_TM0CNT,	0x4000100

.EQU	REG_DMA1SAD,	0x40000BC
.EQU	REG_DMA1DAD,	0x40000C0
.EQU	REG_DMA1CNT,	0x40000C4

.EQU	REG_DMA2SAD,	0x40000C8
.EQU	REG_DMA2DAD,	0x40000CC
.EQU	REG_DMA2CNT,	0x40000D0

.EQU	REG_DMA3SAD,	0x40000D4
.EQU	REG_DMA3DAD,	0x40000D8
.EQU	REG_DMA3CNT,	0x40000DC

.EQU	REG_SGFIFOA,	0x40000A0
.EQU	REG_SGFIFOB,	0x40000A4
.EQU	REG_VCOUNT,	0x4000006

@ MIXER CHANNEL FORMAT

.equ	CHN_SIZE, 24

.equ	CHN_SRC,0
.equ	CHN_READ,4
.equ	CHN_VOL,8
.equ	CHN_PAN,9
// 10
// 11
.equ	CHN_FREQ,12
.equ	CHN_SIZE,16

/////////////////////.equ	CHN_LOOP,8
/////////////////////.equ	CHN_LEN,16

@-------------------------------------

.equ	FETCH_SIZE, 384
.equ	FETCH_THRESHOLD, (6016)//7040		// frequency threshold (dont use fetch for high freqs!)

@======================================================================
@                               MEMORY
@======================================================================

.section .bss
.align 2

mp_writepos: 	.space 4	@ wavebuffer write position

mm_mixbuffer:	.space 4	@ dont move (see init)
mm_mixchannels:	.space 4	@ dont move
mm_wavebuffer:	.space 4	@ dont move
mm_mixlen:	.space 4	@ dont move
mm_mixch_count:	.space 4	@ dont move
mm_mixch_end:	.space 4	@ dont move
mm_ratescale:	.space 4	@ dont move
mm_timerfreq:	.space 4	@ dont move

mm_bpmdv:	.space 4

mp_mix_seg:	.space 1	@ mixing segment select

.align 2

mm_fetch:	.space FETCH_SIZE+16

@ 11-bit mixed sample buffer
@ data is interleaved
@ left,left,right,right,left,left,etc...

@===========================================================================
@                                  PROGRAM
@===========================================================================

.section .iwram, "ax", %progbits

.ARM
.ALIGN 2

mpm_nullsample:
.byte	128

.align 2
@-----------------------------------------------------------------------------------------------------
mmVBlank:				@ vblank wrapper, used to reset dma...HIGH PRIORITY PLEASE!
@-----------------------------------------------------------------------------------------------------
	
	b	.mpvb_disabled		@ disable until ready.. (overwrite this area with NOP)

	ldr	r0,=mp_mix_seg		@ swap mixing segmentl
	ldrsb	r1, [r0]
	mvns	r1, r1
	strb	r1, [r0]
	beq	.mpvb_exit
	
	ldr	r1,=0x040000c6		@ dma control	RESTART DMA
	ldr	r0,=0x0440		@ disable dma
	strh	r0, [r1]
	strh	r0, [r1,#12]
	ldr	r0,=0xB600		@ restart dma
	strh	r0, [r1]
	strh	r0, [r1,#12]
	
	b	.mpvb_dontreset
.mpvb_exit:
	ldr	r0,=mp_writepos		@ restart write position
@	ldr	r1,=mp_playbuffer_l	@ ...
	ldr	r1,=mm_wavebuffer
	ldr	r1, [r1]
	str	r1, [r0]		@ ...
.mpvb_dontreset:

.mpvb_disabled:
	ldr	r0,=0x3007FF8		@ acknowledge interrupt
	ldrh	r2, [r0]
	orr	r2, r2, #0x1
	strh	r2, [r0]
	
	ldr	r0,=mm_vblank_function
	ldr	r0, [r0]
	cmp	r0, #0
	bxeq	lr
	bx	r0
.pool

.align 2
@-------------------------------------------------------------------------
mmMixerMix:				@ params={ samples_count }
@-------------------------------------------------------------------------
	
@ exit function if samples == 0
@ it will malfunction.
	
	cmp	r0, #0
	bne	.mp_zerocheck
	bx	lr

.mp_zerocheck:

@ preserve registers

	stmfd	sp!, {r4-r11,lr}

	stmfd	sp!, {r0}		@ preserve mixing count

@------------------------------------------------------------------------
@ SECTOR 0, INITIALIZATION
@------------------------------------------------------------------------

@ clear mixing buffers

	and	r10, r0, #7
	mov	r2, r0, lsr#3		@ clearing samps*2*2 bytes (hword*stereo) 32 bytes at a time
	ldr	r0,=mm_mixbuffer
	ldr	r0,[r0]
	mov	r1, #0			@ zero variable
	mov	r3, r1
	mov	r4, r1
	mov	r5, r1
	mov	r6, r1
	mov	r7, r1
	mov	r8, r1
	mov	r9, r1

	cmp	r2, #0
	beq	2f

@ clear 32 bytes/write

1:	stmia	r0!, {r1,r3-r9}
	subs	r2, r2, #1
	bne	1b
2:
	
@ clear remainder
	cmp	r10, #0
	beq	2f

1:	str	r1, [r0], #4
	subs	r10, r10, #1
	bne	1b
2:

@----------------------------------------------------------------------------------
@ BEGIN MIXING ROUTINE
@----------------------------------------------------------------------------------

	ldr	r12,=mm_mixchannels
	ldr	r12,[r12]
	mov	r11, #0			@ volume addition
	
@--------------------
.mpm_cloop:
@--------------------

@----------------------------------------------------------------------
@ SECTOR 1, CALCULATIONS
@----------------------------------------------------------------------

@ aliases
#define rchan	r12
#define rvolA	r11
#define rsrc	r10
#define	rfreq	r9
#define rmixb	r8
#define rread	r7
#define rvolL	r5
#define rvolR	r6
#define rmixc	r4
#define rmixcc	r3

@ read source address

	ldr	rsrc, [rchan, #CHN_SRC]
	cmp	rsrc, #0
	bmi	.mpm_next		@ EXIT if MSB is set ------------>

@ read frequency value
	
	ldr	rfreq, [rchan, #CHN_FREQ]
	cmp	rfreq, #0
	beq	.mpm_next		@ EXIT if zero ----------------=->
	
	ldr	r0,=mm_ratescale
	ldr	r0, [r0]
	mul	rfreq, r0
	lsr	rfreq, #14
	
@ load mixing buffers

	ldr	rmixb,=mm_mixbuffer
	ldr	rmixb,[rmixb]

@ get read position
	
	ldr	rread, [rchan, #CHN_READ]

@ calculate volume

	ldrb	rvolR,[rchan, #CHN_VOL]	@ volume = 0-255
	ldrb	r0, [rchan, #CHN_PAN]	@ pan = 0-255
	
	rsb	r0, r0, #256
	mul	rvolL, r0, rvolR	@ (vol*pan) = right volume
	mov	rvolL, rvolL, lsr#8
	add	rvolA, rvolA, rvolL	@ add to volume counter
	rsb	r0, r0, #256
	mul	rvolR, r0, rvolR	@ calc left volume (256-pan)*vol
	mov	rvolR, rvolR, lsr#8
	add	rvolA, rvolA, rvolR, lsl#16
	
	ldr	rmixc, [sp]		@ get mix count
	
@****************************************************************
.mpm_remix_test:
@****************************************************************
	
	mov	r2, #0
	mul	r1, rmixc, rfreq	// get number of samples that will be read
	
	cmp	rfreq, #FETCH_THRESHOLD
	bge	1f
	
	cmp	r1, #FETCH_SIZE<<12	// check if its > fetch size
	movhi	r1, #FETCH_SIZE<<12	// if so: clamp to fetch size and set flag
	movhi	r2, #1
	
1:
//	ldr	r0, [rchan, #CHN_LEN]	// now subtract length - read to get # samples remaining
	ldr	r0, [rsrc, #-C_SAMPLE_DATA+C_SAMPLE_LEN]
	rsb	r0, rread, r0, lsl#SAMPFRAC
//	sub	r0, r0, rread		// in the source
	cmp	r1, r0			// clamp mix count
	movhi	r1, r0
	bhi	.calc_mix
	cmp	r2, #0
	beq	.mpm_mix_full
	
.calc_mix:
	
	
	push	{r1}		// preserve sample count

	
//--------------------------------
.macro DIV_ITER shift
//--------------------------------
	cmp	r0, rfreq, lsl#\shift
	subcs	r0, rfreq, lsl#\shift
	addcs	r2, #1<<\shift
.endm
//--------------------------------
.macro DIVIDER shift
//--------------------------------
	DIV_ITER \shift
.if \shift != 0
	DIVIDER (\shift-1)
.endif
.endm
//--------------------------------
	
	mov	r0, r1			// divide samples / frequency (24bit/16bit)
	mov	r2, #0
	
1:	subs	r0, rfreq, lsl#16	// divide top part
	addcs	r2, #1<<16
	bcs	1b
	add	r0, rfreq, lsl#16
	
	DIVIDER 15			// divide the rest...
	
	cmp	r0, #1			// round up result
	adc	r0, r2, #0
	
//	mov	r0,r1
//	mov	r1,rfreq
//	swi	0x060000
//	cmp	r1, #0
//	addne	r0, #1
	
	pop	{r1}		// restore sample count
	sub	rmixc, r0	// subtract from mixcount
	mov	rmixcc, r0
	b	.mpm_mix_short

@------------------------------------------------------------------------
@ SECTOR 2, MIXING
@------------------------------------------------------------------------
	
@-----------------------------------------
.mpm_mix_full:
@-----------------------------------------

@ mix all samples
	
	mov	rmixcc, rmixc		@ <-- move mixing count
	mov	rmixc, #0		@ clear mixing count
.mpm_mix_short:
	
@------------------------------------------------------
.mpm_remix:
@------------------------------------------------------

@ mix samples...
@ preserve registers
	stmfd	sp!, {rmixc,rvolA,rchan}

@ zero mixing count??
	cmp	rmixcc, #0
	beq	.mpm_mix_complete	@ exit -------->
	
	cmp	rfreq, #FETCH_THRESHOLD
	bge	.dont_use_fetch
	
	/*
	ldr	r0,=mm_fetch		// transfer samples from ROM to RAM
	add	r1, #2<<14		// add threshold for safety
	ldr	r2,=REG_DMA3SAD
	str	r0, [r2, #4]
	add	r0, r10, rread, lsr#12
	bic	r0, #0b11
	str	r0, [r2, #0]
	mov	r0, #(1<<31)+(1<<26)
	add	r0, r1, lsr#14
	str	r0, [r2, #8]
	b	fooo
	*/
	
	// [cycle timings assume 3,1 ROM waitstates]
	
	push	{r3-r12}
	// r10 is SRC!
	ldr	r0,=mm_fetch		// destination
	add	r10, r10, rread, lsr#12	// add read offset to source
	bic	r10, #0b11		// align to 32 bits
	add	r1, #4<<12		// add safety threshold
	subs	r1, #40<<12		// subtract 36
	
	bcc	.exit_fetch		// skip large fetch if negative
.fetch:	ldmia	r10!, {r2-r9,r11,r14}	// read 40 samples	[44 cycles]
	stmia	r0!, {r2-r9,r11,r14}	// write 40 samples	[11 cycles]
	subs	r1, #40<<12		// count		[1  cycle ]
	bcc	.exit_fetch		// exit if done		[1  cycle ]
	ldmia	r10!, {r2-r9,r11,r14}	// read 40 samples	[44 cycles]
	stmia	r0!, {r2-r9,r11,r14}	// write 40 samples	[11 cycles]
	subs	r1, #40<<12		// count		[1  cycle ]
	bcc	.exit_fetch		// exit if done		[1  cycle ]
	ldmia	r10!, {r2-r9,r11,r14}	// read 40 samples	[44 cycles]
	stmia	r0!, {r2-r9,r11,r14}	// write 40 samples	[11 cycles]
	subs	r1, #40<<12		// count		[1  cycle ]
	bcs	.fetch			// loop if remaining	[3  cycles]
					//			[173 cycles/120 samples]
.exit_fetch:

	adds	r1, #(40<<12)-(24<<12)
	bmi	.end_medfetch
.medfetch:
	ldmia	r10!, {r2-r7}		// read 24 samples	[26]
	stmia	r0!, {r2-r7}		// write 24 samples	[7 ]
	subs	r1, #24<<12		// count		[1 ]
	bcc	.end_medfetch		// exit if done		[1 ]
	ldmia	r10!, {r2-r7}		// read 24 samples	[26]
	stmia	r0!, {r2-r7}		// write 24 samples	[7 ]
	subs	r1, #24<<12		// count		[1 ]
	bcc	.end_medfetch		// exit if done		[1 ]
	ldmia	r10!, {r2-r7}		// read 24 samples	[26]
	stmia	r0!, {r2-r7}		// write 24 samples	[7 ]
	subs	r1, #24<<12		// count		[1 ]
	bcs	.medfetch		// loop			[3 ]
.end_medfetch:				//			[107]

	adds	r1, #24<<12		// add 24 back
	bmi	.end_fetch		// exit if <= 0
.fetchsmall:
	ldr	r2, [r10], #4		// read 4 samples	[8]
	str	r2, [r0], #4		// write 4 samples	[2]
	subs	r1, #4<<12		// count		[1]
	ble	.end_fetch		// exit maybe		[1]
	ldr	r2, [r10], #4		// read 4 samples	[8]
	str	r2, [r0], #4		// write 4 samples	[2]
	subs	r1, #4<<12		// count		[1]
	bgt	.fetchsmall		// exit maybe		[3]
.end_fetch:
	
	pop	{r3-r12}		// restore regs
	
fooo:
	
	mov	r0, rread, lsr#12	// get read integer
	push	{r0, rsrc}		// preserve regs
	bic	rread, r0, lsl#12	// clear integer
	and	r0, #0b11		// mask low bits
	ldr	rsrc,=mm_fetch		//
	add	rsrc, rsrc, r0		// offset source (fetch is word aligned!)
	
.dont_use_fetch:
	
	tst	rmixb, #0b11		// test alignment of work output
	beq	.mpm_aligned
	
#define rsamp1	r1 
#define	rsampa	r0
#define rsampb	r2
							@ routine to WORD align mixing sector
	ldrb	rsampa, [rsrc, rread, lsr#SAMPFRAC]	@ load sample
	add	rread, rread, rfreq			@ add frequency
	mul	rsampb, rsampa, rvolL			@ multiply by left volume
	ldrh	rsamp1, [rmixb]				@ add to mixing buffer (left)
	add	rsamp1, rsamp1, rsampb, lsr#5		
	strh	rsamp1, [rmixb], #4
	mul	rsampb, rsampa, rvolR			@ multiply by right volume
	ldrh	rsamp1, [rmixb]				@ add to mixing buffer (right)
	add	rsamp1, rsamp1, rsampb, lsr#5
	strh	rsamp1, [rmixb], #2
	sub	rmixcc, rmixcc, #1			@ decrement mix count
	
#undef rsamp1
#undef rsampa
#undef rsampb

.mpm_aligned:
					// determine mixing mode
	cmp	rvolL, rvolR		// if volume_left == volume_right then assume 'center' mixing
	beq	.mpm_mix_ac		
	cmp	rvolL, #0		// if left volume is zero, do 'right' mixing
	beq	.mpm_mix_ar
	cmp	rvolR, #0		// if right volume is zero, do 'left' mixing
	beq	.mpm_mix_al
	b	mmMix_ArbPanning	// otherwise do arb mixing
.mpm_mix_al:
	b	mmMix_HardLeft
.mpm_mix_ar:
	b	mmMix_HardRight

@ center mixing------------
.mpm_mix_ac:
	cmp	rvolL, #0		// check if volume is zero
	bne	mmMix_CenteredPanning	// mix samples if not zero
	b	mmMix_Skip		// skip samples if zero
.mpm_mix_complete:

	cmp	rfreq, #FETCH_THRESHOLD
	poplt	{r0, rsrc}			// restore regs
	addlt	rread, rread, r0, lsl#12	// add old integer to read
	ldmfd	sp!, {rmixc,rvolA,rchan}	// restore more regs
	
	ldr	r1, [rsrc, #-C_SAMPLE_DATA+C_SAMPLE_LEN]
	lsl	r1, #SAMPFRAC
	//ldr	r1, [rchan, #CHN_LEN]		// check length against position
	cmp	r1, rread
	bgt	.mpm_channelfinished
	
//	ldr	r1, [rchan, #CHN_LOOP]		// read channel loop
	
	ldr	r1, [rsrc, #-C_SAMPLE_DATA+C_SAMPLE_LOOP]
	cmp	r1, #0
//	movs	r1, r1, lsl#8			// mask out high byte (that is VOLUME)
	bmi	.mpm_channel_stop		// MSB = no loop, stop channel ->
	

	sub	rread,rread,r1,lsl#(SAMPFRAC)	// subtract loop length (<<SAMPFRAC) from position

	cmp	rmixc, #0			// mix more samples?	
	ble	.mpm_channelfinished		// no ->
	b	.mpm_remix_test			// yes: loop

@----------------------------------------------------------------
.mpm_channel_stop:
@----------------------------------------------------------------

@ *** END OF SAMPLE
	mov	r1, #1<<31			// disable channel
	str	r1, [rchan, #CHN_SRC]

@ mix zero into the rest of the buffer
	ldr	rsrc,=mpm_nullsample		// mix zero into the rest of the buffer
	mov	rfreq, #0			// zero freq
	mov	rread, #0			// zero freq
	movs	rmixcc, rmixc			// mix remaining amount
	ble	.mpm_channelfinished		// (exit if zero)
	mov	rmixc, #0
	mov	r1, #0
	b	.mpm_remix			// mix 'zero' into the rest of the data
	
@---------------------------------------------------------------
.mpm_channelfinished:
@---------------------------------------------------------------
	
	cmp	rmixc, #0			// mix more samples?
	bne	.mpm_remix_test			// yes: loop
	
@  *** END OF MIXING ***

	str	rread, [rchan, #CHN_READ]	// save read position
	
@-----------------------
.mpm_next:
@-----------------------

	add	rchan, rchan, #CHN_SIZE		// seek to next channel
	ldr	r0,=mm_mixch_end		// compare with ending
	ldr	r0,[r0]				
	cmp	rchan, r0			
	bne	.mpm_cloop			// loop if !=

@----------------------------------------------------------------------------------
@ SECTOR 3, POST-PROCESSING
@----------------------------------------------------------------------------------

#define	prmixl		r0
#define	prwritel	r2
#define	prwriter	r3
#define prcount		r4
#define prvolR		r12
#define prvolL		r11
#define	prsamp1		r6
#define prsamp2		r5
#define prsamp3		r7
	
	ldr	prmixl,=mm_mixbuffer
	ldr	prmixl,[prmixl]
	
	ldr	prwritel,=mp_writepos
	ldr	prwritel, [prwritel]
	ldr	prwriter,=mm_mixlen
	ldr	prwriter, [prwriter]
	add	prwriter, prwritel, prwriter, lsl#1 @#MP_MIXLEN*2
	ldmfd	sp!, {prcount}

@ get volume accumulators
	
	mov	prvolR, rvolA, lsr#16+1
	mov	prvolR, prvolR, lsl#3
	
	mov	prvolL, rvolA, lsl#16
	mov	prvolL, prvolL, lsr#16+1
	mov	prvolL, prvolL, lsl#3
	
	subs	prcount, prcount, #1
	ble	.mpm_copy2_end

@--------------------------------------------------
.mpm_copy2:
@--------------------------------------------------

@ ***************** LEFT OUTPUT ******************

	ldr	prsamp1, [prmixl], #4		@ get 2 mixed samples
	sub	prsamp2, prsamp1, prvolL	@ convert to signed

	mov	prsamp2, prsamp2, lsl#16	@ mask low hword with sign extension
	movs	prsamp2, prsamp2, asr#16+3	@ and convert 11-bit to 8-bit

	cmp	prsamp2, #-128			@ clamp
	movlt	prsamp2, #-128			@
	cmp	prsamp2, #127			@
	movgt	prsamp2, #127			@
	
						@ next sample...
	rsbs	prsamp3, prvolL, prsamp1,lsr#16	@ convert to signed
	movs	prsamp3, prsamp3, asr#3		@ convert 11-bit to 8-bit

	cmp	prsamp3, #-128			@ clamp
	movlt	prsamp3, #-128			@
	cmp	prsamp3, #127			@
	movgt	prsamp3, #127			@
	
	and	prsamp2, prsamp2, #255		@ write to output
	orr	prsamp2, prsamp3, lsl#8		@
	strh	prsamp2, [prwritel], #2		@

@ **************** RIGHT OUTPUT ******************
	
	ldr	prsamp1, [prmixl], #4		@ get 2 mixed samples
	sub	prsamp2, prsamp1, prvolR	@ convert to signed
	
	mov	prsamp2, prsamp2, lsl#16	@ mask low hword and convert 11-bit to 8-bit
	movs	prsamp2, prsamp2, asr#16+3	@
	
	cmp	prsamp2, #-128			@ clamp value
	movlt	prsamp2, #-128			@
	cmp	prsamp2, #127			@
	movgt	prsamp2, #127			@
	
						@ next sample...
	rsbs	prsamp3, prvolR, prsamp1,lsr#16	@ convert to signed
	movs	prsamp3, prsamp3, asr#3		@ convert 11-bit to 8-bit
	
	cmp	prsamp3, #-128			@ clamp value
	movlt	prsamp3, #-128			@
	cmp	prsamp3, #127			@
	movgt	prsamp3, #127			@
	
	and	prsamp2, prsamp2, #255		@ write to output
	orr	prsamp2, prsamp3, lsl#8		@
	strh	prsamp2, [prwriter], #2		@

	subs	prcount, prcount, #2		@ loop
	bgt	.mpm_copy2			@

@------------------------------------------------------------------

.mpm_copy2_end:
	ldr	r0,=mp_writepos			@ store new write position
	str	prwritel, [r0]

@------------------------------------------------------------------

	ldmfd	sp!, {r4-r11,lr}		// restore registers
	bx	lr				// phew!

.pool

@================================================================================
@                                 MIXING ROUTINES
@================================================================================

.macro READ_AND_INCREMENT reg
	ldrb	\reg, [rsrc, rread, lsr#SAMPFRAC]
	add	rread, rread, rfreq
.endm

.macro READ_D reg, tmp
	READ_AND_INCREMENT \reg
	READ_AND_INCREMENT \tmp
	orr	\reg, \tmp, lsl#16
.endm

.macro MIX_D vol, tmp1, tmp2
	READ_D	\tmp1, \tmp2			// load&combine 2 samples
	mul	\tmp2, \tmp1, \vol		// multiply by volume
	bic	\tmp2, \tmp2, #0x1F0000		// prepare for shift
.endm

.macro MIX_DA vol, target, tmp1, tmp2
	MIX_D	\vol, \tmp1, \tmp2
	add	\target, \target, \tmp2, lsr#5	// add 11-bit values
.endm

.macro MIX_DC vol, target_a, target_b, tmp1, tmp2
	MIX_D	\vol, \tmp1, \tmp2
	add	\target_a, \target_a, \tmp2, lsr#5	// add 11-bit values
	add	\target_b, \target_b, \tmp2, lsr#5
.endm

.macro	MIX_DB	vol_l, vol_r, target_a, target_b, tmp1, tmp2
	READ_D	\tmp1, \tmp2
	
.if \target_a != 0
	mul	\tmp2, \tmp1, \vol_l			// multiply by volume
	bic	\tmp2, \tmp2, #0x1F0000			// prepare for shift
	add	\target_a, \target_a, \tmp2, lsr#5	// add 11-bit values
.endif
.if \target_b != 0
	mul	\tmp2, \tmp1, \vol_r			// multiply by volume
	bic	\tmp2, \tmp2, #0x1F0000			// prepare for shift
	add	\target_b, \target_b, \tmp2, lsr#5	// add 11-bit values
.endif
.endm

@-----------------------------------------------------------------------------------
mmMix_Skip:
@-----------------------------------------------------------------------------------

@ mix nothing
	mul	r0, rmixcc, rfreq	@ read += samples * frequency
	add	rread, rread, r0
	b	.mpm_mix_complete

@-----------------------------------------------------------------------------------
mmMix_HardLeft:
@-----------------------------------------------------------------------------------
@ mix hard panned left

	bl	mmMix_SingleChannel	// mix left channel
	bgt	mmMix_Remainder		// mix remaining amount
	b	.mpm_mix_complete	// return

@-----------------------------------------------------------------------------------
mmMix_HardRight:
@-----------------------------------------------------------------------------------
@ hard panned right

	mov	rvolL, rvolR		// move volume
	add	rmixb, rmixb, #4	// offset to 'right' data
	bl	mmMix_SingleChannel	// mix routine
	mov	rvolL, #0		// clear left volume again
	sub	rmixb, rmixb, #4	// remove offet
	bgt	mmMix_Remainder		// mix remaining count
	b	.mpm_mix_complete	// return

@----------------------------------------
mmMix_SingleChannel:
@----------------------------------------

#define rsamp1	r1 
#define rsamp2	r2
#define rsamp3	r11
#define	rsamp4	r12
#define	rsampa	r0
#define rsampb	r4

@ hard panned mixing (single channel mono)
@ interleaving really cuts this method's effectiveness :(

@ mix 8 samples/loop

	subs	rmixcc, rmixcc, #8
	bmi	.mpmah_8e
.mpmah_8:
	ldmia	rmixb, {rsamp1, rsamp2, rsamp3}		// load data [2nd word not used]
	MIX_DA	rvolL, rsamp1, rsampa, rsampb		// mix data
	str	rsamp1, [rmixb], #8			// store mixed word
	MIX_DA	rvolL, rsamp3, rsampa, rsampb		// mix data
	str	rsamp3, [rmixb], #8			// store mixed word
	ldmia	rmixb, {rsamp1, rsamp2, rsamp3}		// load data [2nd word not used]
	MIX_DA	rvolL, rsamp1, rsampa, rsampb		// mix data
	str	rsamp1, [rmixb], #8			// store mixed word
	MIX_DA	rvolL, rsamp3, rsampa, rsampb		// mix data
	str	rsamp3, [rmixb], #8			// store mixed word
	subs	rmixcc, rmixcc, #8			// decrement 8 samples
	bpl	.mpmah_8				// loop if >= 0
.mpmah_8e:

@ mix remainder samples

	adds	rmixcc, rmixcc, #8			// fix mixing count
	bx	lr					// return

#undef rsamp1
#undef rsamp2
#undef rsamp3
#undef rsamp4
#undef rsampa
#undef rsampb

@----------------------------------------------------------
mmMix_CenteredPanning:
@----------------------------------------------------------
	
#define rsamp1	r1 
#define rsamp2	r2
#define rsamp3	r4
#define rsamp4	r6
#define	rsamp5	r11
#define rsamp6	r12
#define	rsampa	r0
#define rsampb	lr

@ mix center panning (double channel mono)
	
	subs	rmixcc, rmixcc, #6
	bmi	.mpmac_6e
.mpmac_6:
	
	ldmia	rmixb, {rsamp1,rsamp2,rsamp3,rsamp4,rsamp5,rsamp6}	// read words
	MIX_DC	rvolL, rsamp1, rsamp2, rsampa, rsampb			// mix data
	MIX_DC	rvolL, rsamp3, rsamp4, rsampa, rsampb
	MIX_DC	rvolL, rsamp5, rsamp6, rsampa, rsampb
	stmia	rmixb!, {rsamp1,rsamp2,rsamp3,rsamp4,rsamp5,rsamp6}	// write words
	subs	rmixcc, rmixcc, #6					// count
	bpl	.mpmac_6						// loop
.mpmac_6e:

	mov	rvolR, rvolL			// restore right volume (same as left)
	adds	rmixcc, rmixcc, #6		// fix mix count
	bgt	mmMix_Remainder			// mix remaining amount
	b	.mpm_mix_complete		// finished mixing segment
	
#undef rsamp1
#undef rsamp2
#undef rsamp3
#undef rsamp4
#undef rsamp5
#undef rsamp6
#undef rsampa
#undef rsampb

@---------------------------------------------------
mmMix_ArbPanning:		// SLOWEST!
@---------------------------------------------------

#define rsamp1	r1 
#define rsamp2	r2
#define rsamp3	r4
#define rsamp4	r11
#define	rsampa	r0
#define rsampb	r12
#define rsamp5	r14

	subs	r3, r3, #10
	bmi	.mpmaa_10e
.mpmaa_10:
	
	ldmia	rmixb, {rsamp1,rsamp2,rsamp3,rsamp4,rsamp5}	// load bufferdata
	MIX_DB	rvolL, rvolR, rsamp1, rsamp2, rsampa, rsampb	// mix
	MIX_DB	rvolL, rvolR, rsamp3, rsamp4, rsampa, rsampb	// mix
	MIX_DB	rvolL, rvolR, rsamp5, 0, rsampa, rsampb		// mix half
	stmia	rmixb!, {rsamp1,rsamp2,rsamp3,rsamp4,rsamp5}	// store bufferdata
	ldmia	rmixb, {rsamp1,rsamp2,rsamp3,rsamp4,rsamp5}	// load bufferdata
	mul	rsampb, rsampa, rvolR				// mix other half
	bic	rsampb, rsampb, #0x1F0000			// ..
	add	rsamp1, rsamp1, rsampb, lsr#5			// ..
	MIX_DB	rvolL, rvolR, rsamp2, rsamp3, rsampa, rsampb	// mix
	MIX_DB	rvolL, rvolR, rsamp4, rsamp5, rsampa, rsampb	// mix
	stmia	rmixb!, {rsamp1,rsamp2,rsamp3,rsamp4,rsamp5}	// store bufferdata
	subs	rmixcc, rmixcc, #10				// count
	bpl	.mpmaa_10					// loop
.mpmaa_10e:
	
	adds	rmixcc, rmixcc, #10
	bgt	mmMix_Remainder
	b	.mpm_mix_complete

.pool

#undef rsamp1  
#undef rsamp2 
#undef rsamp3
#undef rsamp4
#undef rsampa
#undef rsampb

//---------------------------------------------------------------------------
mmMix_Remainder:
//---------------------------------------------------------------------------
// (slow function to mix remaining amount of samples)
// assumes mix count isn't zero!

#define rsamp1 r1
#define rsamp2 r2
#define rvol   r11
#define rsampa r0
#define rsampb r4

	orr	rvol, rvolL, rvolR, lsl#16
	
.mix_remaining:
/*	ldrb	rsampa, [rsrc, rread, lsr#SAMPFRAC]	@ 3 load sample
	add	rread, rread, rfreq			@ 1 add frequency
	mul	rsampb, rsampa, rvolL			@ 2 multiply by volume
	ldrh	rsamp1, [rmixb]				@ 3 load mix buffer entry
	add	rsamp1, rsamp1, rsampb, lsr#5		@ 1 add
	strh	rsamp1, [rmixb], #2			@ 2 store
	ldrh	rsamp1, [rmixb, #2]			@ 3
	mul	rsampb, rsampa, rvolR			@ 2
	add	rsamp1, rsamp1, rsampb, lsr#5		@ 1
	strh	rsamp1, [rmixb, #2]			@ 2
*/
	
	ldrb	rsampa, [rsrc, rread, lsr#SAMPFRAC]	@ 3 load sample
	add	rread, rread, rfreq			@ 1 add frequency
	mul	rsampb, rvol, rsampa			@ 2 multiply by volume
	ldrh	rsamp1, [rmixb]				@ 3 load mix buffer entry (left)
	bic	rsamp2, rsampb, #0xFF0000		@ 1 prep for shift
	add	rsamp1, rsamp1, rsamp2, lsr#5		@ 1 add
	strh	rsamp1, [rmixb], #2			@ 2 store (left)
	ldrh	rsamp1, [rmixb, #2]			@ 3 load (right)
	add	rsamp1, rsamp1, rsampb, lsr#16+5	@ 1 add values
	strh	rsamp1, [rmixb, #2]			@ 2 store (right)
	
	subs	rmixcc, rmixcc, #2			@ 2
	blt	.end_mix_remaining			@ 1/exit

/*	ldrb	rsampa, [rsrc, rread, lsr#SAMPFRAC]	@ load sample
	add	rread, rread, rfreq			@ add frequency
	mul	rsampb, rsampa, rvolL			@ multiply by volume
	ldrh	rsamp1, [rmixb]				@ load mix buffer entry
	add	rsamp1, rsamp1, rsampb, lsr#5		@ add
	strh	rsamp1, [rmixb], #4			@ store
	ldrh	rsamp1, [rmixb]
	mul	rsampb, rsampa, rvolR
	add	rsamp1, rsamp1, rsampb, lsr#5
	strh	rsamp1, [rmixb], #2
*/

	ldrb	rsampa, [rsrc, rread, lsr#SAMPFRAC]	@ 3 load sample
	add	rread, rread, rfreq			@ 1 add frequency
	mul	rsampb, rvol, rsampa			@ 2 multiply by volume
	ldrh	rsamp1, [rmixb]				@ 3 load mix buffer entry (left)
	bic	rsamp2, rsampb, #0xFF0000		@ 1 prep for shift
	add	rsamp1, rsamp1, rsamp2, lsr#5		@ 1 add
	strh	rsamp1, [rmixb], #4			@ 2 store (left)
	ldrh	rsamp1, [rmixb]				@ 3 load (right)
	add	rsamp1, rsamp1, rsampb, lsr#16+5	@ 1 add values
	strh	rsamp1, [rmixb], #2			@ 2 store (right)

	bgt	.mix_remaining
.end_mix_remaining:
	
	b	.mpm_mix_complete
	
#undef rsamp1
#undef rsamp2
#undef rvol
#undef rsampa
#undef rsampb

@============================================================================
@                                 END OF MIXER
@============================================================================

	.TEXT
	.THUMB
	.ALIGN 2

/****************************************************************************
 * mmMixerSetSource( channel, p_sample )
 *
 * Set channel source
 ****************************************************************************/
					.thumb_func
mmMixerSetSource: 

	mov	r2, #CHN_SIZE		// get channel pointer from index
	mul	r0, r2			//
	ldr	r2,=mm_mixchannels	//
	ldr	r2, [r2]		//
	add	r0, r2			//
	
	add	r1, #C_SAMPLE_DATA	// set sample data address
	str	r1, [r0, #CHN_SRC]	//
	
	mov	r1, #0			// reset read position
	str	r1, [r0, #CHN_READ]	//
	
	bx	lr

/****************************************************************************
 * mmMixerSetRead( channel, value )
 *
 * Set channel read position
 ****************************************************************************/
					.thumb_func
mmMixerSetRead:

	mov	r2, #CHN_SIZE		// get channel pointer from index
	mul	r0, r2			//
	ldr	r2,=mm_mixchannels	//
	ldr	r2,[r2]			//
	add	r0, r2			//
	
	str	r1, [r0, #CHN_READ]	// store new offset
	bx	lr			//

/****************************************************************************
 * mmMixerSetFreq
 *
 * Set channel mixing rate
 ****************************************************************************/
					.thumb_func
mmMixerSetFreq:

//	push	{r3, r4}		// FIXME: why would you preserve r3?
	
	mov	r2, #CHN_SIZE		// get channel pointer from index
	mul	r0, r2, r0		// 
	ldr	r2,=mm_mixchannels	//
	ldr	r2,[r2]			//
	add	r0, r0, r2		//
	
	lsl	r1, #2 // ...
	
	strh	r1, [r0, #CHN_FREQ]
	bx	lr
	
/*
	ldr	r4,=mm_freqscalar	// r4=scale
	ldr	r4,[r4]			//

	add	r2, pc, #0		// switch to arm for a nice long multiply
	bx	r2
	
	.ARM
	
	//------------------------------------
	// fix frequency to match mixing rate
	// a = specified frequency
	// hz = a*2y13 / pr
	//------------------------------------
	
	umull	r3, r2, r1, r4		// long multiply
	add	r3, r3, #32768		// shift, etc..
	mov	r3, r3, lsr#16		//
	orr	r3, r3, r2, lsl#16	//
	
	str	r3, [r0, #CHN_FREQ]	// set chan frequency
	
	ldmfd	sp!, {r3,r4}		// pop registers
	bx	lr			// return
	*/
	.THUMB

/****************************************************************************
 * mmMixerMulFreq( channel, value )
 *
 * Scale mixing frequency
 ****************************************************************************/
					.thumb_func
mmMixerMulFreq:

	mov	r2, #CHN_SIZE		// get channel pointer from index
	mul	r0, r2			//
	ldr	r2,=mm_mixchannels	//
	ldr	r2, [r2]		//
	add	r0, r2			//

	ldr	r3, [r0, #CHN_FREQ]	// scale
	mul	r3, r1			//
	lsr	r3, #10			//
	str	r3, [r0, #CHN_FREQ]	//
	bx	lr

/****************************************************************************
 * mmMixerStopChannel( channel )
 *
 * Stop mixing channel
 ****************************************************************************/
					.thumb_func
mmMixerStopChannel:

	mov	r1, #CHN_SIZE		// get channel pointer from index
	mul	r0, r1			//
	ldr	r1,=mm_mixchannels	//
	ldr	r1,[r1]			//
	add	r0, r1			//
		
	mov	r1, #1			// set MSB (disable) of source
	lsl	r1, #31			//
	str	r1, [r0]		//
	bx	lr

/****************************************************************************
 * mmMixerChannelActive( channel )
 *
 * Test if mixing channel is active
 ****************************************************************************/
					.thumb_func
mmMixerChannelActive:

	mov	r1, #CHN_SIZE		// get channel pointer from index
	mul	r0, r1			//
	ldr	r1,=mm_mixchannels	//
	ldr	r1,[r1]			//
	add	r0, r1			//
	
mp_Mixer_ChannelEnabledA:
	ldr	r0, [r0, #CHN_SRC]	// nonzero (-1) = enabled
	asr	r0, #31			// zero         = disabled
	mvn	r0, r0			//
	bx	lr

/****************************************************************************
 * mmMixerSetVolume( channel, volume )
 * 
 * Set channel volume
 ****************************************************************************/
					.thumb_func
mmMixerSetVolume:

	mov	r2, #CHN_SIZE		// get channel pointer from index
	mul	r0, r2			//
	ldr	r2,=mm_mixchannels	//
	ldr	r2,[r2]			//
	add	r0, r2			//
	
	strb	r1, [r0, #CHN_VOL]	// set volume
	
	bx	lr
	
/****************************************************************************
 * mmMixerSetPan( channel, panning )
 *
 * Set channel panning
 ****************************************************************************/
					.thumb_func
mmMixerSetPan:

	mov	r2, #CHN_SIZE		// get channel pointer from index
	mul	r0, r2			//
	ldr	r2,=mm_mixchannels	//
	ldr	r2,[r2]			//
	add	r0, r2			//
	
	strb	r1, [r0, #CHN_PAN]	// set panning
	bx	lr
	
/****************************************************************************
 * mmMixerInit( system )
 *
 * Initialize mixer
 ****************************************************************************/
					.thumb_func
mmMixerInit:

	ldr	r2,=mm_mixbuffer
	ldr	r1, [r0,#MM_GBA_SYSTEM_ACH_COUNT]
	str	r1, [r2,#16]
	
	mov	r3, #CHN_SIZE
	mul	r1, r3
	ldr	r3, [r0,#MM_GBA_SYSTEM_MIXCH]
	str	r3, [r2,#4]
	add	r3, r1
	str	r3, [r2,#20]
	ldr	r1, [r0,#MM_GBA_SYSTEM_MIXMEM]
	str	r1, [r2,#0]
	ldr	r1, [r0,#MM_GBA_SYSTEM_WAVEMEM]
	str	r1, [r2,#8]
	ldr	r1, [r0,#MM_GBA_SYSTEM_MODE]
	lsl	r1, #1
	adr	r3, mp_mixing_lengths
	ldrh	r3, [r3,r1]
	str	r3, [r2,#12]
	adr	r3, mp_rate_scales
	ldrh	r3, [r3, r1]
	str	r3, [r2, #24]
	adr	r3, mp_timing_sheet
	ldrh	r3, [r3, r1]
	str	r3, [r2, #28]
	adr	r3, mp_bpm_divisors
	lsl	r1, #1
	ldr	r3, [r3,r1]

	ldr	r2,=mm_bpmdv
	str	r3, [r2,#0]

	
	ldr	r0,=mm_wavebuffer		@ clear wave buffer
	ldr	r0,[r0]
	ldr	r1,=mm_mixlen
	ldr	r1, [r1]
	mov	r2, #0				@ ..
.mpi_loop1:					@ ..
	stmia	r0!, {r2}			@ ..
	sub	r1, r1, #1			@ ..
	bne	.mpi_loop1			@ ..
	
	ldr	r0,=mp_mix_seg			@ reset mixing segment
	strb	r2, [r0]			@ ..
	
	ldr	r0,=mm_mixchannels		@ disable mixing channels
	ldr	r1,[r0,#12]@ nchannels
	ldr	r0,[r0]
	ldr	r3,=1<<31
.mpi_loop2:
	str	r3, [r0, #CHN_SRC]
	add	r0, #CHN_SIZE
	sub	r1, #1
	bne	.mpi_loop2

	ldr	r0,=mmVBlank			@ enable vblank routine
	ldr	r1,=0xE1A00000			@ ..
	str	r1, [r0]			@ ..
	
	ldr	r0,=REG_SGFIFOA			@ clear fifo data
	str	r2, [r0]			@ ..
	str	r2, [r0, #4]			@ ..
	ldr	r0,=REG_SOUNDCNT_H		@ reset direct sound
	strh	r2, [r0]			@ ..
	ldr	r1,=0x9A0C			@ setup sound [DIRECT SOUND A/B reset,timer0,A=left,B=right,volume=100%]
	strh	r1, [r0]			@ ..
	ldr	r0,=REG_DMA1SAD			@ setup DMA source addresses (playback buffers)
	ldr	r1,=mm_wavebuffer
	ldr	r2, [r1, #4]@mixlen
	ldr	r1, [r1]
	
	@ldr	r1,=mp_playbuffer_l		@ ..
	str	r1, [r0]			@ ..

	add	r1,r2
	add	r1,r2

@	ldr	r1,=mp_playbuffer_r		@ ..
	str	r1, [r0, #12]			@ ..
	
	ldr	r1,=REG_SGFIFOA			@ setup DMA destination (sound fifo)
	str	r1, [r0, #4]			@ ..
	add	r1, #4				@ ..
	str	r1, [r0, #16]			@ ..
	
	ldr	r1,=0xB6000000			@ enable DMA (enable,fifo request,32-bit,repeat)
	str	r1, [r0, #8]			@ ..
	str	r1, [r0, #20]			@ ..
	
	ldr	r0,=REG_SOUNDCNT_X		@ master sound enable
	mov	r1, #0x80			@ ..
	strh	r1, [r0]			@ ..
	
	ldr	r0,=REG_VCOUNT			@ wait for new frame
.mpi_vsync:					@ ..
	ldrh	r1, [r0]			@ skip current vblank period
	cmp	r1, #160			@ ..
	bge	.mpi_vsync			@ ..
.mpi_vsync2:
	ldrh	r1, [r0]			@ wait for new one
	cmp	r1, #160			@ ..
	blt	.mpi_vsync2			@ ..
	
.mpi_vsync_2:					@ pass#2
	ldrh	r1, [r0]			@ skip current vblank period
	cmp	r1, #160			@ ..
	bge	.mpi_vsync_2			@ ..
.mpi_vsync2_2:
	ldrh	r1, [r0]			@ wait for new one
	cmp	r1, #160			@ ..
	blt	.mpi_vsync2_2			@ ..
	
	ldr	r0,=REG_TM0CNT				@ enable sampling timer
	ldr	r1,=mm_timerfreq
	ldr	r1,[r1]
	mov	r2, #0x80
	lsl	r2, #16
	orr	r1, r2
	@ldr	r1,=(-MP_TIMERFREQ&0xFFFF) | (0x80<<16)	@ ..
	str	r1, [r0]				@ ..
	bx	lr					@ finished
	

// round(rate / 59.737)
.align 2
mp_mixing_lengths:
	.hword	136,  176,  224,  264,  304,  352,  448,  528
	@      8khz,10khz,13khz,16khz,18khz,21khz,27khz,32khz

.align 2
//mp_freq_scales:		@ (16khz -> real)
//	.hword 33056, 25536, 20064, 17024, 14784, 12768

// 15768*16384 / rate
mp_rate_scales:
	.hword 31812, 24576, 19310, 16384, 14228, 12288,  9655,  8192
	@       8khz, 10khz, 13khz, 16khz, 18khz, 21khz, 27khz, 32khz
	@       8121, 10512, 13379, 15768, 18157, 21024, 26758, 31536,

.align 2
// gbaclock / rate
mp_timing_sheet:
	.hword -2066,-1596,-1254,-1064, -924, -798, -627, -532
	@       8khz,10khz,13khz,16khz,18khz,21khz,27khz,32khz

.align 2
// rate * 2.5
mp_bpm_divisors:
	.word 20302,26280,33447,39420,45393,52560,66895,78840

	
.end
