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

@ Layer Information
@ -----------------

.equ	MPL_TICK	,0
.equ	MPL_ROW		,1
.equ	MPL_POSITION	,2
.equ	MPL_NROWS	,3
.equ	MPL_GV		,4
.equ	MPL_SPEED	,5
.equ	MPL_ISPLAYING	,6
.equ	MPL_BPM		,7
.equ	MPL_INSTTABLE	,8
.equ	MPL_SAMPTABLE	,12
.equ	MPL_PATTTABLE	,16
.equ	MPL_SONGADR	,20
.equ	MPL_FLAGS	,24
.equ	MPL_OLDEFFECTS	,25
.equ	MPL_PATTJUMP	,26
.equ	MPL_PATTJUMP_ROW,27
.equ	MPL_FPATTDELAY	,28
.equ	MPL_PATTDELAY	,29

.equ	MPL_PLOOP_ROW	,30
.equ	MPL_PLOOP_TIMES	,31
.equ	MPL_PLOOP_ADR	,32
.equ	MPL_PATTREAD	,36
.equ	MPL_PLOOP_JUMP	,40
.equ	MPL_VALID		,41

.equ	MPL_TICKRATE	,42	@ 1.15 fixed point OR sample count
.equ	MPL_SAMPCOUNT	,44	@ sample timing
.equ	MPL_TICKFRAC	,44	@ vsync  timing 0.16 fixed point

.equ	MPL_MODE	,46
.equ	mpl_reserved2	,47
.equ	MPL_MCH_UPDATE	,48
.equ	MPL_VOLUME	,52
.equ	mpl_reserved3	,54
.equ	MPL_SIZE	,56

@ active information
@--------------------

.equ	MPV_PATTREAD_P	,4
.equ	MPV_AFVOL	,8
.equ	MPV_SAMPOFF	,9
.equ	MPV_VOLPLUS	,10
.equ	MPV_NOTEDELAY	,11
.equ	MPV_PANPLUS	,12
.equ	MPV_SIZE	,14

@ Module Channel
@ --------------

.EQU	MCH_ALLOC,	0 @ ALLOCATED ACTIVE CHANNEL#
.EQU	MCH_CFLAGS,	1 @ PATTERN COMRESSION FLAGS, called "maskvariable" in ITTECH.TXT
.EQU	MCH_PANNING,	2
.EQU	MCH_VOLCMD,	3 @ VOLUME COLUMN COMMAND
.EQU	MCH_EFFECT,	4 @ EFFECT#				} COMBINED
.EQU	MCH_PARAM,	5 @ EFFECT PARAMETER	}
.EQU	MCH_FXMEM,	6 @ EFFECT MEMORY
.EQU	MCH_NOTE,	7 @ TRANSLATED NOTE
.EQU	MCH_FLAGS,	8 @ CHANNEL FLAGS
.EQU	MCH_INST,	9 @ INSTRUMENT#
//.EQU	MCH_PFLAGS,	10 @ PLAYBACK FLAGS (???)
.EQU	MCH_VIBDEP,	11
.EQU	MCH_VIBSPD,	12
.EQU	MCH_VIBPOS,	13
.EQU	MCH_VOLUME,	14 //}combined
.EQU	MCH_CVOLUME,	15 //}
.EQU	MCH_PERIOD,	16
.EQU	MCH_BFLAGS,	20
.EQU	MCH_PNOTE,	22 @ PATTERN NOTE
.EQU	MCH_MEMORY,	23 @ 15 bytes
.EQU	MCH_SIZE,	40 @ sizeof( module_channel ) @ 2 extra bytes

@ Active Channel
@ --------------

.EQU	MCA_PERIOD,	0	@ internal period
.EQU	MCA_FADE,	4	//   }
.EQU	MCA_ENVC_VOL,	6	//   }
.EQU	MCA_ENVC_PAN,	8	//   } COMBINED
.EQU	MCA_ENVC_PIC,	10	//   } during volume reset
.EQU	MCA_AVIB_DEP,	12	//   } AND NEWNOTE, CHECK NEWNOTE ON CHANGES
.EQU	MCA_AVIB_POS,	14	//   }
.EQU	MCA_FVOL,	16	// } COMBINED for SPEED
.EQU	MCA_TYPE,	17	// }
.EQU	MCA_INST,	18
.EQU	MCA_PANNING,	19
.EQU	MCA_VOLUME,	20
.EQU	MCA_SAMPLE,	21
.EQU	MCA_PARENT,	22 // } COMBINED
.EQU	MCA_FLAGS,	23 // }
.EQU	MCA_ENVN_VOL,	24
.EQU	MCA_ENVN_PAN,	25
.EQU	MCA_ENVN_PIC,	26
.EQU	MCA_SFX,	27	@ can store this anywhere
.EQU	MCA_SIZE,	28

@ Active Channel Flags
@ --------------------

.EQU	MCAF_KEYON	,1	@ key is on... LOCKED
.EQU	MCAF_FADE	,2	@ note-fade is activated
.EQU	MCAF_START	,4	@ [re]start sample
.EQU	MCAF_UPDATED	,8	@ already updated by pchannel routine
.EQU	MCAF_ENVEND	,16	@ end of envelope
.EQU	MCAF_VOLENV	,32	@ volume envelope enabled
.EQU	MCAF_SUB	,64	@ sublayer.................locked..
.EQU	MCAF_EFFECT	,128	@ subsublayer.............................LOCKED (mpEffect)

@ Active Channel Types
@ --------------------

.EQU	ACHN_DISABLED	,0	@ LOCKED (multiple routines)
.EQU	ACHN_RESERVED	,1	@ (can't be used [alloc channel])
.EQU	ACHN_BACKGROUND	,2	@ LOCKED (alloc channel)
.EQU	ACHN_FOREGROUND	,3
.EQU	ACHN_CUSTOM	,4

@ Module Channel Flags
@ --------------------

.equ	MF_START	,1
.equ	MF_DVOL		,2
.equ	MF_HASVCMD	,4
.equ	MF_HASFX	,8
.equ	MF_NEWINSTR	,16

.EQU	MF_NOTEOFF	,64	@ LOCKED
.EQU	MF_NOTECUT	,128	@ LOCKED

@.equ	MF_NEWINSTR	,1	@ new instrument
@.EQU	MF_DVOL		,2
@.EQU	MF_START	,4
@.EQU	MF_HASFX	,8
@.EQU	MF_HASVCMD	,16
@.EQU	MF_NOTEOFF	,64	@ LOCKED
@.EQU	MF_NOTECUT	,128	@ LOCKED

@ Other Definitions
@ -----------------

.equ	IT_NNA_CUT	,0	@ New note actions
.equ	IT_NNA_CONT	,1
.equ	IT_NNA_OFF	,2
.equ	IT_NNA_FADE	,3

.equ	IT_DCA_CUT	,0	@ Duplicate check actions
.equ	IT_DCA_OFF	,1
.equ	IT_DCA_FADE	,2

@ Misc Reference
@ --------------

@ BFLAGS:
@ /////ctv nnppttvv
@ nn...............new note action
@ pp...............panbrello waveform
@ tt...............tremolo waveform
@ vv...............vibrato waveform
@ dd...............duplicate check type
@ v................volume envelope enabled
@ t................tremor variable...
@ c................cut channel volume
@ //////...........reserved
