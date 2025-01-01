/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITES_MANAGER_ITEM_H
#define BN_SPRITES_MANAGER_ITEM_H

#include "bn_display.h"
#include "bn_sort_key.h"
#include "bn_camera_ptr.h"
#include "bn_intrusive_list.h"
#include "bn_display_manager.h"
#include "bn_sprites_manager.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_sprite_affine_mats_manager.h"
#include "../hw/include/bn_hw_sprites.h"

namespace bn
{
    class sprite_builder;

    using sprite_affine_mat_attach_node_type = intrusive_list_node_type;
}

namespace bn::sorted_sprites
{
    class layer;
}

namespace bn
{

class sprites_manager_item : public intrusive_list_node_type
{

public:
    sprite_affine_mat_attach_node_type affine_mat_attach_node;
    hw::sprites::handle_type handle;
    fixed_point position;
    point hw_position;
    unsigned usages = 1;
    sort_key sprite_sort_key;
    optional<sprite_tiles_ptr> tiles;
    optional<sprite_palette_ptr> palette;
    optional<sprite_affine_mat_ptr> affine_mat;
    optional<camera_ptr> camera;
    int16_t sort_layer_ptr_diff;
    int8_t handles_index = -1;
    int8_t half_width;
    int8_t half_height;
    uint8_t double_size_mode: 2;
    bool double_size: 1;
    bool blending_enabled: 1;
    bool visible: 1;
    bool remove_affine_mat_when_not_needed: 1;
    bool on_screen: 1;
    bool check_on_screen: 1;

    [[nodiscard]] static sprites_manager_item& affine_mat_attach_node_item(
            sprite_affine_mat_attach_node_type& attach_node)
    {
        auto item_address = reinterpret_cast<intptr_t>(&attach_node);
        item_address -= sizeof(intrusive_list_node_type);

        auto item = reinterpret_cast<sprites_manager_item*>(item_address);
        return *item;
    }

    sprites_manager_item(const fixed_point& _position, const sprite_shape_size& shape_size,
                         sprite_tiles_ptr&& _tiles, sprite_palette_ptr&& _palette) :
        position(_position),
        sprite_sort_key(3, 0),
        tiles(move(_tiles)),
        palette(move(_palette)),
        double_size_mode(uint8_t(sprite_double_size_mode::AUTO)),
        double_size(false),
        blending_enabled(false),
        visible(true),
        remove_affine_mat_when_not_needed(true),
        on_screen(false),
        check_on_screen(true)
    {
        const sprite_palette_ptr& palette_ref = *palette;
        hw::sprites::setup_regular(shape_size, tiles->id(), palette_ref.id(), palette_ref.bpp(),
                                   display_manager::blending_fade_enabled(), handle);
        update_half_dimensions();
    }

    explicit sprites_manager_item(sprite_builder&& builder) :
        position(builder.position()),
        sprite_sort_key(builder.bg_priority(), builder.z_order()),
        tiles(builder.release_tiles()),
        palette(builder.release_palette()),
        affine_mat(builder.release_affine_mat()),
        camera(builder.release_camera()),
        double_size_mode(uint8_t(builder.double_size_mode())),
        double_size(false),
        blending_enabled(builder.blending_enabled()),
        visible(builder.visible()),
        remove_affine_mat_when_not_needed(builder.remove_affine_mat_when_not_needed()),
        on_screen(false),
        check_on_screen(builder.visible())
    {
        _builder_init(builder);
    }

    sprites_manager_item(sprite_builder&& builder, sprite_tiles_ptr&& _tiles, sprite_palette_ptr&& _palette) :
        position(builder.position()),
        sprite_sort_key(builder.bg_priority(), builder.z_order()),
        tiles(move(_tiles)),
        palette(move(_palette)),
        affine_mat(builder.release_affine_mat()),
        camera(builder.release_camera()),
        double_size_mode(uint8_t(builder.double_size_mode())),
        double_size(false),
        blending_enabled(builder.blending_enabled()),
        visible(builder.visible()),
        remove_affine_mat_when_not_needed(builder.remove_affine_mat_when_not_needed()),
        on_screen(false),
        check_on_screen(builder.visible())
    {
        _builder_init(builder);
    }

