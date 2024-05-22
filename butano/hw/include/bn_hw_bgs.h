/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_BGS_H
#define BN_HW_BGS_H

#include "bn_affine_bg_builder.h"
#include "bn_regular_bg_builder.h"
#include "bn_hw_bfn.h"
#include "bn_hw_dma.h"
#include "bn_hw_memory.h"

namespace bn::hw::bgs
{
    struct alignas(int) regular_offset
    {

    public:
        uint16_t x = 0;
        uint16_t y = 0;
    };

    struct affine_attributes : public BG_AFFINE
    {
        affine_attributes()
        {
            pa = 256;
            pb = 0;
            pc = 0;
            pd = 256;
            dx = 0;
            dy = 0;
        }
    };

    static_assert(sizeof(affine_attributes) == sizeof(BG_AFFINE));
    static_assert(alignof(affine_attributes) == alignof(BG_AFFINE));

    struct alignas(int) commit_data
    {
        uint16_t cnts[4] = { 0 };
        regular_offset regular_offsets[4];
        affine_attributes affine_attribute_sets[2];
    };
    
    [[nodiscard]] constexpr int count()
    {
        return 4;
    }

    [[nodiscard]] constexpr int affine_count()
    {
        return 2;
    }

    [[nodiscard]] constexpr int affine_precision()
    {
        return 8;
    }

    inline void setup_regular(const regular_bg_builder& builder, uint16_t& cnt)
    {
        cnt = uint16_t(BG_PRIO(builder.priority()) | (builder.mosaic_enabled() << 6));
    }

    inline void setup_affine(const affine_bg_builder& builder, uint16_t& cnt)
    {
        cnt = uint16_t(BG_PRIO(builder.priority()) | (builder.mosaic_enabled() << 6) |
                       (builder.wrapping_enabled() << 13) | BG_8BPP);
    }

    inline void set_tiles_cbb(int tiles_cbb, uint16_t& cnt)
    {
        BN_BFN_SET(cnt, tiles_cbb, BG_CBB);
    }

    inline void set_map_sbb(int map_sbb, uint16_t& cnt)
    {
        BN_BFN_SET(cnt, map_sbb, BG_SBB);
    }

    inline void set_map_dimensions(int size, uint16_t& cnt)
    {
        BN_BFN_SET(cnt, size, BG_SIZE);
    }

    [[nodiscard]] inline bpp_mode bpp(uint16_t cnt)
    {
        return (cnt & BG_8BPP) ? bpp_mode::BPP_8 : bpp_mode::BPP_4;
    }

    inline void set_bpp(bpp_mode bpp, uint16_t& cnt)
    {
        if(bpp == bpp_mode::BPP_8)
        {
            cnt |= BG_8BPP;
        }
        else
        {
            cnt &= ~BG_8BPP;
        }
    }

    inline void set_affine_mat_attributes(const affine_mat_attributes& mat_attributes, affine_attributes& attributes)
    {
        attributes.pa = int16_t(mat_attributes.pa_register_value());
        attributes.pb = int16_t(mat_attributes.pb_register_value());
        attributes.pc = int16_t(mat_attributes.pc_register_value());
        attributes.pd = int16_t(mat_attributes.pd_register_value());
    }

    inline void set_priority(int priority, uint16_t& cnt)
    {
        BN_BFN_SET(cnt, priority, BG_PRIO);
    }

    [[nodiscard]] inline bool wrapping_enabled(uint16_t cnt)
    {
        return cnt & BG_WRAP;
    }

    inline void set_wrapping_enabled(bool wrapping_enabled, uint16_t& cnt)
    {
        if(wrapping_enabled)
        {
            cnt |= BG_WRAP;
        }
        else
        {
            cnt &= ~BG_WRAP;
        }
    }

    [[nodiscard]] inline bool mosaic_enabled(uint16_t cnt)
    {
        return cnt & BG_MOSAIC;
    }

    inline void set_mosaic_enabled(bool mosaic_enabled, uint16_t& cnt)
    {
        if(mosaic_enabled)
        {
            cnt |= BG_MOSAIC;
        }
        else
        {
            cnt &= ~BG_MOSAIC;
        }
    }

    inline void commit(const commit_data& data, bool use_dma)
    {
        auto destination = reinterpret_cast<void*>(REG_BASE + 0x0008);

        if(use_dma)
        {
            dma::copy_words(&data, sizeof(commit_data) / 4, destination);
        }
        else
        {
            memory::copy_words(&data, sizeof(commit_data) / 4, destination);
        }
    }

    inline void stop()
    {
        REG_BG_AFFINE[2] = affine_attributes();
    }

    [[nodiscard]] inline uint16_t* regular_horizontal_position_register(int id)
    {
        return reinterpret_cast<uint16_t*>(REG_BASE + 0x0010 + (0x0004 * id));
    }

    [[nodiscard]] inline uint16_t* regular_vertical_position_register(int id)
    {
        return reinterpret_cast<uint16_t*>(REG_BASE + 0x0012 + (0x0004 * id));
    }

    [[nodiscard]] inline affine_attributes* affine_mat_register(int id)
    {
        BG_AFFINE& result = REG_BG_AFFINE[id];
        return static_cast<affine_attributes*>(&result);
    }

    [[nodiscard]] inline uint16_t* attributes_register(int id)
    {
        return reinterpret_cast<uint16_t*>(REG_BASE + 0x0008 + (0x0002 * id));
    }
}

#endif
