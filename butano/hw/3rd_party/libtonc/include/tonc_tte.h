//
// Tonc Text Engine main header
//
//! \file tonc_tte.h
//! \author J Vijn
//! \date 20070517 - 20080503
//
/* === NOTES ===
  * 20080503: WARNING : added the 'heights' field to TFont. All 
    older fonts should be updated for the change.
  * 20080225: tte_get_context() calls are optimized out. I checked.
	After a function call all bets are off, of course.
  * 20070723: PONDER: Make positional items signed?
*/

#ifndef TONC_TTE
#define TONC_TTE

#include <stdio.h>

#include "tonc_memmap.h"
#include "tonc_surface.h"



/*! \addtogroup grpTTE
	\brief	A generalized raster text system.

	As of v1.3, Tonc has a completely new way of handling text. It can 
	handle (practically) all modes, VRAM types and font sizes and brings 
	them together under a unified interface. It uses function pointers to 
	store \e drawg and \e erase functions of each rendering family. The 
	families currently supported are:

	- <b>ase</b>:	Affine screen entries (Affine tiled BG)
	- <b>bmp8</b>:	8bpp bitmaps (Mode 4)
	- <b>bmp16</b>	16bpp bitmaps (Mode 3/5)
	- <b>chr4c</b>	4bpp characters, column-major (Regular tiled BG)
	- <b>chr4r</b>	4bpp characters, row-major (Regular tiled BG)
	- <b>obj</b>	Objects
	- <b>se</b>		Regular screen entries (Regular tiled BG)

	Each of these consists of an initializer, \c tte_init_foo, and 
	one or more glyph rendering functions, \c foo_puts_bar, The \c bar 
	part of the renderer denotes the style of the particular renderer, 
	which can indicate:

		- Expected bitdepth of font data (\c b1 for 1bpp, etc)
		- Expected sizes of the character (\c w8 and h8, for example).
		- Application of system colors (\c c ).
		- Transparent or opaque background pixels (\c t or \c o ).
		- Whether the font-data is in 'strip' layout (\c s )

	The included renderers here are usually transparent, 
	recolored, using 1bpp strip glyphs (\c _b1cts ). The initializer 
	takes a bunch of options specific to each family, as well as font 
	and renderer pointers. You can provide your own font and renderers, 
	provided they're formatted correcty. For the default font/renderers, 
	use \c NULL.<br>

	After the calling the initializer, you can write utf-8 encoded text 
	with tte_write() or tte_write_ex(). You can also enable stdio-related 
	functions by calling tte_init_con().<br>

	The system also supposed rudimentary scripting for positions, colors, 
	margins and erases. See tte_cmd_default() and con_cmd_parse() for 
	details.
	\sa	grpSurface
*/


/*!	\defgroup grpTTEOps Operations
	\ingroup grpTTE
	\brief	Basic operations.

	This covers most of the things you can actually use TTE for, 
	like writing the text, getting information about a glyph and setting 
	color attributes.
*/

/*!	\defgroup grpTTEAttr Attributes
	\ingroup grpTTE
	\brief	Basic getters and setters.
*/

/*! \defgroup grpTTEConio Console IO
	\ingroup grpTTE
	\brief	Stdio functionality.

	These functions allow you to use stdio routines for writing, like 
	printf, puts and such. Note that tte_printf is just iprintf ... 
	at least for now.
*/

/*! \defgroup grpTTEMap Tilemap text
	\ingroup grpTTE
	\brief	Text for regular and affine tilemaps. 
	
	The tilemap sub-system loads the tiles into memory first, then 
	writes to the map to show the letters. For this to work properly, 
	the glyph sizes should be 8-pixel aligned.
	\note	At present, the regular tilemap text ignores screenblock 
		boundaries, so 512px wide maps may not work properly.
*/

/*! \defgroup grpTTEChr4c Character text, column-major
	\ingroup grpTTE
	\brief	Text on surface composed of 4bpp tiles, mapped in column-major order.

	There are actually two \e chr4 systems. The difference 
	between the two is the ordering of the tiles: column-major 
	versus row-major. Since column-major is 'better', this is 
	considered the primary sub-system for tiled text.
	\sa grpSchr4c
*/

