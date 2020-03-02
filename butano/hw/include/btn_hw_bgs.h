#ifndef BTN_HW_BGS_H
#define BTN_HW_BGS_H

#include "tonc.h"
#include "btn_size.h"
#include "btn_memory.h"
#include "btn_regular_bg_builder.h"

namespace btn::hw::bgs
{
    class handle
    {

    public:
        uint16_t cnt;
        uint16_t hofs;
        uint16_t vofs;
    };

    [[nodiscard]] constexpr int count()
    {
        return 4;
    }

    [[nodiscard]] constexpr int max_priority()
    {
        return count() - 1;
    }

    [[nodiscard]] inline unsigned regular_control(int priority, int tiles_id, bool bpp_8, bool mosaic_enabled)
    {
        return unsigned(BG_PRIO(priority) | BG_CBB(tiles_id) | (bpp_8 << 7) | (mosaic_enabled << 6));
    }

    inline void setup_regular(const regular_bg_builder& builder, int tiles_id, palette_bpp_mode bpp_mode, handle& bg)
    {
        int bpp_8 = bpp_mode == palette_bpp_mode::BPP_8;
        bg.cnt = regular_control(builder.priority(), tiles_id, bpp_8, builder.mosaic_enabled());
    }

    inline void set_tiles(int tiles_id, handle& bg)
    {
        BFN_SET(bg.cnt, tiles_id, BG_CBB);
    }

    inline void set_map(int map_id, const size& map_dimensions, handle& bg)
    {
        BFN_SET(bg.cnt, map_id, BG_SBB);

        int size = (map_dimensions.width() > 32) + ((map_dimensions.height() > 32) * 2);
        BFN_SET(bg.cnt, size, BG_SIZE);
    }

    inline void set_position(int x, int y, handle& bg)
    {
        bg.hofs = uint16_t(x);
        bg.vofs = uint16_t(y);
    }

    [[nodiscard]] inline int priority(const handle& bg)
    {
        return BFN_GET(bg.cnt, BG_PRIO);
    }

    inline void set_priority(int priority, handle& bg)
    {
        BFN_SET(bg.cnt, priority, BG_PRIO);
    }

    [[nodiscard]] inline bool mosaic_enabled(const handle& bg)
    {
        return bg.cnt & BG_MOSAIC;
    }

    inline void set_mosaic_enabled(bool mosaic_enabled, handle& bg)
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

    inline void commit(const handle& bgs_ref)
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

    [[nodiscard]] inline uint16_t* regular_horizontal_position_register(int id)
    {
        return reinterpret_cast<uint16_t*>(REG_BASE + 0x0010 + (0x0004 * id));
    }

    [[nodiscard]] inline uint16_t* regular_vertical_position_register(int id)
    {
        return reinterpret_cast<uint16_t*>(REG_BASE + 0x0012 + (0x0004 * id));
    }

    [[nodiscard]] inline uint16_t* attributes_register(int id)
    {
        return reinterpret_cast<uint16_t*>(REG_BASE + 0x0008 + (0x0002 * id));
    }
}

#endif
