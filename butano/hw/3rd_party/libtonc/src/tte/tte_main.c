
//
// Main TTE functionality
//
//! \file tte_main.c
//! \author J Vijn
//! \date 20070517 - 20080229
//
/* === NOTES ===
  * 20070718: On wrapping: wraps on character, not white-space. 
	Additionally, wrapping uses cell-width, not char-width, to keep 
	the rise in overhead manageable.

  * Timings:
		none	cellW	charW	// wrap method
se_old	  70
bm8_old	1672
dummy:	 116	 174	 244
se8x8	 317	 377	 467
chr4b1			2941			// -> 2040 with nopx-check & non-shifts
chr4b1_fast		 607
chr4b4			2828			// smallFont (->2297 with nopx)
chr4b4_fast		 683			// smallFont
bmp8	2266	2314	2386
bmp16	1596	1655	1726
	cellW-wrap adds 60. charW-wrap adds 130.

If __tte_main_context in IWRAM: 50 less overhead. Yay.

bmp8+sys8: 2251. Huh.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <tonc.h>

#include "tonc_tte.h"

#include "tonc_warn.h"


void dummy_drawg(uint gid);
void dummy_erase(int left, int top, int right, int bottom);

// --------------------------------------------------------------------
// CONSTANTS
// --------------------------------------------------------------------

// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------

EWRAM_DATA TTC	__tte_main_context;


// --------------------------------------------------------------------
// INLINES
// --------------------------------------------------------------------

INLINE char *eatwhite(const char *str)
{
	while(isspace(*str))
		str++;

	return (char*)str;
}

// --------------------------------------------------------------------
// OPERATIONS
// --------------------------------------------------------------------

void dummy_drawg(uint gid)
{
	//# TODO: assert?
}

void dummy_erase(int left, int top, int right, int bottom)
{
	//# TODO: assert?
}

//! Set the master context pointer.
void tte_set_context(TTC *tc)
{
    // gp_tte_context= tc ? tc : &__tte_main_context;
}

//! Set color attribute of \a type to \a cattr.
void tte_set_color_attr(eint type, u16 cattr)
{
	TTC *tc= tte_get_context();
	tc->cattr[type]= cattr;
}

//! Load important color attribute data.
void tte_set_color_attrs(const u16 cattrs[])
{
	int ii;
	TTC *tc= tte_get_context();

	for(ii=0; ii<4; ii++)
		tc->cattr[ii]= cattrs[ii];
}

//! Set color attribute of \a type to \a cattr.
void tte_set_color(eint type, u16 color)
{
	TTC *tc= tte_get_context();
	
	if(tc->dst.palData != NULL)
		tc->dst.palData[tc->cattr[type]]= color;
	else
		tc->cattr[type]= color;
}

//! Load important color data.
void tte_set_colors(const u16 colors[])
{
	int ii;
	TTC *tc= tte_get_context();

	if(tc->dst.palData != NULL)
		for(ii=0; ii<4; ii++)
			tc->dst.palData[tc->cattr[ii]]= colors[ii];
	else
		for(ii=0; ii<4; ii++)
			tc->cattr[ii]= colors[ii];
}


//! Base initializer of a TTC.
void tte_init_base(const TFont *font, fnDrawg drawProc, fnErase eraseProc)
{
	if(tte_get_context() == NULL)
		tte_set_context(&__tte_main_context);
	
	TTC *tc= tte_get_context();
	memset(tc, 0, sizeof(TTC));
		
    tc->font= (TFont*)(font);
	tc->drawgProc= drawProc ? drawProc : dummy_drawg;
	tc->eraseProc= eraseProc ? eraseProc : dummy_erase;

	// Default is SBB 0
	const TSurface srf= { (u8*)se_mem, 32*2, 32, 32, 8, SRF_BMP16, 256, pal_bg_mem };
	tc->dst = srf;

	tc->cattr[TTE_INK]    = 0xF1;
	tc->cattr[TTE_SHADOW] = 0xF2;
	tc->cattr[TTE_PAPER]  = 0;
	tc->cattr[TTE_SPECIAL]= 0;
	
	tc->marginRight= SCREEN_WIDTH;
	tc->marginBottom= SCREEN_HEIGHT;	
}


// --------------------------------------------------------------------
// String interpretations
// --------------------------------------------------------------------


//! Retrieve a single multibyte utf8 character.
uint utf8_decode_char(const char *ptr, char **endptr)
{
	uchar *src= (uchar*)ptr;
	uint ch8, ch32;

	// Poor man's try-catch.
	do
	{
		// UTF8 formats:
		// 0aaaaaaa                            ->                   0aaaaaaa
		// 110aaaaa 10bbbbbb                   ->          00000aaa aabbbbbb 
		// 1110aaaa 10bbbbbb 10cccccc          ->          aaaabbbb bbcccccc
		// 11110aaa 10bbbbbb 10cccccc 10dddddd -> 000aaabb bbbbcccc ccdddddd
		// 
		// Any invalid format will be returned as a single byte.

		ch8= *src;
		if(ch8 < 0x80)						// 7b
		{
			ch32= ch8;
		}
		else if(0xC0<=ch8 && ch8<0xE0)		// 11b
		{
			ch32  = (*src++&0x1F)<< 6;	if((*src>>6)!=2)	break;
			ch32 |= (*src++&0x3F)<< 0;
		}
		else if(0xE0<=ch8 && ch8<0xF0)		// 16b
		{
			ch32  = (*src++&0x0F)<<12;	if((*src>>6)!=2)	break;
			ch32 |= (*src++&0x3F)<< 6;	if((*src>>6)!=2)	break;
			ch32 |= (*src++&0x3F)<< 0;
		}
		else if(0xF0<=ch8 && ch8<0xF8)		// 21b
		{
			ch32  = (*src++&0x0F)<<18;	if((*src>>6)!=2)	break;
			ch32 |= (*src++&0x3F)<<12;	if((*src>>6)!=2)	break;
			ch32 |= (*src++&0x3F)<< 6;	if((*src>>6)!=2)	break;
			ch32 |= (*src++&0x3F)<< 0;
		}
		else
			break;

		// Proper UTF8 char: set endptr and return
		if(endptr)
			*endptr= (char*)src;

		return ch32;

	} while(0);


	// Not really UTF: interpret as single byte.
	src= (uchar*)ptr;
	ch32= *src++;
	if(endptr)
		*endptr= (char*)src;

	return ch32;
}

//! Extended string writer, with positional and color info
int tte_write_ex(int x0, int y0, const char *text, const u16 *cattrs)
{
	TTC *tc= tte_get_context();
	tc->cursorX= x0;
	tc->cursorY= y0;

	if(cattrs)
		tte_set_color_attrs(cattrs);

	return tte_write(text);
}

// Generic TTE putc (to be tested later)
// NOTE: the glyph inlines seem as fast as manual retrieval, 
//   even though the former get the TTC and font again as well. 
//	Yay.

//! Plot a single character; does wrapping too.
/*!
	\param ch	Character to plot (not glyph-id).
	\return		Character width.
	\note		Overhead: ~70 cycles.
*/
int tte_putc(int ch)
{
	TTC *tc= tte_get_context();
	TFont *font= tc->font;
	
	uint gid= tte_get_glyph_id(ch);
	int charW= tte_get_glyph_width(gid);
	
	if(tc->cursorX+charW > tc->marginRight)
	{
		tc->cursorY += font->charH;
		tc->cursorX  = tc->marginLeft;
	}

	// Draw and update position
	tc->drawgProc(gid);
	tc->cursorX += charW;

	return charW;
}


