/*

posprintf - a condensed version of sprintf for Thumb, esp. GBA
Copyright (C) 2003 Dan Posluns

The person or persons who have associated work with this document (the "Dedicator" or "Certifier") hereby either (a) certifies that, to the best of his knowledge, the work of authorship identified is in the public domain of the country from which the work is published, or (b) hereby dedicates whatever copyright the dedicators holds in the work of authorship identified below (the "Work") to the public domain. A certifier, moreover, dedicates any copyright interest he may have in the associated work, and for these purposes, is described as a "dedicator" below.

A certifier has taken reasonable steps to verify the copyright status of this work. Certifier recognizes that his good faith efforts may not shield him from liability if in fact the work certified is not in the public domain.

Dedicator makes this dedication for the benefit of the public at large and to the detriment of the Dedicator's heirs and successors. Dedicator intends this dedication to be an overt act of relinquishment in perpetuity of all present and future rights under copyright law, whether vested or contingent, in the Work. Dedicator understands that such relinquishment of all rights includes the relinquishment of all rights to enforce (by lawsuit or otherwise) those copyrights in the Work.

Dedicator recognizes that, once placed in the public domain, the Work may be freely reproduced, distributed, transmitted, used, modified, built upon, or otherwise exploited by anyone for any purpose, commercial or non-commercial, and in any way, including by methods that have not yet been invented or conceived.

Author contact e-mail: dan at danposluns dot com


register map:

MAIN LOOP:							PROCESS16:
r0 <- dest string address			r0 <- d0
r1 <- source string address			r1 <- d1
r2 <- integer to print				r2 <- d2
r3 <-								r3 <- d3
r4 <- current char					r4 <- d4
r5 <- 								r5 <- work register
r6 <- 								r6 <- work register
r7 <- 								r7 <- dest string address
r8 <- number of digits to print		r8 <- number of digits to print
r9 <- leading char (' ' or '0')		r9 <- leading char (' ' or '0')
r10 <- current parameter pointer	r10 <- current parameter ptr
r11 <-								r11 <- 
r12 <-								r12 <- source string address
r14 <-								r14 <- lr

Function parameters:

r0 <- destination string address
r1 <- source string address
r2 <- param1
r3 <- param2

*/
	.thumb
	.thumb_func
	.align
____thumb_aeabi_uidiv:
	ldr     r3, =__aeabi_uidiv
	bx      r3

	.thumb 
	.thumb_func
	.align
	.global	posprintf
	.type	posprintf,function
posprintf:

	push	{r3}					@ push our second and third parameters
	push	{r2}					@ onto the stack in reverse order
	mov		r12, sp					@ r12 <- first parameter pointer

	push	{r4-r7}					@ save clobbered registers
	mov		r4, r8
	mov		r5, r9
	mov		r6, r10
	push	{r4-r6, lr}
	mov		r10, r12				@ r10 <- first parameter pointer

.L_STRINGLOOP:
	ldrb	r4, [r1]				@ load a char from r1
	add		r1, #1					@ advance pointer to next char
	cmp		r4, #'%'				@ if char == '%' then
	beq		.L_FORMATENTRY			@			handle the format specifier
	strb	r4, [r0]				@ store the char back to memory
	add		r0, #1					@ advance pointer to next char
	cmp		r4, #0					@ if char != 0 then
	bne		.L_STRINGLOOP			@			repeat for next char
	@ cleanup and exit
	pop		{r4-r7}					@ restore clobbered registers
	mov		r8, r4
	mov		r9, r5
	mov		r10, r6
	mov		lr, r7
	pop		{r4-r7}
	add		sp, #8					@ remove parameters from stack
	bx		lr						@ return from subroutine
	
.L_FORMATENTRY:
	mov		r5, #0					@ assume no leading character for numbers
	mov		r6, #' '				@ assume print spaces if we do print leads
	mov		r8, r5
	mov		r9, r6
.L_FORMATSPEC:
	ldrb	r4, [r1]				@ load the next char from r1
	add		r1, #1					@ advance pointer to next char
	cmp		r4, #'d'				@ if char == 'd'
	beq		.L_PRINT16				@			print 16-bit number
	cmp		r4, #'s'				@ if char == 's'
	beq		.L_PRINTSTR				@			print string
	cmp		r4, #'0'				@ if char == '0'
	beq		.L_SETLEAD				@			print with leading zeros
	cmp		r4, #'%'				@ if char == '%'
	beq		.L_PRINTSYMBOL			@			print '%' character
	cmp		r4, #'l'				@ if char == 'l'
	beq		.L_PRINT29				@			print 29-bit number
	cmp		r4, #'X'				@ if char == 'X'
	beq		.L_PRINTHEXUC			@			print hexadecimal uppercase
	cmp		r4, #'x'				@ if char == 'x'
	beq		.L_PRINTHEXLC			@			print hexadecimal lowercase
	@ we now assume that we are choosing a number of leading digits to display
	sub		r4, #'0'
	mov		r8, r4					@ r8 <- char - '0'
	b		.L_FORMATSPEC

