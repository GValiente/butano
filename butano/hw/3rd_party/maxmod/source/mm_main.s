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

/******************************************************************************
 *
 * Memory
 *
 ******************************************************************************/

	.BSS
	.ALIGN 2

/******************************************************************************
 * mmCallback
 *
 * Function pointer to user event handler
 ******************************************************************************/
					.global mmCallback
mmCallback:	.space 4

/******************************************************************************
 * mm_ch_mask
 *
 * Bitmask to select which hardware/software channels can be used
 ******************************************************************************/
					.global mm_ch_mask
mm_ch_mask:	.space 4

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

//-----------------------------------------------------------------------------
.end
//-----------------------------------------------------------------------------

