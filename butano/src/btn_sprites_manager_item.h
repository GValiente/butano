#ifndef BTN_SPRITES_MANAGER_ITEM_H
#define BTN_SPRITES_MANAGER_ITEM_H

#include "btn_size.h"
#include "btn_camera.h"
#include "btn_display.h"
#include "btn_sprites.h"
#include "btn_optional.h"
#include "btn_fixed_point.h"
#include "btn_intrusive_list.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

class sprite_builder;

class sprites_manager_item : public intrusive_list_node_type
{

public:
    hw::sprites::handle handle;
    fixed_point position;
    fixed_point hw_position;
    size half_dimensions;
    unsigned usages = 1;
    sort_key sprite_sort_key;
    sprite_tiles_ptr tiles;
    int8_t handles_index = -1;
    optional<sprite_affine_mat_ptr> affine_mat;
    sprite_palette_ptr palette;
    unsigned double_size_mode: 2;
    unsigned visible: 1;
    unsigned ignore_camera: 1;
    unsigned remove_affine_mat_when_not_needed: 1;
    unsigned on_screen: 1;
    unsigned check_on_screen: 1;

    sprites_manager_item(const fixed_point& position, const sprite_shape_size& shape_size,
                         sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette);

    sprites_manager_item(sprite_builder&& builder, sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette);

    [[nodiscard]] bool double_size() const;

    [[nodiscard]] int bg_priority() const
    {
        return sprite_sort_key.priority();
    }

    void set_bg_priority(int bg_priority)
    {
        sprite_sort_key.set_priority(bg_priority);
    }

    [[nodiscard]] int z_order() const
    {
        return sprite_sort_key.z_order();
    }

    void set_z_order(int z_order)
    {
        sprite_sort_key.set_z_order(z_order);
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

        hw_position.set_x(real_position.x() + (display::width() / 2) - half_dimensions.width());
        hw_position.set_y(real_position.y() + (display::height() / 2) - half_dimensions.height());
        hw::sprites::set_position(hw_position.x().integer(), hw_position.y().integer(), handle);
    }

    void update_hw_position(const fixed_point& camera_position)
    {
        fixed_point real_position = position;

        if(! ignore_camera)
        {
            real_position -= camera_position;
        }

        hw_position.set_x(real_position.x() + (display::width() / 2) - half_dimensions.width());
        hw_position.set_y(real_position.y() + (display::height() / 2) - half_dimensions.height());
        hw::sprites::set_position(hw_position.x().integer(), hw_position.y().integer(), handle);
    }
};

}

#endif