.L_SETLEAD:
	mov		r6, #'0'
	mov		r9, r6					@ print leading zeros instead of spaces
	b		.L_FORMATSPEC

.L_PRINTSYMBOL:
	strb	r4, [r0]				@ store '%' symbol to memory
	add		r0, #1					@ advance pointer to next char
	b		.L_STRINGLOOP

.L_PRINTSTR:
	mov		r4, r10					@ r4 <- current parameter pointer
	ldr		r2, [r4]				@ r2 <- address of string to print
	add		r4, #4
	mov		r10, r4					@ increase parameter pointer
.L_PRINTSTRLOOP:
	ldrb	r4, [r2]				@ load a char from r2
	add		r2, #1					@ advance pointer to next char
	cmp		r4, #0					@ if char == 0
	beq		.L_STRINGLOOP			@			then we are done
	strb	r4, [r0]				@ store the char back to memory
	add		r0, #1					@ advance pointer to next char
	b		.L_PRINTSTRLOOP

.L_PRINT16:
	mov		r7, r0					@ r7 <- dest string address
	mov		r4, r10					@ r4 <- current parameter pointer
	ldr		r0, [r4]				@ r0 <- 16-bit integer to print
	add		r4, #4
	mov		r10, r4					@ increase parameter pointer
	mov		r3, #0					@ temp marker for L_PRINTSIGN
	cmp		r0, #0					@ if integer to print is negative
	blt		.L_PRINTSIGN			@		print the sign and adjust
.L_SIGNDONE:
	mov		lr, pc					@ save this location
	bl		.L_PROCESS16			@ process a 16-bit number
	b		.L_STRINGLOOP			@ return when done

.L_PRINTSIGN:
	mov		r4, #'-'
	strb	r4, [r7]				@ print '-' character
	add		r7, #1					@ advance pointer to next char
	neg		r0, r0					@ r2 is now positive
	mov		r4, r8
	sub		r4, #1					@ print one fewer character
	mov		r8, r4					@ r8 <- new value
	cmp		r3, #0					@ check to see who called us
	beq		.L_SIGNDONE
	b		.L_SIGN29DONE

.L_PRINT29:
	mov		r7, r0					@ r7 <- dest string address
	mov		r4, r10					@ r4 <- current parameter pointer
	ldr		r0, [r4]				@ r0 <- 16-bit integer to print
	add		r4, #4
	mov		r10, r4					@ increase parameter pointer
	mov		r3, #1					@ temp marker for L_PRINTSIGN
	cmp		r0, #0					@ if integer to print is negative
	blt		.L_PRINTSIGN			@		print the sign and adjust
.L_SIGN29DONE:
	mov		r12, r1
	mov		r1, #0x27
	lsl		r1, r1, #8
	add		r1, #0x10				@ r1 <- 0x2710 == 10000
	bl		____thumb_aeabi_uidiv	@ split number by dividing by 10000
	mov		r3, #0
	sub		r3, #4
	add		r8, r3					@ subtract 4 from digits to display
	cmp		r0, #0					@ if the first chunk is empty
	beq		.L_P29SKIP				@		then skip it
	push	{r1}					@ save the second number to display
	mov		r1, r12
	mov		lr, pc					@ save this location
	bl		.L_PROCESS16			@ process a 16-bit number
	mov		r12, r1
	pop		{r1}					@ load in the second number
	mov		r3, #0
	mov		r8, r3					@ print leading symbols now!
	mov		r3, #'0'
	mov		r9, r3					@ make sure they are zeros!
.L_P29SKIP:
	mov		r0, r1					@ get ready to print second number
	mov		r1, #4
	add		r8, r1					@ add 4 back on to digits
	mov		r1, r12
	mov		lr, pc					@ save this location
	bl		.L_PROCESS16			@ process a 16-bit number
	b		.L_STRINGLOOP

.L_PRINTHEXLC:
	mov		r7, #39
	mov		r12, r7					@ lowercase offset
	b		.L_PRINTHEX
.L_PRINTHEXUC:
	mov		r7, #7
	mov		r12, r7					@ uppercase offset
