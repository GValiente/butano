//
// Object plotter
//
//! \file obj_drawg.c
//! \author J Vijn
//! \date 20070715 - 20070822
//
/* === NOTES ===
  * 20070715: The object system works a little differently than the 
	others. Objects are used as a descending stack with dstPitch as the
	backwards obj index. Also, INK, SHADOW and SPECIAL are base 
	attributes of the objects. obj_erase unwinds the whole obj-stack.
*/

#include "tonc_types.h"
#include "tonc_memdef.h"
#include "tonc_core.h"
#include "tonc_oam.h"

#include "tonc_tte.h"

#include "tonc_warn.h"


//! Unwind the object text-buffer
void obj_erase(int left, int top, int right, int bottom)
{	
	TTC *tc= tte_get_context();
	int ii=0, nn= tc->dst.pitch;
	OBJ_ATTR *obj= (OBJ_ATTR*)tc->dst.data;

	for(ii=0; ii<nn; ii++)
		obj_hide(obj--);

	tc->dst.pitch= 0;
}


// --------------------------------------------------------------------
// drawg routines
// --------------------------------------------------------------------


//! Character-plot for objects. 
void obj_drawg(uint gid)
{
	// PONDER: no obj for ' ' ?

	TTC *tc= tte_get_context();
	TFont *font= tc->font;
	uint x0= tc->cursorX, y0= tc->cursorY;

	uint id= tc->dst.pitch;
	OBJ_ATTR *obj= &((OBJ_ATTR*)tc->dst.data)[-id];
	
	tc->dst.pitch= (id+1 < tc->dst.width ? id+1 : 0);

	obj->attr0= tc->cattr[0] | BFN_PREP(y0, ATTR0_Y);
	obj->attr1= tc->cattr[1] | BFN_PREP(x0, ATTR1_X);
	obj->attr2= tc->cattr[2] + gid*font->cellW*font->cellH/64;
}

// EOF
