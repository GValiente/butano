//
// Header for graphics surfaces.
//
//! \file tonc_surface.h
//! \author J Vijn
//! \date 20080119 - 20080514
//
/* === NOTES ===
*/


#ifndef TONC_SURFACE
#define TONC_SURFACE

#include "tonc_memmap.h"
#include "tonc_core.h"

/*! \defgroup	grpSurface	Surface functions
	\ingroup	grpVideo
	Tonclib's Surface system provides the basic functionality for 
	drawing onto graphic surfaces of different types. This includes 
	- <b>bmp16</b>: 16bpp bitmap surfaces
	- <b>bmp8</b>: 8bpp bitmap surfaces.
	- <b>chr4</b>(c/r): 4bpp tiled surfaces.
	This covers almost all of the GBA graphic modes.
*/

/*! \defgroup	grpSbmp16	16bpp bitmap surfaces
	\ingroup	grpSurface
	Routines for 16bpp linear surfaces. For use in modes 3 and 5. Can 
	also be used for regular tilemaps to a point.
*/


/*! \defgroup	grpSbmp8	8bpp bitmap surfaces
	\ingroup	grpSurface
	Routines for 8bpp linear surfaces. For use in mode 4 and 
	affine tilemaps.
*/


/*! \defgroup	grpSchr4c	4bpp tiled surfaces, column major
	\ingroup	grpSurface
	<p>
	A (4bpp) tiled surface is formed when each tilemap entry 
	references a unique tile (this is done by schr4c_prep_map()). 
	The pixels on the tiles will then uniquely map onto pixels on the 
	screen.
	</p>
	<p>
	There are two ways of map-layout here: row-major indexing and 
	column-major indexing. The difference if is that tile 1 is to the 
	right of tile 0 in the former, but under it in the latter.
	</p> 
<pre>
30x20t screen:
  Row-major:
     0  1  2  3 ...
    30 31 32 33 ...
    60 61 62 63 ...

  Column-major:
     0 20 40 60 ...
     1 21 41 61 ...
     2 22 41 62 ...
</pre>
	<p>
	With 4bpp tiles, the column-major version makes the <i>y</i> 
	coordinate match up nicely with successive words. For this reason, 
	column-major is preferred over row-major.
	</p>
*/


/*! \defgroup	grpSchr4r	4bpp tiled surfaces, row major
	\ingroup	grpSurface
	<p>
	A (4bpp) tiled surface is formed when each tilemap entry 
	references a unique tile (this is done by schr4r_prep_map()). 
	The pixels on the tiles will then uniquely map onto pixels on the 
	screen.
	</p>
	<p>
	There are two ways of map-layout here: row-major indexing and 
	column-major indexing. The difference if is that tile 1 is to the 
	right of tile 0 in the former, but under it in the latter.
	</p> 
<pre>
30x20t screen:
  Row-major:
     0  1  2  3 ...
    30 31 32 33 ...
    60 61 62 63 ...

  Column-major:
     0 20 40 60 ...
     1 21 41 61 ...
     2 22 41 62 ...
</pre>
	<p>
	With 4bpp tiles, the column-major version makes the <i>y</i> 
	coordinate match up nicely with successive words. For this reason, 
	column-major is preferred over row-major.
	</p>
*/


/*! \addtogroup grpSurface	*/
/*!	\{	*/

// --------------------------------------------------------------------
// CLASSES
// --------------------------------------------------------------------

//! Surface types
typedef enum ESurfaceType
{
	SRF_NONE		=0,		//!< No specific type. 
	SRF_BMP16		=1,		//!< 16bpp linear (bitmap/tilemap).
	SRF_BMP8		=2,		//!< 8bpp linear (bitmap/tilemap).
	//SRF_SBB		=3,		//!< 16bpp tilemap in screenblocks
	SRF_CHR4R		=4,		//!< 4bpp tiles, row-major.
	SRF_CHR4C		=5,		//!< 4bpp tiles, column-major.
	SRF_CHR8		=6,		//!< 8bpp tiles, row-major.
	SRF_ALLOCATED	= 0x80	//!< Pointers have been allocated.
} ESurfaceType;


