#ifndef POLYGON_H
#define POLYGON_H

#include "btn_array.h"
#include "btn_display.h"
#include "btn_sprite_ptr.h"
#include "btn_sprite_affine_mat_actions.h"
#include "btn_sprite_affine_mat_hblank_effects.h"

class polygon
{

public:
    polygon(const btn::sprite_item& sprite_item, btn::fixed x, btn::fixed y, int z_order);

    [[nodiscard]] btn::fixed x() const
    {
        return _sprite.x();
    }

    void set_x(btn::fixed x)
    {
        _sprite.set_x(x);
    }

    [[nodiscard]] btn::fixed y() const
    {
        return _sprite.y();
    }

    void set_y(btn::fixed y);

    [[nodiscard]] btn::fixed scale_x() const
    {
        return _scale_x;
    }

    void set_scale_x(btn::fixed scale_x);

    [[nodiscard]] btn::fixed scale_y() const
    {
        return _scale_y;
    }

    void set_scale_y(btn::fixed scale_y);

    [[nodiscard]] btn::fixed panning() const
    {
        return _panning;
    }

    void set_panning(btn::fixed panning);

    polygon(const polygon& other) = delete;

    polygon& operator=(const polygon& other) = delete;

    void update();

private:
    using p_fixed = btn::fixed_t<10>;

    btn::sprite_affine_mat_ptr _affine_mat;
    btn::sprite_ptr _sprite;
    btn::fixed _panning;
    btn::fixed _scale_x = 1;
    btn::fixed _scale_y = 1;
    btn::array<int16_t, btn::display::height()> _affine_mat_register_values;
    btn::sprite_affine_mat_third_register_hblank_effect_ptr _hblank_effect;
    bool _update = false;
    bool _update_hblank_effect = false;

    [[nodiscard]] int _base_y() const
    {
        return (btn::display::height() / 2) + y().integer() - 32;
    }

    BTN_CODE_IWRAM void _setup_affine_mat_register_values(p_fixed tymul, p_fixed scale_y, int base_y, bool negative);
};

#endif
