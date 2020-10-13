#include "btn_sprites_manager_item.h"

#include "btn_sprite_builder.h"
#include "btn_display_manager.h"
#include "btn_sprite_affine_mats_manager.h"

namespace btn
{

sprites_manager_item::sprites_manager_item(const fixed_point& _position, const sprite_shape_size& shape_size,
                                           sprite_tiles_ptr&& _tiles, sprite_palette_ptr&& _palette) :
    position(_position),
    sprite_sort_key(3, 0),
    tiles(move(_tiles)),
    palette(move(_palette)),
    double_size_mode(unsigned(sprite_double_size_mode::AUTO)),
    blending_enabled(false),
    remove_affine_mat_when_not_needed(true)
{
    const sprite_palette_ptr& palette_ref = *palette;
    hw::sprites::setup_regular(shape_size, tiles->id(), palette_ref.id(), palette_ref.bpp_mode(),
                               display_manager::blending_fade_enabled(), handle);
    update_half_dimensions();
    on_screen = false;
    visible = true;
    check_on_screen = true;
}

sprites_manager_item::sprites_manager_item(sprite_builder&& builder, sprite_tiles_ptr&& _tiles,
                                           sprite_palette_ptr&& _palette) :
    position(builder.position()),
    sprite_sort_key(builder.bg_priority(), builder.z_order()),
    tiles(move(_tiles)),
    palette(move(_palette)),
    affine_mat(builder.release_affine_mat()),
    camera(builder.release_camera()),
    double_size_mode(unsigned(builder.double_size_mode())),
    blending_enabled(builder.blending_enabled()),
    remove_affine_mat_when_not_needed(builder.remove_affine_mat_when_not_needed())
{
    const sprite_palette_ptr& palette_ref = *palette;

    if(affine_mat)
    {
        const sprite_affine_mat_ptr& affine_mat_ref = *affine_mat;

        if(remove_affine_mat_when_not_needed && affine_mat_ref.flipped_identity())
        {
            hw::sprites::setup_regular(builder, tiles->id(), palette_ref.id(), palette_ref.bpp_mode(),
                                       affine_mat_ref.horizontal_flip(), affine_mat_ref.vertical_flip(),
                                       display_manager::blending_fade_enabled(), handle);
            affine_mat.reset();
        }
        else
        {
            int affine_mat_id = affine_mat_ref.id();
            hw::sprites::setup_affine(builder, tiles->id(), palette_ref.id(), palette_ref.bpp_mode(),
                                      display_manager::blending_fade_enabled(), handle);
            hw::sprites::set_affine_mat(affine_mat_id, double_size(), handle);
            sprite_affine_mats_manager::attach_sprite(affine_mat_id, affine_mat_attach_node);
        }
    }
    else
    {
        hw::sprites::setup_regular(builder, tiles->id(), palette_ref.id(), palette_ref.bpp_mode(),
                                   display_manager::blending_fade_enabled(), handle);
    }

    update_half_dimensions();
    on_screen = false;

    if(builder.visible())
    {
        visible = true;
        check_on_screen = true;
    }
    else
    {
        visible = false;
        check_on_screen = false;
    }
}

bool sprites_manager_item::double_size() const
{
    switch(sprite_double_size_mode(double_size_mode))
    {

    case sprite_double_size_mode::AUTO:
        return sprite_affine_mats_manager::double_size(affine_mat->id());

    case sprite_double_size_mode::ENABLED:
        return true;

    case sprite_double_size_mode::DISABLED:
        return false;

    default:
        BTN_ERROR("Invalid double size mode: ", int(double_size_mode));
        return false;
    }
}

}