/*! \defgroup grpTTEChr4r Character text, row-major
	\ingroup grpTTE
	\brief	Text on surface composed of 4bpp tiles, mapped in row-major order.

	There are actually two \e chr4 systems, with row-major and 
	column-major tile indexing. The column-major version is more 
	advanced, so use that when possible.
	\sa grpSchr4r
*/

/*! \defgroup grpTTEBmp Bitmap text
	\ingroup grpTTE
	\brief	Text for 16bpp and 8bpp bitmap surfaces: modes 3, 4 and 5. 

	Note that TTE does not update the pointer of the surface for 
	page-flipping. You'll have to do that yourself.
*/

/*! \defgroup grpTTEObj Object text
	\ingroup grpTTE
	\brief	Text using objects.

	This is similar to tilemaps, in that the glyphs are loaded into 
	object VRAM first and pointed to by the objects. Unlike tilemaps, 
	though, variable-width fonts are possible here. The members of 
	the surface member are used a little differently here, though. 
	the <code>pitch</code> is used as an index to the current 
	object, and <code>width</code> is the number of objects allowed 
	to be used for text. 
*/


// --------------------------------------------------------------------
// CONSTANTS
// --------------------------------------------------------------------

/*! \addtogroup grpTTE	*/
/*!	\{	*/

#define TTE_TAB_WIDTH	24

//! \name Color lut indices
//\{
#define TTE_INK			0
#define TTE_SHADOW		1
#define TTE_PAPER		2
#define TTE_SPECIAL		3
//\}


// --------------------------------------------------------------------
// MACROS
// --------------------------------------------------------------------

//! \name drawg helper macros
/*! Each \c drawg renderer usually starts with the same thing: 
	- Get the system and font pointers.
	- Translate from ascii-character to glyph offset.
	- Get the glyph (and glyph-cell) dimensions.
	- Get the source and destination pointers and positions.
	These macros will make declarint and defining that easier.
*/
//\{

//! Declare and define base drawg variables
#define TTE_BASE_VARS(_tc, _font)		\
	TTC *_tc= tte_get_context();		\
	TFont *_font= tc->font;				\


//! Declare and define basic source drawg variables
#define TTE_CHAR_VARS(font, gid, src_t, _sD, _sL, _chW, _chH)			\
	src_t *_sD= (src_t*)(font->data+gid*font->cellSize), *_sL= _sD;		\
	uint _chW= font->widths ? font->widths[gid] : font->charW;			\
	uint _chH= font->charH;

//! Declare and define basic destination drawg variables
#define TTE_DST_VARS(tc, dst_t, _dD, _dL, _dP, _x, _y)					\
	uint _x= tc->cursorX, _y= tc->cursorY, _dP= tc->dst.pitch;			\
	dst_t *_dD= (dst_t*)(tc->dst.data+_y*_dP), *_dL;

//\}

//! \name Default fonts
//\{
#define fwf_default	sys8Font		//!< Default fixed-width font
#define vwf_default	verdana9Font	//!< Default vairable-width font
//\}

//! \name Default glyph renderers
//\{
#define ase_drawg_default		((fnDrawg)ase_drawg_s)
#define bmp8_drawg_default		((fnDrawg)bmp8_drawg_b1cts)
#define bmp16_drawg_default		((fnDrawg)bmp16_drawg_b1cts)
#define chr4c_drawg_default		((fnDrawg)chr4c_drawg_b1cts)
#define chr4r_drawg_default		((fnDrawg)chr4r_drawg_b1cts)
#define obj_drawg_default		((fnDrawg)obj_drawg)
#define se_drawg_default		((fnDrawg)se_drawg_s)
//\}


//! \name Default initializers
//\{

#define tte_init_se_default(bgnr, bgcnt)								\
	tte_init_se( bgnr, bgcnt, 0xF000, CLR_YELLOW, 0, &fwf_default, NULL)
		