.L_PRINTHEX:
	mov		r4, r10					@ r4 <- current parameter pointer
	ldr		r2, [r4]				@ r2 <- integer to print
	add		r4, #4
	mov		r10, r4					@ increase parameter pointer
	mov		r4, #28					@ r4 <- 8 digits to cycle through
	mov		r6, #0xF				@ r6 <- nibble mask
	mov		r7, #0					@ r7 <- print flag
.L_PRINTHEXLOOP:
	mov		r3, r2
	lsr		r3, r4
	and		r3, r6					@ r3 <- (n >> (cycle * 4)) & 0xF
	orr		r7, r3					@ if we have not encountered a digit
	beq		.L_PH_LEADZERO			@ 			then it is a leading zero
	add		r3, #'0'
	mov		r5, r12					@ get ready to print a letter
	cmp		r3, #'9'				@ if the digit is in the alpha range
	bgt		.L_PH_ALPHA				@			then print a letter
	mov		r5, #0					@ else do nothing
.L_PH_ALPHA:
	add		r3, r5					@ add offset to correct letter
	strb	r3, [r0]				@ store the char in memory
	add		r0, #1					@ advance pointer to next char
	sub		r4, #4					@ advance to next digit
	bge		.L_PRINTHEXLOOP			@ loop until done
	b		.L_STRINGLOOP

.L_PH_LEADZERO:
	lsr		r5, r4, #2				@ r5 <- which digit we are on
	sub		r4, #4					@ if this is our last digit
	blt		.L_PH_FINAL				@		then print a zero for sure
	cmp		r8, r5					@ if r8 < current digit
	ble		.L_PRINTHEXLOOP			@		then keep looping
	mov		r5, r9					@ r5 <- leading symbol to print
	strb	r5, [r0]				@ store the char in memory
	add		r0, #1					@ advance pointer to next char
	b		.L_PRINTHEXLOOP
.L_PH_FINAL:
	mov		r3, #'0'				@ if n == 0, print at least one 0
	strb	r3, [r0]
	add		r0, #1
	b		.L_STRINGLOOP

.L_PROCESS16:
	mov		r12, r1					@ free up registers
	mov		r5, #0xF
	lsr		r1, r0, #4
	lsr		r2, r0, #8
	lsr		r3, r0, #12
	and		r0, r5					@ r0 <- n & 0xF
	and		r1, r5					@ r1 <- (n >> 4) & 0xF
	and		r2, r5					@ r2 <- (n >> 8) & 0xF
	and		r3, r5					@ r3 <- (n >> 12) & 0xF
	mov		r6, r3
	add		r6, r2
	add		r6, r1
	lsl		r5, r6, #2
	lsl		r6, r6, #1
	add		r0, r6
	add		r0, r5					@ r0 <- 6 * (d3 + d2 + d1) + d0
	@ divide by ten: multiply by 0x19A shifted right by 12
	lsr		r5, r0, #2
	add		r5, r0
	lsr		r5, r5, #1				@ r5 <- ((d0 >> 2) + i) >> 1
	add		r5, r0
	lsr		r5, r5, #3				@ r5 = (r5 + d0) >> 3
	add		r5, r0
	lsr		r5, r5, #1				@ r5 = (r5 + d0) >> 1
	add		r5, r0
	lsr		r5, r5, #4				@ r5 <- d0 / 10
	@ calculate remainder as d0
	lsl		r6, r5, #2
	add		r6, r5
	lsl		r6, r6, #1				@ r6 <- q * 10
	sub		r0, r6					@ r0 <- d0 - (q * 10)
	@ finished with d0, now calculate d1
	lsl		r6, r3, #3
	add		r5, r6
	add		r5, r3					@ r5 <- q + 9 * d3
	lsl		r6, r2, #2
	add		r5, r6
	add		r5, r2					@ r5 <- q + 9 * d3 + 5 * d2
	add		r1, r5					@ r1 <- d1 + r5
	beq		.L_LEAD_D1
	@ divide d1 by ten: multiply by 0x19A shifted right by 12
	lsr		r5, r1, #2
	add		r5, r1
	lsr		r5, r5, #1
	add		r5, r1
	lsr		r5, r5, #3
	add		r5, r1
	lsr		r5, r5, #1
	add		r5, r1
	lsr		r5, r5, #4				@ r5 <- d1 / 10
	@ calculate remainder as d1
	lsl		r6, r5, #2
	add		r6, r5
	lsl		r6, r6, #1
	sub		r1, r6					@ r1 <- d1 - (q * 10)
	@ finished with d1, now calculate d2
	lsl		r2, r2, #1
	add		r2, r5					@ r2 <- 2 * d2 + q
	mov		r5, r2
	orr		r5, r3					@ if (!d2) && (!d3)
	beq		.L_LEAD_D2				@			then skip
	@ divide d2 by ten: multiply by 0x1A >> 8 is sufficient
	lsr		r5, r2, #2
	add		r5, r2
	lsr		r5, r5, #1
	add		r5, r2
	lsr		r5, r5, #4				@ r5 <- d2 / 10
	@ calculate remainder as d2
	lsl		r6, r5, #2
	add		r6, r5
	lsl		r6, r6, #1
	sub		r2, r6					@ r2 <- d2 - (q * 10)
	@ finished with d2, now calculate d3
	lsl		r3, r3, #2
	add		r3, r5
	beq		.L_LEAD_D3
	@ divide d3 by ten: multiply by 0x1A >> 8 is sufficient
	lsr		r5, r3, #2
	add		r5, r3
	lsr		r5, r5, #1
	add		r5, r3
	lsr		r5, r5, #4				@ r5 <- d3 / 10
	@ calculate remainder as d3
	lsl		r6, r5, #2
	add		r6, r5
	lsl		r6, r6, #1
	sub		r3, r6					@ r3 <- d3 - (q * 10)
	@ finished with d3, d4 will automatically be quotient
	mov		r4, r5
	beq		.L_LEAD_D4
	@ now print any leading digits if we are using all five
	mov		r5, r8
	mov		r6, r9
	sub		r5, #4					@ already printed five digits
