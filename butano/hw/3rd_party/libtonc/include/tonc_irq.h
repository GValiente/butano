//
//  Interrupt header
//
//! \file tonc_irq.h
//! \author J Vijn
//! \date 20060508 - 20080326
//
// === NOTES ===


#ifndef TONC_IRQ
#define TONC_IRQ

#include "tonc_memmap.h"
#include "tonc_memdef.h"


// --------------------------------------------------------------------
// CONSTANTS
// --------------------------------------------------------------------


/*! \addtogroup grpIrq
	\brief	Hardware interrupt management.


	For details, see 
	<a href="http://www.coranac.com/tonc/text/interrupts.htm">tonc:irq</a> 
	*/
/*! \{	*/

//! IRQ indices, to be used in most functions.
typedef enum eIrqIndex
{
	II_VBLANK=0,II_HBLANK,	II_VCOUNT,	II_TIMER0,
	II_TIMER1,	II_TIMER2,	II_TIMER3,	II_SERIAL,
	II_DMA0,	II_DMA1,	II_DMA2,	II_DMA3,
	II_KEYPAD,	II_GAMEPAK,	II_MAX
} eIrqIndex;


//! \name Options for irq_set
//\{

#define ISR_LAST		0x0040	//!< Last isr in line (Lowest priority)
#define ISR_REPLACE		0x0080	//!< Replace old isr if existing (prio ignored)

#define ISR_PRIO_MASK	0x003F	//!< 
#define ISR_PRIO_SHIFT		 0
#define ISR_PRIO(n)		((n)<<ISR_PRIO_SHIFT)

#define ISR_DEF			(ISR_LAST|ISR_REPLACE)

//\}


// --------------------------------------------------------------------
// MACROS 
// --------------------------------------------------------------------


//! Default irq_init() call: use irq_master_nest() for switchboard.
#define IRQ_INIT()		irq_init(NULL)

//! Default irq_set() call: no isr, add to back of priority stack
#define IRQ_SET(irq_id)	irq_set(II_##irq_id, NULL, ISR_DEF)

// Default irq_add() call: no isr
#define IRQ_ADD(irq_id)	irq_add(II_##irq_id, NULL)


// --------------------------------------------------------------------
// CLASSES 
// --------------------------------------------------------------------


//! Struct for prioritized irq table
typedef struct IRQ_REC	
{
	u32 flag;	//!< Flag for interrupt in REG_IF, etc
	fnptr isr;	//!< Pointer to interrupt routine
} IRQ_REC;


// --------------------------------------------------------------------
// GLOBALS 
// --------------------------------------------------------------------


extern IRQ_REC __isr_table[II_MAX+1];


// --------------------------------------------------------------------
// PROTOTYPES 
// --------------------------------------------------------------------


IWRAM_CODE void isr_master(void);
IWRAM_CODE void isr_master_nest(void);

void irq_init(fnptr isr);
fnptr irq_set_master(fnptr isr);

fnptr irq_add(enum eIrqIndex irq_id, fnptr isr);
fnptr irq_delete(enum eIrqIndex irq_id);

fnptr irq_set(enum eIrqIndex irq_id, fnptr isr, u32 opts);

void irq_enable(enum eIrqIndex irq_id);
void irq_disable(enum eIrqIndex irq_id);


// --------------------------------------------------------------------
// INLINES 
// --------------------------------------------------------------------


/*! \}	*/

#endif // TONC_IRQ

