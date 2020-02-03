#ifndef BTN_HW_SPRITES_H
#define BTN_HW_SPRITES_H

#include "tonc.h"
#include "btn_size.h"
#include "btn_memory.h"
#include "btn_sprite_builder.h"

namespace btn::hw::sprites
{
    class alignas(sizeof(int)) handle
    {

    public:
        uint16_t attr0;
        uint16_t attr1;
        uint16_t attr2;
        int16_t fill;

        void copy_to(handle& other)
        {
            other.attr0 = attr0;
            other.attr1 = attr1;
            other.attr2 = attr2;
        }
    };

    [[nodiscard]] constexpr int count()
    {
        return 128;
    }

    [[nodiscard]] constexpr int max_bg_priority()
    {
        return 3;
    }

    namespace
    {
        static_assert(sizeof(OBJ_ATTR) == sizeof(handle));
        static_assert(alignof(OBJ_ATTR) == alignof(handle));

        handle* vram()
        {
            return reinterpret_cast<handle*>(MEM_OAM);
        }
    }


    inline void init()
    {
        oam_init(reinterpret_cast<OBJ_ATTR*>(vram()), unsigned(count()));
    }

    inline void setup_regular(const sprite_builder& builder, int tiles_id, int palette_id, palette_bpp_mode bpp_mode,
                              handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        int a0 = ATTR0_BUILD(0, int(builder.shape()), 0, 0, builder.mosaic_enabled(), builder.blending_enabled(), 0);
        a0 |= int(bpp_mode) * ATTR0_8BPP;

        int a1 = ATTR1_BUILDR(0, int(builder.size()), builder.horizontal_flip(), builder.vertical_flip());
        int a2 = ATTR2_BUILD(tiles_id, palette_id, builder.bg_priority());
        obj_set_attr(sprite_ptr, uint16_t(a0), uint16_t(a1), uint16_t(a2));
    }

    inline void setup_affine(const sprite_builder& builder, int tiles_id, int palette_id, palette_bpp_mode bpp_mode,
                             handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        int a0 = ATTR0_BUILD(0, int(builder.shape()), 0, 0, builder.mosaic_enabled(), builder.blending_enabled(), 0);
        a0 |= int(bpp_mode) * ATTR0_8BPP;

        int a1 = ATTR1_BUILDA(0, int(builder.size()), 0);
        int a2 = ATTR2_BUILD(tiles_id, palette_id, builder.bg_priority());
        obj_set_attr(sprite_ptr, uint16_t(a0), uint16_t(a1), uint16_t(a2));
    }

    [[nodiscard]] inline bool double_size(const handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
        return sprite_ptr->attr0 & ATTR0_AFF_DBL_BIT;
    }

    [[nodiscard]] inline size dimensions(const handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
        const uint8_t* obj_size = obj_get_size(sprite_ptr);
        size result(obj_size[0], obj_size[1]);

        if(double_size(sprite))
        {
            result *= 2;
        }

        return result;
    }

    inline void set_tiles(int tiles_id, handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        BFN_SET(sprite_ptr->attr2, tiles_id, ATTR2_ID);
    }

    inline void set_palette(int palette_id, handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        BFN_SET(sprite_ptr->attr2, palette_id, ATTR2_PALBANK);
    }

    inline void remove_affine_mat(handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        sprite_ptr->attr0 &= ~ATTR0_AFF_DBL;
    }

    inline void set_affine_mat(int affine_mat_id, bool double_size, handle& sprite)
    {
        remove_affine_mat(sprite);

        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        sprite_ptr->attr0 |= double_size ? ATTR0_AFF_DBL : ATTR0_AFF;
        BFN_SET(sprite_ptr->attr1, affine_mat_id, ATTR1_AFF_ID);
    }

    inline void set_position(int x, int y, handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        obj_set_pos(sprite_ptr, x, y);
    }

    inline void set_bg_priority(int bg_priority, handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        BFN_SET(sprite_ptr->attr2, bg_priority, ATTR2_PRIO);
    }

    [[nodiscard]] inline bool horizontal_flip(const handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
        return sprite_ptr->attr1 & ATTR1_HFLIP;
    }

    inline void set_horizontal_flip(bool horizontal_flip, handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);

        if(horizontal_flip)
        {
            sprite_ptr->attr1 |= ATTR1_HFLIP;
        }
        else
        {
            sprite_ptr->attr1 &= ~ATTR1_HFLIP;
        }
    }

    [[nodiscard]] inline bool vertical_flip(const handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
        return sprite_ptr->attr1 & ATTR1_VFLIP;
    }

    inline void set_vertical_flip(bool vertical_flip, handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);

        if(vertical_flip)
        {
            sprite_ptr->attr1 |= ATTR1_VFLIP;
        }
        else
        {
            sprite_ptr->attr1 &= ~ATTR1_VFLIP;
        }
    }

    [[nodiscard]] inline bool mosaic_enabled(const handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
        return sprite_ptr->attr0 & ATTR0_MOSAIC;
    }

    inline void set_mosaic_enabled(bool mosaic_enabled, handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);

        if(mosaic_enabled)
        {
            sprite_ptr->attr0 |= ATTR0_MOSAIC;
        }
        else
        {
            sprite_ptr->attr0 &= ~ATTR0_MOSAIC;
        }
    }

    [[nodiscard]] inline bool blending_enabled(const handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
        return sprite_ptr->attr0 & ATTR0_BLEND;
    }

    inline void set_blending_enabled(bool blending_enabled, handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);

        if(blending_enabled)
        {
            sprite_ptr->attr0 |= ATTR0_BLEND;
        }
        else
        {
            sprite_ptr->attr0 &= ~ATTR0_BLEND;
        }
    }

    inline void hide(handle& sprite)
    {
        auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
        obj_hide(sprite_ptr);
    }

    inline void commit(const handle& sprites_ref, int offset, int count)
    {
        memory::copy((&sprites_ref)[offset], count, vram()[offset]);
    }
}

#endif