    [[nodiscard]] bool new_double_size(int affine_mat_id) const
    {
        switch(sprite_double_size_mode(double_size_mode))
        {

        case sprite_double_size_mode::AUTO:
            return sprite_affine_mats_manager::sprite_double_size(affine_mat_id, hw::sprites::shape_size(handle));

        case sprite_double_size_mode::ENABLED:
            return true;

        case sprite_double_size_mode::DISABLED:
            return false;

        default:
            BN_ERROR("Invalid double size mode: ", double_size_mode);
            return false;
        }
    }

    [[nodiscard]] bool new_double_size(int affine_mat_id, const sprite_shape_size& shape_size) const
    {
        switch(sprite_double_size_mode(double_size_mode))
        {

        case sprite_double_size_mode::AUTO:
            return sprite_affine_mats_manager::sprite_double_size(affine_mat_id, shape_size);

        case sprite_double_size_mode::ENABLED:
            return true;

        case sprite_double_size_mode::DISABLED:
            return false;

        default:
            BN_ERROR("Invalid double size mode: ", double_size_mode);
            return false;
        }
    }

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
        pair<int, int> dimensions = hw::sprites::dimensions(handle, double_size);
        half_width = int8_t(dimensions.first / 2);
        half_height = int8_t(dimensions.second / 2);
        update_hw_position();
    }

    void update_hw_position()
    {
        int real_x = position.x().right_shift_integer();
        int real_y = position.y().right_shift_integer();

        if(const camera_ptr* camera_ptr = camera.get())
        {
            const fixed_point& camera_position = camera_ptr->position();
            real_x -= camera_position.x().right_shift_integer();
            real_y -= camera_position.y().right_shift_integer();
        }

        update_hw_x(real_x);
        update_hw_y(real_y);
    }

    void update_hw_x(int real_x)
    {
        int hw_x = real_x + (display::width() / 2) - int(half_width);
        hw_position.set_x(hw_x);
        hw::sprites::set_x(hw_x, handle);
    }

    void update_hw_y(int real_y)
    {
        int hw_y = real_y + (display::height() / 2) - int(half_height);
        hw_position.set_y(hw_y);
        hw::sprites::set_y(hw_y, handle);
    }

private:
    void _builder_init(const sprite_builder& builder)
    {
        const sprite_palette_ptr& palette_ref = *palette;
        int tiles_id = tiles->id();

        if(const sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
        {
            if(remove_affine_mat_when_not_needed && affine_mat_ptr->flipped_identity())
            {
                hw::sprites::setup_regular(builder, tiles_id, palette_ref.id(), palette_ref.bpp(),
                                           affine_mat_ptr->horizontal_flip(), affine_mat_ptr->vertical_flip(),
                                           display_manager::blending_fade_enabled(), handle);
                affine_mat.reset();
            }
            else
            {
                int affine_mat_id = affine_mat_ptr->id();
                double_size = new_double_size(affine_mat_id, builder.shape_size());
                hw::sprites::setup_affine(builder, tiles_id, palette_ref.id(), palette_ref.bpp(),
                                          display_manager::blending_fade_enabled(), handle);
                hw::sprites::set_affine_mat(affine_mat_id, handle);
                hw::sprites::show_affine(double_size, handle);
                sprite_affine_mats_manager::attach_sprite(affine_mat_id, affine_mat_attach_node);
            }
        }
        else
        {
            hw::sprites::setup_regular(builder, tiles_id, palette_ref.id(), palette_ref.bpp(),
                                       display_manager::blending_fade_enabled(), handle);
        }

        update_half_dimensions();
    }
};

}

#endif
