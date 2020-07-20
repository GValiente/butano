//
//  Core functionality
//
//! \file tonc_core.h
//! \author J Vijn
//! \date 20060508 - 20080128
//
/* === NOTES ===
  * Contents: bits, random, dma, timer
  * 20080129,jv: added tonccpy/set routines.
*/


#ifndef TONC_CORE
#define TONC_CORE

#include "tonc_memmap.h"
#include "tonc_memdef.h"


// --------------------------------------------------------------------
// BITS and BITFIELDS
// --------------------------------------------------------------------


/*! \defgroup grpCoreBit	Bit(field) macros
	\ingroup grpCore
*/
/*!	\{	*/

//! \name Simple bit macros
//\{

//! Create value with bit \a n set
#define BIT(n)				( 1<<(n) )

//! Shift \a a by \a n
#define BIT_SHIFT(a, n)		( (a)<<(n) )

//! Create a bitmask \a len bits long
#define BIT_MASK(len)		( BIT(len)-1 )

//! Set the \a flag bits in \a word
#define BIT_SET(y, flag)	( y |=  (flag) )

//! Clear the \a flag bits in \a word
#define BIT_CLEAR(y, flag)	( y &= ~(flag) )

//! Flip the \a flag bits in \a word
#define BIT_FLIP(y, flag)	( y ^=  (flag) )

//! Test whether all the \a flag bits in \a word are set
#define BIT_EQ(y, flag)		( ((y)&(flag)) == (flag) )



//! Create a bitmask of length \a len starting at bit \a shift.
#define BF_MASK(shift, len)		( BIT_MASK(len)<<(shift) )


//! Retrieve a bitfield mask of length \a starting at bit \a shift from \a y.
#define _BF_GET(y, shift, len)	( ((y)>>(shift))&BIT_MASK(len) )

//! Prepare a bitmask for insertion or combining.
#define _BF_PREP(x, shift, len)	( ((x)&BIT_MASK(len))<<(shift) )

//! Insert a new bitfield value \a x into \a y.
#define _BF_SET(y, x, shift, len)	\
	( y= ((y) &~ BF_MASK(shift, len)) | _BF_PREP(x, shift, len) )

//\}


/*! \name some EVIL bit-field operations, >:)
*	These allow you to mimic bitfields with macros. Most of the 
*	bitfields in the registers have <i>foo</i>_SHIFT and 
*	<i>foo</i>_SHIFT macros indicating the mask and shift values 
*	of the bitfield named <i>foo</i> in a variable.
*	These macros let you prepare, get and set the bitfields.
*/
//\{

//! Prepare a named bit-field for for insterion or combination.
#define BFN_PREP(x, name)	( ((x)<<name##_SHIFT) & name##_MASK )

//! Get the value of a named bitfield from \a y. Equivalent to (var=) y.name
#define BFN_GET(y, name)	( ((y) & name##_MASK)>>name##_SHIFT )

//! Set a named bitfield in \a y to \a x. Equivalent to y.name= x.
#define BFN_SET(y, x, name)	(y = ((y)&~name##_MASK) | BFN_PREP(x,name) )

//! Compare a named bitfield to named literal \a x.
#define BFN_CMP(y, x, name)	( ((y)&name##_MASK) == (x) )


//! Massage \a x for use in bitfield \a name with pre-shifted \a x
#define BFN_PREP2(x, name)	( (x) & name##_MASK )

//! Get the value of bitfield \a name from \a y, but don't down-shift
#define BFN_GET2(y, name)	( (y) & name##_MASK )

//! Set bitfield \a name from \a y to \a x with pre-shifted \a x
#define BFN_SET2(y,x,name)	( y = ((y)&~name##_MASK) | BFN_PREP2(x,name) )

//\}

INLINE u32 bf_get(u32 y, uint shift, uint len);
INLINE u32 bf_merge(u32 y, u32 x, uint shift, uint len);
INLINE u32 bf_clamp(int x, uint len);

INLINE int bit_tribool(u32 x, uint plus, uint minus);
INLINE u32 ROR(u32 x, uint ror);


/*!	\}	*/


// --------------------------------------------------------------------
// DATA
// --------------------------------------------------------------------


/*! \defgroup grpData	Data routines
	\ingroup grpCore
*/
/*!	\{	*/

//! Get the number of elements in an array
#define countof(_array)	( sizeof(_array)/sizeof(_array[0]) )

//! Align \a x to the next multiple of \a width.
INLINE uint align(uint x, uint width);

//! \name Copying and filling routines
//\{

//! Simplified copier for GRIT-exported data.
#define GRIT_CPY(dst, name)	memcpy16(dst, name, name##Len/2)


// Base memcpy/set replacements.
void *tonccpy(void *dst, const void *src, uint size);

