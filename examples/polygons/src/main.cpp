/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"

#include "bn_color.h"
#include "bn_keypad.h"
#include "bn_random.h"
#include "bn_unique_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "demo_polygon.h"
#include "polygon_sprite.h"

#include "common_info.h"
#include "common_stats.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    constexpr bn::string_view info_text_lines[] = {
        "",
        "",
        "Polygons rendering with sprites",
        "",
        "L + PAD: move top left vertex",
        "R + PAD: move top right vertex",
        "B + PAD: move bottom left vertex",
        "A + PAD: move bottom right vertex",
        "START: hide/show back polygons",
    };

    void _move_vertex(int vertex_index, polygon& polygon, polygon_sprite& polygon_sprite)
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

            polygon_sprite.reload_polygons();
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

            polygon_sprite.reload_polygons();
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

            polygon_sprite.reload_polygons();
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

            polygon_sprite.reload_polygons();
        }
    }

    void _create_demo_polygon_sprite(bn::fixed x, bn::ivector<demo_polygon>& demo_polygons,
                                     bn::ivector<polygon_sprite>& demo_polygon_sprites)
    {
        demo_polygon* demo_polygons_data = demo_polygons.data() + demo_polygons.size();
        demo_polygons.emplace_back(x, 1);
        demo_polygons.emplace_back(x, 81);

        const polygon* demo_polygons_array[] = {
            demo_polygons_data,
            demo_polygons_data + 1
        };

        demo_polygon_sprites.emplace_back(demo_polygons_array, demo_polygon_sprites.size() + 1, 1);
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
    bn::vector<demo_polygon, 4> demo_polygons;
    bn::unique_ptr<bn::vector<polygon_sprite, 2>> demo_polygon_sprites(new bn::vector<polygon_sprite, 2>());
    bool demo_shown = false;

    while(true)
    {
        if(bn::keypad::l_held())
        {
            _move_vertex(0, user_polygon, user_polygon_sprite);
        }
        else if(bn::keypad::r_held())
        {
            _move_vertex(1, user_polygon, user_polygon_sprite);
        }
        else if(bn::keypad::a_held())
        {
            _move_vertex(2, user_polygon, user_polygon_sprite);
        }
        else if(bn::keypad::b_held())
        {
            _move_vertex(3, user_polygon, user_polygon_sprite);
        }

        if(! demo_shown || bn::keypad::start_pressed())
        {
            if(demo_polygons.empty())
            {
                _create_demo_polygon_sprite(16, demo_polygons, *demo_polygon_sprites);
                _create_demo_polygon_sprite(bn::display::width() - 64 - 16, demo_polygons, *demo_polygon_sprites);
                demo_shown = true;
            }
            else
            {
                demo_polygons.clear();
                demo_polygon_sprites->clear();
            }
        }

        for(demo_polygon& demo_polygon : demo_polygons)
        {
            demo_polygon.update(random);
        }

        for(polygon_sprite& demo_polygon_sprite : *demo_polygon_sprites)
        {
            demo_polygon_sprite.reload_polygons();
            demo_polygon_sprite.update();
        }

        user_polygon_sprite.update();
        info.update();
        stats.update();
        bn::core::update();
    }
}
