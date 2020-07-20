//
// Object glyph plotter initializer
//
//! \file tte_init_obj.c
//! \author J Vijn
//! \date 20070715 - 20080229
//
/* === NOTES ===
  * 20070715: The object system works a little differently than the 
	others. Objects are used as a descending stack with dstPitch as the
	backwards obj index. Also, INK, SHADOW and SPECIAL are base 
	attributes of the objects. obj_erase unwinds the whole obj-stack.
*/

#include <string.h>

#include "tonc_memdef.h"
#include "tonc_core.h"
#include "tonc_bios.h"
#include "tonc_tte.h"


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

//! Initialize text system for screen-entry fonts.
/*!
	\param obj		Destination object.
	\param attr0	Base obj.attr0. 
	\param attr1	Base obj.attr1.
	\param attr2	Base obj.attr2.
	\param clrs		colors to use for the text. The palette entries
	  used depends on \a attr2 and \a bupofs.
	\param bupofs	Flags for font bit-unpacking. Basically indicates
	  pixel values (and hence palette use).
	\param font		Font to initialize with.
	\param proc		Character plotting procedure.
	\note	The TTE-obj system uses the surface differently than then 
		rest. Be careful when modifying the surface data.
	\todo	Multi-bpp.
*/
void tte_init_obj(OBJ_ATTR *obj, u32 attr0, u32 attr1, u32 attr2, 
	u32 clrs, u32 bupofs, const TFont *font, fnDrawg proc)
{
	if(font==NULL)	font= &fwf_default;
	if(proc==NULL)	proc= obj_drawg_default;

	tte_init_base(font, proc, obj_erase);

	TTC *tc= tte_get_context();
	TSurface *srf= &tc->dst;

	srf->data= (u8*)(obj ? obj : &oam_mem[127]);
	srf->pitch= 0;		// NOTE: works as a backwards index, not pitch.
	srf->width= 128;
	srf->height= 0;
	srf->bpp= 4;
	srf->type= SRF_NONE;
	srf->palSize= 16;
	//srf->palData= pal_obj_mem;

	// --- Prep color-lut and palette ---
	uint tid= BFN_GET(attr2, ATTR2_ID);
	uint pb = BFN_GET(attr2, ATTR2_PALBANK);
	srf->palData= pal_obj_bank[pb];

	srf->palData[(bupofs+1)&15]= clrs&0xFFFF;
	if( (bupofs&~BUP_ALL_OFS) > 15)
		srf->palData[(bupofs>>4)&15]= clrs>>16;

	tc->cattr[0]= attr0 &~ ATTR0_Y_MASK;
	tc->cattr[1]= attr1 &~ ATTR1_X_MASK;
	tc->cattr[2]= attr2;
	//tc->cattr[3]= 0;

	// --- Unpack font ---
	u32 dstS= font->charCount*font->cellSize;

	BUP bup= { dstS, font->bpp, 4, bupofs };
	BitUnPack(font->data, &tile_mem[4][tid], &bup);
}

// EOF
