//
// Basic surface functionality..
//
//! \file tonc_surface.c
//! \author J Vijn
//! \date 20080409 - 20080409
//
/* === NOTES ===
*/

#include <string.h>
#include "tonc_surface.h"
#include "tonc_video.h"

// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------

//! Initalize a surface for \a type formatted graphics.
/*!
	\param srf		Surface to initialize.
	\param type		Surface type. See \sa ESurfaceType for details.
	\param data		Pointer to the surface memory.
	\param width	Width of surface.
	\param height	Height of surface.
    \param bpp		Bitdepth. If \a type is not 0, this value will be ignored.
	\param pal		Pointer to the surface's palette.
*/
void srf_init(TSurface *srf, enum ESurfaceType type, const void *data, 
	uint width, uint height, uint bpp, u16 *pal)
{
	memset(srf, 0, sizeof(TSurface));
	srf->data= (u8*)data;
	srf->type= type;

	switch(type &~ SRF_ALLOCATED)
	{
	case SRF_CHR4R:
		bpp= 4;
		srf->pitch= srf_align(width, 4)*8;
		break;

	case SRF_CHR4C:
		bpp= 4;
        srf->pitch= srf_align(height, 4)*8;
		break;

	case SRF_CHR8:
		bpp= 8;
		srf->pitch= srf_align(width, 8)*8;
		break;

	case SRF_BMP8:
		bpp= 8;
		srf->pitch= srf_align(width, bpp);
		break;

	case SRF_BMP16:
		bpp= 16;
		srf->pitch= srf_align(width, bpp);
		pal= NULL;
		break;
	default:
		srf->pitch= srf_align(width, bpp);
	}

	srf->width= width;
	srf->height= height;
	srf->bpp= bpp;

	if(pal != NULL)
	{
		srf->palSize= 1<<bpp;
		srf->palData= pal;
	}
}

//! Get the byte address of coordinates (\a x, \a y) on the surface.
void *srf_get_ptr(const TSurface *srf, uint x, uint y)
{
	uint bpp= srf->bpp, pitch= srf->pitch;
	switch(srf->type &~ SRF_ALLOCATED)
	{
	case SRF_CHR4R:
	case SRF_CHR8:
		return &srf->data[y/8*pitch + y%8*bpp + (x/8*8)*bpp + (x%8)*bpp/8];

	case SRF_CHR4C:
		return &srf->data[y*4 + x/8*pitch + (x%8)/2];
		
	default:
		return &srf->data[y*pitch + x*bpp/8];
	}
}



// EOF
