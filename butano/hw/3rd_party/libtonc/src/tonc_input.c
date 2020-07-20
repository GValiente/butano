//
//  Input implementation
//
//! \file tonc_input.c
//! \author J Vijn
//! \date 20070406 - 20070406
//
// === NOTES ===


#include "tonc_input.h"
#include "tonc_bios.h"

#include "tonc_warn.h"

// --------------------------------------------------------------------
// CLASSES 
// --------------------------------------------------------------------

//! Repeated keys struct
typedef struct REPEAT_REC
{
	u16	keys;	//!< Repeated keys.
	u16	mask;	//!< Only check repeats for these keys.
	u8	count;	//!< Repeat counter.
	u8	delay;	//!< Limit for first repeat.
	u8	repeat;	//!< Limit for successive repeats.
} ALIGN4 REPEAT_REC;


// --------------------------------------------------------------------
// GLOBALS 
// --------------------------------------------------------------------


u16 __key_curr= 0, __key_prev= 0;

REPEAT_REC __key_rpt= { 0, KEY_MASK, 60, 60, 30 };


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


//! Poll for keystates and repeated keys
void key_poll(void)
{
	__key_prev= __key_curr;
	__key_curr= ~REG_KEYINPUT & KEY_MASK;

	REPEAT_REC *rpt= &__key_rpt;

	rpt->keys= 0;	// Clear repeats again

	if(rpt->delay)
	{
		// Change in masked keys: reset repeat
		// NOTE: this also counts as a repeat!
		if(key_transit(rpt->mask))
		{
			rpt->count= rpt->delay;
			rpt->keys= __key_curr;
		}
		else
			rpt->count--;

		// Time's up: set repeats (for this frame)
		if(rpt->count == 0)
		{
			rpt->count= rpt->repeat;
			rpt->keys= __key_curr & rpt->mask;
		}
	}
}

//! Wait until \a key is hit.
void key_wait_till_hit(u16 key)
{
	while(1)
	{
		VBlankIntrWait();
		key_poll();
		if(key_hit(key))
			return;
	}	
}

// === Repeated keys functions ===

//! Get status of repeated keys.
u32 key_repeat(u32 keys)
{
	return __key_rpt.keys & keys;
}

//! Set repeat mask. Only these keys will be considered for repeats.
void key_repeat_mask(u32 mask)
{
	__key_rpt.mask= mask & KEY_MASK;
}

//! Set the delay and repeat limits for repeated keys
/*!
	\param delay	Set first repeat limit. If 0, repeats are off.
	\param repeat	Sets later repeat limit.
	\note	Both limits have a range of [0, 255]. If either argument 
	  is <0, the old value will be kept.
*/
void key_repeat_limits(uint delay, uint repeat)
{
	REPEAT_REC *rpt= &__key_rpt;

	if(delay >= 0)
		rpt->delay= delay;
	if(repeat >= 0)
		rpt->repeat= repeat;

	rpt->count= delay;		// Reset counter.
}
