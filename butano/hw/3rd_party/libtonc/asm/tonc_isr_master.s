//
//  Default ISR for interrupts. No automatic nestings.
//
//! \file tonc_isr_master.s
//! \author J Vijn
//! \date 20080320 - 20090801
//
// === NOTES ===

#include "../include/tonc_asminc.h"

	.file	"tonc_isr_master.s"
	.extern	__isr_table;

/*! \fn IWRAM_CODE void isr_master()
	\brief	Default irq dispatcher (no automatic nesting)
*/
	@ Register list
	@ r0 : &REG_IE
	@ r1 : __isr_table / isr
	@ r2 : IF & IE
	@ r3 : tmp
	@ ip : (IF,IE)	
BEGIN_FUNC_ARM(isr_master, CSEC_IWRAM)
	@ Read IF/IE 
	mov		r0, #0x04000000
	ldr		ip, [r0, #0x200]!
	and		r2, ip, ip, lsr #16		@ irq= IE & IF

	@ Acknowledge irq in IF and for BIOS
	strh	r2, [r0, #2]
	ldr		r3, [r0, #-0x208]
	orr		r3, r3, r2
	str		r3, [r0, #-0x208]

	@ Search for irq.
	ldr		r1, =__isr_table

.Lirq_search:
		ldr		r3, [r1], #8 
		tst		r3, r2
		bne		.Lpost_search		@ Found one, break off search
		cmp		r3, #0
		bne		.Lirq_search		@ Not here; try next irq

	@ Search over : return if no isr, otherwise continue.
.Lpost_search:
	ldrne	r1, [r1, #-4]		@ isr= __isr_table[ii-1].isr
	cmpne	r1, #0
	bxeq	lr					@ If no isr: quit

	@ --- If we're here, we have an isr ---

	ldr		r3, [r0, #8]			@ Read IME 
	strb	r0, [r0, #8]			@ Clear IME
	bic		r2, ip, r2
	strh	r2, [r0]				@ Clear current irq in IE
	
	mrs		r2, spsr
	stmfd	sp!, {r2-r3, ip, lr}	@ sprs, IME, (IE,IF), lr_irq

	@ Set mode to usr
	mrs		r3, cpsr
	bic		r3, r3, #0xDF
	orr		r3, r3, #0x1F
	msr		cpsr, r3

	@ Call isr
	stmfd	sp!, {r0,lr}		@ &REG_IE, lr_sys
	mov		lr, pc
	bx		r1
	ldmfd	sp!, {r0,lr}		@ &REG_IE, lr_sys

	@ --- Unwind ---
	strb	r0, [r0, #8]		@ Clear IME again (safety)

	@ Reset mode to irq
	mrs		r3, cpsr
	bic		r3, r3, #0xDF
	orr		r3, r3, #0x92
	msr		cpsr, r3

	ldmfd	sp!, {r2-r3, ip, lr}	@ sprs, IME, (IE,IF), lr_irq
	msr		spsr, r2			@ Restore spsr
	strh	ip, [r0]			@ Restore IE
	str		r3, [r0, #8]		@ Restore IME
	
	bx		lr
END_FUNC(isr_master)

@ EOF

