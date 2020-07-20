//
//  Basic video functions
//
//! \file tonc_video.h
//! \author J Vijn
//! \date 20060604 - 20080311
//
// === NOTES ===
// * Basic video-IO, color, background and object functionality

#ifndef TONC_VIDEO
#define TONC_VIDEO

#include "tonc_memmap.h"
#include "tonc_memdef.h"
#include "tonc_core.h"

/*! \defgroup grpVideoPal	Colors
	\ingroup grpVideo
*/

/*! \defgroup grpVideoBg	Tiled Backgrounds
	\ingroup grpVideo
*/

/*! \defgroup grpVideoBmp	Bitmaps
	\ingroup grpVideo

	Basic functions for dealing with bitmapped graphics.
	\deprecated	The bmp8/bmp16 functions have been superceded by the 
		surface functions (sbmp8/sbmp16) for the most part. The 
		former group has been kept mostly for reference purposes.
*/

/*! \defgroup grpVideoObj	Objects
	\ingroup grpVideo
*/

/*! \defgroup grpVideoAffine	Affine functions
	\ingroup grpVideo
*/


// --------------------------------------------------------------------
// VIDEO CORE
// --------------------------------------------------------------------


// --- Constants ------------------------------------------------------

// sizes in pixels
#define SCREEN_WIDTH		240
#define SCREEN_HEIGHT		160

#define M3_WIDTH			SCREEN_WIDTH
#define M3_HEIGHT			SCREEN_HEIGHT
#define M4_WIDTH			SCREEN_WIDTH
#define M4_HEIGHT			SCREEN_HEIGHT
#define M5_WIDTH			160
#define M5_HEIGHT			128

// sizes in tiles
#define SCREEN_WIDTH_T		(SCREEN_WIDTH/8)
#define SCREEN_HEIGHT_T		(SCREEN_HEIGHT/8)

// total scanlines
#define SCREEN_LINES		228

// or a bit shorter
#define SCR_W		SCREEN_WIDTH
#define SCR_H		SCREEN_HEIGHT
#define SCR_WT		SCREEN_WIDTH_T
#define SCR_HT		SCREEN_HEIGHT_T

#define LAYER_BG0	0x0001
#define LAYER_BG1	0x0002
#define LAYER_BG2	0x0004
#define LAYER_BG3	0x0008
#define LAYER_OBJ	0x0010
#define LAYER_BD	0x0020

// --- Prototypes -----------------------------------------------------

INLINE void vid_vsync(void);
void vid_wait(uint frames);
u16 *vid_flip(void);


// --------------------------------------------------------------------
// COLOR and PALETTE
// --------------------------------------------------------------------


//! \addtogroup grpVideoPal
/*!	\{	*/

//! \name Base Color constants
//\{

#define CLR_BLACK			0x0000
#define CLR_RED				0x001F
#define CLR_LIME			0x03E0	// yup. Green == darker green
#define CLR_YELLOW			0x03FF
#define CLR_BLUE			0x7C00
#define CLR_MAG				0x7C1F
#define CLR_CYAN			0x7FE0
#define CLR_WHITE			0x7FFF

//\}

//! \name Additional colors
//\{

#define CLR_DEAD			0xDEAD
#define CLR_MAROON			0x0010
#define CLR_GREEN			0x0200
#define CLR_OLIVE			0x0210
#define CLR_ORANGE			0x021F
#define CLR_NAVY			0x4000
#define CLR_PURPLE			0x4010
#define CLR_TEAL			0x4200
#define CLR_GRAY			0x4210
#define CLR_MEDGRAY			0x5294
#define CLR_SILVER			0x6318
#define CLR_MONEYGREEN		0x6378
#define CLR_FUCHSIA			0x7C1F
#define CLR_SKYBLUE			0x7B34
#define CLR_CREAM			0x7BFF

//\}


#define CLR_MASK			0x001F

