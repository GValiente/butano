//
//  Core stuff: keys, interrupts and misc
//
//! \file tonc_core.c
//! \author J Vijn
//! \date 20060508 - 20060508
//
// === NOTES ===

#include "tonc_memmap.h"
#include "tonc_core.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// CONSTANTS
// --------------------------------------------------------------------


const u8 oam_sizes[3][4][2]=
{
	{ { 8, 8}, {16,16}, {32,32}, {64,64} }, 
	{ {16, 8}, {32, 8}, {32,16}, {64,32} },
	{ { 8,16}, { 8,32}, {16,32}, {32,64} },
};

const BG_AFFINE bg_aff_default= { 256, 0, 0, 256, 0, 0 };

const u32 __snd_rates[12]=
{
	8013, 7566, 7144, 6742, // C , C#, D , D#
	6362, 6005, 5666, 5346, // E , F , F#, G
	5048, 4766, 4499, 4246  // G#, A , A#, B
};


// --------------------------------------------------------------------
// GLOBALS 
// --------------------------------------------------------------------


int __qran_seed= 42;
COLOR *vid_page= vid_mem_back;


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


// --- data -----------------------------------------------------------

//! VRAM-safe cpy.
/*!	This version mimics memcpy in functionality, with 
	the benefit of working for VRAM as well. It is also 
	slightly faster than the original memcpy, but faster 
	implementations can be made.
	\param dst	Destination pointer.
	\param src	Source pointer.
	\param size	Fill-length in bytes.
	\return		\a dst.
	\note	The pointers and size need not be word-aligned.
*/
void *tonccpy(void *dst, const void *src, uint size)
{
	if(size == 0 || dst==NULL || src==NULL)
		return dst;

	uint count;
	u16 *dst16;		// hword destination
	u8  *src8;		// byte source

	// Ideal case: copy by 4x words. Leaves tail for later.
	if( ((u32)src|(u32)dst)%4==0 && size>=4)
	{
		u32 *src32= (u32*)src, *dst32= (u32*)dst;

		count= size/4;
		uint tmp= count&3;
		count /= 4;

		// Duff, bitch!
		switch(tmp) {
			do {	*dst32++ = *src32++;
		case 3:		*dst32++ = *src32++;
		case 2:		*dst32++ = *src32++;
		case 1:		*dst32++ = *src32++;
		case 0:		; }	while(count--);
		}

		// Check for tail
		size &= 3;
		if(size == 0)
			return dst;

		src8= (u8*)src32;
		dst16= (u16*)dst32;
	}
	else		// Unaligned.
	{
		uint dstOfs= (u32)dst&1;
		src8= (u8*)src;
		dst16= (u16*)(dst-dstOfs);

		// Head: 1 byte.
		if(dstOfs != 0)
		{
			*dst16= (*dst16 & 0xFF) | *src8++<<8;
			dst16++;
			if(--size==0)
				return dst;
		}
	}

	// Unaligned main: copy by 2x byte.
	count= size/2;
	while(count--)
	{
		*dst16++ = src8[0] | src8[1]<<8;
		src8 += 2;
	}

	// Tail: 1 byte.
	if(size&1)
		*dst16= (*dst16 &~ 0xFF) | *src8;

	return dst;
}

//! VRAM-safe memset, internal routine.
/*!	This version mimics memset in functionality, with 
	the benefit of working for VRAM as well. It is also 
	slightly faster than the original memset.
	\param dst	Destination pointer.
	\param fill	Word to fill with.
	\param size	Fill-length in bytes.
	\return		\a dst.
	\note	The \a dst pointer and \a size need not be 
		word-aligned. In the case of unaligned fills, \a fill 
		will be masked off to match the situation.
*/
void *__toncset(void *dst, u32 fill, uint size)
{
	if(dst==NULL || size==0)
		return dst;

	uint left= (u32)dst&3;
	u32 *dst32= (u32*)(dst-left);
	u32 count, mask;

	// Unaligned head.
	if(left != 0)
	{
		// Adjust for very small stint.
		if(left+size<4)
		{
			mask= BIT_MASK(size*8)<<(left*8);	
			*dst32= (*dst32 &~ mask) | (fill & mask);
			return dst;
		}

		mask= BIT_MASK(left*8);
		*dst32= (*dst32 & mask) | (fill&~mask);
		dst32++;
		size -= 4-left;
	}

	// Main stint.
	count= size/4;
	uint tmp= count&3;
	count /= 4;

	switch(tmp) {
		do {	*dst32++ = fill;
	case 3:		*dst32++ = fill;
	case 2:		*dst32++ = fill;
	case 1:		*dst32++ = fill;
	case 0:		; }	while(count--);
	}

	// Tail
	size &= 3;
	if(size)
	{
		mask= BIT_MASK(size*8);
		*dst32= (*dst32 &~ mask) | (fill & mask);
	}

	return dst;
}


// --- random numbers -------------------------------------------------

int sqran(int seed)
{	
	int old= __qran_seed;
	__qran_seed= seed; 
	return old;	
}

// --- misc -----------------------------------------------------------

//! Get the octant that (\a x, \a y) is in.
/*!	This function divides the circle in 8 parts. The angle 
	starts at the \a y=0 line and then moves in the direction
	of the \a x=0 line. On the screen, this would be like
	starting at the 3 o'clock position and moving clockwise 
*/
u32 octant(int x, int y)
{
    u32 oct=0;
    if(y<0) { oct ^= 7; y=-y; }
    if(x<0) { oct ^= 3; x=-x; }
    if(x<y) { oct ^= 1; }
    return oct;
}

//! Get the rotated octant that (\a x, \a y) is in.
/*!	Like \c octant() but with a twist. The 0-octant starts 
	22.5° earlier so that 3 o'clock falls in the middle of 
	octant 0, instead of at its start. This can be useful for 
	8 directional pointing.
*/
u32 octant_rot(int x0, int y0)
{
	//rotate by pi/8: (.9: sin: 195.9; cos 473.0)
	int x= 473*x0 - 196*y0;
	int y= 196*x0 + 473*y0;

    u32 oct=0;
    if(y<0) { oct ^= 7; y=-y; }
    if(x<0) { oct ^= 3; x=-x; }
    if(x<y) { oct ^= 1; }
    return oct;
}

//	EOF