#define tte_init_ase_default(bgnr, bgcnt)								\
	tte_init_ase(bgnr, bgcnt, 0x0000, CLR_YELLOW, 0, &fwf_default, NULL)

		
#define tte_init_chr4c_default(bgnr, bgcnt)								\
	tte_init_chr4c(bgnr, bgcnt, 0xF000, 0x0201, CLR_ORANGE<<16|CLR_YELLOW,	\
		&vwf_default, NULL)

#define tte_init_chr4r_default(bgnr, bgcnt)								\
	tte_init_chr4r(bgnr, bgcnt, 0xF000, 0x0201, CLR_ORANGE<<16|CLR_YELLOW,	\
		&vwf_default, NULL)

#define tte_init_chr4c_b4_default(bgnr, bgcnt)							\
	tte_init_chr4c(bgnr, bgcnt, 0xF000, 0x0201, CLR_ORANGE<<16|CLR_YELLOW,	\
		&verdana9_b4Font, chr4c_drawg_b4cts)


#define tte_init_bmp_default(mode)											\
	tte_init_bmp(mode, &vwf_default, NULL)


#define tte_init_obj_default(pObj)										\
	tte_init_obj(pObj, 0, 0, 0xF000, CLR_YELLOW, 0, &fwf_default, NULL)

//\}



// --------------------------------------------------------------------
// CLASSES
// --------------------------------------------------------------------

//! Glyph render function format.
typedef void (*fnDrawg)(uint gid);

//! Erase rectangle function format.
typedef void (*fnErase)(int left, int top, int right, int bottom);



//! Font description struct.
/*!	The \c TFont contains a description of the font, including pointers
	to the glyph data and width data (for VWF fonts), an ascii-offset
	for when the first glyph isn't for ascii-null (which is likely. 
	Usually it starts at ' ' (32)).<br>
	The font-bitmap is a stack of cells, each containing one glyph 
	each. The cells and characters need not be the same size, but 
	the character glyph must fit within the cell.<br>

	The formatting of the glyphs themselves should fit the rendering 
	procedure. The default renderers use 1bpp 8x8 tiled graphics, 
	where for multi-tiled cells the tiles are in a <b>vertical</b> 
	'strip' format. In an 16x16 cell, the 4 tiles would be arranged as:

	<table border=1 cellpadding=2 cellspacing=0>
	<tr> <td>0</td> <td>2</td></tr>
	<tr> <td>1</td> <td>3</td></tr>
	</table>
*/
typedef struct TFont
{
	const void	*data;		//!< Character data.
	const u8	*widths;	//!< Width table for variable width font.
	const u8	*heights;	//!< Height table for variable height font.
	u16	charOffset;			//!< Character offset
	u16	charCount;			//!< Number of characters in font.
	u8	charW;				//!< Character width (fwf).
	u8	charH;				//!< Character height.
	u8	cellW;				//!< Glyph cell width.
	u8	cellH;				//!< Glyph cell height.
	u16	cellSize;			//!< Cell-size (bytes).
	u8	bpp;				//!< Font bitdepth;
	u8	extra;				//!< Padding. Free to use.	
} TFont;


//! TTE context struct.
typedef struct TTC
{
	// Members for renderers
	TSurface dst;			//!< Destination surface.
	s16	cursorX;			//!< Cursor X-coord.
	s16	cursorY;			//!< Cursor Y-coord.
	TFont *font;			//!< Current font.
	u8	*charLut;			//!< Character mapping lut. (if any).
	u16	cattr[4];			//!< ink, shadow, paper and special color attributes.
	// Higher-up members
	u16	flags0;
	u16	ctrl;				//!< BG control flags.	(PONDER: remove?)
	u16	marginLeft;
	u16	marginTop;
	u16	marginRight;
	u16 marginBottom;
	s16	savedX;
	s16	savedY;
	// Callbacks and table pointers
	fnDrawg	drawgProc;			//!< Glyph render procedure.
	fnErase	eraseProc;			//!< Text eraser procedure.
	const TFont	**fontTable;	//!< Pointer to font table for \{f}.
	const char	**stringTable;	//!< Pointer to string table for \{s}.
} TTC;


// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------


extern TTC __tte_main_context;

//! \name Internal fonts
//\{

// --- Main Font data ---
extern const TFont sys8Font;		//!< System font ' '-127. FWF  8x 8\@1.

extern const TFont verdana9Font;	//!< Verdana 9 ' '-'ÿ'. VWF  8x12\@1.
extern const TFont verdana9bFont;	//!< Verdana 9 bold ' '-'ÿ'. VWF  8x12\@1.
extern const TFont verdana9iFont;	//!< Verdana 9 italic ' '-'ÿ'. VWF  8x12\@1.

extern const TFont verdana10Font;	//!< Verdana 10 ' '-'ÿ'. VWF 16x14\@1.
		
extern const TFont verdana9_b4Font;	//!< Verdana 9 ' '-'ÿ'. VWF  8x12\@4.


// --- Extra font data ---

extern const unsigned int sys8Glyphs[192];

extern const unsigned int verdana9Glyphs[896];
extern const unsigned char verdana9Widths[224];

extern const unsigned int verdana9bGlyphs[896];
extern const unsigned char verdana9bWidths[224];

extern const unsigned int verdana9iGlyphs[896];
extern const unsigned char verdana9iWidths[224];

extern const unsigned int verdana10Glyphs[1792];
extern const unsigned char verdana10Widths[224];

extern const unsigned int verdana9_b4Glyphs[3584];
extern const unsigned char verdana9_b4Widths[224];

//\}

/*!	\} */	// grpTTE


// --------------------------------------------------------------------
// PROTOTYPES
// --------------------------------------------------------------------

// === Operations =====================================================

/*! \addtogroup grpTTEOps		*/
/*!	\{	*/

void tte_set_context(TTC *tc);
INLINE TTC	*tte_get_context();

INLINE uint	tte_get_glyph_id(int ch);
INLINE int	tte_get_glyph_width(uint gid);
INLINE int  tte_get_glyph_height(uint gid);
INLINE const void *tte_get_glyph_data(uint gid);


void tte_set_color(eint type, u16 clr);
void tte_set_colors(const u16 colors[]);

void tte_set_color_attr(eint type, u16 cattr);
void tte_set_color_attrs(const u16 cattrs[]);

int tte_putc(int ch);
int	tte_write(const char *text);
int	tte_write_ex(int x, int y, const char *text, const u16 *clrlut);


void tte_erase_rect(int left, int top, int right, int bottom);
void tte_erase_screen(void);
void tte_erase_line(void);

POINT16 tte_get_text_size(const char *str);

void tte_init_base(const TFont *font, fnDrawg drawProc, fnErase eraseProc);

/*! \}	*/	// grpTTEOps


// === Attributes functions ===========================================

/*! \addtogroup grpTTEAttr		*/
/*!	\{	*/

// --- getters ---

INLINE void tte_get_pos(int *x, int *y);
INLINE u16 tte_get_ink(void);
INLINE u16 tte_get_shadow(void);
INLINE u16 tte_get_paper(void);
INLINE u16 tte_get_special(void);

INLINE TSurface *tte_get_surface(void);
INLINE TFont *tte_get_font(void);
INLINE fnDrawg tte_get_drawg(void);
INLINE fnErase tte_get_erase(void);

INLINE char **tte_get_string_table(void);
INLINE TFont **tte_get_font_table(void);

// --- setters ---
INLINE void tte_set_pos(int x, int y);
INLINE void tte_set_ink(u16 cattr);
INLINE void tte_set_shadow(u16 cattr);
INLINE void tte_set_paper(u16 cattr);
INLINE void tte_set_special(u16 cattr);

INLINE void tte_set_surface(const TSurface *srf);
INLINE void tte_set_font(const TFont *font);
INLINE void tte_set_drawg(fnDrawg proc);
INLINE void tte_set_erase(fnErase proc);

INLINE void tte_set_string_table(const char *table[]);
INLINE void tte_set_font_table(const TFont *table[]);

void tte_set_margins(int left, int top, int right, int bottom);


/*! \}	*/	// grpTTEAttr


