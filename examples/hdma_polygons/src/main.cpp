/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"

#include "bn_hdma.h"
#include "bn_color.h"
#include "bn_keypad.h"
#include "bn_random.h"
#include "bn_unique_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "demo_polygon.h"
#include "polygon_sprite.h"

#include "../../butano/hw/include/bn_hw_sprites.h"

#include "common_info.h"
#include "common_stats.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    constexpr bn::string_view info_text_lines[] = {
        "",
        "",
        "Polygons rendering with HDMA sprites",
        "",
        "L + PAD: move top left vertex",
        "R + PAD: move top right vertex",
        "B + PAD: move bottom left vertex",
        "A + PAD: move bottom right vertex"
    };

    void _move_vertex(int vertex_index, polygon& polygon)
    {
        bn::fixed_point& vertex = polygon.vertices()[vertex_index];

        if(bn::keypad::left_held())
        {
            if(vertex_index == 0 || vertex_index == 3)
            {
                vertex.set_x(bn::max(vertex.x() - 1, bn::fixed(120 - 31)));
            }
            else
            {
                vertex.set_x(bn::max(vertex.x() - 1, bn::fixed(120)));
            }
        }
        else if(bn::keypad::right_held())
        {
            if(vertex_index == 0 || vertex_index == 3)
            {
                vertex.set_x(bn::min(vertex.x() + 1, bn::fixed(120)));
            }
            else
            {
                vertex.set_x(bn::min(vertex.x() + 1, bn::fixed(120 + 31)));
            }
        }

        if(bn::keypad::up_held())
        {
            if(vertex_index == 0 || vertex_index == 1)
            {
                vertex.set_y(bn::max(vertex.y() - 1, bn::fixed(1)));
            }
            else
            {
                vertex.set_y(bn::max(vertex.y() - 1, bn::fixed(80)));
            }
        }
        else if(bn::keypad::down_held())
        {
            if(vertex_index == 0 || vertex_index == 1)
            {
                vertex.set_y(bn::min(vertex.y() + 1, bn::fixed(80)));
            }
            else
            {
                vertex.set_y(bn::min(vertex.y() + 1, bn::fixed(159)));
            }
        }
    }

    void _create_demo_polygon_sprite(bn::fixed x, bn::fixed y, bn::ivector<demo_polygon>& demo_polygons,
                                     bn::ivector<polygon_sprite>& demo_polygon_sprites)
    {
        demo_polygon* demo_polygons_data = demo_polygons.data() + demo_polygons.size();
        demo_polygons.emplace_back(x, y);
        demo_polygons.emplace_back(x, y + 65);

        const polygon* demo_polygons_array[] = {
            demo_polygons_data,
            demo_polygons_data + 1
        };

        int z_order = demo_polygon_sprites.size() + 1;
        demo_polygon_sprites.emplace_back(demo_polygons_array, z_order, z_order);
    }
}

int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(8, 8, 8));

    bn::sprite_text_generator big_text_generator(common::variable_8x16_sprite_font);
    common::info info(info_text_lines, big_text_generator);

    bn::sprite_text_generator small_text_generator(common::variable_8x8_sprite_font);
    common::stats stats(small_text_generator);

    const bn::fixed_point vertices[] = {
        bn::fixed_point(120 - 31, 1),
        bn::fixed_point(120 + 31, 30),
        bn::fixed_point(120 + 24, 159),
        bn::fixed_point(120 - 24, 128)
    };

    polygon user_polygon(vertices);
    polygon_sprite user_polygon_sprite(user_polygon, 0, 0);

    bn::random random;
    constexpr int demo_polygons_count = 16;
    bn::unique_ptr<bn::vector<demo_polygon, demo_polygons_count>> demo_polygons(
                new bn::vector<demo_polygon, demo_polygons_count>());
    bn::unique_ptr<bn::vector<polygon_sprite, demo_polygons_count / 2>> demo_polygon_sprites(
                new bn::vector<polygon_sprite, demo_polygons_count / 2>());
    bn::fixed first_y = 1;
    bn::fixed second_y = 159 - 65 - 64;
    _create_demo_polygon_sprite(0, first_y, *demo_polygons, *demo_polygon_sprites);
    _create_demo_polygon_sprite(16, second_y, *demo_polygons, *demo_polygon_sprites);
    _create_demo_polygon_sprite((16 * 2), first_y, *demo_polygons, *demo_polygon_sprites);
    _create_demo_polygon_sprite((16 * 3), second_y, *demo_polygons, *demo_polygon_sprites);
    _create_demo_polygon_sprite(240 - 64 - (16 * 3), first_y, *demo_polygons, *demo_polygon_sprites);
    _create_demo_polygon_sprite(240 - 64 - (16 * 2), second_y, *demo_polygons, *demo_polygon_sprites);
    _create_demo_polygon_sprite(240 - 64 - 16, first_y, *demo_polygons, *demo_polygon_sprites);
    _create_demo_polygon_sprite(240 - 64, second_y, *demo_polygons, *demo_polygon_sprites);

    const int max_polygon_sprites = (demo_polygons_count / 2) + 1;
    using hdma_source_array = bn::array<uint16_t, bn::display::height() * 4 * max_polygon_sprites>;
    bn::unique_ptr<hdma_source_array> hdma_source_ptr(new hdma_source_array());
    hdma_source_array& hdma_source = *hdma_source_ptr;
    uint16_t* hdma_source_data = hdma_source.data();
    bn::hdma::start(hdma_source, bn::hw::sprites::vram()[128 - max_polygon_sprites].attr0);

    while(true)
    {
        if(bn::keypad::l_held())
        {
            _move_vertex(0, user_polygon);
        }
        else if(bn::keypad::r_held())
        {
            _move_vertex(1, user_polygon);
        }
        else if(bn::keypad::a_held())
        {
            _move_vertex(2, user_polygon);
        }
        else if(bn::keypad::b_held())
        {
            _move_vertex(3, user_polygon);
        }

        for(demo_polygon& demo_polygon : *demo_polygons)
        {
            demo_polygon.update(random);
        }

        for(polygon_sprite& demo_polygon_sprite : *demo_polygon_sprites)
        {
            demo_polygon_sprite.update(max_polygon_sprites, hdma_source_data);
        }

        user_polygon_sprite.update(max_polygon_sprites, hdma_source_data);

        info.update();
        stats.update();
        bn::core::update();
    }
}