void *__toncset(void *dst, u32 fill, uint size);
INLINE void *toncset(void *dst, u8 src, uint count);
INLINE void *toncset16(void *dst, u16 src, uint count);
INLINE void *toncset32(void *dst, u32 src, uint count);


// Fast memcpy/set
void memset16(void *dst, u16 hw, uint hwcount);
void memcpy16(void *dst, const void* src, uint hwcount);

IWRAM_CODE void memset32(void *dst, u32 wd, uint wcount);
IWRAM_CODE void memcpy32(void *dst, const void* src, uint wcount);


//!	Fastfill for halfwords, analogous to memset()
/*!	Uses <code>memset32()</code> if \a hwcount>5
*	\param dst	Destination address.
*	\param hw	Source halfword (not address).
*	\param hwcount	Number of halfwords to fill.
*	\note	\a dst <b>must</b> be halfword aligned.
*	\note \a r0 returns as \a dst + \a hwcount*2.
*/
void memset16(void *dst, u16 hw, uint hwcount);

//!	\brief Copy for halfwords.
/*!	Uses <code>memcpy32()</code> if \a hwn>6 and 
	  \a src and \a dst are aligned equally.
	\param dst	Destination address.
	\param src	Source address.
	\param hwcount	 Number of halfwords to fill.
	\note \a dst and \a src <b>must</b> be halfword aligned.
	\note \a r0 and \a r1 return as 
	  \a dst + \a hwcount*2 and \a src + \a hwcount*2.
*/
void memcpy16(void *dst, const void* src, uint hwcount);


//!	Fast-fill by words, analogous to memset()
/*! Like CpuFastSet(), only without the requirement of 
	  32byte chunks and no awkward store-value-in-memory-first issue.
	\param dst	Destination address.
	\param wd	Fill word (not address).
	\param wdcount	Number of words to fill.
	\note	\a dst <b>must</b> be word aligned.
	\note \a r0 returns as \a dst + \a wdcount*4.
*/
IWRAM_CODE void memset32(void *dst, u32 wd, uint wdcount);


//!	\brief Fast-copy by words.
/*! Like CpuFastFill(), only without the requirement of 32byte chunks
	\param dst	Destination address.
	\param src	Source address.
	\param wdcount	Number of words.
	\note	\a src and \a dst <b>must</b> be word aligned.
	\note	\a r0 and \a r1 return as 
	  \a dst + \a wdcount*4 and \a src + \a wdcount*4.
*/
IWRAM_CODE void memcpy32(void *dst, const void* src, uint wdcount);

//\}


/*! \name Repeated-value creators
	These function take a hex-value and duplicate it to all fields, 
	like 0x88 -> 0x88888888.
*/
//\{
INLINE u16 dup8(u8 x);
INLINE u32 dup16(u16 x);
INLINE u32 quad8(u8 x);
INLINE u32 octup(u8 x);
//\}

//!	\name Packing routines.
//\{
INLINE u16 bytes2hword(u8 b0, u8 b1);
INLINE u32 bytes2word(u8 b0, u8 b1, u8 b2, u8 b3);
INLINE u32 hword2word(u16 h0, u16 h1);
//\}

/*!	\}	*/


// --------------------------------------------------------------------
// DMA
// --------------------------------------------------------------------


/*!	\addtogroup grpDma	*/
/*!	\{	*/

//! General purpose DMA transfer macro
/*!	\param _dst	Destination address.
	\param _src	Source address.
	\param count	Number of transfers.
	\param ch	DMA channel.
	\param mode	DMA mode.
*/
#define DMA_TRANSFER(_dst, _src, count, ch, mode)	\
do {										\
	REG_DMA[ch].cnt= 0;						\
	REG_DMA[ch].src= (const void*)(_src);	\
	REG_DMA[ch].dst= (void*)(_dst);			\
	REG_DMA[ch].cnt= (count) | (mode);		\
} while(0)


INLINE void dma_cpy(void *dst, const void *src, uint count, uint ch, u32 mode);
INLINE void dma_fill(void *dst, volatile u32 src, uint count, uint ch, u32 mode);

INLINE void dma3_cpy(void *dst, const void *src, uint size);
INLINE void dma3_fill(void *dst, volatile u32 src, uint size);

/*! \}	*/


// --------------------------------------------------------------------
// TIMER
// --------------------------------------------------------------------


INLINE void profile_start(void);
INLINE uint profile_stop(void);


// --------------------------------------------------------------------
// TONE GENERATOR
// --------------------------------------------------------------------


typedef enum 
{	
	NOTE_C=0, NOTE_CIS, NOTE_D,   NOTE_DIS, 
	NOTE_E,   NOTE_F,   NOTE_FIS, NOTE_G, 
	NOTE_GIS, NOTE_A,   NOTE_BES, NOTE_B
} eSndNoteId;