#define RED_MASK			0x001F
#define RED_SHIFT				 0
#define GREEN_MASK			0x03E0
#define GREEN_SHIFT				 5
#define BLUE_MASK			0x7C00
#define BLUE_SHIFT				10


void clr_rotate(COLOR *clrs, uint nclrs, int ror);
void clr_blend(const COLOR *srca, const COLOR *srcb, COLOR *dst, 
		u32 nclrs, u32 alpha);
void clr_fade(const COLOR *src, COLOR clr, COLOR *dst, 
		u32 nclrs, u32 alpha);

void clr_grayscale(COLOR *dst, const COLOR *src, uint nclrs);
void clr_rgbscale(COLOR *dst, const COLOR *src, uint nclrs, COLOR clr);

void clr_adj_brightness(COLOR *dst, const COLOR *src, uint nclrs, FIXED bright);
void clr_adj_contrast(COLOR *dst, const COLOR *src, uint nclrs, FIXED contrast);
void clr_adj_intensity(COLOR *dst, const COLOR *src, uint nclrs, FIXED intensity);

void pal_gradient(COLOR *pal, int first, int last);
void pal_gradient_ex(COLOR *pal, int first, int last, COLOR clr_first, COLOR clr_last);


//!	Blends color arrays \a srca and \a srcb into \a dst.
/*!	\param srca	Source array A.
*	\param srcb	Source array B
*	\param dst	Destination array.
*	\param nclrs	Number of colors.
*	\param alpha	Blend weight (range: 0-32).
*	\note Handles 2 colors per loop. Very fast.
*/
IWRAM_CODE void clr_blend_fast(COLOR *srca, COLOR *srcb, COLOR *dst, 
	uint nclrs, u32 alpha);

//!	Fades color arrays \a srca to \a clr into \a dst.
/*!	\param src	Source array.
*	\param clr	Final color (at alpha=32).
*	\param dst	Destination array.
*	\param nclrs	Number of colors.
*	\param alpha	Blend weight (range: 0-32).
*	\note Handles 2 colors per loop. Very fast.
*/
IWRAM_CODE void clr_fade_fast(COLOR *src, COLOR clr, COLOR *dst, 
	uint nclrs, u32 alpha);


INLINE COLOR RGB15(int red, int green, int blue);
INLINE COLOR RGB15_SAFE(int red, int green, int blue);

INLINE COLOR RGB8(u8 red, u8 green, u8 blue);

/*!	\}	*/


/*! \addtogroup grpVideoBmp	*/
/*	\{	*/

//! \name Generic 8bpp bitmaps
//\{

void bmp8_plot(int x, int y, u32 clr, void *dstBase, uint dstP);

void bmp8_hline(int x1, int y, int x2, u32 clr, void *dstBase, uint dstP);
void bmp8_vline(int x, int y1, int y2, u32 clr, void *dstBase, uint dstP);
void bmp8_line(int x1, int y1, int x2, int y2, u32 clr, 
	void *dstBase, uint dstP);

void bmp8_rect(int left, int top, int right, int bottom, u32 clr,
	void *dstBase, uint dstP);
void bmp8_frame(int left, int top, int right, int bottom, u32 clr,
	void *dstBase, uint dstP);
//\}

//! \name Generic 16bpp bitmaps
//\{
void bmp16_plot(int x, int y, u32 clr, void *dstBase, uint dstP);

void bmp16_hline(int x1, int y, int x2, u32 clr, void *dstBase, uint dstP);
void bmp16_vline(int x, int y1, int y2, u32 clr, void *dstBase, uint dstP);
void bmp16_line(int x1, int y1, int x2, int y2, u32 clr, void *dstBase, uint dstP);

void bmp16_rect(int left, int top, int right, int bottom, u32 clr,
	void *dstBase, uint dstP);
void bmp16_frame(int left, int top, int right, int bottom, u32 clr,
	void *dstBase, uint dstP);
//\}

/*!	\}	*/


