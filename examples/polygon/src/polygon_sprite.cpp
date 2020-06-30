#include "polygon_sprite.h"

#include "btn_sprite_builder.h"
#include "btn_sprite_items_texture.h"
#include "polygon.h"

namespace
{
    [[nodiscard]] btn::sprite_ptr _create_sprite(int graphics_index, int z_order)
    {
        btn::sprite_builder builder(btn::sprite_items::texture, graphics_index);
        builder.set_position(-(btn::display::width() / 2) + 32, -(btn::display::height() / 2) + 32);
        builder.set_z_order(z_order);
        builder.set_ignore_camera(true);
        return builder.release_build();
    }
}

polygon_sprite::polygon_sprite(const polygon& polygon, int graphics_index, int z_order) :
    _polygons(1, &polygon),
    _sprite(_create_sprite(graphics_index, z_order)),
    _vertical_hblank_effect(btn::sprite_position_hblank_effect_ptr::create_vertical(_sprite, _vertical_values)),
    _horizontal_hblank_effect(btn::sprite_position_hblank_effect_ptr::create_horizontal(_sprite, _horizontal_values))
{
}

polygon_sprite::polygon_sprite(const btn::span<const polygon*>& polygons, int graphics_index, int z_order) :
    _sprite(_create_sprite(graphics_index, z_order)),
    _vertical_hblank_effect(btn::sprite_position_hblank_effect_ptr::create_vertical(_sprite, _vertical_values)),
    _horizontal_hblank_effect(btn::sprite_position_hblank_effect_ptr::create_horizontal(_sprite, _horizontal_values))
{
    for(const polygon* polygon : polygons)
    {
        _polygons.push_back(polygon);
    }
}

void polygon_sprite::update()
{
    if(_update)
    {
        btn::array<hline, btn::display::height()> hlines;
        hline* hlines_data = hlines.data();
        _update = false;

        for(const polygon* polygon : _polygons)
        {
            const btn::ivector<btn::fixed_point>& vertices = polygon->vertices();

            for(int index = 0, limit = vertices.size() - 1; index < limit; ++index)
            {
                _draw_line(vertices[index], vertices[index + 1], hlines_data);
            }

            _draw_line(vertices.back(), vertices.front(), hlines_data);
        }

        _setup_attributes(hlines_data, _vertical_values.data(), _horizontal_values.data());
        _vertical_hblank_effect.reload_deltas_ref();
        _horizontal_hblank_effect.reload_deltas_ref();
    }
}