.L_EXTRA_LEAD_LOOP:
	sub		r5, #1
	ble		.L_DONE_EXTRA_LEAD
	strb	r6, [r7]				@ print a leading character
	add		r7, #1
	b		.L_EXTRA_LEAD_LOOP
.L_DONE_EXTRA_LEAD:
	@ now print the fifth digit (d4)
	add		r4, #'0'				@ r4 <- d4 + '0'
	strb	r4, [r7]				@ store a character
	add		r7, #1					@ advance string pointer
.L_DONE_D4:
	add		r3, #'0'
	strb	r3, [r7]
	add		r7, #1
.L_DONE_D3:
	add		r2, #'0'
	strb	r2, [r7]
	add		r7, #1
.L_DONE_D2:
	add		r1, #'0'
	strb	r1, [r7]
	add		r7, #1
.L_DONE_D1:
	add		r0, #'0'
	strb	r0, [r7]
	add		r7, #1
	@ Done at last! Clean up and return to calling routine
	mov		r0, r7					@ restore r0 <- dest string address
	mov		r1, r12					@ restore r1 <- source string address
	mov		pc, lr					@ return from subroutine
	
.L_LEAD_D4:
	mov		r5, r9					@ r5 <- leading character
	mov		r6, r8
	sub		r6, #4					@ r6 <- # of chars to print
	ble		.L_DONE_D4
.L_IN_D4:
	strb	r5, [r7]				@ store a character
	add		r7, #1					@ advance string pointer
	sub		r6, #1					@ if chars to print > 0
	bgt		.L_IN_D4				@			then loop
	b		.L_DONE_D4
	
.L_LEAD_D3:
	mov		r5, r9					@ r5 <- leading character
	mov		r6, r8
	sub		r6, #3					@ r6 <- # of chars to print
	ble		.L_DONE_D3
.L_IN_D3:
	strb	r5, [r7]				@ store a character
	add		r7, #1					@ advance string pointer
	sub		r6, #1					@ if chars to print > 0
	bgt		.L_IN_D3				@			then loop
	b		.L_DONE_D3

.L_LEAD_D2:
	mov		r5, r9					@ r5 <- leading character
	mov		r6, r8
	sub		r6, #2					@ r6 <- # of chars to print
	ble		.L_DONE_D2
.L_IN_D2:
	strb	r5, [r7]				@ store a character
	add		r7, #1					@ advance string pointer
	sub		r6, #1					@ if chars to print > 0
	bgt		.L_IN_D2				@			then loop
	b		.L_DONE_D2

.L_LEAD_D1:
	mov		r5, r9					@ r5 <- leading character
	mov		r6, r8
	sub		r6, #1					@ r6 <- # of chars to print
	ble		.L_DONE_D1
.L_IN_D1:
	strb	r5, [r7]				@ store a character
	add		r7, #1					@ advance string pointer
	sub		r6, #1					@ if chars to print > 0
	bgt		.L_IN_D1				@			then loop
	b		.L_DONE_D1
