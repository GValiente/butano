//
//  OAM functions
//
//! \file tonc_oam.c
//! \author J Vijn
//! \date 20060604 - 20060604
//
// === NOTES ===

#include "tonc_memmap.h"
#include "tonc_memdef.h"
#include "tonc_oam.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


// 01 23 45 67 89 ab cd ef
// -- -1 -- -0 -- -0 -- -1

//! Initialize an array of \a count OBJ_ATTRs with with safe values.
void oam_init(OBJ_ATTR *obj, u32 count)
{
	u32 nn= (count+3)>>2;
	u32 *dst= (u32*)obj;

	// Hide all and set OBJ_AFFINEs to I
	while(nn--)
	{
		*dst++= ATTR0_HIDE;
		*dst++= 0x0100<<16;
		*dst++= ATTR0_HIDE;
		*dst++= 0;
		*dst++= ATTR0_HIDE;
		*dst++= 0;
		*dst++= ATTR0_HIDE;
		*dst++= 0x0100<<16;
	}
	// init oam
	oam_copy(oam_mem, obj, count);
}

//! Copy attributes 0-2 in  \a count OBJ_ATTRs.
void obj_copy(OBJ_ATTR *dst, const OBJ_ATTR *src, u32 count)
{
	int ii;
	for(ii=0; ii<count; ii++)
	{
		dst[ii].attr0= src[ii].attr0;
		dst[ii].attr1= src[ii].attr1;
		dst[ii].attr2= src[ii].attr2;
	}
}

//! Hide an array of OBJ_ATTRs
void obj_hide_multi(OBJ_ATTR *obj, u32 count)
{
	while(count--)
	{
		obj_hide(obj);
		obj++;
	}
}


void obj_unhide_multi(OBJ_ATTR *obj, u16 mode, u32 count)
{
	while(count--)
	{
		obj_unhide(obj, mode);
		obj++;
	}
}

