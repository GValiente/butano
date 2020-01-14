#include "../include/btn_hw_sprites.h"

#include "tonc.h"
#include "btn_size.h"
#include "btn_memory.h"
#include "btn_sprite_builder.h"

namespace btn::hw::sprites
{

namespace
{
    static_assert(sizeof(OBJ_ATTR) == sizeof(handle));
    static_assert(alignof(OBJ_ATTR) == alignof(handle));

    handle* vram()
    {
        return reinterpret_cast<handle*>(MEM_OAM);
    }
}

void init()
{
    oam_init(reinterpret_cast<OBJ_ATTR*>(vram()), unsigned(count()));
}

void setup_regular(const sprite_builder& builder, int tile_id, int palette_id, bool eight_bits_per_pixel,
                   handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    int a0 = ATTR0_BUILD(0, int(builder.shape()), eight_bits_per_pixel, 0, builder.mosaic_enabled(), 0, 0);
    int a1 = ATTR1_BUILDR(0, int(builder.size()), builder.horizontal_flip(), builder.vertical_flip());
    int a2 = ATTR2_BUILD(tile_id, palette_id, builder.bg_priority());
    obj_set_attr(sprite_ptr, uint16_t(a0), uint16_t(a1), uint16_t(a2));
}

void setup_affine(const sprite_builder& builder, int tile_id, int palette_id, int affine_mat_id,
                  bool eight_bits_per_pixel, handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    int affine_mode = (builder.double_size() * 2) + 1;
    int a0 = ATTR0_BUILD(0, int(builder.shape()), eight_bits_per_pixel, affine_mode, builder.mosaic_enabled(), 0, 0);
    int a1 = ATTR1_BUILDA(0, int(builder.size()), affine_mat_id);
    int a2 = ATTR2_BUILD(tile_id, palette_id, builder.bg_priority());
    obj_set_attr(sprite_ptr, uint16_t(a0), uint16_t(a1), uint16_t(a2));
}

size dimensions(const handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
    const uint8_t* obj_size = obj_get_size(sprite_ptr);
    size result(obj_size[0], obj_size[1]);

    if(sprite_ptr->attr0 & ATTR0_AFF_DBL_BIT)
    {
        result *= 2;
    }

    return result;
}

void set_tile(int tile_id, handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    BFN_SET(sprite_ptr->attr2, tile_id, ATTR2_ID);
}

void set_palette(int palette_id, handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    BFN_SET(sprite_ptr->attr2, palette_id, ATTR2_PALBANK);
}

void set_affine_mat(int affine_mat_id, bool double_size, handle& sprite)
{
    remove_affine_mat(sprite);

    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    sprite_ptr->attr0 |= double_size ? ATTR0_AFF_DBL : ATTR0_AFF;
    BFN_SET(sprite_ptr->attr1, affine_mat_id, ATTR1_AFF_ID);
}

void remove_affine_mat(handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    sprite_ptr->attr0 &= ~ATTR0_AFF_DBL;
}

void set_position(int x, int y, handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    size dims = dimensions(sprite);
    obj_set_pos(sprite_ptr, x - (dims.width() / 2), y - (dims.height() / 2));
}

void set_bg_priority(int bg_priority, handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    BFN_SET(sprite_ptr->attr2, bg_priority, ATTR2_PRIO);
}

bool horizontal_flip(const handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
    return sprite_ptr->attr1 & ATTR1_HFLIP;
}

void set_horizontal_flip(bool horizontal_flip, handle& sprite)
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

bool vertical_flip(const handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
    return sprite_ptr->attr1 & ATTR1_VFLIP;
}

void set_vertical_flip(bool vertical_flip, handle& sprite)
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

bool mosaic_enabled(const handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<const OBJ_ATTR*>(&sprite);
    return sprite_ptr->attr0 & ATTR0_MOSAIC;
}

void set_mosaic_enabled(bool mosaic_enabled, handle& sprite)
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

void hide(handle& sprite)
{
    auto sprite_ptr = reinterpret_cast<OBJ_ATTR*>(&sprite);
    obj_hide(sprite_ptr);
}

void commit(const handle& sprites_ref, int offset, int count)
{
    memory::copy((&sprites_ref)[offset], count, vram()[offset]);
}

}
