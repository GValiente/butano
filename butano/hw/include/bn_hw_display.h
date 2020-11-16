/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DISPLAY_H
#define BN_HW_DISPLAY_H

#include "bn_point.h"
#include "bn_hw_tonc.h"

#define REG_DISPCNT_U16     *(u16*)(REG_BASE+0x0000)
#define REG_DISPCNT_U16_2   *(u16*)(REG_BASE+0x0002)
#define REG_MOSAIC_U16		*(u16*)(REG_BASE+0x004C)

namespace bn::hw::display
{
    enum class window_flag
    {
        BG_0 = 0x0001,
        BG_1 = 0x0002,
        BG_2 = 0x0004,
        BG_3 = 0x0008,
        SPRITES = 0x0010,
        ALL = 0x001F,
        BLENDING = 0x0020
    };

    [[nodiscard]] constexpr int windows_count()
    {
        return 4;
    }

    [[nodiscard]] constexpr int inside_windows_count()
    {
        return 3;
    }

    [[nodiscard]] constexpr int rect_windows_count()
    {
        return 2;
    }

    inline void init()
    {
        REG_DISPCNT_U16 = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D;
    }

    inline void set_bg_enabled(int bg, bool enabled)
    {
        if(enabled)
        {
            REG_DISPCNT_U16 |= unsigned(DCNT_BG0 << bg);
        }
        else
        {
            REG_DISPCNT_U16 &= ~unsigned(DCNT_BG0 << bg);
        }
    }

    inline void set_mosaic(int sprites_horizontal_stretch, int sprites_vertical_stretch,
                           int bgs_horizontal_stretch, int bgs_vertical_stretch, uint16_t& mosaic_cnt)
    {
        mosaic_cnt = MOS_BUILD(unsigned(bgs_horizontal_stretch), unsigned(bgs_vertical_stretch),
                               unsigned(sprites_horizontal_stretch), unsigned(sprites_vertical_stretch));
    }

    inline void set_mosaic(int sprites_horizontal_stretch, int sprites_vertical_stretch,
                           int bgs_horizontal_stretch, int bgs_vertical_stretch)
    {
        set_mosaic(sprites_horizontal_stretch, sprites_vertical_stretch, bgs_horizontal_stretch, bgs_vertical_stretch,
                   REG_MOSAIC_U16);
    }

    [[nodiscard]] inline uint16_t* mosaic_register()
    {
        return &REG_MOSAIC_U16;
    }

    enum class blending_mode
    {
        OFF,
        TRANSPARENCY,
        FADE_TO_WHITE,
        FADE_TO_BLACK
    };

    [[nodiscard]] inline int blending_layers(const bool* bgs_ptr, int count, bool fade)
    {
        unsigned result = fade ? BLD_OBJ : 0;

        for(int index = 0; index < count; ++index)
        {
            if(bgs_ptr[index])
            {
                result |= unsigned(1 << index);
            }
        }

        return int(result);
    }

    inline void set_blending_cnt(int layers, blending_mode mode)
    {
        REG_BLDCNT = uint16_t(BLD_BUILD(layers, BLD_ALL | BLD_BACKDROP, unsigned(mode)));
    }

    inline void set_blending_transparency(int transparency_alpha, int intensity_alpha,
                                          uint16_t& blending_transparency_cnt)
    {
        blending_transparency_cnt =
                uint16_t(BLDA_BUILD(transparency_alpha, max(16 - transparency_alpha, intensity_alpha)));
    }

    inline void set_blending_transparency(int transparency_alpha, int intensity_alpha)
    {
        set_blending_transparency(transparency_alpha, intensity_alpha, const_cast<uint16_t&>(REG_BLDALPHA));
    }

    [[nodiscard]] inline uint16_t* blending_transparency_register()
    {
        return const_cast<uint16_t*>(&REG_BLDALPHA);
    }

    inline void set_blending_fade(int fade_alpha, uint16_t& blending_fade_cnt)
    {
        blending_fade_cnt = BLDY_BUILD(fade_alpha);
    }

    inline void set_blending_fade(int fade_alpha)
    {
        set_blending_fade(fade_alpha, const_cast<uint16_t&>(REG_BLDY));
    }

    [[nodiscard]] inline uint16_t* blending_fade_register()
    {
        return const_cast<uint16_t*>(&REG_BLDY);
    }

    inline void set_inside_window_enabled(int window, bool enabled)
    {
        if(enabled)
        {
            REG_DISPCNT_U16 |= unsigned(DCNT_WIN0 << window);
        }
        else
        {
            REG_DISPCNT_U16 &= ~unsigned(DCNT_WIN0 << window);
        }
    }

    inline void set_windows_flags(const unsigned* flags_ptr)
    {
        REG_WININ = uint16_t(WIN_BUILD(flags_ptr[0], flags_ptr[1]));
        REG_WINOUT = uint16_t(WIN_BUILD(flags_ptr[3], flags_ptr[2]));
    }

    inline void set_window_boundaries(int first, int second, uint16_t& window_cnt)
    {
        window_cnt = uint16_t((first << 8) + second);
    }

    inline void set_windows_boundaries(const point* boundaries_ptr)
    {
        REG_WIN0H = uint16_t((boundaries_ptr[0].x() << 8) + boundaries_ptr[1].x());
        REG_WIN0V = uint16_t((boundaries_ptr[0].y() << 8) + boundaries_ptr[1].y());
        REG_WIN1H = uint16_t((boundaries_ptr[2].x() << 8) + boundaries_ptr[3].x());
        REG_WIN1V = uint16_t((boundaries_ptr[2].y() << 8) + boundaries_ptr[3].y());
    }

    [[nodiscard]] inline uint16_t* window_horizontal_boundaries_register(int id)
    {
        return reinterpret_cast<uint16_t*>(REG_BASE + 0x0040 + (0x0002 * id));
    }

    [[nodiscard]] inline uint16_t* window_vertical_boundaries_register(int id)
    {
        return reinterpret_cast<uint16_t*>(REG_BASE + 0x0044 + (0x0002 * id));
    }

    inline void set_green_swap_enabled(bool enabled, uint16_t& green_swap_cnt)
    {
        if(enabled)
        {
            green_swap_cnt |= 0x0001;
        }
        else
        {
            green_swap_cnt &= unsigned(~0x0001);
        }
    }

    inline void set_green_swap_enabled(bool enabled)
    {
        set_green_swap_enabled(enabled, REG_DISPCNT_U16_2);
    }

    [[nodiscard]] inline uint16_t* green_swap_register()
    {
        return &REG_DISPCNT_U16_2;
    }

    inline void sleep()
    {
        REG_DISPCNT_U16 |= DCNT_BLANK;
    }

    inline void wake_up()
    {
        REG_DISPCNT_U16 &= unsigned(~DCNT_BLANK);
    }

    inline void stop()
    {
        REG_BLDCNT = 0;
        REG_MOSAIC_U16 = 0;
    }

    inline void set_show_mode()
    {
        stop();
        REG_DISPCNT_U16 = DCNT_MODE3 | DCNT_BG2;
    }
}

#endif
