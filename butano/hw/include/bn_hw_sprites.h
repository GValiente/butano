/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_SPRITES_H
#define BN_HW_SPRITES_H

#include "bn_sprite_builder.h"
#include "bn_hw_bfn.h"
#include "bn_hw_dma.h"
#include "bn_hw_memory.h"

namespace bn::hw::sprites
{
    using handle_type = OBJ_ATTR;

    namespace
    {
        constexpr uint8_t _obj_sizes[3][4][2]=
        {
            { { 8, 8}, {16,16}, {32,32}, {64,64} },
            { {16, 8}, {32, 8}, {32,16}, {64,32} },
            { { 8,16}, { 8,32}, {16,32}, {32,64} },
        };
    }

    inline void copy_handle(const handle_type& from, handle_type& to)
    {
        to.attr0 = from.attr0;
        to.attr1 = from.attr1;
        to.attr2 = from.attr2;
    }

    namespace
    {
        [[nodiscard]] inline handle_type* vram()
        {
            return reinterpret_cast<handle_type*>(MEM_OAM);
        }
    }

    [[nodiscard]] inline int first_attributes(int y, sprite_shape shape, bpp_mode bpp, int view_mode,
                                              bool mosaic_enabled, bool blending_enabled, bool window_enabled,
                                              bool fade_enabled)
    {
        if(fade_enabled)
        {
            blending_enabled = ! blending_enabled;
        }

        return (y & 255) | view_mode | (blending_enabled << 10) | (window_enabled << 11) | (mosaic_enabled << 12) |
                (int(bpp) << 13) | (int(shape) << 14);
    }

    [[nodiscard]] inline int second_attributes(int x, sprite_size size, bool horizontal_flip, bool vertical_flip)
    {
        return (x & 511) | (horizontal_flip << 12) | (vertical_flip << 13) | (int(size) << 14);
    }

    [[nodiscard]] inline int second_attributes(int x, sprite_size size, int affine_mat_id)
    {
        return (x & 511) | (affine_mat_id << 9) | (int(size) << 14);
    }

    [[nodiscard]] inline int third_attributes(int tiles_id, int palette_id, int bg_priority)
    {
        return tiles_id | (palette_id << 12) | (bg_priority << 10);
    }

    inline void setup_regular(const sprite_shape_size& shape_size, int tiles_id, int palette_id, bpp_mode bpp,
                              bool fade_enabled, handle_type& sprite)
    {
        sprite.attr0 = uint16_t(first_attributes(0, shape_size.shape(), bpp, 0, false, false, false, fade_enabled));
        sprite.attr1 = uint16_t(second_attributes(0, shape_size.size(), false, false));
        sprite.attr2 = uint16_t(third_attributes(tiles_id, palette_id, 3));
    }

    inline void setup_regular(const sprite_builder& builder, int tiles_id, int palette_id, bpp_mode bpp,
                              bool fade_enabled, handle_type& sprite)
    {
        const sprite_shape_size& builder_shape_size = builder.shape_size();
        sprite.attr0 = uint16_t(first_attributes(0, builder_shape_size.shape(), bpp, 0, builder.mosaic_enabled(),
                                                 builder.blending_enabled(), builder.window_enabled(), fade_enabled));
        sprite.attr1 = uint16_t(second_attributes(0, builder_shape_size.size(), builder.horizontal_flip(),
                                                  builder.vertical_flip()));
        sprite.attr2 = uint16_t(third_attributes(tiles_id, palette_id, builder.bg_priority()));
    }

    inline void setup_regular(const sprite_builder& builder, int tiles_id, int palette_id, bpp_mode bpp,
                              bool horizontal_flip, bool vertical_flip, bool fade_enabled, handle_type& sprite)
    {
        const sprite_shape_size& builder_shape_size = builder.shape_size();
        sprite.attr0 = uint16_t(first_attributes(0, builder_shape_size.shape(), bpp, 0, builder.mosaic_enabled(),
                                                 builder.blending_enabled(), builder.window_enabled(), fade_enabled));
        sprite.attr1 = uint16_t(second_attributes(0, builder_shape_size.size(), horizontal_flip, vertical_flip));
        sprite.attr2 = uint16_t(third_attributes(tiles_id, palette_id, builder.bg_priority()));
    }

