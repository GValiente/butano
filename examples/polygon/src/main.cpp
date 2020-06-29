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

namespace
{
    constexpr const btn::string_view info_text_lines[] = {
        "Polygon rendering with sprites",
        "",
        "PAD: move the polygon",
        "L + PAD: move top left vertex",
        "R + PAD: move top right vertex",
        "B + PAD: move bottom left vertex",
        "A + PAD: move bottom right vertex",
        /*"",
        "START: show more polygons :D",*/
    };

    void _move_vertex(int vertex_index, polygon& polygon)
    {
        btn::fixed_point vertex = polygon.vertex(vertex_index);

        if(btn::keypad::left_held())
        {
            vertex.set_x(btn::max(vertex.x() - 1, btn::fixed(-32)));
        }
        else if(btn::keypad::right_held())
        {
            vertex.set_x(btn::min(vertex.x() + 1, btn::fixed(32)));
        }

        if(btn::keypad::up_held())
        {
            vertex.set_y(btn::max(vertex.y() - 1, btn::fixed(-32)));
        }
        else if(btn::keypad::down_held())
        {
            vertex.set_y(btn::min(vertex.y() + 1, btn::fixed(32)));
        }

        polygon.set_vertex(vertex_index, vertex);
    }
}

int main()
{
    btn::core::init();
    btn::bg_palettes::set_transparent_color(btn::color(8, 8, 8));

    btn::sprite_text_generator big_text_generator(variable_8x16_sprite_font);
    info info(info_text_lines, big_text_generator);

    /*btn::sprite_text_generator small_text_generator(variable_8x8_sprite_font);
    stats stats(small_text_generator);*/

    const btn::fixed_point vertices[] = {
        btn::fixed_point(-16, -32),
        btn::fixed_point(32, -32),
        btn::fixed_point(32, 16),
        btn::fixed_point(-32, 32)
    };

    polygon polygon(vertices, 0, 0, 0, 0);

    /*btn::random random;
    btn::unique_ptr<btn::vector<demo_polygon, 6>> demo_polygons(new btn::vector<demo_polygon, 6>());*/

    while(true)
    {
        if(btn::keypad::l_held())
        {
            _move_vertex(0, polygon);
        }
        else if(btn::keypad::r_held())
        {
            _move_vertex(1, polygon);
        }
        else if(btn::keypad::a_held())
        {
            _move_vertex(2, polygon);
        }
        else if(btn::keypad::b_held())
        {
            _move_vertex(3, polygon);
        }
        else
        {
            if(btn::keypad::left_held())
            {
                polygon.set_x(btn::max(polygon.x() - btn::fixed(1), btn::fixed(-120 + 32)));
            }
            else if(btn::keypad::right_held())
            {
                polygon.set_x(btn::min(polygon.x() + btn::fixed(1), btn::fixed(120 - 32)));
            }

            if(btn::keypad::up_held())
            {
                polygon.set_y(btn::max(polygon.y() - btn::fixed(1), btn::fixed(-80 + 32)));
            }
            else if(btn::keypad::down_held())
            {
                polygon.set_y(btn::min(polygon.y() + btn::fixed(1), btn::fixed(80 - 32)));
            }
        }

        /*if(btn::keypad::start_pressed())
        {
            if(demo_polygons->empty())
            {
                btn::fixed base_x = -120 + 32;
                btn::fixed x = base_x;
                btn::fixed y = -80 + 32;
                demo_polygons->emplace_back(vertices, x, y, 1, 1);
                x += 64;
                demo_polygons->emplace_back(vertices, x, y, 2, 1);
                x += 64;
                demo_polygons->emplace_back(vertices, x, y, 3, 1);
                x = base_x;
                y += 48;
                demo_polygons->emplace_back(vertices, x, y, 4, 1);
                x += 64;
                demo_polygons->emplace_back(vertices, x, y, 5, 1);
                x += 64;
                demo_polygons->emplace_back(vertices, x, y, 6, 1);
            }
            else
            {
                demo_polygons->clear();
            }
        }

        for(demo_polygon& demo_polygon : *demo_polygons)
        {
            demo_polygon.update(random);
        }*/

        polygon.update();
        info.update();
        /*stats.update();*/
        btn::core::update();
    }
}
