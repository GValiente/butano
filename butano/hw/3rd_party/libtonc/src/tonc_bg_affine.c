//
//  BG Affine functions
//
//! \file tonc_bg_affine.c
//! \author J Vijn
//! \date 20060916 - 20060916
//
// === NOTES ===

#include "tonc_memmap.h"
#include "tonc_core.h"

#include "tonc_video.h"
#include "tonc_math.h"
#include "tonc_bios.h"


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


//! Set bg matrix to counter-clockwise rotation.
/*! 
	\param bgaff	Object affine struct to set.
	\param alpha	CCW angle. full-circle is 10000h.
*/
void bg_aff_rotate(BG_AFFINE *bgaff, u16 alpha)
{
	int ss= lu_sin(alpha)>>4, cc= lu_cos(alpha)>>4;

	bgaff->pa= cc;	bgaff->pb=-ss;
	bgaff->pc= ss;	bgaff->pd= cc;
}

//! Set bg matrix to 2d scaling, then counter-clockwise rotation.
/*! 
	\param bgaff	Object affine struct to set.
	\param sx	Horizontal scale (zoom). .8 fixed point.
	\param sy	Vertical scale (zoom). .8 fixed point.
	\param alpha	CCW angle. full-circle is 10000h.
*/
void bg_aff_rotscale(BG_AFFINE *bgaff, int sx, int sy, u16 alpha)
{
	int ss= lu_sin(alpha), cc= lu_cos(alpha);

	bgaff->pa= cc*sx>>12;	bgaff->pb=-ss*sx>>12;
	bgaff->pc= ss*sy>>12;	bgaff->pd= cc*sy>>12;	
}

//! Pre-multiply \a dst by \a src: D = S*D
void bg_aff_premul(BG_AFFINE *dst, const BG_AFFINE *src)
{
	FIXED tmp_a, tmp_b, tmp_c, tmp_d;
	tmp_a= dst->pa; tmp_b= dst->pb;
	tmp_c= dst->pc; tmp_d= dst->pd;

	dst->pa= (src->pa*tmp_a + src->pb*tmp_c)>>8;
	dst->pb= (src->pa*tmp_b + src->pb*tmp_d)>>8;
	dst->pc= (src->pc*tmp_a + src->pd*tmp_c)>>8;
	dst->pd= (src->pc*tmp_b + src->pd*tmp_d)>>8;
}

//! Post-multiply \a dst by \a src: D= D*S
void bg_aff_postmul(BG_AFFINE *dst, const BG_AFFINE *src)
{
	FIXED tmp_a, tmp_b, tmp_c, tmp_d;
	tmp_a= dst->pa; tmp_b= dst->pb;
	tmp_c= dst->pc; tmp_d= dst->pd;

	dst->pa= (tmp_a*src->pa + tmp_b*src->pc)>>8;
	dst->pb= (tmp_a*src->pb + tmp_b*src->pd)>>8;
	dst->pc= (tmp_c*src->pa + tmp_d*src->pc)>>8;
	dst->pd= (tmp_c*src->pb + tmp_d*src->pd)>>8;
}

//! Set bg matrix to 2d scaling, then counter-clockwise rotation.
/*! 
	\param bgaff	Object affine struct to set.
	\param as Struct with scales and angle.
*/
void bg_aff_rotscale2(BG_AFFINE *bgaff, const AFF_SRC *as)
{
	int ss= lu_sin(as->alpha), cc= lu_cos(as->alpha);

	bgaff->pa= cc*as->sx>>12;	bgaff->pb=-ss*as->sx>>12;
	bgaff->pc= ss*as->sy>>12;	bgaff->pd= cc*as->sy>>12;	
}

//! Set bg affine matrix to a rot/scale around an arbitrary point.
/*! Rotate and scale round an arbitrary point using the \a asx data.
	\param bgaff	BG affine data to set.
	\param asx	Affine source data: screen and texture origins, 
	  scales and angle.
*/
void bg_rotscale_ex(BG_AFFINE *bgaff, const AFF_SRC_EX *asx)
{
	int sx= asx->sx, sy= asx->sy;
	int sina= lu_sin(asx->alpha), cosa= lu_cos(asx->alpha);

	FIXED pa, pb, pc, pd;
	pa=  sx*cosa>>12;	pb=-sx*sina>>12;	// .8f
	pc=  sy*sina>>12;	pd= sy*cosa>>12;	// .8f
	
	bgaff->pa= pa;	bgaff->pb= pb;
	bgaff->pc= pc;	bgaff->pd= pd;

	bgaff->dx= asx->tex_x - (pa*asx->scr_x + pb*asx->scr_y);
	bgaff->dy= asx->tex_y - (pc*asx->scr_x + pd*asx->scr_y);
}

// EOF