// === Console functions ==============================================

/*! \addtogroup grpTTEConio	*/
/*!	\{	*/

/*! Wrapper 'function' to hide that we're making iprintf do
	things it doesn't usually do.
*/
#define tte_printf iprintf

/*!	\}	*/


// === Render families ================================================

/*! \addtogroup grpTTEMap	*/
/*!	\{	*/

//! \name Regular tilemaps
//\{
void tte_init_se(int bgnr, u16 bgcnt, SCR_ENTRY se0, u32 clrs, u32 bupofs, 
	const TFont *font, fnDrawg proc);

void se_erase(int left, int top, int right, int bottom);

void se_drawg_w8h8(uint gid);
void se_drawg_w8h16(uint gid);
void se_drawg(uint gid);
void se_drawg_s(uint gid);
//\}

//! \name Affine tilemaps
//\{
void tte_init_ase(int bgnr, u16 bgcnt, u8 ase0, u32 clrs, u32 bupofs, 
	const TFont *font, fnDrawg proc);

void ase_erase(int left, int top, int right, int bottom);

void ase_drawg_w8h8(uint gid);
void ase_drawg_w8h16(uint gid);
void ase_drawg(uint gid);
void ase_drawg_s(uint gid);
//\}

/*!	\}	*/


/*! \addtogroup grpTTEChr4c	*/
/*!	\{	*/

//! \name 4bpp tiles
//\{
void tte_init_chr4c(int bgnr, u16 bgcnt, u16 se0, u32 cattrs, u32 clrs, 
	const TFont *font, fnDrawg proc);

void chr4c_erase(int left, int top, int right, int bottom);

void chr4c_drawg_b1cts(uint gid);
IWRAM_CODE void chr4c_drawg_b1cts_fast(uint gid);

void chr4c_drawg_b4cts(uint gid);
IWRAM_CODE void chr4c_drawg_b4cts_fast(uint gid);

//void chr4c_drawg_b4cos(uint gid);
//IWRAM_CODE int chr4c_drawg_co_fast(uint gid);
//\}

/*!	\}	*/


/*! \addtogroup grpTTEChr4r	*/
/*!	\{	*/

//! \name 4bpp tiles
//\{
void tte_init_chr4r(int bgnr, u16 bgcnt, u16 se0, u32 cattrs, u32 clrs, 
	const TFont *font, fnDrawg proc);

void chr4r_erase(int left, int top, int right, int bottom);

void chr4r_drawg_b1cts(uint gid);
IWRAM_CODE void chr4r_drawg_b1cts_fast(uint gid);

//\}

/*!	\}	*/


/*! \addtogroup grpTTEBmp	*/
/*!	\{	*/

void tte_init_bmp(int vmode, const TFont *font, fnDrawg proc);

//! \name 8bpp bitmaps
//\(
void bmp8_drawg(uint gid);
void bmp8_drawg_t(uint gid);

void bmp8_drawg_b1cts(uint gid);
IWRAM_CODE void bmp8_drawg_b1cts_fast(uint gid);
void bmp8_drawg_b1cos(uint gid);
//\}

//! \name 16bpp bitmaps
//\{
void bmp16_erase(int left, int top, int right, int bottom);

void bmp16_drawg(uint gid);
void bmp16_drawg_t(uint gid);

void bmp16_drawg_b1cts(uint gid);
void bmp16_drawg_b1cos(uint gid);
//\}

/*!	\}	*/


/*! \addtogroup grpTTEObj		*/
/*	\{	*/

void tte_init_obj(OBJ_ATTR *dst, u32 attr0, u32 attr1, u32 attr2, 
	u32 clrs, u32 bupofs, const TFont *font, fnDrawg proc);

void obj_erase(int left, int top, int right, int bottom);

void obj_drawg(uint gid);

/*!	\}	*/


// --------------------------------------------------------------------
// INLINES
// --------------------------------------------------------------------

//! Get the master text-system
INLINE TTC *tte_get_context(void)
{	return &__tte_main_context;							}


// --- Font-specific functions ---