// --------------------------------------------------------------------
// TILED BACKGROUNDS
// --------------------------------------------------------------------


//! \addtogroup grpVideoBg
/*!	\{	*/

// --- Macros ---------------------------------------------------------

#define CBB_CLEAR(cbb) memset32(&tile_mem[cbb], 0, CBB_SIZE/4)

#define SBB_CLEAR(sbb) memset32(&se_mem[sbb], 0, SBB_SIZE/4)
#define SBB_CLEAR_ROW(sbb, row)		\
	memset32(&se_mem[sbb][(row)*32], 0, 32/2)


// --- bg-types and availability checks for vid-modes 0,1,2 ---
//         3 2 1 0  avail type
// mode 0  r r r r   000F 0000
// mode 1  - a r r   0070 0040
// mode 2  a a - -   0C00 0C00  |
//                 0x0C7F0C40  
#define  __BG_TYPES ((0x0C7F<<16)|(0x0C40))

// Get affinity and availability of background n (output is 0 or 1)
#define BG_IS_AFFINE(n) ( (__BG_TYPES>>(4*(REG_DISPCNT&7)+(n)   ))&1 )
#define BG_IS_AVAIL(n)  ( (__BG_TYPES>>(4*(REG_DISPCNT&7)+(n)+16))&1 )


INLINE void se_fill(SCR_ENTRY *sbb, SCR_ENTRY se);
INLINE void se_plot(SCR_ENTRY *sbb, int x, int y, SCR_ENTRY se);
INLINE void se_rect(SCR_ENTRY *sbb, int left, int top, int right, int bottom, SCR_ENTRY se);
INLINE void se_frame(SCR_ENTRY *sbb, int left, int top, int right, int bottom, SCR_ENTRY se);

void se_window(SCR_ENTRY *sbb, int left, int top, int right, int bottom, SCR_ENTRY se0);

void se_hline(SCR_ENTRY *sbb, int x0, int x1, int y, SCR_ENTRY se);
void se_vline(SCR_ENTRY *sbb, int x, int y0, int y1, SCR_ENTRY se);


// --- Prototypes -----------------------------------------------------

// --- affine ---
INLINE void bg_aff_set(BG_AFFINE *bgaff, 
	FIXED pa, FIXED pb, FIXED pc, FIXED pd);
INLINE void bg_aff_identity(BG_AFFINE *bgaff);
INLINE void bg_aff_scale(BG_AFFINE *bgaff, FIXED sx, FIXED sy);
INLINE void bg_aff_shearx(BG_AFFINE *bgaff, FIXED hx);
INLINE void bg_aff_sheary(BG_AFFINE *bgaff, FIXED hy);

void bg_aff_rotate(BG_AFFINE *bgaff, u16 alpha);
void bg_aff_rotscale(BG_AFFINE *bgaff, int sx, int sy, u16 alpha);
void bg_aff_premul(BG_AFFINE *dst, const BG_AFFINE *src);
void bg_aff_postmul(BG_AFFINE *dst, const BG_AFFINE *src);
void bg_aff_rotscale2(BG_AFFINE *bgaff, const AFF_SRC *as);
void bg_rotscale_ex(BG_AFFINE *bgaff, const AFF_SRC_EX *asx);

/*!	\}	*/


// --------------------------------------------------------------------
// BITMAPS
// --------------------------------------------------------------------


//! \addtogroup grpVideoBmp
/*!	\{	*/

//! \name mode 3
//\{

#define M3_CLEAR()  memset32(vid_mem,  0, M3_SIZE/4)

INLINE void m3_fill(COLOR clr);
INLINE void m3_plot(int x, int y, COLOR clr);

INLINE void m3_hline(int x1, int y, int x2, COLOR clr);
INLINE void m3_vline(int x, int y1, int y2, COLOR clr);
INLINE void m3_line(int x1, int y1, int x2, int y2, COLOR clr);

