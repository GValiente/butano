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

#ifndef MP_DEFS_INC
#define MP_DEFS_INC

@ song 'mode' can be one of the following:
.equ		MPP_PLAY_LOOP	,0
.equ		MPP_PLAY_ONCE	,1
.equ		MPP_PLAY_JINGLE	,2

@ other definitions
#ifdef SYS_GBA

.struct 0
MM_GBA_SYSTEM_MODE:		.space 4
MM_GBA_SYSTEM_MCH_COUNT:	.space 4
MM_GBA_SYSTEM_ACH_COUNT:	.space 4
MM_GBA_SYSTEM_MODCH:		.space 4
MM_GBA_SYSTEM_ACTCH:		.space 4
MM_GBA_SYSTEM_MIXCH:		.space 4
MM_GBA_SYSTEM_MIXMEM:		.space 4
MM_GBA_SYSTEM_WAVEMEM:		.space 4
MM_GBA_SYSTEM_SOUNDBANK:	.space 4
MM_GBA_SYSTEM_SIZE:

.equ		SAMPFRAC,		12	@ # of bits used in fractional part of sample reading
#endif

.equ		MP_SCHANNELS	,4

@ callback parameters

.equ	MMCB_SONGREQUEST	,0x1A	@ nds9
.equ	MMCB_SAMPREQUEST	,0x1B	@ nds9
.equ	MMCB_DELETESONG		,0x1C	@ nds9
.equ	MMCB_DELETESAMPLE	,0x1D	@ nds9

@.equ	MPCB_SAMPMEMORY		,0x1E	@ ---
@.equ	MPCB_SONGMEMORY		,0x1F	@ ---
.equ	MMCB_BANKMEMORY		,0x1E	@ nds9

.equ	MPCB_SONGMESSAGE	,0x2A	@ gba/nds7 song playback
.equ	MPCB_SONGFINISHED	,0x2B	@ gba/nds7

.struct 0
mms_rate:	.space 4
mms_len:	.space 4
mms_function:	.space 4
mms_format:	.space 4
mms_timer:	.space 4
mms_manual:	.space 1
mms_size:

.struct 0				// reverb cfg struct
mmrc_flags:		.space 4
mmrc_memory:		.space 4
mmrc_delay:		.space 2
mmrc_rate:		.space 2
mmrc_feedback:		.space 2
mmrc_panning:		.space 1

.equ	MMRFS_MEMORY,		0
.equ	MMRFS_DELAY,		1
.equ	MMRFS_RATE,		2
.equ	MMRFS_FEEDBACK,		3
.equ	MMRFS_PANNING,		4
.equ	MMRFS_LEFT,		5
.equ	MMRFS_RIGHT,		6
.equ	MMRFS_INVERSEPAN,	7
.equ	MMRFS_NODRYLEFT,	8
.equ	MMRFS_NODRYRIGHT,	9
.equ	MMRFS_8BITLEFT,		10
.equ	MMRFS_16BITLEFT,	11
.equ	MMRFS_8BITRIGHT, 	12
.equ	MMRFS_16BITRIGHT,	13
.equ	MMRFS_DRYLEFT,		14
.equ	MMRFS_DRYRIGHT,		15

// reverbcfg flags
.equ	MMRF_MEMORY,	1<<MMRFS_MEMORY
.equ	MMRF_DELAY,	1<<MMRFS_DELAY
.equ	MMRF_RATE,	1<<MMRFS_RATE
.equ	MMRF_FEEDBACK,	1<<MMRFS_FEEDBACK
.equ	MMRF_PANNING,	1<<MMRFS_PANNING
.equ	MMRF_LEFT,	1<<MMRFS_LEFT
.equ	MMRF_RIGHT,	1<<MMRFS_RIGHT
.equ	MMRF_INVERSEPAN,1<<MMRFS_INVERSEPAN
.equ	MMRF_NODRYLEFT,	1<<MMRFS_DRYLEFT
.equ	MMRF_NODRYRIGHT,1<<MMRFS_DRYRIGHT
.equ	MMRF_8BITLEFT,	1<<MMRFS_8BITLEFT
.equ	MMRF_16BITLEFT,	1<<MMRFS_16BITLEFT
.equ	MMRF_8BITRIGHT, 1<<MMRFS_8BITRIGHT
.equ	MMRF_16BITRIGHT,1<<MMRFS_16BITRIGHT
.equ	MMRF_DRYLEFT,	1<<MMRFS_DRYLEFT
.equ	MMRF_DRYRIGHT,	1<<MMRFS_DRYRIGHT

#endif
