#ifndef BTN_SPRITES_MANAGER_ITEM_H
#define BTN_SPRITES_MANAGER_ITEM_H

#include "btn_size.h"
#include "btn_camera.h"
#include "btn_optional.h"
#include "btn_fixed_point.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

class sprite_builder;

class sprites_manager_node
{

public:
    sprites_manager_node* prev = nullptr;
    sprites_manager_node* next = nullptr;
};

class sprites_manager_item : public sprites_manager_node
{

public:
    [[nodiscard]] static constexpr int min_z_order()
    {
        return -max_z_order();
    }

    [[nodiscard]] static constexpr int max_z_order()
    {
        return btn::numeric_limits<int16_t>::max() - 1;
    }

    [[nodiscard]] static constexpr unsigned z_orders()
    {
        return btn::numeric_limits<uint16_t>::max();
    }

    hw::sprites::handle handle;
    fixed_point position;
    point hw_position;
    size half_dimensions;
    unsigned usages = 1;
    unsigned sort_key;
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

    sprites_manager_item(sprite_builder&& builder, sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette);

    [[nodiscard]] bool double_size() const;

    [[nodiscard]] int bg_priority() const
    {
        return sort_key / z_orders();
    }

    [[nodiscard]] int z_order() const
    {
        return int(sort_key % z_orders()) - max_z_order();
    }

    void update_half_dimensions()
    {
        half_dimensions = hw::sprites::dimensions(handle) / 2;
        update_hw_position();
    }

    void update_hw_position()
    {
        fixed_point real_position = position;

        if(! ignore_camera)
        {
            real_position -= camera::position();
        }

        hw_position.set_x(real_position.x().integer() - half_dimensions.width());
        hw_position.set_y(real_position.y().integer() - half_dimensions.height());
        hw::sprites::set_position(hw_position.x(), hw_position.y(), handle);
    }

    void update_hw_position(const fixed_point& camera_position)
    {
        fixed_point real_position = position;

        if(! ignore_camera)
        {
            real_position -= camera_position;
        }

        hw_position.set_x(real_position.x().integer() - half_dimensions.width());
        hw_position.set_y(real_position.y().integer() - half_dimensions.height());
        hw::sprites::set_position(hw_position.x(), hw_position.y(), handle);
    }

    void update_sort_key(int bg_priority, int z_order)
    {
        sort_key = (unsigned(bg_priority) * z_orders()) + unsigned(z_order + max_z_order());
    }
};

}

#endif