INLINE void m3_rect(int left, int top, int right, int bottom, COLOR clr);
INLINE void m3_frame(int left, int top, int right, int bottom, COLOR clr);

//\}


//! \name mode 4
//\{

#define M4_CLEAR()  memset32(vid_page, 0, M4_SIZE/4)

INLINE void m4_fill(u8 clrid);
INLINE void m4_plot(int x, int y, u8 clrid);

INLINE void m4_hline(int x1, int y, int x2, u8 clrid);
INLINE void m4_vline(int x, int y1, int y2, u8 clrid);
INLINE void m4_line(int x1, int y1, int x2, int y2, u8 clrid);

INLINE void m4_rect(int left, int top, int right, int bottom, u8 clrid);
INLINE void m4_frame(int left, int top, int right, int bottom, u8 clrid);

//\}


//! \name mode 5
//\{

#define M5_CLEAR()  memset32(vid_page, 0, M5_SIZE/4)

INLINE void m5_fill(COLOR clr);
INLINE void m5_plot(int x, int y, COLOR clr);

INLINE void m5_hline(int x1, int y, int x2, COLOR clr);
INLINE void m5_vline(int x, int y1, int y2, COLOR clr);
INLINE void m5_line(int x1, int y1, int x2, int y2, COLOR clr);

INLINE void m5_rect(int left, int top, int right, int bottom, COLOR clr);
INLINE void m5_frame(int left, int top, int right, int bottom, COLOR clr);

//\}

/*!	\}	*/


// --------------------------------------------------------------------
// INLINES
// --------------------------------------------------------------------



// --- General --------------------------------------------------------

INLINE void vid_vsync(void)
{
	while(REG_VCOUNT >= 160);   // wait till VDraw
	while(REG_VCOUNT < 160);    // wait till VBlank
}



// --- Colors ---------------------------------------------------------


//! Create a 15bit BGR color.
INLINE COLOR RGB15(int red, int green, int blue)
{	return red + (green<<5) + (blue<<10);					}

//! Create a 15bit BGR color, with proper masking of R,G,B components.
INLINE COLOR RGB15_SAFE(int red, int green, int blue)
{	return (red&31) + ((green&31)<<5) + ((blue&31)<<10);	}


//! Create a 15bit BGR color, using 8bit components
INLINE COLOR RGB8(u8 red, u8 green, u8 blue)
{	return  (red>>3) + ((green>>3)<<5) + ((blue>>3)<<10);	}


// --- Backgrounds ----------------------------------------------------


//! Fill screenblock \a sbb with \a se
INLINE void se_fill(SCR_ENTRY *sbb, SCR_ENTRY se)
{	memset32(sbb, dup16(se), SBB_SIZE/4);					}

//! Plot a screen entry at (\a x,\a y) of screenblock \a sbb.
INLINE void se_plot(SCR_ENTRY *sbb, int x, int y, SCR_ENTRY se)
{	sbb[y*32+x]= se;										}

//! Fill a rectangle on \a sbb with \a se.
INLINE void se_rect(SCR_ENTRY *sbb, int left, int top, int right, int bottom, 
	SCR_ENTRY se)
{	bmp16_rect(left, top, right, bottom, se, sbb, 32*2);				}

//! Create a border on \a sbb with \a se.
INLINE void se_frame(SCR_ENTRY *sbb, int left, int top, int right, int bottom, 
	SCR_ENTRY se)
{	bmp16_frame(left, top, right, bottom, se, sbb, 32*2);				}


// --- Affine ---


//! Copy bg affine aprameters
INLINE void bg_aff_copy(BG_AFFINE *dst, const BG_AFFINE *src)
{	*dst = *src;	}

//! Set the elements of an \a bg affine matrix.
INLINE void bg_aff_set(BG_AFFINE *bgaff, 
	FIXED pa, FIXED pb, FIXED pc, FIXED pd)
{
	bgaff->pa= pa;	bgaff->pb= pb;
	bgaff->pc= pc;	bgaff->pd= pd;
}