    inline void setup_affine(const sprite_builder& builder, int tiles_id, int palette_id, bpp_mode bpp,
                             bool fade_enabled, handle_type& sprite)
    {
        const sprite_shape_size& builder_shape_size = builder.shape_size();
        sprite.attr0 = uint16_t(first_attributes(0, builder_shape_size.shape(), bpp, 0, builder.mosaic_enabled(),
                                                 builder.blending_enabled(), builder.window_enabled(), fade_enabled));
        sprite.attr1 = uint16_t(second_attributes(0, builder_shape_size.size(), 0));
        sprite.attr2 = uint16_t(third_attributes(tiles_id, palette_id, builder.bg_priority()));
    }

    [[nodiscard]] inline int view_mode(const handle_type& sprite)
    {
        return BN_BFN_GET_SHIFTED(sprite.attr0, ATTR0_MODE);
    }

    inline void show_regular(handle_type& sprite)
    {
        BN_BFN_SET_SHIFTED(sprite.attr0, ATTR0_REG, ATTR0_MODE);
    }

    inline void show_affine(bool double_size, uint16_t& attr0)
    {
        int mode = ATTR0_AFF + (int(double_size) << 9);
        BN_BFN_SET_SHIFTED(attr0, mode, ATTR0_MODE);
    }

    inline void show_affine(bool double_size, handle_type& sprite)
    {
        show_affine(double_size, sprite.attr0);
    }

    inline void hide(uint16_t& attr0)
    {
        BN_BFN_SET_SHIFTED(attr0, ATTR0_HIDE, ATTR0_MODE);
    }

    inline void hide(handle_type& sprite)
    {
        hide(sprite.attr0);
    }

    inline void hide_and_destroy(uint16_t& attr0)
    {
        attr0 = ATTR0_HIDE;
    }

    inline void hide_and_destroy(handle_type& sprite)
    {
        hide(sprite.attr0);
    }

    [[nodiscard]] inline sprite_shape shape(const handle_type& sprite)
    {
        return sprite_shape(sprite.attr0 >> 14);
    }

    [[nodiscard]] inline sprite_size size(const handle_type& sprite)
    {
        return sprite_size(sprite.attr1 >> 14);
    }

    [[nodiscard]] inline sprite_shape_size shape_size(const handle_type& sprite)
    {
        return sprite_shape_size(shape(sprite), size(sprite));
    }

    [[nodiscard]] inline pair<int, int> base_dimensions(const handle_type& sprite)
    {
        const uint8_t* obj_size = _obj_sizes[sprite.attr0 >> 14][sprite.attr1 >> 14];
        return pair<int, int>(obj_size[0], obj_size[1]);
    }

    [[nodiscard]] inline pair<int, int> dimensions(const handle_type& sprite, bool double_size)
    {
        pair<int, int> result = base_dimensions(sprite);

        if(double_size)
        {
            result.first *= 2;
            result.second *= 2;
        }

        return result;
    }

    inline void set_shape_size(const sprite_shape_size& shape_size, handle_type& sprite)
    {
        BN_BFN_SET(sprite.attr0, int(shape_size.shape()), ATTR0_SHAPE);
        BN_BFN_SET(sprite.attr1, int(shape_size.size()), ATTR1_SIZE);
    }

    inline void set_tiles(int tiles_id, handle_type& sprite)
    {
        BN_BFN_SET(sprite.attr2, tiles_id, ATTR2_ID);
    }

    inline void set_palette(int palette_id, handle_type& sprite)
    {
        BN_BFN_SET(sprite.attr2, palette_id, ATTR2_PALBANK);
    }

    inline void set_bpp(bpp_mode bpp, handle_type& sprite)
    {
        if(bpp == bpp_mode::BPP_8)
        {
            sprite.attr0 |= ATTR0_8BPP;
        }
        else
        {
            sprite.attr0 &= ~ATTR0_8BPP;
        }
    }

