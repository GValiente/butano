#include "../include/btn_hw_sprite_tiles.h"

#include "btn_tile.h"
#include "btn_memory.h"

namespace btn::hw::sprite_tiles
{

void plot_tiles(int width, const tile& source_tiles_ref, int source_height, int source_y, int destination_y,
                tile& destination_tiles_ref)
{
    // From TONC:

    auto srcD = reinterpret_cast<const unsigned*>(&source_tiles_ref + source_y / 8);
    auto dstD = reinterpret_cast<unsigned*>(&destination_tiles_ref + destination_y / 8);
    int dstX0 = destination_y & 7;

    if(! dstX0)
    {
        // Simple cases : aligned pixels:

        for(int ix = 0; ix < width; ix += 8)
        {
            memory::copy(*srcD, 8, *dstD);
            srcD += source_height;
            dstD += 8;
        }
    }
    else
    {
        // Hideous cases : srcX0 != dstX0:

        auto lmask = [](int left) {
            return 0xFFFFFFFFU << (((left) & 7) * 4);
        };

        auto rmask = [](int right) {
            return 0xFFFFFFFFU >> ((-(right) & 7) * 4);
        };

        unsigned lsl = 4 * unsigned(dstX0);
        unsigned lsr = 32 - lsl;
        unsigned mask = lmask(0);

        for(int ix = width; ix > 0; ix -= 8)
        {
            if(ix < 8)
            {
                mask &= rmask(ix);
            }

            unsigned* dstL = dstD;
            dstD += 8;

            const unsigned* srcL = srcD;
            srcD += source_height;

            for(int iy = 0; iy < 8; ++iy)
            {
                unsigned px = *srcL++ & mask;

                // Write to left tile:
                dstL[0] = (dstL[0] &~ (mask << lsl)) | (px << lsl);

                // Write to right tile:
                dstL[8] = (dstL[8] &~ (mask >> lsr)) | (px >> lsr);

                ++dstL;
            }

            mask = 0xFFFFFFFF;
        }
    }
}

}