// --------------------------------------------------------------------
// CLASSES
// --------------------------------------------------------------------

typedef struct TSurface
{
	u8	*data;		//!< Surface data pointer.
	u32	pitch;		//!< Scanline pitch in bytes (PONDER: alignment?).
	u16 width;		//!< Image width in pixels.	
	u16 height;		//!< Image width in pixels.
	u8	bpp;		//!< Bits per pixel.
	u8	type;		//!< Surface type (not used that much).
	u16	palSize;	//!< Number of colors.
	u16	*palData;	//!< Pointer to palette.
} TSurface;

/*!	\}	*/

//! \name Rendering procedure types 
//\{
typedef u32 (*fnGetPixel)(const TSurface *src, int x, int y);

typedef void (*fnPlot)(const TSurface *dst, int x, int y, u32 clr);
typedef void (*fnHLine)(const TSurface *dst, int x1, int y, int x2, u32 clr);
typedef void (*fnVLine)(const TSurface *dst, int x, int y1, int y2, u32 clr);
typedef void (*fnLine)(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr);

typedef void (*fnRect)(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);
typedef void (*fnFrame)(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);

typedef void (*fnBlit)(const TSurface *dst, int dstX, int dstY, 
	uint width, uint height, const TSurface *src, int srcX, int srcY);
typedef void (*fnFlood)(const TSurface *dst, int x, int y, u32 clr);

// Rendering procedure table
typedef struct TSurfaceProcTab
{
	const char	*name;
	fnGetPixel	getPixel;
	fnPlot		plot;
	fnHLine		hline;
	fnVLine		vline;
	fnLine		line;
	fnRect		rect;
	fnFrame		frame;
	fnBlit		blit;
	fnFlood		flood;
} TSurfaceProcTab;

//\}

// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------

extern const TSurface m3_surface;
extern EWRAM_DATA TSurface m4_surface;
extern EWRAM_DATA TSurface m5_surface;

extern const TSurfaceProcTab bmp16_tab;
extern const TSurfaceProcTab bmp8_tab;
extern const TSurfaceProcTab chr4c_tab;


// --------------------------------------------------------------------
// PROTOTYPES
// --------------------------------------------------------------------

/*! \addtogroup grpSurface
	\brief	Basic video surface API.
	The TSurface struct and the various functions working on it 
	provide a basic API for working with different types of 
	graphic surfaces, like 16bpp bitmaps, 8bpp bitmaps, but also 
	tiled surfaces.<br>

	- <b>SRF_BMP8</b>:	8bpp linear (Mode 4 / affine BGs)
	- <b>SRF_BMP16</b>	16bpp bitmaps (Mode 3/5 / regular BGs to some extent)
	- <b>SRF_CHR4C</b>	4bpp tiles, column-major (Regular tiled BG)
	- <b>SRF_CHR4R</b>	4bpp tiles, row-major (Regular tiled BG, OBJs)

	For each of these functions exist for the most important drawing 
	options: plotting, lines and rectangles. For BMP8/BMP16 and to 
	some extent CHR4C, there are blitters as well.
*/
/*!	\{	*/

void srf_init(TSurface *srf, enum ESurfaceType type, const void *data, 
	uint width, uint height, uint bpp, u16 *pal);
void srf_pal_copy(const TSurface *dst, const TSurface *src, uint count);

void *srf_get_ptr(const TSurface *srf, uint x, uint y);


INLINE uint srf_align(uint width, uint bpp);
INLINE void srf_set_ptr(TSurface *srf, const void *ptr);
INLINE void srf_set_pal(TSurface *srf, const u16 *pal, uint size);

INLINE void *_srf_get_ptr(const TSurface *srf, uint x, uint y, uint stride);


/*!	\}	*/


/*! \addtogroup grpSbmp16	*/
/*!	\{	*/

u32  sbmp16_get_pixel(const TSurface *src, int x, int y);

