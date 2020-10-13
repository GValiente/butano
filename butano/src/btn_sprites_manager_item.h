#ifndef BTN_SPRITES_MANAGER_ITEM_H
#define BTN_SPRITES_MANAGER_ITEM_H

#include "btn_display.h"
#include "btn_sprites.h"
#include "btn_optional.h"
#include "btn_camera_ptr.h"
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

using sprite_affine_mat_attach_node_type = intrusive_list_node_type;

class sprites_manager_item : public intrusive_list_node_type
{

public:
    sprite_affine_mat_attach_node_type affine_mat_attach_node;
    hw::sprites::handle_type handle;
    fixed_point position;
    fixed_point hw_position;
    unsigned usages = 1;
    sort_key sprite_sort_key;
    optional<sprite_tiles_ptr> tiles;
    optional<sprite_palette_ptr> palette;
    optional<sprite_affine_mat_ptr> affine_mat;
    optional<camera_ptr> camera;
    int8_t handles_index = -1;
    int8_t half_width;
    int8_t half_height;
    unsigned double_size_mode: 2;
    bool blending_enabled: 1;
    bool visible: 1;
    bool remove_affine_mat_when_not_needed: 1;
    bool on_screen: 1;
    bool check_on_screen: 1;

    [[nodiscard]] static sprites_manager_item& affine_mat_attach_node_item(
            sprite_affine_mat_attach_node_type& attach_node)
    {
        auto item_address = reinterpret_cast<int>(&attach_node);
        item_address -= sizeof(intrusive_list_node_type);

        auto item = reinterpret_cast<sprites_manager_item*>(item_address);
        return *item;
    }

    sprites_manager_item(const fixed_point& _position, const sprite_shape_size& shape_size,
                         sprite_tiles_ptr&& _tiles, sprite_palette_ptr&& _palette);

    sprites_manager_item(sprite_builder&& builder, sprite_tiles_ptr&& _tiles, sprite_palette_ptr&& _palette);

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
        pair<int, int> dimensions = hw::sprites::dimensions(handle);
        half_width = int8_t(dimensions.first / 2);
        half_height = int8_t(dimensions.second / 2);
        update_hw_position();
    }

    void update_hw_position()
    {
        fixed_point real_position(position.x() + (display::width() / 2) - int(half_width),
                                  position.y() + (display::height() / 2) - int(half_height));

        if(camera)
        {
            real_position -= camera->position();
        }

        hw_position = real_position;
        hw::sprites::set_x(real_position.x().integer(), handle);
        hw::sprites::set_y(real_position.y().integer(), handle);
    }
};

}

#endif
