#ifndef POLYGON_H
#define POLYGON_H

#include "btn_array.h"
#include "btn_vector.h"
#include "btn_display.h"
#include "btn_sprite_ptr.h"
#include "btn_fixed_point.h"
#include "btn_sprite_hblank_effects.h"
#include "btn_sprite_first_attributes.h"
#include "btn_sprite_regular_second_attributes.h"

class polygon
{

public:
    polygon(const btn::span<const btn::fixed_point>& vertices, btn::fixed x, btn::fixed y, int graphics_index,
            int z_order);

    polygon(const polygon& other) = delete;

    polygon& operator=(const polygon& other) = delete;

    [[nodiscard]] const btn::fixed_point& vertex(int index) const
    {
        return _vertices[index];
    }

    void set_vertex(int index, const btn::fixed_point& vertex);

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

    void update();

private:
    btn::vector<btn::fixed_point, 8> _vertices;
    btn::sprite_ptr _sprite;
    btn::vector<btn::sprite_first_attributes, btn::display::height()> _first_attributes;
    btn::sprite_first_attributes_hblank_effect_ptr _first_hblank_effect;
    btn::vector<btn::sprite_regular_second_attributes, btn::display::height()> _second_attributes;
    btn::sprite_regular_second_attributes_hblank_effect_ptr _second_hblank_effect;
    bool _update = true;
};

#endif
