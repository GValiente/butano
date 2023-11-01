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

.ifndef MP_FORMAT_MAS_INC
.equ	MP_FORMAT_MAS_INC, 1

@ MAS header structure..................................
.equ	C_MAS_LEN	,0
.equ	C_MAS_INSTN	,1
.equ	C_MAS_SAMPN	,2	
.equ	C_MAS_PATTN	,3
.equ	C_MAS_FLAGS	,4
.equ	C_MAS_GV	,5
.equ	C_MAS_SPEED	,6
.equ	C_MAS_TEMPO	,7
.equ	C_MAS_REP	,8

.equ	C_MAS_CHANVOL	,12
.equ	C_MAS_CHANPAN	,44
.equ	C_MAS_ORDER	,76
.equ	C_MAS_TABLES	,276

.equ	C_FLAGS_GS	,1
.equ	C_FLAGS_OS	,2
.equ	C_FLAGS_SS	,3
.equ	C_FLAGS_XS	,4
.equ	C_FLAGS_DS	,5
///////.equ	C_FLAGS_LS	,5		HUH???
.equ	C_FLAGS_LS	,6

.equ	C_FLAGS_X,	(1<<3)

@ instrument struct.....................................
.equ	C_MASI_GVOL	,0
.equ	C_MASI_FADE	,1
.equ	C_MASI_RANDVOL	,2
.equ	C_MASI_DCT	,3
.equ	C_MASI_NNA	,4
.equ	C_MASI_ENVFLAGS	,5
.equ	C_MASI_PAN	,6
.equ	C_MASI_DCA	,7
.equ	C_MASI_MAP	,8
.equ	C_MASI_ENVELOPES,12
//.equ	C_MASI_ENVELOPES,248

.equ	C_MASIE_SIZE	,0
.equ	C_MASIE_LSTART	,1
.equ	C_MASIE_LEND	,2
.equ	C_MASIE_SSTART	,3
.equ	C_MASIE_SEND	,4
.equ	C_MASIE_NODEC	,5
.equ	C_MASIE_FILTER	,6
.equ	C_MASIE_NODES	,8

.EQU	ENVFLAG_A,	0b1000

@ sample structure......................................
.equ	C_MASS_DV	,0
.equ	C_MASS_PAN	,1
.equ	C_MASS_FREQ	,2
.equ	C_MASS_VIT	,4
.equ	C_MASS_VID	,5
.equ	C_MASS_VIS	,6
.equ	C_MASS_VIR	,8
.equ	C_MASS_GV	,7

.equ	C_MASS_MSLID	,10

@ pattern structure.....................................
.equ	C_MASP_NROWS	,0
.equ	C_MASP_DATA	,1

@ sample structure......................................
.equ	C_SAMPLE_LEN	,0
.equ	C_SAMPLE_LOOP	,4
.equ	C_SAMPLE_POINT	,12
.equ	C_SAMPLE_DATA	,16

.equ	C_SAMPLEN_LSTART,0
.equ	C_SAMPLEN_LEN	,4
.equ	C_SAMPLEN_FORMAT,8
.equ	C_SAMPLEN_REP	,9
.equ	C_SAMPLEN_POINT	,12
.equ	C_SAMPLEN_DATA	,16

.equ	C_SAMPLEC_DFREQ	,10


.endif
