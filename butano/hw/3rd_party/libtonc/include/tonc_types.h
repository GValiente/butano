//
//  Basic structs and typedefs
//
//! \file tonc_types.h
//! \author J Vijn
//! \date 20060508 - 20080111
//
// === NOTES ===
// * When doing anything,  always, ALWAYS!!! check the type. 
//   Especially when you're combining things from different sources.
//   Look around on the forum and count the number of times people
//   have copied, say, from a u32 source to a u16 destination.


#ifndef TONC_TYPES
#define TONC_TYPES


/*!	\defgroup grpTypes	Types and attributes	*/


// --------------------------------------------------------------------
// GCC ATTRIBUTES
// --------------------------------------------------------------------


/*!	\defgroup grpTypeAttr	Type attributes
*	\ingroup grpTypes
*/
/*!	\{	*/

// If you want your data in specific sections, add this 
// to your variables or functions.
// Example:
//
// //Declaration
// IWRAM_CODE void function(int x, int y, etc);
//
// //Definition
// IWRAM_CODE void function(int x, int y, etc)
// {
//     // code 
// }


//! Put variable in IWRAM (default).
#define IWRAM_DATA __attribute__((section(".iwram")))

//! Put variable in EWRAM.
#define EWRAM_DATA __attribute__((section(".ewram")))

//! Put <b>non</b>-initialized variable in EWRAM.
#define  EWRAM_BSS __attribute__((section(".sbss")))

//! Put function in IWRAM.
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

//! Put function in EWRAM.
#define EWRAM_CODE __attribute__((section(".ewram"), long_call))

//! Force a variable to an \a n-byte boundary
#define ALIGN(n)	__attribute__((aligned(n)))

//! Force word alignment.
/*! \note	In the old days, GCC aggregates were always word aligned.
	  In the EABI environment (devkitPro r19 and higher), they are 
	  aligned to their widest member. While technically a good thing, 
	  it may cause problems for struct-copies. If you have aggregates 
	  that can multiples of 4 in size but don't have word members, 
	  consider using this attribute to make struct-copies possible again.
*/
#define ALIGN4		__attribute__((aligned(4)))

//! Pack aggregate members
/*! By default, members in aggregates are aligned to their native 
	  boundaries. Adding this prevents that. It will slow access though.
*/
#define PACKED		__attribute__((packed))

//! Deprecated notice.
/*! Indicates that this function/type/variable should not be used anymore.
	Replacements are (usually) present somewhere as well. 
*/
#define DEPRECATED	__attribute__((deprecated))

//! Inline function declarator
/*!	`inline' inlines the function when -O > 0 when called, 
	  but also creates a body for the function itself
	`static' removes the body as well
*/
#define INLINE static inline

/*	\}	*/


// --------------------------------------------------------------------
// TYPES
// --------------------------------------------------------------------


// === primary typedefs ===============================================

/*!	\defgroup grpTypePrim	Primary types
	\ingroup grpTypes
*/
/*!	\{	*/

/*! \name Base types
	Basic signed and unsigned types for 8, 16, 32 and 64-bit integers.
	<ul>
	   <li>s# : signed #-bit integer. </li>
	   <li>u#/u{type} : unsigned #-bit integer.</li>
	   <li>e{type} : enum'ed #-bit integer.</li>

	</ul> 
*/
//\{
typedef unsigned char  u8,  byte, uchar, echar;
typedef unsigned short u16, hword, ushort, eshort;
typedef unsigned int   u32, word, uint, eint;
typedef unsigned long long u64;

typedef signed char  s8;
typedef signed short s16;
typedef signed int   s32;
typedef signed long long s64;
//\}

/*! \name Volatile types
*	Volatile types for registers
*/
//\{
typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;
//\}

/*! \name Const types
*	Const types for const function aprameters
*/
//\{
typedef const u8  cu8;
typedef const u16 cu16;
typedef const u32 cu32;
typedef const u64 cu64;

typedef const s8  cs8;
typedef const s16 cs16;
typedef const s32 cs32;
typedef const s64 cs64;
//\}

//! 8-word type for fast struct-copies
typedef struct { u32 data[8];  } BLOCK;

//! Type for consting a string as well as the pointer than points to it.
typedef const char * const CSTR;

/*	\}	*/


// === secondary typedefs =============================================

/*!	\defgroup grpTypeSec	Secondary types
*	\ingroup grpTypes
*/
/*!	\{	*/

typedef s32 FIXED;					//!< Fixed point type
typedef u16 COLOR;					//!< Type for colors
typedef u16 SCR_ENTRY, SE;			//!< Type for screen entries
typedef u8  SCR_AFF_ENTRY, SAE;		//!< Type for affine screen entries

//! 4bpp tile type, for easy indexing and copying of 4-bit tiles
typedef struct { u32 data[8];  } TILE, TILE4;

//! 8bpp tile type, for easy indexing and 8-bit tiles
typedef struct { u32 data[16]; } TILE8;


#ifndef __cplusplus
//! Boolean type
typedef enum { false, true } bool;
#endif

#ifndef BOOL
typedef u8 BOOL;	// C++ bool == u8 too, that's why
#define TRUE 1
#define FALSE 0
#endif


// --- function pointer ---

