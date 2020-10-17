#include "btn_core.h"
#include "btn_math.h"
#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_blending.h"
#include "btn_bgs_mosaic.h"
#include "btn_bg_palettes.h"
#include "btn_regular_bg_actions.h"
#include "btn_regular_bg_builder.h"
#include "btn_regular_bg_attributes.h"
#include "btn_sprite_text_generator.h"
#include "btn_regular_bg_position_hblank_effect_ptr.h"
#include "btn_regular_bg_attributes_hblank_effect_ptr.h"

#include "btn_sprite_items_turtle.h"
#include "btn_regular_bg_items_red.h"
#include "btn_regular_bg_items_blue.h"
#include "btn_regular_bg_items_green.h"
#include "btn_regular_bg_items_yellow.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void regular_bgs_visibility_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: hide/show BG",
            "",
            "START: go to next scene",
        };

        info info("Regular BGs visibility", info_text_lines, text_generator);

        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red.create_bg(0, 0);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                red_bg.set_visible(! red_bg.visible());
            }

            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_visibility_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Regular BGs visibility actions", info_text_lines, text_generator);

        btn::regular_bg_ptr green_bg = btn::regular_bg_items::green.create_bg(0, 0);
        btn::regular_bg_visible_toggle_action action(green_bg, 60);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_position_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "PAD: move BG",
            "",
            "START: go to next scene",
        };

        info info("Regular BGs position", info_text_lines, text_generator);

        btn::regular_bg_ptr blue_bg = btn::regular_bg_items::blue.create_bg(0, 0);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::left_held())
            {
                blue_bg.set_x(blue_bg.x() - 1);
            }
            else if(btn::keypad::right_held())
            {
                blue_bg.set_x(blue_bg.x() + 1);
            }

            if(btn::keypad::up_held())
            {
                blue_bg.set_y(blue_bg.y() - 1);
            }
            else if(btn::keypad::down_held())
            {
                blue_bg.set_y(blue_bg.y() + 1);
            }

            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_position_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Regular BGs position actions", info_text_lines, text_generator);

        btn::fixed amplitude = 30;
        btn::regular_bg_ptr yellow_bg = btn::regular_bg_items::yellow.create_bg(-amplitude, -amplitude);
        btn::regular_bg_move_loop_action action(yellow_bg, 120, amplitude, amplitude);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_position_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Regular BGs position H-Blank effect", info_text_lines, text_generator);

        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red.create_bg(0, 0);

        btn::array<btn::fixed, btn::display::height()> horizontal_deltas;

        btn::regular_bg_position_hblank_effect_ptr horizontal_hblank_effect =
                btn::regular_bg_position_hblank_effect_ptr::create_horizontal(red_bg, horizontal_deltas);

        btn::fixed base_degrees_angle;

        while(! btn::keypad::start_pressed())
        {
            base_degrees_angle += 4;

            if(base_degrees_angle >= 360)
            {
                base_degrees_angle -= 360;
            }

            btn::fixed degrees_angle = base_degrees_angle;

            for(int index = 0, limit = btn::display::height() / 2; index < limit; ++index)
            {
                degrees_angle += 16;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                btn::fixed desp = btn::degrees_sin(degrees_angle) * 8;
                horizontal_deltas[(btn::display::height() / 2) + index] = desp;
                horizontal_deltas[(btn::display::height() / 2) - index - 1] = desp;
            }

            horizontal_hblank_effect.reload_deltas_ref();
            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_priority_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease red BG priority",
            "RIGHT: increase red BG priority",
            "",
            "START: go to next scene",
        };

        info info("Regular BGs priority", info_text_lines, text_generator);

        btn::regular_bg_ptr green_bg = btn::regular_bg_items::green.create_bg(-64, -64);
        green_bg.set_priority(0);

        btn::regular_bg_ptr blue_bg = btn::regular_bg_items::blue.create_bg(-64, 64);
        blue_bg.set_priority(1);

        btn::regular_bg_ptr yellow_bg = btn::regular_bg_items::yellow.create_bg(64, 64);
        yellow_bg.set_priority(2);

        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red.create_bg(0, 0);
        red_bg.set_priority(2);

        btn::sprite_ptr turtle_sprite = btn::sprite_items::turtle.create_sprite(64, -32);
        turtle_sprite.set_bg_priority(2);

        while(! btn::keypad::start_pressed())
        {
            int priority = red_bg.priority();

            if(btn::keypad::left_pressed())
            {
                red_bg.set_priority(btn::max(priority - 1, 0));
            }
            else if(btn::keypad::right_pressed())
            {
                red_bg.set_priority(btn::min(priority + 1, 3));
            }

            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_z_order_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease red BG Z order",
            "RIGHT: increase red BG Z order",
            "",
            "START: go to next scene",
        };

        info info("Regular BGs Z order", info_text_lines, text_generator);

        btn::regular_bg_ptr green_bg = btn::regular_bg_items::green.create_bg(-64, -64);
        green_bg.set_z_order(0);

        btn::regular_bg_ptr blue_bg = btn::regular_bg_items::blue.create_bg(-64, 64);
        blue_bg.set_z_order(1);

        btn::regular_bg_ptr yellow_bg = btn::regular_bg_items::yellow.create_bg(64, 64);
        yellow_bg.set_z_order(2);

        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red.create_bg(0, 0);
        red_bg.set_z_order(2);

        btn::sprite_ptr turtle_sprite = btn::sprite_items::turtle.create_sprite(64, -32);
        turtle_sprite.set_bg_priority(2);

        while(! btn::keypad::start_pressed())
        {
            int z_order = red_bg.z_order();

            if(btn::keypad::left_pressed())
            {
                red_bg.set_z_order(btn::max(z_order - 1, 0));
            }
            else if(btn::keypad::right_pressed())
            {
                red_bg.set_z_order(btn::min(z_order + 1, 3));
            }

            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_put_above_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "B: put green BG above red BG",
            "A: put blue BG above red BG",
            "L: put yellow BG above red BG",
            "R: put red BG above all",
            "",
            "START: go to next scene",
        };

        info info("Regular BGs put above", info_text_lines, text_generator);

        btn::regular_bg_ptr green_bg = btn::regular_bg_items::green.create_bg(-64, -64);
        btn::regular_bg_ptr blue_bg = btn::regular_bg_items::blue.create_bg(-64, 64);
        btn::regular_bg_ptr yellow_bg = btn::regular_bg_items::yellow.create_bg(64, 64);
        btn::optional<btn::regular_bg_ptr> red_bg = btn::regular_bg_items::red.create_bg(0, 0);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::b_pressed())
            {
                green_bg.put_above(*red_bg);
            }

            if(btn::keypad::a_pressed())
            {
                blue_bg.put_above(*red_bg);
            }

            if(btn::keypad::l_pressed())
            {
                yellow_bg.put_above(*red_bg);
            }

            if(btn::keypad::r_pressed())
            {
                red_bg.reset();
                red_bg = btn::regular_bg_items::red.create_bg(0, 0);
            }

            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_attributes_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "B: copy green BG attribs. to red BG",
            "A: copy blue BG attribs. to red BG",
            "L: copy yellow BG attribs. to red BG",
            "R: restore red BG attributes",
            "",
            "START: go to next scene",
        };

        info info("Regular BGs attributes", info_text_lines, text_generator);

        btn::regular_bg_ptr green_bg = btn::regular_bg_items::green.create_bg(-64, -64);
        btn::regular_bg_ptr blue_bg = btn::regular_bg_items::blue.create_bg(-64, 64);
        btn::regular_bg_ptr yellow_bg = btn::regular_bg_items::yellow.create_bg(64, 64);
        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red.create_bg(0, 0);
        btn::regular_bg_attributes red_bg_attributes = red_bg.attributes();

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::b_pressed())
            {
                red_bg.set_attributes(green_bg.attributes());
            }

            if(btn::keypad::a_pressed())
            {
                red_bg.set_attributes(blue_bg.attributes());
            }

            if(btn::keypad::l_pressed())
            {
                red_bg.set_attributes(yellow_bg.attributes());
            }

            if(btn::keypad::r_pressed())
            {
                red_bg.set_attributes(red_bg_attributes);
            }

            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_attributes_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Regular BGs attributes H-Blank effect", info_text_lines, text_generator);

        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red.create_bg(0, 0);
        btn::regular_bg_map_ptr green_map = btn::regular_bg_items::green.create_bg(0, 0).map();
        btn::regular_bg_attributes red_attributes = red_bg.attributes();
        btn::regular_bg_attributes green_attributes = red_attributes;
        green_attributes.set_map(green_map);

        btn::vector<btn::regular_bg_attributes, btn::display::height()> attributes;

        for(int index = 0; index < btn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(green_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        btn::span<const btn::regular_bg_attributes> attributes_ref(attributes.data(), attributes.size());
        btn::regular_bg_attributes_hblank_effect_ptr hblank_effect =
                btn::regular_bg_attributes_hblank_effect_ptr::create(red_bg, attributes_ref);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }

    void regular_bgs_builder_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Regular BGs builder", info_text_lines, text_generator);

        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red.create_bg(0, 0);
        btn::bgs_mosaic::set_stretch(0.2);
        btn::blending::set_transparency_alpha(0.6);

        btn::regular_bg_builder builder(btn::regular_bg_items::blue);
        builder.set_position(30, 10);
        builder.set_mosaic_enabled(true);
        builder.set_blending_enabled(true);

        btn::regular_bg_ptr blue_bg = builder.build();

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    while(true)
    {
        regular_bgs_visibility_scene(text_generator);
        btn::core::update();

        regular_bgs_visibility_actions_scene(text_generator);
        btn::core::update();

        regular_bgs_position_scene(text_generator);
        btn::core::update();

        regular_bgs_position_actions_scene(text_generator);
        btn::core::update();

        regular_bgs_position_hblank_effect_scene(text_generator);
        btn::core::update();

        regular_bgs_priority_scene(text_generator);
        btn::core::update();

        regular_bgs_z_order_scene(text_generator);
        btn::core::update();

        regular_bgs_put_above_scene(text_generator);
        btn::core::update();

        regular_bgs_attributes_scene(text_generator);
        btn::core::update();

        regular_bgs_attributes_hblank_effect_scene(text_generator);
        btn::core::update();

        regular_bgs_builder_scene(text_generator);
        btn::core::update();
    }
}
