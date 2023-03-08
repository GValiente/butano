/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_memory.h"

namespace bn::hw::memory
{

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"

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
    \file tonc_core.c
    \author J Vijn
    \date 20060508 - 20060508
*/
void _iwram_toncset(void *dst, u32 fill, uint size)
{
	uint left= (u32)dst&3;
	u32 *dst32= (u32*)(dst-left);
    u32 mask;

    // Fix fill.
    fill = quad8(fill);

	// Unaligned head.
	if(left != 0)
	{
        if(size==0)
        {
            return;
        }

		// Adjust for very small stint.
		if(left+size<4)
		{
			mask= BIT_MASK(size*8)<<(left*8);	
			*dst32= (*dst32 &~ mask) | (fill & mask);
            return;
		}

		mask= BIT_MASK(left*8);
		*dst32= (*dst32 & mask) | (fill&~mask);
		dst32++;
		size -= 4-left;
	}

    // Main stint.
    set_words(fill, size / 4, dst32);
    dst32 += size / 4;

	// Tail
	size &= 3;
	if(size)
	{
		mask= BIT_MASK(size*8);
		*dst32= (*dst32 &~ mask) | (fill & mask);
    }
}

#pragma GCC diagnostic pop

}
