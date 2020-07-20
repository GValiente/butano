//
//  Interrupt source code
//
//! \file tonc_irq.c
//! \author J Vijn
//! \date 20060908 - 20060928
//
// === NOTES ===
// * General note: every irq function should start with 
//   saving/clearing REG_IME and end with restoring it. If it doesn't 
//   it's a fair bet that I screwed up :(

#include "tonc_memmap.h"
#include "tonc_memdef.h"
#include "tonc_core.h"
#include "tonc_irq.h"


// --------------------------------------------------------------------
// CLASSES 
// --------------------------------------------------------------------


//! IRQ Sender information
typedef struct IRQ_SENDER
{
	u16 reg_ofs;	//!< sender reg - REG_BASE
	u16 flag;		//!< irq-bit in sender reg
} ALIGN4 IRQ_SENDER;


// --------------------------------------------------------------------
// GLOBALS 
// --------------------------------------------------------------------


// One extra entry for guaranteed zero
IRQ_REC __isr_table[II_MAX+1];

// yeah, yeah, I really should use the registers and defines
// I have else where.
// NOTE: haven't really tested this very much; if inaccurate, 
// plz tell me
static const IRQ_SENDER __irq_senders[] = 
{
	{ 0x0004, 0x0008 },		// REG_DISPSTAT,	DSTAT_VBL_IRQ
	{ 0x0004, 0x0010 },		// REG_DISPSTAT,	DSTAT_VHB_IRQ
	{ 0x0004, 0x0020 },		// REG_DISPSTAT,	DSTAT_VCT_IRQ
	{ 0x0102, 0x0040 },		// REG_TM0CNT,		TM_IRQ
	{ 0x0106, 0x0040 },		// REG_TM1CNT,		TM_IRQ
	{ 0x010A, 0x0040 },		// REG_TM2CNT,		TM_IRQ
	{ 0x010E, 0x0040 },		// REG_TM3CNT,		TM_IRQ
	{ 0x0128, 0x4000 },		// REG_SIOCNT		SIO_IRQ
	{ 0x00BA, 0x4000 },		// REG_DMA0CNT_H,	DMA_IRQ>>16
	{ 0x00C6, 0x4000 },		// REG_DMA1CNT_H,	DMA_IRQ>>16
	{ 0x00D2, 0x4000 },		// REG_DMA2CNT_H,	DMA_IRQ>>16
	{ 0x00DE, 0x4000 },		// REG_DMA3CNT_H,	DMA_IRQ>>16
	{ 0x0132, 0x4000 },		// REG_KEYCNT,		KCNT_IRQ
	{ 0x0000, 0x0000 },		// cart: none
};


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


//! Initialize irq business
/*! Clears ISR table and sets up a master isr.
	\param isr	Master ISR. If NULL, \a isr_master_nest is used
*/
void irq_init(fnptr isr)
{
	REG_IME= 0;	

	// clear interrupt table (just in case)
	memset32(__isr_table, 0, (II_MAX+1)*sizeof(IRQ_REC)/4);

	REG_ISR_MAIN= (isr) ? isr : (fnptr)isr_master;

	REG_IME= 1;
}

//! Set a master ISR
/*!	\param isr	Master ISR. If NULL, \a isr_master_multi is used
*	\return	Previous master ISR
*/
fnptr irq_set_master(fnptr isr)
{
	u16 ime= REG_IME;
	REG_IME= 0;

	fnptr old_isr= REG_ISR_MAIN;
	REG_ISR_MAIN= (isr) ? isr : (fnptr)isr_master;

	REG_IME= ime;

	return old_isr;
}

// NOTE could be optimised a little more.
// and it would actually be a lot cleaner in asm :(

