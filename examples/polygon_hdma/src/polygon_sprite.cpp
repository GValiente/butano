#include "polygon_sprite.h"

#include "btn_sprite_items_texture.h"
#include "polygon.h"
#include "../../butano/hw/include/btn_hw_sprites.h"

polygon_sprite::polygon_sprite(const polygon& polygon, int graphics_index, int z_order) :
    _polygons(1, &polygon),
    _tiles(btn::sprite_items::texture.tiles_item().create_tiles(graphics_index)),
    _palette(btn::sprite_items::texture.palette_item().create_palette()),
    _z_order(z_order)
{
}

polygon_sprite::polygon_sprite(const btn::span<const polygon*>& polygons, int graphics_index, int z_order) :
    _tiles(btn::sprite_items::texture.tiles_item().create_tiles(graphics_index)),
    _palette(btn::sprite_items::texture.palette_item().create_palette()),
    _z_order(z_order)
{
    for(const polygon* polygon : polygons)
    {
        _polygons.push_back(polygon);
    }
}

void polygon_sprite::update(int max_polygon_sprites, uint16_t* hdma_source)
{
    btn::array<hline, btn::display::height()> hlines;
    hline* hlines_data = hlines.data();

    for(const polygon* polygon : _polygons)
    {
        const btn::ivector<btn::fixed_point>& vertices = polygon->vertices();

        for(int index = 0, limit = vertices.size() - 1; index < limit; ++index)
        {
            _draw_line(vertices[index], vertices[index + 1], hlines_data);
        }

        _draw_line(vertices.back(), vertices.front(), hlines_data);
    }

    btn::hw::sprites::handle_type base_sprite_handle;
    btn::hw::sprites::setup_regular(btn::sprite_items::texture.shape_size(), _tiles.id(), _palette.id(),
                                    _palette.bpp_mode(), base_sprite_handle);
    _setup_attributes(&base_sprite_handle, hlines_data, _z_order, max_polygon_sprites, hdma_source);
}