    inline void set_affine_mat(int affine_mat_id, handle_type& sprite)
    {
        BN_BFN_SET(sprite.attr1, affine_mat_id, ATTR1_AFF_ID);
    }

    inline void set_x(int x, uint16_t& attr1)
    {
        BN_BFN_SET(attr1, x & 511, ATTR1_X);
    }

    inline void set_x(int x, handle_type& sprite)
    {
        set_x(x, sprite.attr1);
    }

    inline void set_y(int y, uint16_t& attr0)
    {
        BN_BFN_SET(attr0, y & 255, ATTR0_Y);
    }

    inline void set_y(int y, handle_type& sprite)
    {
        set_y(y, sprite.attr0);
    }

    inline void set_bg_priority(int bg_priority, handle_type& sprite)
    {
        BN_BFN_SET(sprite.attr2, bg_priority, ATTR2_PRIO);
    }

    [[nodiscard]] inline bool horizontal_flip(const handle_type& sprite)
    {
        return sprite.attr1 & ATTR1_HFLIP;
    }

    inline void set_horizontal_flip(bool horizontal_flip, handle_type& sprite)
    {
        if(horizontal_flip)
        {
            sprite.attr1 |= ATTR1_HFLIP;
        }
        else
        {
            sprite.attr1 &= ~ATTR1_HFLIP;
        }
    }

    [[nodiscard]] inline bool vertical_flip(const handle_type& sprite)
    {
        return sprite.attr1 & ATTR1_VFLIP;
    }

    inline void set_vertical_flip(bool vertical_flip, handle_type& sprite)
    {
        if(vertical_flip)
        {
            sprite.attr1 |= ATTR1_VFLIP;
        }
        else
        {
            sprite.attr1 &= ~ATTR1_VFLIP;
        }
    }

    [[nodiscard]] inline bool mosaic_enabled(const handle_type& sprite)
    {
        return sprite.attr0 & ATTR0_MOSAIC;
    }

    inline void set_mosaic_enabled(bool mosaic_enabled, handle_type& sprite)
    {
        if(mosaic_enabled)
        {
            sprite.attr0 |= ATTR0_MOSAIC;
        }
        else
        {
            sprite.attr0 &= ~ATTR0_MOSAIC;
        }
    }

    inline void set_blending_enabled(bool blending_enabled, bool fade_enabled, handle_type& sprite)
    {
        if(fade_enabled)
        {
            blending_enabled = ! blending_enabled;
        }

        if(blending_enabled)
        {
            sprite.attr0 |= ATTR0_BLEND;
        }
        else
        {
            sprite.attr0 &= ~ATTR0_BLEND;
        }
    }

    [[nodiscard]] inline bool window_enabled(const handle_type& sprite)
    {
        return sprite.attr0 & ATTR0_WINDOW;
    }

    inline void set_window_enabled(bool window_enabled, handle_type& sprite)
    {
        if(window_enabled)
        {
            sprite.attr0 |= ATTR0_WINDOW;
        }
        else
        {
            sprite.attr0 &= ~ATTR0_WINDOW;
        }
    }

    inline void commit(const handle_type& sprites_ref, int offset, int count, bool use_dma)
    {
        const void* source = (&sprites_ref) + offset;
        int words = count * int(sizeof(handle_type) / 4);
        void* destination = vram() + offset;

        if(use_dma)
        {
            hw::dma::copy_words(source, words, destination);
        }
        else
        {
            hw::memory::copy_words(source, words, destination);
        }
    }

    [[nodiscard]] inline uint16_t* first_attributes_register(int id)
    {
        handle_type& handle = vram()[id];
        return &handle.attr0;
    }

    [[nodiscard]] inline uint16_t* second_attributes_register(int id)
    {
        handle_type& handle = vram()[id];
        return &handle.attr1;
    }

    [[nodiscard]] inline uint16_t* third_attributes_register(int id)
    {
        handle_type& handle = vram()[id];
        return &handle.attr2;
    }
}

#endif