//! General IRQ manager
/*!	This routine manages the ISRs of interrupts and their priorities.
	\param irq_id	Index of irq.
	\param isr	Interrupt service routine for this irq; can be NULL
	\param opts	ISR options
	\return	Previous specific ISR
	\note	\a irq_id is \e NOT a bit-mask, it is an index!
*/
fnptr irq_set(enum eIrqIndex irq_id, fnptr isr, u32 opts)
{
	u16 ime= REG_IME;
	REG_IME= 0;

	int ii, slot, prio;
	u16 irq_flag= BIT(irq_id);
	fnptr old_isr;
	IRQ_REC	*pir= __isr_table;

	// Enable irq
	const IRQ_SENDER *sender= &__irq_senders[irq_id];
	*(u16*)(REG_BASE+sender->reg_ofs) |= sender->flag;
	REG_IE |= irq_flag;

	// Search for previous occurance
	for(slot=0; pir[slot].flag; slot++)
		if(pir[slot].flag == irq_flag)
			break;
	
	prio= opts&(ISR_LAST|ISR_PRIO_MASK);

	// IRS already exists; replace?
	if(pir[slot].flag == irq_flag)
	{
		// Replace desired -> do so
		if((prio & ISR_REPLACE) || slot == prio)
		{
			old_isr= pir[slot].isr;
			pir[slot].isr= isr;
			REG_IME= ime;
			return old_isr;
		}
		else	// No replace: Move rest of isrs up
		{
			// NOTE: optimisable
			while(1)
			{
				pir[slot]= pir[slot+1];
				if(!pir[slot].flag)
					break;
				slot++;
			}
		}
	}

	// Priority shuffle; make room
	if(prio<slot)
	{
		for(ii=slot; ii>prio; ii--)
			pir[ii]= pir[ii-1];
		slot= ii;
	}

	old_isr= pir[slot].isr;
	pir[slot].isr= isr;
	pir[slot].flag= irq_flag;

	REG_IME= ime;
	return old_isr;	
}



//! Add a specific ISR
/*! Special case of \c irq_set. If the interrupt has an ISR already
	it'll be replaced; if not it will add it in the back.
	\param irq_id	Index of irq.
	\param isr	Interrupt service routine for this irq; can be NULL
	\return	Previous ISR
	\note	\a irq_id is \e NOT a bit-mask, it is an index!
*/
fnptr irq_add(enum eIrqIndex irq_id, fnptr isr)
{
	u16 ime= REG_IME;
	REG_IME= 0;

	int ii;
	u16 irq_flag= BIT(irq_id);
	fnptr old_isr;
	IRQ_REC	*pir= __isr_table;

	// Enable irq
	const IRQ_SENDER *sender= &__irq_senders[irq_id];
	*(u16*)(REG_BASE+sender->reg_ofs) |= sender->flag;
	REG_IE |= irq_flag;

	// Search for previous occurance, or empty slot
	for(ii=0; pir[ii].flag; ii++)
		if(pir[ii].flag == irq_flag)
			break;
	
	old_isr= pir[ii].isr;
	pir[ii].isr= isr;
	pir[ii].flag= irq_flag;

	REG_IME= ime;
	return old_isr;
}

//! Remove an ISR
/*! it'll be replaced; if not it will add it in the back.
	\param irq_id	Index of irq.
	\return	Previous ISR
	\note	\a irq_id is \e NOT a bit-mask, it is an index!
*/
fnptr irq_delete(enum eIrqIndex irq_id)
{
	u16 ime= REG_IME;
	REG_IME= 0;

	int ii;
	u16 irq_flag= BIT(irq_id);
	fnptr old_isr;
	IRQ_REC	*pir= __isr_table;

	// Disable irq
	const IRQ_SENDER *sender= &__irq_senders[irq_id];
	*(u16*)(REG_BASE+sender->reg_ofs) &= ~sender->flag;
	REG_IE &= ~irq_flag;

	// Find the ISR
	for(ii=0; pir[ii].flag; ii++)
		if(pir[ii].flag == irq_flag)
			break;

	old_isr= pir[ii].isr;
	// Found it; move rest up
	for( ; pir[ii].flag; ii++)
		pir[ii]= pir[ii+1];

	REG_IME= ime;
	return old_isr;	
}

void irq_enable(enum eIrqIndex irq_id)
{
	u16 ime= REG_IME;
	REG_IME= 0;

	const IRQ_SENDER *sender= &__irq_senders[irq_id];
	*(u16*)(REG_BASE+sender->reg_ofs) |= sender->flag;

	REG_IE |= BIT(irq_id);
	REG_IME= ime;
}

void irq_disable(enum eIrqIndex irq_id)
{
	u16 ime= REG_IME;
	REG_IME= 0;

	const IRQ_SENDER *sender= &__irq_senders[irq_id];
	*(u16*)(REG_BASE+sender->reg_ofs) &= ~sender->flag;

	REG_IE &= ~BIT(irq_id);
	REG_IME= ime;
}

// EOF
