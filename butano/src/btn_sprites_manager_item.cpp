#include "btn_sprites_manager_item.h"

#include "btn_sprite_builder.h"
#include "btn_sprite_affine_mats_manager.h"

namespace btn
{

sprites_manager_item::sprites_manager_item(sprite_builder&& builder, sprite_tiles_ptr&& tiles,
                                           sprite_palette_ptr&& palette) :
    position(builder.position()),
    tiles_ptr(move(tiles)),
    affine_mat_ptr(builder.release_affine_mat()),
    palette_ptr(move(palette)),
    double_size_mode(unsigned(builder.double_size_mode())),
    ignore_camera(builder.ignore_camera()),
    remove_affine_mat_when_not_needed(builder.remove_affine_mat_when_not_needed())
{
    if(affine_mat_ptr)
    {
        if(remove_affine_mat_when_not_needed && affine_mat_ptr->is_identity())
        {
            affine_mat_ptr.reset();
            hw::sprites::setup_regular(builder, tiles_ptr.id(), palette_ptr.id(), palette_ptr.bpp_mode(), handle);
        }
        else
        {
            hw::sprites::setup_affine(builder, tiles_ptr.id(), palette_ptr.id(), palette_ptr.bpp_mode(), handle);
            hw::sprites::set_affine_mat(affine_mat_ptr->id(), double_size(), handle);
        }
    }
    else
    {
        hw::sprites::setup_regular(builder, tiles_ptr.id(), palette_ptr.id(), palette_ptr.bpp_mode(), handle);
    }

    update_half_dimensions();
    update_sort_key(builder.bg_priority(), builder.z_order());
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
        return sprite_affine_mats_manager::double_size(affine_mat_ptr->id());

    case sprite_double_size_mode::ENABLED:
        return true;

    case sprite_double_size_mode::DISABLED:
        return false;
    }

    return false;
}

}