//! Get the glyph index of character \a ch.
INLINE uint tte_get_glyph_id(int ch)
{	
	TTC *tc= tte_get_context();
	ch -= tc->font->charOffset;
	return tc->charLut ? tc->charLut[ch] : ch;
}

//! Get the glyph data of glyph \a id.
INLINE const void *tte_get_glyph_data(uint gid)
{
	TTC *tc= tte_get_context();
	TFont *font= tc->font;
	return ((u8*)font->data) + gid*font->cellSize;
}

//! Get the width of glyph \a id.
INLINE int tte_get_glyph_width(uint gid)
{
	TFont *font= tte_get_context()->font;
	return font->widths ? font->widths[gid] : font->charW;
}

//! Get the height of glyph \a id.
INLINE int tte_get_glyph_height(uint gid)
{
	TFont *font= tte_get_context()->font;
	return font->heights ? font->heights[gid] : font->charH;
}

// === Attributes ===



//! Set the text  surface.
INLINE void tte_set_surface(const TSurface *srf)
{	tte_get_context()->dst= *srf;					}

//! Get a pointer to the text surface.
INLINE TSurface *tte_get_surface()
{	return &tte_get_context()->dst;					}



//! Set cursor position
INLINE void tte_set_pos(int x, int y)
{	
	tte_get_context()->cursorX= x;
	tte_get_context()->cursorY= y;
}

//! Get cursor position
INLINE void tte_get_pos(int *x, int *y)
{
	if(x)	*x= tte_get_context()->cursorX;
	if(y)	*y= tte_get_context()->cursorY;	
}


//! Set the font
INLINE void tte_set_font(const TFont *font)
{	tte_get_context()->font= (TFont*)font;			}

//! Get the active font
INLINE TFont *tte_get_font(void)
{	return tte_get_context()->font;					}


//! Set ink color attribute.
INLINE void tte_set_ink(u16 cattr)
{	tte_get_context()->cattr[TTE_INK]= cattr;		}

//! Set shadow color attribute.
INLINE void tte_set_shadow(u16 cattr)
{	tte_get_context()->cattr[TTE_SHADOW]= cattr;	}

//! Set paper color attribute.
INLINE void tte_set_paper(u16 cattr)
{	tte_get_context()->cattr[TTE_PAPER]= cattr;		}

//! Set special color attribute.
INLINE void tte_set_special(u16 cattr)
{	tte_get_context()->cattr[TTE_SPECIAL]= cattr;	}


//! Get ink color attribute.
INLINE u16 tte_get_ink(void)
{	return tte_get_context()->cattr[TTE_INK];		}

//! Get shadow color attribute.
INLINE u16 tte_get_shadow(void)
{	return tte_get_context()->cattr[TTE_SHADOW];	}

//! Get paper color attribute.
INLINE u16 tte_get_paper(void)
{	return tte_get_context()->cattr[TTE_PAPER];		}

//! Get special color attribute.
INLINE u16 tte_get_special(void)
{	return tte_get_context()->cattr[TTE_SPECIAL];	}


//! Set the character plotter
INLINE void tte_set_drawg(fnDrawg proc)
{	tte_get_context()->drawgProc= proc;				}

//! Get the active character plotter
INLINE fnDrawg tte_get_drawg(void)
{	return tte_get_context()->drawgProc;			}


//! Set the character plotter
INLINE void tte_set_erase(fnErase proc)
{	tte_get_context()->eraseProc= proc;				}

//! Get the character plotter
INLINE fnErase tte_get_erase(void)
{	return tte_get_context()->eraseProc;			}


//! Set string table
INLINE void tte_set_string_table(const char *table[])
{	tte_get_context()->stringTable= table;			}

//! Get string table
INLINE char **tte_get_string_table(void)
{	return (char**)tte_get_context()->stringTable;	}


//! Set font table
INLINE void tte_set_font_table(const TFont *table[])
{	tte_get_context()->fontTable= table;			}

//! Get font table
INLINE TFont **tte_get_font_table(void)
{	return (TFont**)tte_get_context()->fontTable;	}


#endif // TONC_TTE


// EOF