//! Render a string.
/*!
	\param text	String to parse and write.
	\return		Number of parsed characters.
*/
int	tte_write(const char *text)
{
	if(text == NULL)
		return 0;

	uint ch, gid;
	char *str= (char*)text;
	TTC *tc= tte_get_context();
	TFont *font;

	while( (ch=*str) != '\0' )
	{
		str++;
		switch(ch)
		{
		// --- Newline/carriage return ---
		case '\r':
			if(str[0] == '\n')	// deal with CRLF pair
				str++;
			// FALLTHRU
		case '\n':
			tc->cursorY += tc->font->charH;
			tc->cursorX  = tc->marginLeft;
			break;
		// --- Tab ---
		case '\t':
			tc->cursorX= (tc->cursorX/TTE_TAB_WIDTH+1)*TTE_TAB_WIDTH;
			break;

		// --- Normal char ---
                default:
			// Escaped command: skip '\\' and print '#'
                        if(ch=='\\' && str[0]=='#')
				ch= *str++;
			// Check for UTF8 code
			else if(ch>=0x80)
				ch= utf8_decode_char(str-1, &str);

			// Get glyph index and call renderer
			font= tc->font;
			gid= ch - font->charOffset;
			if(tc->charLut)
				gid= tc->charLut[gid];

			// Character wrap
			int charW= font->widths ? font->widths[gid] : font->charW;
			if(tc->cursorX+charW > tc->marginRight)
			{
				tc->cursorY += font->charH;
				tc->cursorX  = tc->marginLeft;
			}

			// Draw and update position
			tc->drawgProc(gid);
			tc->cursorX += charW;
		}
	}

	// Return characters used (PONDER: is this really the right thing?)
	return str - text;
}