void sbmp16_plot(const TSurface *dst, int x, int y, u32 clr);
void sbmp16_hline(const TSurface *dst, int x1, int y, int x2, u32 clr);
void sbmp16_vline(const TSurface *dst, int x, int y1, int y2, u32 clr);
void sbmp16_line(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr);

void sbmp16_rect(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);
void sbmp16_frame(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);
void sbmp16_blit(const TSurface *dst, int dstX, int dstY, 
	uint width, uint height, const TSurface *src, int srcX, int srcY);
void sbmp16_floodfill(const TSurface *dst, int x, int y, u32 clr);

// Fast inlines .
INLINE void _sbmp16_plot(const TSurface *dst, int x, int y, u32 clr);
INLINE u32 _sbmp16_get_pixel(const TSurface *src, int x, int y);

/*!	\}	*/


/*! \addtogroup grpSbmp8	*/
/*!	\{	*/

u32  sbmp8_get_pixel(const TSurface *src, int x, int y);

void sbmp8_plot(const TSurface *dst, int x, int y, u32 clr);
void sbmp8_hline(const TSurface *dst, int x1, int y, int x2, u32 clr);
void sbmp8_vline(const TSurface *dst, int x, int y1, int y2, u32 clr);
void sbmp8_line(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr);

void sbmp8_rect(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);
void sbmp8_frame(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);
void sbmp8_blit(const TSurface *dst, int dstX, int dstY, 
	uint width, uint height, const TSurface *src, int srcX, int srcY);
void sbmp8_floodfill(const TSurface *dst, int x, int y, u32 clr);

// Fast inlines .
INLINE void _sbmp8_plot(const TSurface *dst, int x, int y, u32 clr);
INLINE u32 _sbmp8_get_pixel(const TSurface *src, int x, int y);

/*!	\}	*/


/*! \addtogroup grpSchr4c	*/
/*!	\{	*/

u32 schr4c_get_pixel(const TSurface *src, int x, int y);

void schr4c_plot(const TSurface *dst, int x, int y, u32 clr);
void schr4c_hline(const TSurface *dst, int x1, int y, int x2, u32 clr);
void schr4c_vline(const TSurface *dst, int x, int y1, int y2, u32 clr);
void schr4c_line(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr);

void schr4c_rect(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);
void schr4c_frame(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);

void schr4c_blit(const TSurface *dst, int dstX, int dstY, 
	uint width, uint height, const TSurface *src, int srcX, int srcY);
void schr4c_floodfill(const TSurface *dst, int x, int y, u32 clr);

// Additional routines
void schr4c_prep_map(const TSurface *srf, u16 *map, u16 se0);
u32 *schr4c_get_ptr(const TSurface *srf, int x, int y);

// Fast inlines .
INLINE void _schr4c_plot(const TSurface *dst, int x, int y, u32 clr);
INLINE u32 _schr4c_get_pixel(const TSurface *src, int x, int y);

/*!	\}	*/


/*! \addtogroup grpSchr4r	*/
/*!	\{	*/

u32 schr4r_get_pixel(const TSurface *src, int x, int y);

void schr4r_plot(const TSurface *dst, int x, int y, u32 clr);
void schr4r_hline(const TSurface *dst, int x1, int y, int x2, u32 clr);
void schr4r_vline(const TSurface *dst, int x, int y1, int y2, u32 clr);
void schr4r_line(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr);

void schr4r_rect(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);
void schr4r_frame(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr);

//void schr4r_blit(const TSurface *dst, int dstX, int dstY, 
//	uint width, uint height, const TSurface *src, int srcX, int srcY);
//void schr4r_floodfill(const TSurface *dst, int x, int y, u32 clr);

// Additional routines
void schr4r_prep_map(const TSurface *srf, u16 *map, u16 se0);
u32 *schr4r_get_ptr(const TSurface *srf, int x, int y);

// Fast inlines.
INLINE void _schr4r_plot(const TSurface *dst, int x, int y, u32 clr);
INLINE u32 _schr4r_get_pixel(const TSurface *src, int x, int y);

