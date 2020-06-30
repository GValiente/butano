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

    [[nodiscard]] btn::sprite_first_attributes_hblank_effect_ptr _create_first_hblank_effect(
        const btn::sprite_ptr& sprite, btn::ivector<btn::sprite_first_attributes>& attributes)
    {
        attributes.resize(attributes.max_size(), sprite.first_attributes());

        btn::span<const btn::sprite_first_attributes> attributes_ref(attributes.data(), attributes.size());
        return btn::sprite_first_attributes_hblank_effect_ptr::create(sprite, attributes_ref);
    }

    [[nodiscard]] btn::sprite_regular_second_attributes_hblank_effect_ptr _create_second_hblank_effect(
        const btn::sprite_ptr& sprite, btn::ivector<btn::sprite_regular_second_attributes>& attributes)
    {
        attributes.resize(attributes.max_size(), sprite.regular_second_attributes());

        btn::span<const btn::sprite_regular_second_attributes> attributes_ref(attributes.data(), attributes.size());
        return btn::sprite_regular_second_attributes_hblank_effect_ptr::create(sprite, attributes_ref);
    }
}

polygon_sprite::polygon_sprite(const polygon& polygon, int graphics_index, int z_order) :
    _polygons(1, &polygon),
    _sprite(_create_sprite(graphics_index, z_order)),
    _first_hblank_effect(_create_first_hblank_effect(_sprite, _first_attributes)),
    _second_hblank_effect(_create_second_hblank_effect(_sprite, _second_attributes))
{
}

polygon_sprite::polygon_sprite(const btn::span<const polygon*>& polygons, int graphics_index, int z_order) :
    _sprite(_create_sprite(graphics_index, z_order)),
    _first_hblank_effect(_create_first_hblank_effect(_sprite, _first_attributes)),
    _second_hblank_effect(_create_second_hblank_effect(_sprite, _second_attributes))
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

        _setup_attributes(hlines_data, _first_attributes.data(), _second_attributes.data());
        _first_hblank_effect.reload_attributes_ref();
        _second_hblank_effect.reload_attributes_ref();
    }
}