//! Erase a porttion of the screen (ignores margins)
void tte_erase_rect(int left, int top, int right, int bottom)
{
	TTC *tc= tte_get_context();

	if(tc->eraseProc)
		tc->eraseProc(left, top, right, bottom);
}

//! Erase the screen (within the margins).
/*!
	\note Ponder: set paper color?
*/
void tte_erase_screen()
{
	TTC *tc= tte_get_context();

	if(tc->eraseProc)
		tc->eraseProc(tc->marginLeft, tc->marginTop, 
			tc->marginRight, tc->marginBottom);
}


//! Erase the whole line (within the margins).
/*!
	\note Ponder: set paper color?
*/
void tte_erase_line()
{
	TTC *tc= tte_get_context();
	int height= tc->font->charH;

	if(tc->eraseProc)
		tc->eraseProc(tc->marginLeft, tc->cursorY, 
			tc->marginRight, tc->cursorY+height);
}


//! Get the size taken up by a string.
/*!
	\param str	String to check.
	\return	width and height, packed into a POINT16.
	\note	This function \e ignores tte commands, so don't use
		on strings that use commands.
*/
POINT16 tte_get_text_size(const char *str)
{
	TTC *tc= tte_get_context();

	int charW, charH= tc->font->charH;

	int x=0, width= 12, height= charH;
	int ch;

	while( (ch= *str++) != 0 )
	{
		switch(ch)
		{
		// --- Newline/carriage return ---
		case '\r':
			if(str[0] == '\n')	// deal with CRLF pair
				str++;
			// FALLTHRU
		case '\n':
			height += charH;
			if(x > width)
				width= x;
			x= 0;
			break;			

		// --- Special char ---
                case '\\':
			break;

		// --- Normal char ---
		default:
			charW= tc->font->cellW;
			if(x+charW > tc->marginRight)
			{
				height += charH;		
				if(x>width)
					width= x;
				x=0;			
			}
			else
				x += tte_get_glyph_width(tte_get_glyph_id(ch));	
		}
	}

	// One more to make sure we got it >_<
	if(x>width)
		width= x;

	POINT16 pt= { width, height };
	return pt;
}

void tte_set_margins(int left, int top, int right, int bottom)
{
	TTC *tc= tte_get_context();

	tc->marginLeft  = left;
	tc->marginTop   = top;
	tc->marginRight = right;
	tc->marginBottom= bottom;
}


// EOF
