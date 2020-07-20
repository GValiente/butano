//
//  Basic math functions
//
//! \file tonc_math.c
//! \author J Vijn
//! \date 20060508 - 20060508
//
// === NOTES ===

#include "tonc_memmap.h"
#include "tonc_core.h"
#include "tonc_math.h"


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


// --- Geometry -------------------------------------------------------

// --- Point functions ---

//! Is a point inside a rectangle
int pt_in_rect(const POINT *pt, const RECT *rc)
{
	if(pt->x < rc->left || pt->x >= rc->right)
		return 0;
	if(pt->y < rc->top || pt->y >= rc->bottom)
		return 0;
	return 1;
}

// --- Vector functions ---

VECTOR *vec_cross(VECTOR *vd, const VECTOR *va, const VECTOR *vb)
{
	vd->x= fxmul(va->y, vb->z) - fxmul(va->z,vb->y);
	vd->y= fxmul(va->y, vb->z) - fxmul(va->z,vb->y);
	vd->z= fxmul(va->y, vb->z) - fxmul(va->z,vb->y);
	return vd;
}

// --- Rect functions ---

RECT *rc_normalize(RECT *rc)
{
	int tmp;
	if(rc->left > rc->right) 
		SWAP3(rc->left, rc->right, tmp);
	if(rc->top > rc->bottom) 
		SWAP3(rc->top, rc->bottom, tmp);
	return rc;
}