//! Set an bg affine matrix to the identity matrix
INLINE void bg_aff_identity(BG_AFFINE *bgaff)
{
	bgaff->pa= 0x0100;	bgaff->pb= 0;
	bgaff->pc= 0;		bgaff->pd= 0x0100;
}

//! Set an bg affine matrix for scaling.
INLINE void bg_aff_scale(BG_AFFINE *bgaff, FIXED sx, FIXED sy)
{
	bgaff->pa= sx;	bgaff->pb= 0;
	bgaff->pb= 0;	bgaff->pd= sy;
}

INLINE void bg_aff_shearx(BG_AFFINE *bgaff, FIXED hx)
{
	bgaff->pa= 0x0100;	bgaff->pb= hx;
	bgaff->pc= 0;		bgaff->pd= 0x0100;
}

INLINE void bg_aff_sheary(BG_AFFINE *bgaff, FIXED hy)
{
	bgaff->pa= 0x0100;	bgaff->pb= 0;
	bgaff->pc= hy;		bgaff->pd= 0x0100;
}


// --- Bitmaps --------------------------------------------------------


// --- mode 3 interface ---


//! Fill the mode 3 background with color \a clr.
INLINE void m3_fill(COLOR clr)	
{	memset32(vid_mem, dup16(clr), M3_SIZE/4);						}


//! Plot a single \a clr colored pixel in mode 3 at (\a x, \a y).
INLINE void m3_plot(int x, int y, COLOR clr)
{	vid_mem[y*M3_WIDTH+x]= clr;	}


//! Draw a \a clr colored horizontal line in mode 3.
INLINE void m3_hline(int x1, int y, int x2, COLOR clr)
{	bmp16_hline(x1, y, x2, clr, vid_mem, M3_WIDTH*2);				}


//! Draw a \a clr colored vertical line in mode 3.
INLINE void m3_vline(int x, int y1, int y2, COLOR clr)
{	bmp16_vline(x, y1, y2, clr, vid_mem, M3_WIDTH*2);				}


//! Draw a \a clr colored line in mode 3.
INLINE void m3_line(int x1, int y1, int x2, int y2, COLOR clr)
{	bmp16_line(x1, y1, x2, y2, clr, vid_mem, M3_WIDTH*2);			}


//! Draw a \a clr colored rectangle in mode 3.
/*! \param left	Left side, inclusive.
*	\param top	Top size, inclusive.
*	\param right	Right size, exclusive.
*	\param bottom	Bottom size, exclusive.
*	\param clr	Color.
*	\note Normalized, but not clipped.
*/
INLINE void m3_rect(int left, int top, int right, int bottom, COLOR clr)
{	bmp16_rect(left, top, right, bottom, clr, vid_mem, M3_WIDTH*2);	}


//! Draw a \a clr colored frame in mode 3.
/*! \param left	Left side, inclusive.
*	\param top	Top size, inclusive.
*	\param right	Right size, exclusive.
*	\param bottom	Bottom size, exclusive.
*	\param clr	Color.
*	\note Normalized, but not clipped.
*/
INLINE void m3_frame(int left, int top, int right, int bottom, COLOR clr)
{	bmp16_frame(left, top, right, bottom, clr, vid_mem, M3_WIDTH*2);	}


// --- mode 4 interface ---


//! Fill the current mode 4 backbuffer with \a clrid
INLINE void m4_fill(u8 clrid) 
{	memset32(vid_page, quad8(clrid), M4_SIZE/4);						}


//! Plot a \a clrid pixel on the current mode 4 backbuffer
INLINE void m4_plot(int x, int y, u8 clrid)
{
	u16 *dst= &vid_page[(y*M4_WIDTH+x)>>1];
	if(x&1)
		*dst= (*dst& 0xFF) | (clrid<<8);
	else
		*dst= (*dst&~0xFF) |  clrid;
}


