#ifndef BTN_HW_SPRITES_H
#define BTN_HW_SPRITES_H

#include "tonc.h"
#include "btn_size.h"
#include "btn_memory.h"
#include "btn_sprite_builder.h"

namespace btn::hw::sprites
{
    using handle = OBJ_ATTR;

    namespace
    {
        handle* vram()
        {
            return reinterpret_cast<handle*>(MEM_OAM);
        }
    }

    [[nodiscard]] inline int first_attributes(int y, sprite_shape shape, palette_bpp_mode bpp_mode, int affine_mode,
                                              bool mosaic_enabled, bool blending_enabled, bool window_enabled)
    {
        int result = ATTR0_BUILD(y, int(shape), 0, affine_mode, mosaic_enabled, blending_enabled, window_enabled);
        result |= unsigned(bpp_mode) * ATTR0_8BPP;
        return result;
    }

    [[nodiscard]] inline int second_attributes(int x, sprite_size size, bool horizontal_flip, bool vertical_flip)
    {
        return ATTR1_BUILDR(x, int(size), horizontal_flip, vertical_flip);
    }

    [[nodiscard]] inline int second_attributes(int x, sprite_size size, int affine_mat_id)
    {
        return ATTR1_BUILDA(x, int(size), affine_mat_id);
    }

    [[nodiscard]] inline int third_attributes(int tiles_id, int palette_id, int bg_priority)
    {
        return ATTR2_BUILD(tiles_id, palette_id, bg_priority);
    }

    inline void setup_regular(const sprite_shape_size& shape_size, int tiles_id, int palette_id,
                              palette_bpp_mode bpp_mode, handle& sprite)
    {
        sprite.attr0 = first_attributes(0, shape_size.shape(), bpp_mode, 0, false, false, false);
        sprite.attr1 = second_attributes(0, shape_size.size(), false, false);
        sprite.attr2 = third_attributes(tiles_id, palette_id, 3);
    }

    inline void setup_regular(const sprite_builder& builder, int tiles_id, int palette_id, palette_bpp_mode bpp_mode,
                              handle& sprite)
    {
        const sprite_shape_size& shape_size = builder.shape_size();
        sprite.attr0 = first_attributes(0, shape_size.shape(), bpp_mode, 0, builder.mosaic_enabled(),
                                             builder.blending_enabled(), builder.window_enabled());
        sprite.attr1 = second_attributes(0, shape_size.size(), builder.horizontal_flip(),
                                              builder.vertical_flip());
        sprite.attr2 = third_attributes(tiles_id, palette_id, builder.bg_priority());
    }

    inline void setup_affine(const sprite_builder& builder, int tiles_id, int palette_id, palette_bpp_mode bpp_mode,
                             handle& sprite)
    {
        const sprite_shape_size& shape_size = builder.shape_size();
        sprite.attr0 = first_attributes(0, shape_size.shape(), bpp_mode, 0, builder.mosaic_enabled(),
                                             builder.blending_enabled(), builder.window_enabled());
        sprite.attr1 = second_attributes(0, shape_size.size(), 0);
        sprite.attr2 = third_attributes(tiles_id, palette_id, builder.bg_priority());
    }

    [[nodiscard]] inline int affine_mode(const handle& sprite)
    {
        return BFN_GET(sprite.attr0, ATTR0_MODE);
    }

    [[nodiscard]] inline bool double_size(const handle& sprite)
    {
        return sprite.attr0 & ATTR0_AFF_DBL_BIT;
    }

    [[nodiscard]] inline sprite_shape shape(const handle& sprite)
    {
        return sprite_shape(sprite.attr0 >> 14);
    }

    [[nodiscard]] inline sprite_size size(const handle& sprite)
    {
        return sprite_size(sprite.attr1 >> 14);
    }

    [[nodiscard]] inline sprite_shape_size shape_size(const handle& sprite)
    {
        return sprite_shape_size(shape(sprite), size(sprite));
    }

    [[nodiscard]] inline btn::size dimensions(const handle& sprite)
    {
        const uint8_t* obj_size = obj_get_size(&sprite);
        btn::size result(obj_size[0], obj_size[1]);

        if(double_size(sprite))
        {
            result *= 2;
        }

        return result;
    }