extern const uint __snd_rates[12];

//! Gives the period of a note for the tone-gen registers.
/*! GBA sound range: 8 octaves: [-2, 5]; 8*12= 96 notes (kinda).
*	\param note	ID (range: [0,11>). See eSndNoteId.
*	\param oct	octave (range [-2,4)>).
*/
#define SND_RATE(note, oct) ( 2048-(__snd_rates[note]>>(4+(oct))) )


// --------------------------------------------------------------------
// MISC
// --------------------------------------------------------------------


/*! \defgroup grpCoreMisc	Miscellaneous routines	
*	\ingroup grpCore
*/
/*!	\{	*/

#define STR(x)		#x

//! Create text string from a literal
#define XSTR(x)		STR(x)


//! \name Inline assembly
//\{

//! Assembly comment
#define ASM_CMT(str) asm volatile("@# " str)

//! No$gba breakpoint
#define ASM_BREAK()	asm volatile("\tmov\t\tr11, r11")

//! No-op; wait a bit.
#define ASM_NOP()	asm volatile("\tnop")

//\}


//! \name Sector checking
//\{

u32 octant(int x, int y);
u32 octant_rot(int x0, int y0);

//\}

//! \name Random numbers
//\{

#define QRAN_SHIFT 15
#define QRAN_MASK  ((1<<QRAN_SHIFT)-1)
#define QRAN_MAX   QRAN_MASK

int sqran(int seed);
INLINE int qran(void);
INLINE int qran_range(int min, int max);

//\}

/*!	\}	*/


// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------


extern const u8 oam_sizes[3][4][2];
extern const BG_AFFINE bg_aff_default;
extern COLOR *vid_page;

extern int __qran_seed;


// --------------------------------------------------------------------
// INLINES
// --------------------------------------------------------------------


// --- Bit and bitfields -----------------------------------------------


//! Get \a len long bitfield from \a y, starting at \a shift.
/*!	\param y	Value containing bitfield.
	\param shift	Bitfield Start;
	\param len	Length of bitfield.
	\return Bitfield between bits \a shift and \a shift + \a length.
*/
INLINE u32 bf_get(u32 y, uint shift, uint len)
{	return (y>>shift) & ( (1<<len)-1 );		}

//! Merge \a x into an \a len long bitfield from \a y, starting at \a shift.
/*!	\param y	Value containing bitfield.
	\param x	Value to merge (will be masked to fit).
	\param shift	Bitfield Start;
	\param len	Length of bitfield.
	\return	Result of merger: (y&~M) | (x<<s & M)
	\note	Does \e not write the result back into \a y (Because pure C 
		does't have references, that's why)
*/
INLINE u32 bf_merge(u32 y, u32 x, uint shift, uint len)
{
	u32 mask= ((u32)(1<<len)-1);
	return (y &~ (mask<<shift)) | (x & mask)<<shift;
}

//! Clamp \a to within the range allowed by \a len bits
INLINE u32 bf_clamp(int x, uint len)
{
	u32 y=x>>len;
	if(y)
		x= (~y)>>(32-len);
	return x;
}


//! Gives a tribool (-1, 0, or +1) depending on the state of some bits.
/*! Looks at the \a plus and \a minus bits of \a flags, and subtracts 
	  their status to give a +1, -1 or 0 result. Useful for direction flags.
	\param flags	Value with bit-flags.
	\param plus		Bit number for positive result.
	\param minus	Bit number for negative result.
	\return	<b>+1</b> if \a plus bit is set but \a minus bit isn't<br>
	  <b>-1</b> if \a minus bit is set and \a plus bit isn't<br>
	  <b>0</b> if neither or both are set.
*/
INLINE int bit_tribool(u32 flags, uint plus, uint minus)
{	return ((flags>>plus)&1) - ((flags>>minus)&1);	}


//! Rotate bits right. Yes, this does lead to a ror instruction.
INLINE u32 ROR(u32 x, uint ror)	
{	return (x<<(32-ror)) | (x>>ror);				}


// --- Data -----------------------------------------------------------

INLINE uint align(uint x, uint width)
{	return (x+width-1)/width*width;					}


//! VRAM-safe memset, byte  version. Size in bytes.
INLINE void *toncset(void *dst, u8 src, uint count)
{	return __toncset(dst, quad8(src), count);		}

//! VRAM-safe memset, halfword version. Size in hwords.
INLINE void *toncset16(void *dst, u16 src, uint count)
{	return __toncset(dst, src|src<<16, count*2);	}

//! VRAM-safe memset, word version. Size in words.
INLINE void *toncset32(void *dst, u32 src, uint count)
{	return __toncset(dst, src, count*4);			}



