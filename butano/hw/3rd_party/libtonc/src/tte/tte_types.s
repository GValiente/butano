@
@ Text types formatted for assembly
@
@! \file tte_types.s
@! \author J Vijn
@! \date 20070517 - 20070813
@
@ === NOTES ===

/*
typedef int (*fn_drawg)(int);

typedef struct TFont
{
	const u32	*data;		//!< Character data.
	const u8	*widths;	//!< Width table for variable width font.
	const u8	*heights;	//!< Height table for variable height font.
	u16 charOffset;			//!< Character offset.
	u16 charCount;			//!< Number of characters in font.
	u8	charW;				//!< Character width (fwf).
	u8	charH;				//!< Character height.
	u8	cellW;				//!< Glyph cell width.
	u8	cellH;				//!< Glyph cell height.
	u16	cellSize;			//!< Cell-size (bytes).
	u8	bpp;				//!< Font bitdepth;
	u8	extra;				//!< Padding. Free to use.	
} TFont;


//! Text context struct
typedef struct TTC
{
	// Members for renderers
	TSurface dst			//!< Destination surface
	u16	cursorX;			//!< Cursor X-coord.
	u16	cursorY;			//!< Cursor Y-coord.
	TFont *font;			//!< Font info.
	u8	*charLut;			//!< Character mapping lut. (if any)
	u16	colors[4];			//!< ink, shadow, paper and special colors.
	// Higher control members
	u16	flags0;
	u16	ctrl;				//!< BG control flags.	(PONDER: remove?)
	u16	marginLeft;
	u16	marginTop;
	u16	marginRight;
	u16 marginBottom;
	u16	savedX;
	u16	savedY;
	// Callbacks and table pointers
	fnCmd	cmdProc;		//!< Text command procedure
	fnDrawg	drawgProc;		//!< Character plot procedure.
	fnErase	eraseProc;		//!< Text eraser procedure
	const TFont	**fontTable;	//!< Pointer to font table for \{c}
	const char	**stringTable;	//!< Pointer to string table for \{s}
} TTC;

extern TVwf *gp_tte_sys;
*/

// --- Color lut indices ---
#define TTE_INK			0
#define TTE_SHADOW		1
#define TTE_PAPER		2
#define TTE_SPECIAL		3

// --- TFont ---
#define TF_data			 0
#define TF_widths		 4
#define TF_heights		 8
#define TF_charOffset	12
#define TF_charCount	14
#define TF_charW		16
#define TF_charH		17
#define TF_cellW		18
#define TF_cellH		19
#define TF_cellS		20
#define TF_bpp			22
#define TF_extra		23

// --- TTC ---
#define TTC_dstBase		 0
#define TTC_dstPitch	 4
#define TTC_dstWidth	 8
#define TTC_dstHeight	10
#define TTC_dstBpp		12
#define TTC_dstPalSize	14
#define TTC_dstPal		16

#define TTC_cursorX		20
#define TTC_cursorY		22
#define TTC_font		24
#define TTC_charLut		28
		
#define TTC_colors		32
#define TTC_ink			32
#define TTC_shadow		34
#define TTC_paper		36
#define TTC_special		38
		
#define TTC_flags0		40
#define TTC_ctrl		42
		
#define TTC_marginLeft		44
#define TTC_marginTop		46
#define TTC_marginRight		48
#define TTC_marginBottom	50
#define TTC_savedX		52
#define TTC_savedY		54
		
#define TTC_cmdProc		56
#define TTC_drawgProc	60
#define TTC_eraseProc	64
#define TTC_fontTable	68
#define TTC_stringTable	72
#define TTC_end			76

	.extern	gp_tte_sys

@ EOF
