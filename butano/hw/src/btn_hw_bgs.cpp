#include "../include/btn_hw_bgs.h"

#include "tonc.h"
#include "btn_size.h"
#include "btn_memory.h"
#include "btn_bg_builder.h"

namespace btn::hw::bgs
{

void setup(const bg_builder& builder, int tiles_id, int map_id, const size& map_dimensions,
           bool eight_bits_per_pixel, handle& bg)
{
    bg.cnt = uint16_t(BG_PRIO(builder.priority()) | BG_CBB(tiles_id));

    if(eight_bits_per_pixel)
    {
        bg.cnt |= BG_8BPP;
    }

    if(builder.mosaic_enabled())
    {
        bg.cnt |= BG_MOSAIC;
    }

    set_map(map_id, map_dimensions, bg);
}

void set_tiles(int tiles_id, handle& bg)
{
    BFN_SET(bg.cnt, tiles_id, BG_CBB);
}

void set_map(int map_id, const size& map_dimensions, handle& bg)
{
    BFN_SET(bg.cnt, map_id, BG_SBB);

    int size = (map_dimensions.width() > 32) + ((map_dimensions.height() > 32) * 2);
    BFN_SET(bg.cnt, size, BG_SIZE);
}

void set_position(int x, int y, handle& bg)
{
    bg.hofs = uint16_t(x);
    bg.vofs = uint16_t(y);
}

int priority(const handle& bg)
{
    return BFN_GET(bg.cnt, BG_PRIO);
}

void set_priority(int priority, handle& bg)
{
    BFN_SET(bg.cnt, priority, BG_PRIO);
}

bool mosaic_enabled(const handle& bg)
{
    return bg.cnt & BG_MOSAIC;
}

void set_mosaic_enabled(bool mosaic_enabled, handle& bg)
{
    if(mosaic_enabled)
    {
        bg.cnt |= BG_MOSAIC;
    }
    else
    {
        bg.cnt &= ~BG_MOSAIC;
    }
}

void commit(const handle& bgs_ref)
{
    const handle& bg0 = (&bgs_ref)[0];
    REG_BG0CNT = bg0.cnt;
    REG_BG0HOFS = bg0.hofs;
    REG_BG0VOFS = bg0.vofs;

    const handle& bg1 = (&bgs_ref)[1];
    REG_BG1CNT = bg1.cnt;
    REG_BG1HOFS = bg1.hofs;
    REG_BG1VOFS = bg1.vofs;

    const handle& bg2 = (&bgs_ref)[2];
    REG_BG2CNT = bg2.cnt;
    REG_BG2HOFS = bg2.hofs;
    REG_BG2VOFS = bg2.vofs;

    const handle& bg3 = (&bgs_ref)[3];
    REG_BG3CNT = bg3.cnt;
    REG_BG3HOFS = bg3.hofs;
    REG_BG3VOFS = bg3.vofs;
}

}