    inline void set_shape_size(const sprite_shape_size& shape_size, handle& sprite)
    {
        BFN_SET(sprite.attr0, int(shape_size.shape()), ATTR0_SHAPE);
        BFN_SET(sprite.attr1, int(shape_size.size()), ATTR1_SIZE);
    }

    inline void set_tiles(int tiles_id, handle& sprite)
    {
        BFN_SET(sprite.attr2, tiles_id, ATTR2_ID);
    }

    inline void set_palette(int palette_id, handle& sprite)
    {
        BFN_SET(sprite.attr2, palette_id, ATTR2_PALBANK);
    }

    inline void set_bpp_mode(palette_bpp_mode bpp_mode, handle& sprite)
    {
        if(bpp_mode == palette_bpp_mode::BPP_8)
        {
            sprite.attr0 |= ATTR0_8BPP;
        }
        else
        {
            sprite.attr0 &= ~ATTR0_8BPP;
        }
    }

    inline void remove_affine_mat(handle& sprite)
    {
        sprite.attr0 &= ~ATTR0_AFF_DBL;
    }

    inline void set_affine_mat(int affine_mat_id, bool double_size, handle& sprite)
    {
        remove_affine_mat(sprite);
        sprite.attr0 |= double_size ? ATTR0_AFF_DBL : ATTR0_AFF;
        BFN_SET(sprite.attr1, affine_mat_id, ATTR1_AFF_ID);
    }

    inline void set_x(int x, handle& sprite)
    {
        BFN_SET(sprite.attr1, x, ATTR1_X);
    }

    inline void set_y(int y, handle& sprite)
    {
        BFN_SET(sprite.attr0, y, ATTR0_Y);
    }

    inline void set_bg_priority(int bg_priority, handle& sprite)
    {
        BFN_SET(sprite.attr2, bg_priority, ATTR2_PRIO);
    }

    [[nodiscard]] inline bool horizontal_flip(const handle& sprite)
    {
        return sprite.attr1 & ATTR1_HFLIP;
    }

    inline void set_horizontal_flip(bool horizontal_flip, handle& sprite)
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

    [[nodiscard]] inline bool vertical_flip(const handle& sprite)
    {
        return sprite.attr1 & ATTR1_VFLIP;
    }

    inline void set_vertical_flip(bool vertical_flip, handle& sprite)
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

    [[nodiscard]] inline bool mosaic_enabled(const handle& sprite)
    {
        return sprite.attr0 & ATTR0_MOSAIC;
    }

    inline void set_mosaic_enabled(bool mosaic_enabled, handle& sprite)
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

    [[nodiscard]] inline bool blending_enabled(const handle& sprite)
    {
        return sprite.attr0 & ATTR0_BLEND;
    }

    inline void set_blending_enabled(bool blending_enabled, handle& sprite)
    {
        if(blending_enabled)
        {
            sprite.attr0 |= ATTR0_BLEND;
        }
        else
        {
            sprite.attr0 &= ~ATTR0_BLEND;
        }
    }

    [[nodiscard]] inline bool window_enabled(const handle& sprite)
    {
        return sprite.attr0 & ATTR0_WINDOW;
    }

    inline void set_window_enabled(bool window_enabled, handle& sprite)
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

    inline void hide(handle& sprite)
    {
        sprite.attr0 = ATTR0_HIDE;
    }

    inline void commit(const handle& sprites_ref, int offset, int count)
    {
        memory::copy((&sprites_ref)[offset], count, vram()[offset]);
    }

    [[nodiscard]] inline uint16_t* first_attributes_register(int id)
    {
        handle& handle = vram()[id];
        return &handle.attr0;
    }

    [[nodiscard]] inline uint16_t* second_attributes_register(int id)
    {
        handle& handle = vram()[id];
        return &handle.attr1;
    }

    [[nodiscard]] inline uint16_t* third_attributes_register(int id)
    {
        handle& handle = vram()[id];
        return &handle.attr2;
    }
}

#endif
