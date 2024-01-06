/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DISPLAY_H
#define BN_HW_DISPLAY_H

#include "bn_point.h"
#include "bn_hw_bgs.h"

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

    inline void set_display(int mode, bool show_sprites, const bool* enabled_bgs,
                            const bool* enabled_inside_windows, uint16_t& display_cnt)
    {
        unsigned dispcnt = unsigned(mode) | DCNT_OBJ_1D;

        if(show_sprites)
        {
            dispcnt |= unsigned(DCNT_OBJ);
        }

        for(int index = 0; index < bgs::count(); ++index)
        {
            if(enabled_bgs[index])
            {
                dispcnt |= unsigned(DCNT_BG0 << index);
            }
        }

        for(int index = 0; index < inside_windows_count(); ++index)
        {
            if(enabled_inside_windows[index])
            {
                dispcnt |= unsigned(DCNT_WIN0 << index);
            }
        }

        display_cnt = uint16_t(dispcnt);
    }

    inline void commit_display(uint16_t display_cnt)
    {
        REG_DISPCNT_U16 = display_cnt;
    }

    inline void set_mosaic(int sprites_horizontal_stretch, int sprites_vertical_stretch,
                           int bgs_horizontal_stretch, int bgs_vertical_stretch, uint16_t& mosaic_cnt)
    {
        mosaic_cnt = uint16_t((sprites_vertical_stretch << 12) | (sprites_horizontal_stretch << 8) |
                              (bgs_vertical_stretch << 4) | bgs_horizontal_stretch);
    }

    inline void commit_mosaic(uint16_t mosaic_cnt)
    {
        REG_MOSAIC_U16 = mosaic_cnt;
    }

    [[nodiscard]] inline uint16_t* mosaic_register()
    {
        return &REG_MOSAIC_U16;
    }

    enum class blending_mode : uint8_t
    {
        OFF,
        TRANSPARENCY,
        FADE_TO_WHITE,
        FADE_TO_BLACK
    };

    [[nodiscard]] inline uint8_t blending_layer(const bool* bgs_ptr, bool sprites, bool backdrop)
    {
        uint8_t result = 0;

        for(int index = 0; index < bgs::count(); ++index)
        {
            if(bgs_ptr[index])
            {
                result |= uint8_t(1 << index);
            }
        }

        if(sprites)
        {
            result |= BLD_OBJ;
        }

        if(backdrop)
        {
            result |= BLD_BACKDROP;
        }

        return result;
    }

    inline void set_blending_cnt(int top, int bottom, blending_mode mode, uint16_t& blending_cnt)
    {
        blending_cnt = uint16_t((bottom << 8) | (int(mode) << 6) | top);
    }

    inline void commit_blending_cnt(uint16_t blending_cnt)
    {
        REG_BLDCNT = blending_cnt;
    }

    inline void set_blending_transparency(int top_weight, int bottom_weight, uint16_t& blending_transparency_cnt)
    {
        blending_transparency_cnt = uint16_t(top_weight | (bottom_weight << 8));
    }

    inline void commit_blending_transparency(uint16_t blending_transparency_cnt)
    {
        REG_BLDALPHA = blending_transparency_cnt;
    }

    [[nodiscard]] inline uint16_t* blending_transparency_register()
    {
        return const_cast<uint16_t*>(&REG_BLDALPHA);
    }

    inline void set_blending_fade(int fade_alpha, uint16_t& blending_fade_cnt)
    {
        blending_fade_cnt = uint16_t(fade_alpha);
    }

    inline void set_blending_fade(int fade_alpha)
    {
        set_blending_fade(fade_alpha, const_cast<uint16_t&>(REG_BLDY));
    }

    [[nodiscard]] inline uint16_t* blending_fade_register()
    {
        return const_cast<uint16_t*>(&REG_BLDY);
    }

    inline void set_windows_flags(const unsigned* flags_ptr)
    {
        REG_WININ = uint16_t((flags_ptr[1] << 8) | flags_ptr[0]);
        REG_WINOUT = uint16_t((flags_ptr[2] << 8) | flags_ptr[3]);
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
