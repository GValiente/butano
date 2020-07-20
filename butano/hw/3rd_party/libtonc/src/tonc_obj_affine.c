//
//  Object Affine functions
//
//! \file tonc_obj_affine.c
//! \author J Vijn
//! \date 20060908 - 20130331
//
// === NOTES ===

#include "tonc_memmap.h"
#include "tonc_core.h"

#include "tonc_bios.h"
#include "tonc_math.h"
#include "tonc_oam.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------

void obj_aff_copy(OBJ_AFFINE *dst, const OBJ_AFFINE *src, u32 count)
{
	int ii;
	for(ii=0; ii<count; ii++)
	{
		dst->pa= src->pa;
		dst->pb= src->pb;
		dst->pc= src->pc;
		dst->pd= src->pd;
		src++;
		dst++;
	}
}


//! Set obj matrix to counter-clockwise rotation.
/*! 
	\param oaff	Object affine struct to set.
	\param alpha	CCW angle. full-circle is 10000h.
*/
void obj_aff_rotate(OBJ_AFFINE *oaff, u16 alpha)
{
	int ss= lu_sin(alpha)>>4, cc= lu_cos(alpha)>>4;
	oaff->pa= cc;	oaff->pb= -ss;
	oaff->pc= ss;	oaff->pd= cc;
}

//! Set obj matrix to 2d scaling, then counter-clockwise rotation.
/*! 
	\param oaff	Object affine struct to set.
	\param sx	Horizontal scale (zoom). .8 fixed point.
	\param sy	Vertical scale (zoom). .8 fixed point.
	\param alpha	CCW angle. full-circle is 10000h.
*/
void obj_aff_rotscale(OBJ_AFFINE *oaff, FIXED sx, FIXED sy, u16 alpha)
{
	int ss= lu_sin(alpha), cc= lu_cos(alpha);

	oaff->pa= cc*sx>>12;	oaff->pb=-ss*sx>>12;
	oaff->pc= ss*sy>>12;	oaff->pd= cc*sy>>12;
}

//! Pre-multiply \a dst by \a src: D = S*D
void obj_aff_premul(OBJ_AFFINE *dst, const OBJ_AFFINE *src)
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
void obj_aff_postmul(OBJ_AFFINE *dst, const OBJ_AFFINE *src)
{
	FIXED tmp_a, tmp_b, tmp_c, tmp_d;
	tmp_a= dst->pa; tmp_b= dst->pb;
	tmp_c= dst->pc; tmp_d= dst->pd;

	dst->pa= (tmp_a*src->pa + tmp_b*src->pc)>>8;
	dst->pb= (tmp_a*src->pb + tmp_b*src->pd)>>8;
	dst->pc= (tmp_c*src->pa + tmp_d*src->pc)>>8;
	dst->pd= (tmp_c*src->pb + tmp_d*src->pd)>>8;
}

//! Set obj matrix to 2d scaling, then counter-clockwise rotation.
/*! 
	\param oaff	Object affine struct to set.
	\param as Struct with scales and angle.
*/
void obj_aff_rotscale2(OBJ_AFFINE *oaff, const AFF_SRC *as)
{
	int ss= lu_sin(as->alpha), cc= lu_cos(as->alpha);

	oaff->pa= cc*as->sx>>12;	oaff->pb=-ss*as->sx>>12;
	oaff->pc= ss*as->sy>>12;	oaff->pd= cc*as->sy>>12;	
}

//! Rot/scale an object around an arbitrary point.
/*!	Sets up \a obj \e and \a oaff for rot/scale transformation
	around an arbitrary point using the \a asx data.
	\param obj	Object to set.
	\param oaff	Object affine data to set.
	\param asx	Affine source data: screen and texture origins, 
	  scales and angle.
*/
void obj_rotscale_ex(OBJ_ATTR *obj, OBJ_AFFINE *oaff, const AFF_SRC_EX *asx)
{
	int sx= asx->sx, sy= asx->sy;
	int sina= lu_sin(asx->alpha)>>4, cosa= lu_cos(asx->alpha)>>4;

	oaff->pa= sx*cosa>>8;		oaff->pb= -sx*sina>>8;
	oaff->pc= sy*sina>>8;		oaff->pd=  sy*cosa>>8;

	// sx = 1/sx, sy = 1/sy (.12f)
	sx= Div(1<<20, sx);
	if(sx != sy)
		sy= Div(1<<20, sy);
	else
		sy= sx;
	FIXED aa, ab, ac, ad;
	aa=  sx*cosa>>12;	ab= sy*sina>>12;	// .8f
	ac= -sx*sina>>12;	ad= sy*cosa>>12;	// .8f

	sx= oam_sizes[obj->attr0>>14][obj->attr1>>14][0];
	sy= oam_sizes[obj->attr0>>14][obj->attr1>>14][1];

	int dx= asx->scr_x, dy= asx->scr_y;	// .0f
	if(obj->attr0&ATTR0_AFF_DBL_BIT)
	{	dx -= sx;		dy -=sy;		}
	else
	{	dx -= sx>>1;	dy -= sy>>1;	}

	sx= asx->tex_x - (sx<<7);		// .8f
	sy= asx->tex_y - (sy<<7);		// .8f
	dx -= (aa*sx + ab*sy)>>16;		// .0 + (.8f*.8f/.16f)
	dy -= (ac*sx + ad*sy)>>16;		// .0 + (.8f*.8f/.16f)

	obj_set_pos(obj, dx, dy);
}

// EOF