//! Duplicate a byte to form a halfword: 0x12 -> 0x1212.
INLINE u16 dup8(u8 x)	{	return x|(x<<8);		}

//! Duplicate a halfword to form a word: 0x1234 -> 0x12341234.
INLINE u32 dup16(u16 x)	{	return x|(x<<16);		}

//! Quadruple a byte to form a word: 0x12 -> 0x12121212.
INLINE u32 quad8(u8 x)	{	return x*0x01010101;	}

//! Octuple a nybble to form a word: 0x1 -> 0x11111111
INLINE u32 octup(u8 x)	{	return x*0x11111111;	}


//! Pack 2 bytes into a word. Little-endian order.
INLINE u16 bytes2hword(u8 b0, u8 b1)
{	return b0 | b1<<8;								}

//! Pack 4 bytes into a word. Little-endian order.
INLINE u32 bytes2word(u8 b0, u8 b1, u8 b2, u8 b3)
{	return b0 | b1<<8 | b2<<16 | b3<<24;			}

INLINE u32 hword2word(u16 h0, u16 h1)
{	return h0 | h1<<16;								}


// --- DMA ------------------------------------------------------------

/*!	\addtogroup grpDma	*/
/*!	\{	*/

//! Generic DMA copy routine.
/*!	\param dst	Destination address.
*	\param src	Source address.
*	\param count	Number of copies to perform.
*	\param ch	DMA channel.
*	\param mode	DMA transfer mode.
*	\note	\a count is the number of copies, not the size in bytes.
*/
INLINE void dma_cpy(void *dst, const void *src, uint count, uint ch, u32 mode)
{
	REG_DMA[ch].cnt= 0;
	REG_DMA[ch].src= src;
	REG_DMA[ch].dst= dst;
	REG_DMA[ch].cnt= mode | count;
}

//! Generic DMA fill routine.
/*!	\param dst	Destination address.
*	\param src	Source value.
*	\param count	Number of copies to perform.
*	\param ch	DMA channel.
*	\param mode	DMA transfer mode.
*	\note	\a count is the number of copies, not the size in bytes.
*/
INLINE void dma_fill(void *dst, volatile u32 src, uint count, uint ch, u32 mode)
{
	REG_DMA[ch].cnt= 0;
	REG_DMA[ch].src= (const void*)&src;
	REG_DMA[ch].dst= dst;
	REG_DMA[ch].cnt= count | mode | DMA_SRC_FIXED;
}

//! Specific DMA copier, using channel 3, word transfers.
/*!	\param dst	Destination address.
*	\param src	Source address.
*	\param size	Number of bytes to copy
*	\note	\a size is the number of bytes
*/
INLINE void dma3_cpy(void *dst, const void *src, uint size)
{	dma_cpy(dst, src, size/4, 3, DMA_CPY32);	}

//! Specific DMA filler, using channel 3, word transfers.
/*!	\param dst	Destination address.
*	\param src	Source value.
*	\param size	Number of bytes to copy
*	\note	\a size is the number of bytes
*/
INLINE void dma3_fill(void *dst, volatile u32 src, uint size)
{	dma_fill(dst, src, size/4, 3, DMA_FILL32);	}

/*! \}	*/


// --- Random ---------------------------------------------------------

//! Quick (and very dirty) pseudo-random number generator 
/*! \return random in range [0,8000h>
*/
INLINE int qran(void)
{	
	__qran_seed= 1664525*__qran_seed+1013904223;
	return (__qran_seed>>16) & QRAN_MAX;
}


//! Ranged random number
/*! \return random in range [\a min, \a max>
*	\note (max-min) must be lower than 8000h
*/
INLINE int qran_range(int min, int max)	
{	return (qran()*(max-min)>>QRAN_SHIFT)+min;		}


// --- Timer ----------------------------------------------------------

/*!	\addtogroup grpTimer	*/
/*!	\{	*/

//! Start a profiling run
/*!	\note Routine uses timers 3 and 3; if you're already using these
*	  somewhere, chaos is going to ensue.
*/
INLINE void profile_start(void)
{
	REG_TM2D= 0;	REG_TM3D= 0;
    REG_TM2CNT= 0;	REG_TM3CNT= 0;
    REG_TM3CNT= TM_ENABLE | TM_CASCADE;
    REG_TM2CNT= TM_ENABLE;
}

//! Stop a profiling run and return the time since its start.
/*!	\return 32bit cycle count
*/
INLINE uint profile_stop(void)
{
   REG_TM2CNT= 0;
   return (REG_TM3D<<16)|REG_TM2D;
}

/*!	\}	/addtogroup	*/


#endif // TONC_CORE

