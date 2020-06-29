#include "polygon.h"

#include "btn_math.h"
#include "btn_memory.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_texture.h"

namespace
{
    [[nodiscard]] btn::sprite_ptr _create_sprite(btn::fixed x, btn::fixed y, int graphics_index, int z_order)
    {
        btn::sprite_builder builder(btn::sprite_items::texture, graphics_index);
        builder.set_position(x, y);
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

    struct hline
    {
        int ixl = 1000;
        int ixr = -1000;
    };

    void _draw_line(const btn::fixed_point& from, const btn::fixed_point& to, hline* hlines)
    {
        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

        int x0 = from.x().integer();
        int y0 = from.y().integer();
        int x1 = to.x().integer();
        int y1 = to.y().integer();
        int dx = btn::abs(x1 - x0);
        int sx = x0 < x1 ? 1 : -1;
        int dy = -btn::abs(y1 - y0);
        int sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;  // error value e_xy

        while(true)
        {
            hline& hline = hlines[y0 + 32];
            hline.ixl = btn::min(hline.ixl, x0);
            hline.ixr = btn::max(hline.ixr, x0);

            if(x0 == x1 && y0 == y1)
            {
                return;
            }

            int e2 = err * 2;

            if(e2 >= dy) // e_xy + e_x > 0
            {
                err += dy;
                x0 += sx;
            }

            if(e2 <= dx) // e_xy + e_y < 0
            {
                err += dx;
                y0 += sy;
            }
        }
    }

    void _setup_first_attributes(int sprite_y, const hline* hlines, btn::sprite_first_attributes* output)
    {
        int base_y = (btn::display::height() / 2) + sprite_y - 32;
        int invalid_y = (btn::display::height() / 2) - sprite_y + 32;

        for(int index = 0; index < 160; ++index)
        {
            output[index].set_y(invalid_y);
        }

        for(int index = 0; index < 64; ++index)
        {
            const hline& hline = hlines[index];
            int length = hline.ixr - hline.ixl;

            if(length > 0)
            {
                output[index + base_y].set_y(-length + index + 1);
            }
        }
    }

    void _setup_second_attributes(int sprite_y, const hline* hlines, btn::sprite_regular_second_attributes* output)
    {
        int base_y = (btn::display::height() / 2) + sprite_y - 32;

        for(int index = 0; index < 64; ++index)
        {
            const hline& hline = hlines[index];
            output[index + base_y].set_x(hline.ixl + 32);
        }
    }
}

polygon::polygon(const btn::span<const btn::fixed_point>& vertices, btn::fixed x, btn::fixed y, int graphics_index,
                 int z_order) :
    _sprite(_create_sprite(x, y, graphics_index, z_order)),
    _first_hblank_effect(_create_first_hblank_effect(_sprite, _first_attributes)),
    _second_hblank_effect(_create_second_hblank_effect(_sprite, _second_attributes))
{
    for(const btn::fixed_point& vertex : vertices)
    {
        _vertices.push_back(vertex);
    }
}

void polygon::set_vertex(int index, const btn::fixed_point& vertex)
{
    btn::fixed_point old_vertex = _vertices[index];
    _vertices[index] = vertex;
    _update |= old_vertex != vertex;
}

void polygon::set_y(btn::fixed y)
{
    int old_y = _sprite.y().integer();
    _sprite.set_y(y);
    _update |= old_y != y.integer();
}

void polygon::update()
{
    if(_update)
    {
        btn::array<hline, 64> hlines;
        hline* hlines_data = hlines.data();
        _update = false;

        for(int index = 0, limit = _vertices.size() - 1; index < limit; ++index)
        {
            _draw_line(_vertices[index], _vertices[index + 1], hlines_data);
        }

        _draw_line(_vertices.back(), _vertices.front(), hlines_data);

        int sprite_y = _sprite.y().integer();
        _setup_first_attributes(sprite_y, hlines_data, _first_attributes.data());
        _first_hblank_effect.reload_attributes_ref();
        _setup_second_attributes(sprite_y, hlines_data, _second_attributes.data());
        _second_hblank_effect.reload_attributes_ref();
    }
}
