/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"

#include "btn_color.h"
#include "btn_keypad.h"
#include "btn_random.h"
#include "btn_optional.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_text_generator.h"
#include "info.h"
#include "stats.h"
#include "variable_8x8_sprite_font.h"
#include "variable_8x16_sprite_font.h"
#include "demo_polygon.h"
#include "polygon_sprite.h"

namespace
{
    constexpr const btn::string_view info_text_lines[] = {
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
        btn::fixed_point& vertex = polygon.vertices()[vertex_index];

        if(btn::keypad::left_held())
        {
            if(vertex_index == 0 || vertex_index == 3)
            {
                vertex.set_x(btn::max(vertex.x() - 1, btn::fixed(120 - 31)));
            }
            else
            {
                vertex.set_x(btn::max(vertex.x() - 1, btn::fixed(120)));
            }

            polygon_sprite.reload_polygons();
        }
        else if(btn::keypad::right_held())
        {
            if(vertex_index == 0 || vertex_index == 3)
            {
                vertex.set_x(btn::min(vertex.x() + 1, btn::fixed(120)));
            }
            else
            {
                vertex.set_x(btn::min(vertex.x() + 1, btn::fixed(120 + 31)));
            }

            polygon_sprite.reload_polygons();
        }

        if(btn::keypad::up_held())
        {
            if(vertex_index == 0 || vertex_index == 1)
            {
                vertex.set_y(btn::max(vertex.y() - 1, btn::fixed(1)));
            }
            else
            {
                vertex.set_y(btn::max(vertex.y() - 1, btn::fixed(80)));
            }

            polygon_sprite.reload_polygons();
        }
        else if(btn::keypad::down_held())
        {
            if(vertex_index == 0 || vertex_index == 1)
            {
                vertex.set_y(btn::min(vertex.y() + 1, btn::fixed(80)));
            }
            else
            {
                vertex.set_y(btn::min(vertex.y() + 1, btn::fixed(159)));
            }

            polygon_sprite.reload_polygons();
        }
    }

    void _create_demo_polygon_sprite(btn::fixed x, btn::ivector<demo_polygon>& demo_polygons,
                                     btn::ivector<polygon_sprite>& demo_polygon_sprites)
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
    btn::core::init();
    btn::bg_palettes::set_transparent_color(btn::color(8, 8, 8));

    btn::sprite_text_generator big_text_generator(variable_8x16_sprite_font);
    info info(info_text_lines, big_text_generator);

    btn::sprite_text_generator small_text_generator(variable_8x8_sprite_font);
    stats stats(small_text_generator);

    const btn::fixed_point vertices[] = {
        btn::fixed_point(120 - 31, 1),
        btn::fixed_point(120 + 31, 30),
        btn::fixed_point(120 + 24, 159),
        btn::fixed_point(120 - 24, 128)
    };

    polygon user_polygon(vertices);
    polygon_sprite user_polygon_sprite(user_polygon, 0, 0);

    btn::random random;
    btn::vector<demo_polygon, 6> demo_polygons;
    btn::unique_ptr<btn::vector<polygon_sprite, 3>> demo_polygon_sprites(new btn::vector<polygon_sprite, 3>());
    bool demo_shown = false;

    while(true)
    {
        if(btn::keypad::l_held())
        {
            _move_vertex(0, user_polygon, user_polygon_sprite);
        }
        else if(btn::keypad::r_held())
        {
            _move_vertex(1, user_polygon, user_polygon_sprite);
        }
        else if(btn::keypad::a_held())
        {
            _move_vertex(2, user_polygon, user_polygon_sprite);
        }
        else if(btn::keypad::b_held())
        {
            _move_vertex(3, user_polygon, user_polygon_sprite);
        }

        if(! demo_shown || btn::keypad::start_pressed())
        {
            if(demo_polygons.empty())
            {
                _create_demo_polygon_sprite(16, demo_polygons, *demo_polygon_sprites);
                _create_demo_polygon_sprite(btn::display::width() - 64 - 8 - 32, demo_polygons, *demo_polygon_sprites);
                _create_demo_polygon_sprite(btn::display::width() - 64 - 8, demo_polygons, *demo_polygon_sprites);
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
        btn::core::update();
    }
}
