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

.ifndef MP_MIXER_DS_INC
.equ MP_MIXER_DS_INC, 1

@ definitions

.equ	MIXER_CHN_SAMP,		0	// mainram address
.equ	MIXER_CHN_CNT,		3	// keyon:1bit,panning:7bits
.equ	MIXER_CHN_FREQ,		4	// unsigned 3.10, top 3 cleared
.equ	MIXER_CHN_VOL,		6	// 0-65535
.equ	MIXER_CHN_READ,		8	// unsigned 22.10
.equ	MIXER_CHN_CVOL,		12	// current volume
.equ	MIXER_CHN_CPAN,		14	// current panning
.equ	MIXER_CHN_SIZE,		16

// scale = 65536*1024*2 / mixrate
.equ	MIXER_SCALE,		4096 //6151

.equ	MIXER_CF_START,		128
.equ	MIXER_CF_SOFT,		2
.equ	MIXER_CF_SURROUND,	4
.equ	MIXER_CF_FILTER,	8
.equ	MIXER_CF_REVERSE,	16

.endif