typedef void (*fnptr)(void);	//!< void foo() function pointer
typedef void (*fn_v_i)(int);	//!< void foo(int x) function pointer
typedef int (*fn_i_i)(int);		//!< int foo(int x) function pointer


//! \name affine structs
//\{
//! Simple scale-rotation source struct.
/*! This can be used with ObjAffineSet, and several of tonc's
*	  affine functions
*/
typedef struct AFF_SRC
{
	s16 sx;		//!< Horizontal zoom	(8.8f)
	s16 sy;		//!< Vertical zoom		(8.8f)
	u16 alpha;	//!< Counter-clockwise angle ( range [0, 0xFFFF] )
} ALIGN4 AFF_SRC, ObjAffineSource;


//! Extended scale-rotate source struct
/*! This is used to scale/rotate around an arbitrary point. See
*	  tonc's main text for all the details.
*/
typedef struct AFF_SRC_EX
{
	s32 tex_x;	//!< Texture-space anchor, x coordinate	(.8f)
	s32 tex_y;	//!< Texture-space anchor, y coordinate	(.8f)
	s16 scr_x;	//!< Screen-space anchor, x coordinate	(.0f)
	s16 scr_y;	//!< Screen-space anchor, y coordinate	(.0f)
	s16 sx;		//!< Horizontal zoom	(8.8f)
	s16 sy;		//!< Vertical zoom		(8.8f)
	u16 alpha;	//!< Counter-clockwise angle ( range [0, 0xFFFF] )
} ALIGN4 AFF_SRC_EX, BgAffineSource;

//! Simple scale-rotation destination struct, BG version.
/*! This is a P-matrix with continuous elements, like the BG matrix.
*	  It can be used with ObjAffineSet.
*/
typedef struct AFF_DST
{
	s16 pa, pb;
	s16 pc, pd;
} ALIGN4 AFF_DST, ObjAffineDest;

//! Extended scale-rotate destination struct
/*! This contains the P-matrix and a fixed-point offset , the 
*	  combination can be used to rotate around an arbitrary point.
*	  Mainly intended for BgAffineSet, but the struct cna be used 
*	  for object transforms too.
*/
typedef struct AFF_DST_EX
{
	s16 pa, pb;
	s16 pc, pd;
	s32 dx, dy;
} ALIGN4 AFF_DST_EX, BgAffineDest;

//\}

/*	\}	*/


// === memory map structs  ============================================

/*!	\defgroup grpTypeTert	Tertiary types
*	These types are used for memory mapping of VRAM, affine registers 
*	  and other areas that would benefit from logical memory mapping.
*	\ingroup grpTypes
*/
/*!	\{	*/


//! \name IO register types
//\{

//! Regular bg points; range: :0010 - :001F
typedef struct POINT16 { s16 x, y; } ALIGN4 POINT16, BG_POINT;

//! Affine parameters for backgrounds; range : 0400:0020 - 0400:003F
typedef struct AFF_DST_EX BG_AFFINE;

//!	DMA struct; range: 0400:00B0 - 0400:00DF
typedef struct DMA_REC
{
	const void *src;
	void *dst;
	u32 cnt;
} DMA_REC;

//! Timer struct, range: 0400:0100 - 0400:010F
/*! \note The attribute is required, because union's counted as u32 otherwise.
*/
typedef struct TMR_REC 
{
	union { u16 start, count; } PACKED;
	u16 cnt;
} ALIGN4 TMR_REC;

//\}


//! \name PAL types 
//\{

//! Palette bank type, for 16-color palette banks 
typedef COLOR PALBANK[16];

//\}


/*! \name VRAM array types
*	These types allow VRAM access as arrays or matrices in their 
*	  most natural types.
*/
//\{
typedef SCR_ENTRY	SCREENLINE[32];
typedef SCR_ENTRY	SCREENMAT[32][32];
typedef SCR_ENTRY	SCREENBLOCK[1024];

typedef COLOR		M3LINE[240];
typedef u8			M4LINE[240];	// NOTE: u8, not u16!!
typedef COLOR		M5LINE[160];

typedef TILE		CHARBLOCK[512];
typedef TILE8		CHARBLOCK8[256];

//\}


/*! \name OAM structs 
*	\note These OBJ_ATTR and OBJ_AFFINE structs are interlaced in OAM. 
*	  When using affine objs, struct/DMA/mem copies will give bad results.
*/
//\{

//! Object attributes.
/*!	\note attribute 3 is padding for the interlace with OBJ_AFFINE. If 
*	not using affine objects, it can be used as a free field
*/
typedef struct OBJ_ATTR
{
	u16 attr0;
	u16 attr1;
	u16 attr2;
	s16 fill;
} ALIGN4 OBJ_ATTR;


//! Object affine parameters.
/*!	\note most fields are padding for the interlace with OBJ_ATTR. 
*/
typedef struct OBJ_AFFINE
{
	u16 fill0[3];	s16 pa;
	u16 fill1[3];	s16 pb;
	u16 fill2[3];	s16 pc;
	u16 fill3[3];	s16 pd;
} ALIGN4 OBJ_AFFINE;

//\}


/*!	\}	*/


// --------------------------------------------------------------------
// DEFINES 
// --------------------------------------------------------------------


#ifndef NULL
#define NULL (void*)0
#endif


#endif // TONC_TYPES

