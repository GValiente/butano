#include "polygon_sprite.h"

#include "bn_sprite_builder.h"
#include "bn_sprite_items_texture.h"
#include "polygon.h"

namespace
{
    [[nodiscard]] bn::sprite_ptr _create_sprite(int graphics_index, int z_order)
    {
        bn::sprite_builder builder(bn::sprite_items::texture, graphics_index);
        builder.set_position(-(bn::display::width() / 2) + 32, -(bn::display::height() / 2) + 32);
        builder.set_z_order(z_order);
        return builder.release_build();
    }
}

polygon_sprite::polygon_sprite(const polygon& polygon, int graphics_index, int z_order) :
    _polygons(1, &polygon),
    _sprite(_create_sprite(graphics_index, z_order)),
    _vertical_hbe(bn::sprite_position_hbe_ptr::create_vertical(_sprite, _vertical_values)),
    _horizontal_hbe(bn::sprite_position_hbe_ptr::create_horizontal(_sprite, _horizontal_values))
{
}

polygon_sprite::polygon_sprite(const bn::span<const polygon*>& polygons, int graphics_index, int z_order) :
    _sprite(_create_sprite(graphics_index, z_order)),
    _vertical_hbe(bn::sprite_position_hbe_ptr::create_vertical(_sprite, _vertical_values)),
    _horizontal_hbe(bn::sprite_position_hbe_ptr::create_horizontal(_sprite, _horizontal_values))
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
        bn::array<hline, bn::display::height()> hlines;
        hline* hlines_data = hlines.data();
        _update = false;

        for(const polygon* polygon : _polygons)
        {
            const bn::fixed_point* vertices_data = polygon->vertices().data();

            bn::point vertices[] = {
                bn::point(vertices_data[0].x().right_shift_integer(), vertices_data[0].y().right_shift_integer()),
                bn::point(vertices_data[1].x().right_shift_integer(), vertices_data[1].y().right_shift_integer()),
                bn::point(vertices_data[2].x().right_shift_integer(), vertices_data[2].y().right_shift_integer()),
                bn::point(vertices_data[3].x().right_shift_integer(), vertices_data[3].y().right_shift_integer())
            };

            int top_index = 0;
            int top_index_y = vertices[0].y();

            for(int index = 1; index < 4; ++index)
            {
                int y = vertices[index].y();

                if(y < top_index_y)
                {
                    top_index = index;
                    top_index_y = y;
                }
            }

            int left_index, right_index, bottom_index;

            if(top_index == 0)
            {
                left_index = 3;
                right_index = 1;
                bottom_index = 2;
            }
            else
            {
                left_index = top_index - 1;
                right_index = (top_index + 1) % 4;
                bottom_index = (top_index + 2) % 4;
            }

            const bn::point& top_vertex = vertices[top_index];
            const bn::point& left_vertex = vertices[left_index];
            const bn::point& right_vertex = vertices[right_index];
            const bn::point& bottom_vertex = vertices[bottom_index];
            bool tlh = top_vertex.y() == left_vertex.y();
            bool trh = top_vertex.y() == right_vertex.y();
            bool lbh = left_vertex.y() == bottom_vertex.y();
            bool rbh = right_vertex.y() == bottom_vertex.y();

            if(! tlh)
            {
                _draw_not_horizontal_line(true,
                                          top_vertex.x(), top_vertex.y(), left_vertex.x(), left_vertex.y(), hlines_data);
            }

            if(! trh)
            {
                _draw_not_horizontal_line(false,
                                          top_vertex.x(), top_vertex.y(), right_vertex.x(), right_vertex.y(), hlines_data);
            }

            if(! lbh)
            {
                _draw_not_horizontal_line(left_vertex.y() < bottom_vertex.y(),
                                          left_vertex.x(), left_vertex.y(), bottom_vertex.x(), bottom_vertex.y(), hlines_data);
            }

            if(! rbh)
            {
                _draw_not_horizontal_line(right_vertex.y() > bottom_vertex.y(),
                                          right_vertex.x(), right_vertex.y(), bottom_vertex.x(), bottom_vertex.y(), hlines_data);
            }
            else
            {
                _draw_horizontal_line(right_vertex.x(), bottom_vertex.x(), bottom_vertex.y(), hlines_data);
            }

            if(tlh)
            {
                _draw_horizontal_line(top_vertex.x(), left_vertex.x(), left_vertex.y(), hlines_data);
            }

            if(trh)
            {
                _draw_horizontal_line(top_vertex.x(), right_vertex.x(), right_vertex.y(), hlines_data);
            }

            if(lbh)
            {
                _draw_horizontal_line(left_vertex.x(), bottom_vertex.x(), bottom_vertex.y(), hlines_data);
            }
        }

        _setup_attributes(hlines_data, _vertical_values.data(), _horizontal_values.data());
        _vertical_hbe.reload_deltas_ref();
        _horizontal_hbe.reload_deltas_ref();
    }
}
