// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)

// timer freq = 2^24 / mixfreq
// mixlen ~= mixfreq * 0.01673
// recommended mixing frequencies: 5734, 7884, 10512, 13379, 15768, 18157,
//                                 21024, 26758, 31536, 36314, 40137, 42048
// other mixing frequencies may cause clicks
// mixlen must be divisible by 2

//==================================================================
//                          GLOBAL SYMBOLS
//==================================================================

    .global     mmMixerMix
    .type       mmMixerMix STT_FUNC

//===============================================
// more definitions
//===============================================

    .equ    SAMPFRAC, 12 @ Number of bits used in fractional part of sample reading

    // Sample structure : mm_mas_gba_sample

    .equ    C_SAMPLE_LEN,  0
    .equ    C_SAMPLE_LOOP, 4
    .equ    C_SAMPLE_DATA, 12

    // Mixer channel : mm_mixer_channel

    .equ    CHN_SIZE, 24

    .equ    CHN_SRC,  0
    .equ    CHN_READ, 4
    .equ    CHN_VOL,  8
    .equ    CHN_PAN,  9
    // 10
    // 11
    .equ    CHN_FREQ, 12
    .equ    CHN_SIZE, 16

//-------------------------------------

    .equ    FETCH_SIZE, 384
    // Frequency threshold (dont use fetch for high freqs!)
    .equ    FETCH_THRESHOLD, (6016) // 7040

//======================================================================
//                               MEMORY
//======================================================================

    .section .bss
    .align 2

mm_fetch:           .space FETCH_SIZE + 16

// 11-bit mixed sample buffer
// data is interleaved
// left, left, right, right, left, left, etc...

//===========================================================================
//                                  PROGRAM
//===========================================================================

    .syntax unified

    .section .iwram, "ax", %progbits
    .arm

    .align  2
//-------------------------------------------------------------------------
mpm_nullsample:
//-------------------------------------------------------------------------
    .byte   128


    .align 2
//-------------------------------------------------------------------------
mmMixerMix: // params = { samples_count }
//-------------------------------------------------------------------------

// exit function if samples == 0
// it will malfunction.

    cmp     r0, #0
    bxeq    lr

// preserve registers

    stmfd   sp!, {r4-r11,lr}

    stmfd   sp!, {r0}           // preserve mixing count

//------------------------------------------------------------------------
// SECTOR 0, INITIALIZATION
//------------------------------------------------------------------------

// clear mixing buffers

    and     r10, r0, #7
    mov     r2, r0, lsr #3      // clearing samps*2*2 bytes (hword*stereo) 32 bytes at a time
    ldr     r0, =mm_mixbuffer
    ldr     r0, [r0]
    mov     r1, #0              // zero variable
    mov     r3, r1
    mov     r4, r1
    mov     r5, r1
    mov     r6, r1
    mov     r7, r1
    mov     r8, r1
    mov     r9, r1

    cmp     r2, #0
    beq     2f

// clear 32 bytes/write

1:  stmia   r0!, {r1, r3-r9}
    subs    r2, r2, #1
    bne     1b
2:

// clear remainder
    cmp     r10, #0
    beq     2f

1:  str     r1, [r0], #4
    subs    r10, r10, #1
    bne     1b
2:

//----------------------------------------------------------------------------------
// BEGIN MIXING ROUTINE
//----------------------------------------------------------------------------------

    ldr    r12, =mm_mix_channels
    ldr    r12, [r12]
    mov    r11, #0              // volume addition

//--------------------
.mpm_cloop:
//--------------------

//----------------------------------------------------------------------
// SECTOR 1, CALCULATIONS
//----------------------------------------------------------------------

// aliases
#define rchan   r12
#define rvolA   r11
#define rsrc    r10
#define rfreq   r9
#define rmixb   r8
#define rread   r7
#define rvolL   r5
#define rvolR   r6
#define rmixc   r4
#define rmixcc  r3

