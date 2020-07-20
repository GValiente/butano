//
//  No$gba debugger messaging
//
//! \file tonc_nocash.s
//! \author J Vijn
//! \date 20080422 - 20080422

#include "../include/tonc_asminc.h"

/* DECLARATIONS:
	int nocash_puts(const char *str);
	EWRAM_CODE void nocash_message();
	extern EWRAM_DATA char nocash_buffer[80];
*/

		.global nocash_puts
		.global nocash_message
		.global nocash_buffer

BEGIN_FUNC_THUMB(nocash_puts, CSEC_TEXT)
	push	{r4, lr}
	ldr		r4,=nocash_message	@ Get messenger address
	ldr 	r1,=nocash_buffer	@ Get buffer address

	mov 	r2, #0
	mov		r12, r2

	@ Iterate over loop parts
.Lmsg_loop:
		mov 	r2, #0

		@ Copy up to 80 chars and print
.Lmsg_cpy:							@ for(ii=0; ii<80; ii++)
			ldrb	r3, [r0, r2]	@	if((dst[ii]=src[ii]) == '\0')
			strb	r3, [r1, r2]	@		break;
			cmp 	r3, #0
			beq 	.Lmsg_print
			add		r2, #1
			cmp		r2, #80
			bne 	.Lmsg_cpy

		@ Print message
.Lmsg_print:
		bl		.Lmsg_far_call
		
		@ If not at end, continue with next part of string
		add		r0, r2
		add		r12, r2
		cmp		r3, #0
		bne		.Lmsg_loop

	@ Full string done. Set result and return
	mov		r0, r12
	pop		{r4}
	pop		{r1}
	bx		r1
.Lmsg_far_call:
	bx		r4
END_FUNC(nocash_puts)

BEGIN_FUNC_THUMB(nocash_message, CSEC_EWRAM)
	mov 	r12, r12 			@ first ID
	b		.Lmsg_end			@ skip the text section
	.hword	0x6464				@ second ID
	.hword	0					@ flags
nocash_buffer:
	.space	82	 				@ Message buffer

.Lmsg_end:
	bx		lr
END_FUNC(nocash_message)

@ EOF
