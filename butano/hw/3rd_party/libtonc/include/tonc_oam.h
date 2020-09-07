//
//  Basic video functions
//
//! \file tonc_oam.h
//! \author J Vijn
//! \date 20060604 - 20060604
//
// === NOTES ===
// * Basic video-IO, color, background and object functionality

#ifndef TONC_OAM
#define TONC_OAM

#include "tonc_memmap.h"
#include "tonc_memdef.h"
#include "tonc_core.h"


// --------------------------------------------------------------------
// OBJECTS
// --------------------------------------------------------------------


//! \addtogroup grpVideoObj
/*!	\{	*/

#define OAM_CLEAR()	memset32(oam_mem, 0, OAM_SIZE/4)

// --- Prototypes -----------------------------------------------------

// --- Full OAM ---
void oam_init(OBJ_ATTR *obj, uint count);
INLINE void oam_copy(OBJ_ATTR *dst, const OBJ_ATTR *src, uint count);

// --- Obj attr only ---
INLINE OBJ_ATTR *obj_set_attr(OBJ_ATTR *obj, u16 a0, u16 a1, u16 a2);
INLINE void obj_set_pos(OBJ_ATTR *obj, int x, int y);
INLINE void obj_hide(OBJ_ATTR *oatr);
INLINE void obj_unhide(OBJ_ATTR *obj, u16 mode);

INLINE const u8 *obj_get_size(const OBJ_ATTR *obj);
INLINE int obj_get_width(const OBJ_ATTR *obj);
INLINE int obj_get_height(const OBJ_ATTR *obj);

void obj_copy(OBJ_ATTR *dst, const OBJ_ATTR *src, uint count);
void obj_hide_multi(OBJ_ATTR *obj, u32 count);
void obj_unhide_multi(OBJ_ATTR *obj, u16 mode, uint count);

// --- Obj affine only ---
void obj_aff_copy(OBJ_AFFINE *dst, const OBJ_AFFINE *src, uint count);

INLINE void obj_aff_set(OBJ_AFFINE *oaff, 
	FIXED pa, FIXED pb, FIXED pc, FIXED pd);
INLINE void obj_aff_identity(OBJ_AFFINE *oaff);
INLINE void obj_aff_scale(OBJ_AFFINE *oaff, FIXED sx, FIXED sy);
INLINE void obj_aff_shearx(OBJ_AFFINE *oaff, FIXED hx);
INLINE void obj_aff_sheary(OBJ_AFFINE *oaff, FIXED hy);

void obj_aff_rotate(OBJ_AFFINE *oaff, u16 alpha);
void obj_aff_rotscale(OBJ_AFFINE *oaff, FIXED sx, FIXED sy, u16 alpha);
void obj_aff_premul(OBJ_AFFINE *dst, const OBJ_AFFINE *src);
void obj_aff_postmul(OBJ_AFFINE *dst, const OBJ_AFFINE *src);

void obj_aff_rotscale2(OBJ_AFFINE *oaff, const AFF_SRC *as);
void obj_rotscale_ex(OBJ_ATTR *obj, OBJ_AFFINE *oaff, const AFF_SRC_EX *asx);


// inverse (object -> screen) functions, could be useful
// inverses (prototypes)
INLINE void obj_aff_scale_inv(OBJ_AFFINE *oa, FIXED wx, FIXED wy);
INLINE void obj_aff_rotate_inv(OBJ_AFFINE *oa, u16 theta);
INLINE void obj_aff_shearx_inv(OBJ_AFFINE *oa, FIXED hx);
INLINE void obj_aff_sheary_inv(OBJ_AFFINE *oa, FIXED hy);

/*!	\}	*/


// --------------------------------------------------------------------
// INLINES
// --------------------------------------------------------------------


/*!	\addtogroup grpVideoObj	*/
/*! \{	*/