//! Draw a \a clrid colored horizontal line in mode 4.
INLINE void m4_hline(int x1, int y, int x2, u8 clrid)
{	bmp8_hline(x1, y, x2, clrid, vid_page, M4_WIDTH);					}


//! Draw a \a clrid colored vertical line in mode 4.
INLINE void m4_vline(int x, int y1, int y2, u8 clrid)
{	bmp8_vline(x, y1, y2, clrid, vid_page, M4_WIDTH);					}


//! Draw a \a clrid colored line in mode 4.
INLINE void m4_line(int x1, int y1, int x2, int y2, u8 clrid)
{	bmp8_line(x1, y1, x2, y2, clrid, vid_page, M4_WIDTH);				}


//! Draw a \a clrid colored rectangle in mode 4.
/*! \param left	Left side, inclusive.
*	\param top	Top size, inclusive.
*	\param right	Right size, exclusive.
*	\param bottom	Bottom size, exclusive.
*	\param clrid	color index.
*	\note Normalized, but not clipped.
*/
INLINE void m4_rect(int left, int top, int right, int bottom, u8 clrid)
{	bmp8_rect(left, top, right, bottom, clrid, vid_page, M4_WIDTH);		}


//! Draw a \a clrid colored frame in mode 4.
/*! \param left	Left side, inclusive.
*	\param top	Top size, inclusive.
*	\param right	Right size, exclusive.
*	\param bottom	Bottom size, exclusive.
*	\param clrid	color index.
*	\note Normalized, but not clipped.
*/
INLINE void m4_frame(int left, int top, int right, int bottom, u8 clrid)
{	bmp8_frame(left, top, right, bottom, clrid, vid_page, M4_WIDTH);	}


// --- mode 5 interface ---


//! Fill the current mode 5 backbuffer with \a clr
INLINE void m5_fill(COLOR clr) 
{	memset32(vid_page, dup16(clr), M5_SIZE/4);							}


//! Plot a \a clrid pixel on the current mode 5 backbuffer
INLINE void m5_plot(int x, int y, COLOR clr)
{	vid_page[y*M5_WIDTH+x]= clr;										}


//! Draw a \a clr colored horizontal line in mode 5.
INLINE void m5_hline(int x1, int y, int x2, COLOR clr)
{	bmp16_hline(x1, y, x2, clr, vid_page, M5_WIDTH*2);					}


//! Draw a \a clr colored vertical line in mode 5.
INLINE void m5_vline(int x, int y1, int y2, COLOR clr)
{	bmp16_vline(x, y1, y2, clr, vid_page, M5_WIDTH*2);					}


//! Draw a \a clr colored line in mode 5.
INLINE void m5_line(int x1, int y1, int x2, int y2, COLOR clr)
{	bmp16_line(x1, y1, x2, y2, clr, vid_page, M5_WIDTH*2);				}


//! Draw a \a clr colored rectangle in mode 5.
/*! \param left	Left side, inclusive.
*	\param top	Top size, inclusive.
*	\param right	Right size, exclusive.
*	\param bottom	Bottom size, exclusive.
*	\param clr	Color.
*	\note Normalized, but not clipped.
*/
INLINE void m5_rect(int left, int top, int right, int bottom, COLOR clr)
{	bmp16_rect(left, top, right, bottom, clr, vid_page, M5_WIDTH*2);	}


//! Draw a \a clr colored frame in mode 5.
/*! \param left	Left side, inclusive.
*	\param top	Top size, inclusive.
*	\param right	Right size, exclusive.
*	\param bottom	Bottom size, exclusive.
*	\param clr	Color.
*	\note Normalized, but not clipped.
*/
INLINE void m5_frame(int left, int top, int right, int bottom, COLOR clr)
{	bmp16_frame(left, top, right, bottom, clr, vid_page, M5_WIDTH*2);	}


#endif // TONC_VIDEO

