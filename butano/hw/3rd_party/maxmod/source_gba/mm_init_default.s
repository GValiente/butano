/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *               Nintendo DS & Gameboy Advance Sound System                 *
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
 
.global mmInitDefault

@----------------------------------------------------------------------------

#include "../asm_include/mm_gba_setup.inc"

#include "../asm_include/mp_defs.inc"

.equ mixlen, 1056	// 16khz

	.bss
	.align 2

__mixbuffer:
	.space mixlen

	.text
	.thumb
	.align 2

#define MM_SIZEOF_MODCH		40
#define MM_SIZEOF_ACTCH		28
#define MM_SIZEOF_MIXCH		24
	
/****************************************************************************
 * mmInitDefault( soundbank, #channels )
 *
 * Init maxmod with default settings.
 ****************************************************************************/
						.thumb_func
mmInitDefault:
	
	push	{r0,r4,r5,r6,r7,lr}		// preserve regs, push soundbank
	
	//0 mode (3)
	//1 mchcount (#channels)
	//2 achcount (#channels)
	//3 modch
	//4 actch
	//5 mixch
	//6 mixmem (__mixbuffer)
	//7 wavemem
	
	mov	r6, r1				// r6=#channels
	ldr	r0,=MM_SIZEOF_MODCH+MM_SIZEOF_ACTCH+MM_SIZEOF_MIXCH
	mul	r0, r6
	ldr	r4,=mixlen
	add	r0, r4
	bl	malloc
	
	mov	r7, r0				// wavemem = beginning of buffer
	add	r3, r0, r4			// split up buffer into addresses [r3,r4,r5]
	mov	r0, #MM_SIZEOF_MODCH		//
	mul	r0, r6				//
	add	r4, r3, r0			//
	mov	r0, #MM_SIZEOF_ACTCH		//
	mul	r0, r6				//
	add	r5, r4, r0			//
	mov	r0, #3				//
	mov	r1, r6				//
	mov	r2, r6				//
	ldr	r6,=__mixbuffer			// r6 = mixbuffer (iwram)
	
	push	{r0-r7}
	
	mov	r0, sp				// init maxmod, pass init struct
	bl	mmInit				//
	
	add	sp, #MM_GBA_SYSTEM_SIZE		// restore stack
	
	pop	{r4-r7}				// return
	pop	{r0}				//
	bx	r0				//
	
.pool