// read source address

    ldr     rsrc, [rchan, #CHN_SRC]
    cmp     rsrc, #0
    bmi     .mpm_next           // EXIT if MSB is set ------------>

// read frequency value

    ldr     rfreq, [rchan, #CHN_FREQ]
    cmp     rfreq, #0
    beq     .mpm_next           // EXIT if zero ----------------=->

    ldr     r0, =mm_ratescale
    ldr     r0, [r0]
    mul     rfreq, r0
    lsr     rfreq, #14

// load mixing buffers

    ldr     rmixb, =mm_mixbuffer
    ldr     rmixb, [rmixb]

// get read position

    ldr     rread, [rchan, #CHN_READ]

// calculate volume

    ldrb    rvolR, [rchan, #CHN_VOL]    // volume = 0-255
    ldrb    r0, [rchan, #CHN_PAN]       // pan = 0-255

    rsb     r0, r0, #256
    mul     rvolL, r0, rvolR            // (vol*pan) = right volume
    mov     rvolL, rvolL, lsr #8
    add     rvolA, rvolA, rvolL         // add to volume counter
    rsb     r0, r0, #256
    mul     rvolR, r0, rvolR            // calc left volume (256-pan)*vol
    mov     rvolR, rvolR, lsr #8
    add     rvolA, rvolA, rvolR, lsl #16

    ldr     rmixc, [sp]                 // get mix count

//****************************************************************
.mpm_remix_test:
//****************************************************************

    mov     r2, #0
    mul     r1, rmixc, rfreq            // get number of samples that will be read

    cmp     rfreq, #FETCH_THRESHOLD
    bge     1f

    cmp     r1, #FETCH_SIZE << 12       // check if its > fetch size
    movhi   r1, #FETCH_SIZE << 12       // if so: clamp to fetch size and set flag
    movhi   r2, #1

1:
//    ldr     r0, [rchan, #CHN_LEN]     // now subtract length - read to get # samples remaining
    ldr     r0, [rsrc, #-C_SAMPLE_DATA + C_SAMPLE_LEN]
    rsb     r0, rread, r0, lsl #SAMPFRAC
//    sub    r0, r0, rread              // in the source
    cmp     r1, r0                      // clamp mix count
    movhi   r1, r0
    bhi     .calc_mix
    cmp     r2, #0
    beq     .mpm_mix_full

.calc_mix:

    push    {r1}                        // preserve sample count

//--------------------------------
.macro DIV_ITER shift
//--------------------------------
    cmp     r0, rfreq, lsl #\shift
    subcs   r0, r0, rfreq, lsl #\shift
    addcs   r2, #1 << \shift
.endm

//--------------------------------
.macro DIVIDER shift
//--------------------------------
    DIV_ITER \shift
.if \shift != 0
    DIVIDER (\shift - 1)
.endif
.endm
//--------------------------------

    mov     r0, r1                      // divide samples / frequency (24bit/16bit)
    mov     r2, #0

1:  subs    r0, r0, rfreq, lsl #16      // divide top part
    addcs   r2, #1 << 16
    bcs     1b
    add     r0, r0, rfreq, lsl #16

    DIVIDER 15                          // divide the rest...

    cmp    r0, #1                       // round up result
    adc    r0, r2, #0

    pop     {r1}                        // restore sample count
    sub     rmixc, r0                   // subtract from mixcount
    mov     rmixcc, r0
    b       .mpm_mix_short

//------------------------------------------------------------------------
// SECTOR 2, MIXING
//------------------------------------------------------------------------

//-----------------------------------------
.mpm_mix_full:
//-----------------------------------------

// mix all samples

    mov     rmixcc, rmixc               // <-- move mixing count
    mov     rmixc, #0                   // clear mixing count
.mpm_mix_short:

//------------------------------------------------------
.mpm_remix:
//------------------------------------------------------

// mix samples...
// preserve registers
    stmfd   sp!, {rmixc,rvolA,rchan}

// zero mixing count??
    cmp     rmixcc, #0
    beq     .mpm_mix_complete           // exit -------->

    cmp     rfreq, #FETCH_THRESHOLD
    bge     .dont_use_fetch

    // [cycle timings assume 3,1 ROM waitstates]

    push    {r3-r12}
    // r10 is SRC!
    ldr     r0,=mm_fetch                        // destination
    add     r10, r10, rread, lsr #12            // add read offset to source
    bic     r10, #0b11                          // align to 32 bits
    add     r1, #4 << 12                        // add safety threshold
    subs    r1, #40 << 12                       // subtract 36

    bcc     .exit_fetch                         // skip large fetch if negative
.fetch:     ldmia    r10!, {r2-r9, r11, r14}    // read 40 samples      [44 cycles]
    stmia   r0!, {r2-r9,r11,r14}                // write 40 samples     [11 cycles]
    subs    r1, #40 << 12                       // count                [1  cycle ]
    bcc     .exit_fetch                         // exit if done         [1  cycle ]
    ldmia   r10!, {r2-r9, r11, r14}             // read 40 samples      [44 cycles]
    stmia   r0!, {r2-r9, r11, r14}              // write 40 samples     [11 cycles]
    subs    r1, #40 << 12                       // count                [1  cycle ]
    bcc     .exit_fetch                         // exit if done         [1  cycle ]
    ldmia   r10!, {r2-r9, r11, r14}             // read 40 samples      [44 cycles]
    stmia   r0!, {r2-r9, r11, r14}              // write 40 samples     [11 cycles]
    subs    r1, #40 << 12                       // count                [1  cycle ]
    bcs     .fetch                              // loop if remaining    [3  cycles]
                                                //                      [173 cycles/120 samples]
.exit_fetch:

    adds    r1, #(40 << 12) - (24 << 12)
    bmi     .end_medfetch
.medfetch:
    ldmia   r10!, {r2-r7}                       // read 24 samples      [26]
    stmia   r0!, {r2-r7}                        // write 24 samples     [7 ]
    subs    r1, #24 << 12                       // count                [1 ]
    bcc     .end_medfetch                       // exit if done         [1 ]
    ldmia   r10!, {r2-r7}                       // read 24 samples      [26]
    stmia   r0!, {r2-r7}                        // write 24 samples     [7 ]
    subs    r1, #24 << 12                       // count                [1 ]
    bcc     .end_medfetch                       // exit if done         [1 ]
    ldmia   r10!, {r2-r7}                       // read 24 samples      [26]
    stmia   r0!, {r2-r7}                        // write 24 samples     [7 ]
    subs    r1, #24 << 12                       // count                [1 ]
    bcs     .medfetch                           // loop                 [3 ]
.end_medfetch:                                  //                      [107]

    adds    r1, #24 << 12                       // add 24 back
    bmi     .end_fetch                          // exit if <= 0
.fetchsmall:
    ldr     r2, [r10], #4                       // read 4 samples       [8]
    str     r2, [r0], #4                        // write 4 samples      [2]
    subs    r1, #4 << 12                        // count                [1]
    ble     .end_fetch                          // exit maybe           [1]
    ldr     r2, [r10], #4                       // read 4 samples       [8]
    str     r2, [r0], #4                        // write 4 samples      [2]
    subs    r1, #4 << 12                        // count                [1]
    bgt    .fetchsmall                          // exit maybe           [3]
.end_fetch:

    pop     {r3-r12}                            // restore regs

fooo:

    mov     r0, rread, lsr #12                  // get read integer
    push    {r0, rsrc}                          // preserve regs
    bic     rread, rread, r0, lsl #12           // clear integer
    and     r0, #0b11                           // mask low bits
    ldr     rsrc, =mm_fetch                     //
    add     rsrc, rsrc, r0                      // offset source (fetch is word aligned!)

.dont_use_fetch:

    tst     rmixb, #0b11                        // test alignment of work output
    beq     .mpm_aligned

#define rsamp1  r1
#define rsampa  r0
#define rsampb  r2
                                                    // routine to WORD align mixing sector
    ldrb    rsampa, [rsrc, rread, lsr #SAMPFRAC]    // load sample
    add     rread, rread, rfreq                     // add frequency
    mul     rsampb, rsampa, rvolL                   // multiply by left volume
    ldrh    rsamp1, [rmixb]                         // add to mixing buffer (left)
    add     rsamp1, rsamp1, rsampb, lsr #5
    strh    rsamp1, [rmixb], #4
    mul     rsampb, rsampa, rvolR                   // multiply by right volume
    ldrh    rsamp1, [rmixb]                         // add to mixing buffer (right)
    add     rsamp1, rsamp1, rsampb, lsr #5
    strh    rsamp1, [rmixb], #2
    sub     rmixcc, rmixcc, #1                      // decrement mix count

#undef rsamp1
#undef rsampa
#undef rsampb

.mpm_aligned:
                                        // determine mixing mode
    cmp     rvolL, rvolR                // if volume_left == volume_right then assume 'center' mixing
    beq     .mpm_mix_ac
    cmp     rvolL, #0                   // if left volume is zero, do 'right' mixing
    beq     .mpm_mix_ar
    cmp     rvolR, #0                   // if right volume is zero, do 'left' mixing
    beq     .mpm_mix_al
    b       mmMix_ArbPanning            // otherwise do arb mixing
.mpm_mix_al:
    b       mmMix_HardLeft
.mpm_mix_ar:
    b       mmMix_HardRight

// center mixing------------
.mpm_mix_ac:
    cmp     rvolL, #0                           // check if volume is zero
    bne     mmMix_CenteredPanning               // mix samples if not zero
    b       mmMix_Skip                          // skip samples if zero
.mpm_mix_complete:

    cmp     rfreq, #FETCH_THRESHOLD
    poplt   {r0, rsrc}                          // restore regs
    addlt   rread, rread, r0, lsl #12           // add old integer to read
    ldmfd   sp!, {rmixc,rvolA,rchan}            // restore more regs

    ldr     r1, [rsrc, #-C_SAMPLE_DATA + C_SAMPLE_LEN]
    lsl     r1, #SAMPFRAC
    //ldr     r1, [rchan, #CHN_LEN]               // check length against position
    cmp     r1, rread
    bgt     .mpm_channelfinished

//    ldr     r1, [rchan, #CHN_LOOP]              // read channel loop

    ldr    r1, [rsrc, #-C_SAMPLE_DATA + C_SAMPLE_LOOP]
    cmp    r1, #0
//    movs    r1, r1, lsl#8                     // mask out high byte (that is VOLUME)
    bmi    .mpm_channel_stop                    // MSB = no loop, stop channel ->


    sub     rread, rread, r1, lsl #(SAMPFRAC)   // subtract loop length (<<SAMPFRAC) from position

    cmp     rmixc, #0                           // mix more samples?
    ble     .mpm_channelfinished                // no ->
    b       .mpm_remix_test                     // yes: loop

//----------------------------------------------------------------
.mpm_channel_stop:
//----------------------------------------------------------------

// *** END OF SAMPLE
    mov     r1, #1 << 31                        // disable channel
    str     r1, [rchan, #CHN_SRC]

// mix zero into the rest of the buffer
    ldr     rsrc, =mpm_nullsample               // mix zero into the rest of the buffer
    mov     rfreq, #0                           // zero freq
    mov     rread, #0                           // zero freq
    movs    rmixcc, rmixc                       // mix remaining amount
    ble     .mpm_channelfinished                // (exit if zero)
    mov     rmixc, #0
    mov     r1, #0
    b       .mpm_remix                          // mix 'zero' into the rest of the data

//---------------------------------------------------------------
.mpm_channelfinished:
//---------------------------------------------------------------

    cmp     rmixc, #0                           // mix more samples?
    bne     .mpm_remix_test                     // yes: loop

//  *** END OF MIXING ***

    str    rread, [rchan, #CHN_READ]            // save read position

//-----------------------
.mpm_next:
//-----------------------

    add     rchan, rchan, #CHN_SIZE             // seek to next channel
    ldr     r0, =mm_mixch_end                   // compare with ending
    ldr     r0, [r0]
    cmp     rchan, r0
    bne     .mpm_cloop                          // loop if !=

//----------------------------------------------------------------------------------
// SECTOR 3, POST-PROCESSING
//----------------------------------------------------------------------------------

#define prmixl      r0
#define prwritel    r2
#define prwriter    r3
#define prcount     r4
#define prvolR      r12
#define prvolL      r11
#define prsamp1     r6
#define prsamp2     r5
#define prsamp3     r7

    ldr     prmixl, =mm_mixbuffer
    ldr     prmixl, [prmixl]

    ldr     prwritel, =mp_writepos
    ldr     prwritel, [prwritel]
    ldr     prwriter, =mm_mixlen
    ldr     prwriter, [prwriter]
    add     prwriter, prwritel, prwriter, lsl #1 // #MP_MIXLEN * 2
    ldmfd   sp!, {prcount}

// get volume accumulators

    mov     prvolR, rvolA, lsr #16 + 1
    mov     prvolR, prvolR, lsl #3

    mov     prvolL, rvolA, lsl #16
    mov     prvolL, prvolL, lsr #16 + 1
    mov     prvolL, prvolL, lsl #3

    subs    prcount, prcount, #1
    ble     .mpm_copy2_end

//--------------------------------------------------
.mpm_copy2:
//--------------------------------------------------

// ***************** LEFT OUTPUT ******************

    ldr     prsamp1, [prmixl], #4               // get 2 mixed samples
    sub     prsamp2, prsamp1, prvolL            // convert to signed

    mov     prsamp2, prsamp2, lsl #16           // mask low hword with sign extension
    movs    prsamp2, prsamp2, asr #16+3         // and convert 11-bit to 8-bit

    cmp     prsamp2, #-128                      // clamp
    movlt   prsamp2, #-128                      //
    cmp     prsamp2, #127                       //
    movgt   prsamp2, #127                       //

                                                // next sample...
    rsbs    prsamp3, prvolL, prsamp1,lsr #16    // convert to signed
    movs    prsamp3, prsamp3, asr #3            // convert 11-bit to 8-bit

    cmp     prsamp3, #-128                      // clamp
    movlt   prsamp3, #-128                      //
    cmp     prsamp3, #127                       //
    movgt   prsamp3, #127                       //

    and     prsamp2, prsamp2, #255              // write to output
    orr     prsamp2, prsamp2, prsamp3, lsl #8   //
    strh    prsamp2, [prwritel], #2             //

// **************** RIGHT OUTPUT ******************

    ldr     prsamp1, [prmixl], #4               // get 2 mixed samples
    sub     prsamp2, prsamp1, prvolR            // convert to signed

    mov     prsamp2, prsamp2, lsl #16           // mask low hword and convert 11-bit to 8-bit
    movs    prsamp2, prsamp2, asr #16 + 3       //

    cmp     prsamp2, #-128                      // clamp value
    movlt   prsamp2, #-128                      //
    cmp     prsamp2, #127                       //
    movgt   prsamp2, #127                       //

                                                // next sample...
    rsbs    prsamp3, prvolR, prsamp1,lsr #16    // convert to signed
    movs    prsamp3, prsamp3, asr #3            // convert 11-bit to 8-bit

    cmp     prsamp3, #-128                      // clamp value
    movlt   prsamp3, #-128                      //
    cmp     prsamp3, #127                       //
    movgt   prsamp3, #127                       //

    and     prsamp2, prsamp2, #255              // write to output
    orr     prsamp2, prsamp2, prsamp3, lsl #8   //
    strh    prsamp2, [prwriter], #2             //

    subs    prcount, prcount, #2                // loop
    bgt     .mpm_copy2                          //

//------------------------------------------------------------------

.mpm_copy2_end:
    ldr     r0, =mp_writepos                    // store new write position
    str     prwritel, [r0]

//------------------------------------------------------------------

    ldmfd   sp!, {r4-r11, lr}                   // restore registers
    bx      lr                                  // phew!

.pool

//================================================================================
//                                 MIXING ROUTINES
//================================================================================

.macro READ_AND_INCREMENT reg
    ldrb    \reg, [rsrc, rread, lsr #SAMPFRAC]
    add     rread, rread, rfreq
.endm

.macro READ_D reg, tmp
    READ_AND_INCREMENT \reg
    READ_AND_INCREMENT \tmp
    orr     \reg, \reg, \tmp, lsl #16
.endm

.macro MIX_D vol, tmp1, tmp2
    READ_D  \tmp1, \tmp2                        // load&combine 2 samples
    mul     \tmp2, \tmp1, \vol                  // multiply by volume
    bic     \tmp2, \tmp2, #0x1F0000             // prepare for shift
.endm

.macro MIX_DA vol, target, tmp1, tmp2
    MIX_D   \vol, \tmp1, \tmp2
    add     \target, \target, \tmp2, lsr #5     // add 11-bit values
.endm

.macro MIX_DC vol, target_a, target_b, tmp1, tmp2
    MIX_D   \vol, \tmp1, \tmp2
    add     \target_a, \target_a, \tmp2, lsr #5 // add 11-bit values
    add     \target_b, \target_b, \tmp2, lsr #5
.endm

.macro    MIX_DB    vol_l, vol_r, target_a, target_b, tmp1, tmp2
    READ_D  \tmp1, \tmp2

.if \target_a != 0
    mul     \tmp2, \tmp1, \vol_l                // multiply by volume
    bic     \tmp2, \tmp2, #0x1F0000             // prepare for shift
    add     \target_a, \target_a, \tmp2, lsr #5 // add 11-bit values
.endif
.if \target_b != 0
    mul     \tmp2, \tmp1, \vol_r                // multiply by volume
    bic     \tmp2, \tmp2, #0x1F0000             // prepare for shift
    add     \target_b, \target_b, \tmp2, lsr #5 // add 11-bit values
.endif
.endm

//-----------------------------------------------------------------------------------
mmMix_Skip:
//-----------------------------------------------------------------------------------

// mix nothing
    mul     r0, rmixcc, rfreq                   // read += samples * frequency
    add     rread, rread, r0
    b       .mpm_mix_complete

//-----------------------------------------------------------------------------------
mmMix_HardLeft:
//-----------------------------------------------------------------------------------
// mix hard panned left

    bl      mmMix_SingleChannel                 // mix left channel
    bgt     mmMix_Remainder                     // mix remaining amount
    b       .mpm_mix_complete                   // return

//-----------------------------------------------------------------------------------
mmMix_HardRight:
//-----------------------------------------------------------------------------------
// hard panned right

    mov     rvolL, rvolR                        // move volume
    add     rmixb, rmixb, #4                    // offset to 'right' data
    bl      mmMix_SingleChannel                 // mix routine
    mov     rvolL, #0                           // clear left volume again
    sub     rmixb, rmixb, #4                    // remove offet
    bgt     mmMix_Remainder                     // mix remaining count
    b       .mpm_mix_complete                   // return

//----------------------------------------
mmMix_SingleChannel:
//----------------------------------------

#define rsamp1  r1
#define rsamp2  r2
#define rsamp3  r11
#define rsamp4  r12
#define rsampa  r0
#define rsampb  r4

// hard panned mixing (single channel mono)
// interleaving really cuts this method's effectiveness :(

// mix 8 samples/loop

    subs    rmixcc, rmixcc, #8
    bmi     .mpmah_8e
.mpmah_8:
    ldmia   rmixb, {rsamp1, rsamp2, rsamp3}     // load data [2nd word not used]
    MIX_DA  rvolL, rsamp1, rsampa, rsampb       // mix data
    str     rsamp1, [rmixb], #8                 // store mixed word
    MIX_DA  rvolL, rsamp3, rsampa, rsampb       // mix data
    str     rsamp3, [rmixb], #8                 // store mixed word
    ldmia   rmixb, {rsamp1, rsamp2, rsamp3}     // load data [2nd word not used]
    MIX_DA  rvolL, rsamp1, rsampa, rsampb       // mix data
    str     rsamp1, [rmixb], #8                 // store mixed word
    MIX_DA  rvolL, rsamp3, rsampa, rsampb       // mix data
    str     rsamp3, [rmixb], #8                 // store mixed word
    subs    rmixcc, rmixcc, #8                  // decrement 8 samples
    bpl     .mpmah_8                            // loop if >= 0
.mpmah_8e:

// mix remainder samples

    adds    rmixcc, rmixcc, #8                  // fix mixing count
    bx      lr                                  // return

#undef rsamp1
#undef rsamp2
#undef rsamp3
#undef rsamp4
#undef rsampa
#undef rsampb

// ----------------------------------------------------------
mmMix_CenteredPanning:
// ----------------------------------------------------------

#define rsamp1  r1
#define rsamp2  r2
#define rsamp3  r4
#define rsamp4  r6
#define rsamp5  r11
#define rsamp6  r12
#define rsampa  r0
#define rsampb  lr

// mix center panning (double channel mono)

    subs    rmixcc, rmixcc, #6
    bmi    .mpmac_6e
.mpmac_6:

    ldmia   rmixb, {rsamp1, rsamp2, rsamp3, rsamp4, rsamp5, rsamp6}     // read words
    MIX_DC  rvolL, rsamp1, rsamp2, rsampa, rsampb                       // mix data
    MIX_DC  rvolL, rsamp3, rsamp4, rsampa, rsampb
    MIX_DC  rvolL, rsamp5, rsamp6, rsampa, rsampb
    stmia   rmixb!, {rsamp1, rsamp2, rsamp3, rsamp4, rsamp5, rsamp6}    // write words
    subs    rmixcc, rmixcc, #6                                          // count
    bpl     .mpmac_6                                                    // loop
.mpmac_6e:

    mov     rvolR, rvolL            // restore right volume (same as left)
    adds    rmixcc, rmixcc, #6      // fix mix count
    bgt     mmMix_Remainder         // mix remaining amount
    b       .mpm_mix_complete       // finished mixing segment

#undef rsamp1
#undef rsamp2
#undef rsamp3
#undef rsamp4
#undef rsamp5
#undef rsamp6
#undef rsampa
#undef rsampb

//---------------------------------------------------
mmMix_ArbPanning: // SLOWEST!
//---------------------------------------------------

#define rsamp1  r1
#define rsamp2  r2
#define rsamp3  r4
#define rsamp4  r11
#define rsampa  r0
#define rsampb  r12
#define rsamp5  r14

    subs    r3, r3, #10
    bmi     .mpmaa_10e

.mpmaa_10:
    ldmia   rmixb, {rsamp1, rsamp2, rsamp3, rsamp4, rsamp5}     // load bufferdata
    MIX_DB  rvolL, rvolR, rsamp1, rsamp2, rsampa, rsampb        // mix
    MIX_DB  rvolL, rvolR, rsamp3, rsamp4, rsampa, rsampb        // mix
    MIX_DB  rvolL, rvolR, rsamp5, 0, rsampa, rsampb             // mix half
    stmia   rmixb!, {rsamp1, rsamp2, rsamp3, rsamp4, rsamp5}    // store bufferdata
    ldmia   rmixb, {rsamp1, rsamp2, rsamp3, rsamp4, rsamp5}     // load bufferdata
    mul     rsampb, rsampa, rvolR                               // mix other half
    bic     rsampb, rsampb, #0x1F0000                           // ..
    add     rsamp1, rsamp1, rsampb, lsr #5                      // ..
    MIX_DB  rvolL, rvolR, rsamp2, rsamp3, rsampa, rsampb        // mix
    MIX_DB  rvolL, rvolR, rsamp4, rsamp5, rsampa, rsampb        // mix
    stmia   rmixb!, {rsamp1, rsamp2, rsamp3, rsamp4, rsamp5}    // store bufferdata
    subs    rmixcc, rmixcc, #10                                 // count
    bpl     .mpmaa_10                                           // loop

.mpmaa_10e:

    adds    rmixcc, rmixcc, #10
    bgt     mmMix_Remainder
    b       .mpm_mix_complete

.pool

#undef rsamp1
#undef rsamp2
#undef rsamp3
#undef rsamp4
#undef rsampa
#undef rsampb

// ---------------------------------------------------------------------------
mmMix_Remainder:
// ---------------------------------------------------------------------------

// (slow function to mix remaining amount of samples)
// assumes mix count isn't zero!

#define rsamp1 r1
#define rsamp2 r2
#define rvol   r11
#define rsampa r0
#define rsampb r4

    orr     rvol, rvolL, rvolR, lsl #16

.mix_remaining:
/*
    ldrb    rsampa, [rsrc, rread, lsr #SAMPFRAC]    // 3 load sample
    add     rread, rread, rfreq                     // 1 add frequency
    mul     rsampb, rsampa, rvolL                   // 2 multiply by volume
    ldrh    rsamp1, [rmixb]                         // 3 load mix buffer entry
    add     rsamp1, rsamp1, rsampb, lsr #5          // 1 add
    strh    rsamp1, [rmixb], #2                     // 2 store
    ldrh    rsamp1, [rmixb, #2]                     // 3
    mul     rsampb, rsampa, rvolR                   // 2
    add     rsamp1, rsamp1, rsampb, lsr #5          // 1
    strh    rsamp1, [rmixb, #2]                     // 2
*/

    ldrb    rsampa, [rsrc, rread, lsr #SAMPFRAC]    // 3 load sample
    add     rread, rread, rfreq                     // 1 add frequency
    mul     rsampb, rvol, rsampa                    // 2 multiply by volume
    ldrh    rsamp1, [rmixb]                         // 3 load mix buffer entry (left)
    bic     rsamp2, rsampb, #0xFF0000               // 1 prep for shift
    add     rsamp1, rsamp1, rsamp2, lsr #5          // 1 add
    strh    rsamp1, [rmixb], #2                     // 2 store (left)
    ldrh    rsamp1, [rmixb, #2]                     // 3 load (right)
    add     rsamp1, rsamp1, rsampb, lsr #16 + 5     // 1 add values
    strh    rsamp1, [rmixb, #2]                     // 2 store (right)

    subs    rmixcc, rmixcc, #2                      // 2
    blt     .end_mix_remaining                      // 1 / exit

/*
    ldrb    rsampa, [rsrc, rread, lsr #SAMPFRAC]    // load sample
    add     rread, rread, rfreq                     // add frequency
    mul     rsampb, rsampa, rvolL                   // multiply by volume
    ldrh    rsamp1, [rmixb]                         // load mix buffer entry
    add     rsamp1, rsamp1, rsampb, lsr #5          // add
    strh    rsamp1, [rmixb], #4                     // store
    ldrh    rsamp1, [rmixb]
    mul     rsampb, rsampa, rvolR
    add     rsamp1, rsamp1, rsampb, lsr #5
    strh    rsamp1, [rmixb], #2
*/

    ldrb    rsampa, [rsrc, rread, lsr #SAMPFRAC]    // 3 load sample
    add     rread, rread, rfreq                     // 1 add frequency
    mul     rsampb, rvol, rsampa                    // 2 multiply by volume
    ldrh    rsamp1, [rmixb]                         // 3 load mix buffer entry (left)
    bic     rsamp2, rsampb, #0xFF0000               // 1 prep for shift
    add     rsamp1, rsamp1, rsamp2, lsr #5          // 1 add
    strh    rsamp1, [rmixb], #4                     // 2 store (left)
    ldrh    rsamp1, [rmixb]                         // 3 load (right)
    add     rsamp1, rsamp1, rsampb, lsr #16 + 5     // 1 add values
    strh    rsamp1, [rmixb], #2                     // 2 store (right)

    bgt     .mix_remaining

.end_mix_remaining:

    b    .mpm_mix_complete

#undef rsamp1
#undef rsamp2
#undef rvol
#undef rsampa
#undef rsampb

.end
