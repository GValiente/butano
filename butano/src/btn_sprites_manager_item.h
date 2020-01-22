#ifndef BTN_SPRITES_MANAGER_ITEM_H
#define BTN_SPRITES_MANAGER_ITEM_H

#include "btn_optional.h"
#include "btn_fixed_point.h"
#include "btn_sorted_sprites.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

class sprite_builder;

class sprites_manager_item
{

public:
    [[nodiscard]] static constexpr unsigned z_orders()
    {
        return btn::numeric_limits<uint16_t>::max();
    }

    hw::sprites::handle handle;
    fixed_point position;
    unsigned usages = 1;
    unsigned sort_key;
    sorted_sprites::iterator sort_iterator;
    sprite_tiles_ptr tiles_ptr;
    int8_t handles_index = -1;
    optional<sprite_affine_mat_ptr> affine_mat_ptr;
    sprite_palette_ptr palette_ptr;
    unsigned double_size_mode: 2;
    unsigned visible: 1;
    unsigned ignore_camera: 1;
    unsigned remove_affine_mat_when_not_needed: 1;
    unsigned on_screen: 1;
    unsigned check_on_screen: 1;

    BTN_CODE_IWRAM static void sort(sprites_manager_item** begin, sprites_manager_item** end);

    sprites_manager_item(sprite_builder&& builder, sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette);

    [[nodiscard]] bool double_size() const;

    [[nodiscard]] int bg_priority() const
    {
        return sort_key / z_orders();
    }

    [[nodiscard]] int z_order() const
    {
        return sort_key % z_orders();
    }

    void update_sort_key(int bg_priority, int z_order)
    {
        sort_key = (unsigned(bg_priority) * z_orders()) + unsigned(z_order);
    }

    void mark_as_removed();
};

}

#endif
