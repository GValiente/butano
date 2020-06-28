#include "polygon.h"

#include "btn_memory.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_affine_mat_attributes.h"

namespace
{
    using fixed = btn::fixed_t<10>;

    [[nodiscard]] btn::sprite_ptr _create_sprite(const btn::sprite_item& sprite_item, btn::fixed x, btn::fixed y,
                                                 int z_order, const btn::sprite_affine_mat_ptr& affine_mat)
    {
        btn::sprite_builder builder(sprite_item);
        builder.set_position(x, y);
        builder.set_z_order(z_order);
        builder.set_affine_mat(affine_mat);
        return builder.release_build();
    }
}

polygon::polygon(const btn::sprite_item& sprite_item, btn::fixed x, btn::fixed y, int z_order) :
    _affine_mat(btn::sprite_affine_mat_ptr::create()),
    _sprite(_create_sprite(sprite_item, x, y, z_order, _affine_mat)),
    _hblank_effect(btn::sprite_affine_mat_third_register_hblank_effect_ptr::create(
                       _affine_mat, btn::span<const int16_t>(_affine_mat_register_values)))
{
}

void polygon::set_y(btn::fixed y)
{
    int old_base_y = _base_y();
    _sprite.set_y(y);

    if(old_base_y != _base_y())
    {
        _update = true;
        _update_hblank_effect = true;
    }
}

void polygon::set_scale_x(btn::fixed scale_x)
{
    _scale_x = scale_x;
    _update = true;
}

void polygon::set_scale_y(btn::fixed scale_y)
{
    _scale_y = scale_y;
    _update = true;
    _update_hblank_effect = true;
}

void polygon::set_panning(btn::fixed panning)
{
    p_fixed old_p_panning = _panning;
    p_fixed new_p_panning = panning;
    _panning = panning;

    if(old_p_panning != new_p_panning)
    {
        _update = true;
        _update_hblank_effect = true;
    }
}

void polygon::update()
{
    if(_update)
    {
        _update = false;

        p_fixed panning = _panning;
        bool negative = panning < 0;
        p_fixed tymul = negative ? 1 - btn::abs(panning) : 1 - panning;
        btn::sprite_affine_mat_attributes affine_mat_attributes;
        p_fixed scale_x = btn::fixed(1).unsafe_division(_scale_x);
        p_fixed scale_y = btn::fixed(1).unsafe_division(_scale_y);
        affine_mat_attributes.set_first_register_value((256 * scale_x).integer());

        // pc: 8*iy*(tymul - 1)/(tymul + 1)
        affine_mat_attributes.set_fourth_register_value(((512 * scale_y).unsafe_division(tymul + 1)).integer());

        if(_update_hblank_effect)
        {
            _update_hblank_effect = false;

            int base_y = _base_y();
            int16_t* register_data = _affine_mat_register_values.data();
            btn::memory::clear(base_y, register_data[0]);
            _setup_affine_mat_register_values(tymul, scale_y, base_y, negative);
            btn::memory::clear(btn::display::height() - base_y - 64, register_data[base_y + 64]);
            _hblank_effect.reload_values_ref();
        }

        _affine_mat.set_attributes(affine_mat_attributes);
    }
}
