//
//  Main ISR for prioritized nested interrupts
//
//! \file tonc_isr_nest.s
//! \author J Vijn
//! \date 20060909 - 20061025
//
// === NOTES ===

#include "../include/tonc_asminc.h"

	.file "tonc_isr_nest.s"
	.extern	__isr_table;

/*! \fn IWRAM_CODE void isr_master_nest()
	\brief	Main ISR for using prioritized nested interrupts
*/
BEGIN_FUNC_ARM(isr_master_nest, CSEC_IWRAM)
	mov		r3, #0x04000000
	ldr		r2, [r3, #0x200]!
	and		r2, r2, r2, lsr #16		@ irq_curr= IE & IF
	
	@ REG_IFBIOS |= irq_curr
	ldr		r1, [r3, #-0x208]
	orr		r1, r1, r2
	str		r1, [r3, #-0x208]

	@ --- Find raised irq in __isr_table ---
	@ r0 := IRQ_REC *pir= __isr_table
	@ r12:= irq_prio (higher priority irqs)
	ldr		r0, =__isr_table
	mov		r12, #0
.Lirq_search:
		ldr		r1, [r0], #8 
		tst		r1, r2
		bne		.Lirq_found		@ Found it, break off search
		orr		r12, r12, r1
		cmp		r1, #0
		bne		.Lirq_search	@ Not here; try next irq_rec
	
	@ --- No irq or ISR: just ack and return ---
.Lirq_none:
	strh	r2, [r3, #2]	@ REG_IF= irq_curr (is this right?)
	bx		lr

	@ If we're here, we found the irq; check for ISR
.Lirq_found:
	ldr		r0, [r0, #-4]	@ isr= pir[ii-1].isr
	cmp		r0, #0
	streqh	r1, [r3, #2]	@ No ISR: ack and return
	bxeq	lr

	@ --- ISR found, prep for nested irqs ---
	@ {r0,r1,r3,r12} == { isr(), irq_flag, &REG_IE, irq_prio }

	@ r2 := ieif= REG_IE|(irq_flag<<16); REG_IE &= irq_prio;
	ldrh	r2, [r3]
	and		r12, r12, r2
	strh	r12, [r3]
	orr		r2, r2, r1, lsl #16

	mrs		r12, spsr
	stmfd	sp!, {r2, r12, lr}	@ sp_irq,{ieif, spsr, lr_irq}

	str		r3, [r3, #8]	@ REG_IME=0 (yeah ugly, I know)
	@ Set CPU to SYS-mode, re-enable IRQ
	mrs		r2, cpsr
	bic		r2, r2, #0xDF
	orr		r2, r2, #0x1F
	msr		cpsr, r2

	stmfd	sp!, {r3, lr}	@ sp_sys, {&REG_IE, lr_sys}
	mov		r2, #1
	str		r2, [r3, #8]	@ REG_IME= 1
	adr		lr, .Lpost_isr
	bx		r0

	@ --- Unroll preparation ---
.Lpost_isr:
	ldmfd	sp!, {r3, lr}	@ sp_sys, {&REG_IE, lr_sys}
	ldr		r0, [r3, #8]
	str		r3, [r3, #8]	@ REG_IME=0 again

	@ Restore CPU to IRQ-mode, disable IRQ
	mrs		r2, cpsr
	bic		r2, r2, #0xDF
	orr		r2, r2, #0x92
	msr		cpsr, r2
	
	ldmfd	sp!, {r2, r12, lr}	@ sp_irq,{ieif, spsr, lr_irq}
	msr		spsr, r12

	str		r2, [r3]		@ REG_IE/REG_IF= ieif
	str		r0, [r3, #8]	@ Restore REG_IME
	bx		lr
END_FUNC(isr_master_nest)

@ EOF
