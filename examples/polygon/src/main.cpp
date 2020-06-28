#include "btn_core.h"

#include "btn_color.h"
#include "btn_keypad.h"
#include "btn_random.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_items_hero_shield.h"
#include "btn_sprite_items_tank_cannon.h"
#include "btn_sprite_items_stage_1_intro.h"
#include "btn_sprite_items_stage_1_characters_1_team.h"
#include "btn_sprite_items_texture.h"
#include "btn_sprite_items_texture_2.h"
#include "btn_sprite_items_tileset_1.h"
#include "btn_sprite_items_tileset_2.h"
#include "btn_sprite_items_tileset_3.h"
#include "btn_sprite_items_tileset_4.h"
#include "btn_sprite_items_tileset_5.h"
#include "btn_sprite_items_tileset_6.h"
#include "btn_sprite_items_tileset_7.h"
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
        "R + PAD: scale the polygon",
        "B and A: change polygon's panning",
        "",
        "START: show more polygons :D",
    };
}

int main()
{
    btn::core::init();
    btn::bg_palettes::set_transparent_color(btn::color(8, 8, 8));

    btn::sprite_text_generator big_text_generator(variable_8x16_sprite_font);
    info info(info_text_lines, big_text_generator);

    btn::sprite_text_generator small_text_generator(variable_8x8_sprite_font);
    stats stats(small_text_generator);

    polygon polygon(btn::sprite_items::texture, 0, 0, 0);
    polygon.set_panning(-0.5);

    btn::random random;
    btn::vector<demo_polygon, 16> demo_polygons;

    while(true)
    {
        if(btn::keypad::b_held())
        {
            polygon.set_panning(btn::min(polygon.panning() + btn::fixed(0.005), btn::fixed(0.99)));
        }
        else if(btn::keypad::a_held())
        {
            polygon.set_panning(btn::max(polygon.panning() - btn::fixed(0.005), btn::fixed(-0.99)));
        }

        if(btn::keypad::r_held())
        {
            if(btn::keypad::left_held())
            {
                polygon.set_scale_x(btn::max(polygon.scale_x() - btn::fixed(0.005), btn::fixed(0.01)));
            }
            else if(btn::keypad::right_held())
            {
                polygon.set_scale_x(btn::min(polygon.scale_x() + btn::fixed(0.005), btn::fixed(1)));
            }

            if(btn::keypad::up_held())
            {
                polygon.set_scale_y(btn::min(polygon.scale_y() + btn::fixed(0.005), btn::fixed(1)));
            }
            else if(btn::keypad::down_held())
            {
                polygon.set_scale_y(btn::max(polygon.scale_y() - btn::fixed(0.005), btn::fixed(0.5)));
            }
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

        if(btn::keypad::start_pressed())
        {
            if(demo_polygons.empty())
            {
                btn::fixed base_x = -120 + 32;
                btn::fixed x = base_x;
                btn::fixed y = -80 + 32;
                demo_polygons.emplace_back(btn::sprite_items::hero_shield, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::tank_cannon, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::stage_1_intro, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::tileset_1, x, y, 1);
                x = base_x;
                y += 48;
                demo_polygons.emplace_back(btn::sprite_items::texture_2, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::tileset_6, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::tileset_2, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::stage_1_characters_1_team, x, y, 1);
                x = base_x;
                y += 48;
                demo_polygons.emplace_back(btn::sprite_items::stage_1_characters_1_team, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::tileset_1, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::tileset_7, x, y, 1);
                x += 64;
                demo_polygons.emplace_back(btn::sprite_items::texture_2, x, y, 1);
            }
            else
            {
                demo_polygons.clear();
            }
        }

        for(demo_polygon& demo_polygon : demo_polygons)
        {
            demo_polygon.update(random);
        }

        polygon.update();
        info.update();
        stats.update();
        btn::core::update();
    }
}