/*!	\}	*/


// --------------------------------------------------------------------
// MAIN INLINES
// --------------------------------------------------------------------

//! Get the word-aligned number of bytes for a scanline.
/*!
	\param width	Number of pixels.
	\param bpp		Bits per pixel.
*/
INLINE uint srf_align(uint width, uint bpp)
{
	return (width*bpp+31)/32*4;
}

//! Set Data-pointer surface for \a srf.
INLINE void srf_set_ptr(TSurface *srf, const void *ptr)
{	
	srf->data= (u8*)ptr;
}


//! Set the palette pointer and its size.
INLINE void srf_set_pal(TSurface *srf, const u16 *pal, uint size)
{	
	srf->palSize= size;
	srf->palData= (u16*)pal;
}


// --------------------------------------------------------------------
// Quick (and dirty) inline routines
// --------------------------------------------------------------------

//! Inline and semi-safe version of srf_get_ptr(). Use with caution.
INLINE void *_srf_get_ptr(const TSurface *srf, uint x, uint y, uint stride)
{
	return (void*)(srf->data + y*srf->pitch + x*stride);
}



//! Get the pixel value of \a src at (\a x, \a y); inline version.
u32 _sbmp16_get_pixel(const TSurface *src, int x, int y)
{
	u16 *srcD= (u16*)(src->data + y*src->pitch);
	return srcD[x];
}

//! Plot a single pixel on a 16-bit buffer; inline version.
void _sbmp16_plot(const TSurface *dst, int x, int y, u32 clr)
{
	u16 *dstD= (u16*)(dst->data + y*dst->pitch);
	dstD[x]= clr;
}



//! Get the pixel value of \a src at (\a x, \a y); inline version.
INLINE u32 _sbmp8_get_pixel(const TSurface *src, int x, int y)
{
	return src->data[y*src->pitch + x];
}


//! Plot a single pixel on a 8-bit surface; inline version.
INLINE void _sbmp8_plot(const TSurface *dst, int x, int y, u32 clr)
{
	u16 *dstD= (u16*)(dst->data + y*dst->pitch+(x&~1));

	if(x&1)	   *dstD= (*dstD& 0xFF) | (clr<<  8); 
	else	   *dstD= (*dstD&~0xFF) | (clr&0xFF);
}


//! Get the pixel value of \a src at (\a x, \a y); inline version.
INLINE u32 _schr4c_get_pixel(const TSurface *src, int x, int y)
{
	uint xx= x, yy= y;
	u32 *srcD= (u32*)(src->data + yy*4 + xx/8*src->pitch);
	uint shift= xx%8*4;

	return (*srcD>>shift)&15;
}

//! Plot a single pixel on a 4bpp tiled,col-jamor surface; inline version.
INLINE void _schr4c_plot(const TSurface *dst, int x, int y, u32 clr)
{
	uint xx= x, yy= y;
	u32 *dstD= (u32*)(dst->data + yy*4 + xx/8*dst->pitch);
	uint shift= xx%8*4;

	*dstD = (*dstD &~ (15<<shift)) | (clr&15)<<shift;
}

//! Get the pixel value of \a src at (\a x, \a y); inline version.
INLINE u32 _schr4r_get_pixel(const TSurface *src, int x, int y)
{
	uint xx= x, yy= y;
	u32 *srcD= (u32*)(src->data+yy/8*src->pitch + yy%8*4 + xx/8*32);
	uint shift= xx%8*4;

	return (*srcD>>shift)&15;
}


//! Plot a single pixel on a 4bpp tiled,row-major surface; inline version.
INLINE void _schr4r_plot(const TSurface *dst, int x, int y, u32 clr)
{
	uint xx= x, yy= y;
	u32 *dstD= (u32*)(dst->data+yy/8*dst->pitch + yy%8*4 + xx/8*32);
	uint shift= xx%8*4;

	*dstD = (*dstD &~ (15<<shift)) | (clr&15)<<shift;
}

#endif // TONC_SURFACE

// EOF