//! Set the attributes of an object.
INLINE OBJ_ATTR *obj_set_attr(OBJ_ATTR *obj, u16 a0, u16 a1, u16 a2)
{
	obj->attr0= a0; obj->attr1= a1; obj->attr2= a2;
	return obj;
}

//! Set the position of \a obj
INLINE void obj_set_pos(OBJ_ATTR *obj, int x, int y)
{
	BFN_SET(obj->attr0, y, ATTR0_Y);
	BFN_SET(obj->attr1, x, ATTR1_X);
}

//! Copies \a count OAM entries from \a src to \a dst.
INLINE void oam_copy(OBJ_ATTR *dst, const OBJ_ATTR *src, uint count)
{	memcpy32(dst, src, count*2);	}

//! Hide an object.
INLINE void obj_hide(OBJ_ATTR *obj)
{	BFN_SET2(obj->attr0, ATTR0_HIDE, ATTR0_MODE);		}

//! Unhide an object.
/*! \param obj	Object to unhide.
*	\param mode	Object mode to unhide to. Necessary because this affects
*	  the affine-ness of the object.
*/
INLINE void obj_unhide(OBJ_ATTR *obj, u16 mode)
{	BFN_SET2(obj->attr0, mode, ATTR0_MODE);				}


//! Get object's sizes as a byte array
INLINE const u8 *obj_get_size(const OBJ_ATTR *obj)
{	return oam_sizes[obj->attr0>>14][obj->attr1>>14];	}

//! Get object's width
INLINE int obj_get_width(const OBJ_ATTR *obj)
{	return obj_get_size(obj)[0];						}
	
//! Gets object's height
INLINE int obj_get_height(const OBJ_ATTR *obj)
{	return obj_get_size(obj)[1];						}


// --- Affine only ---


//! Set the elements of an \a object affine matrix.
INLINE void obj_aff_set(OBJ_AFFINE *oaff, 
	FIXED pa, FIXED pb, FIXED pc, FIXED pd)
{
	oaff->pa= pa;	oaff->pb= pb;
	oaff->pc= pc;	oaff->pd= pd;
}

//! Set an object affine matrix to the identity matrix
INLINE void obj_aff_identity(OBJ_AFFINE *oaff)
{
	oaff->pa= 0x0100l;	oaff->pb= 0;
	oaff->pc= 0;		oaff->pd= 0x0100;
}

//! Set an object affine matrix for scaling.
INLINE void obj_aff_scale(OBJ_AFFINE *oaff, FIXED sx, FIXED sy)
{
	oaff->pa= sx;	oaff->pb= 0;
        oaff->pc= 0;	oaff->pd= sy;
}

INLINE void obj_aff_shearx(OBJ_AFFINE *oaff, FIXED hx)
{
	oaff->pa= 0x0100;	oaff->pb= hx;
	oaff->pc= 0;		oaff->pd= 0x0100;
}

INLINE void obj_aff_sheary(OBJ_AFFINE *oaff, FIXED hy)
{
	oaff->pa= 0x0100;	oaff->pb= 0;
	oaff->pc= hy;		oaff->pd= 0x0100;
}


// --- Inverse operations ---

INLINE void obj_aff_scale_inv(OBJ_AFFINE *oaff, FIXED wx, FIXED wy)
{	obj_aff_scale(oaff, ((1<<24)/wx)>>8, ((1<<24)/wy)>>8);	}

INLINE void obj_aff_rotate_inv(OBJ_AFFINE *oaff, u16 theta)
{	obj_aff_rotate(oaff, -theta);		}

INLINE void obj_aff_shearx_inv(OBJ_AFFINE *oaff, FIXED hx)
{	obj_aff_shearx(oaff, -hx);								}

INLINE void obj_aff_sheary_inv(OBJ_AFFINE *oaff, FIXED hy)
{	obj_aff_sheary(oaff, -hy);								}


/*! \}	*/

#endif // TONC_OAM

