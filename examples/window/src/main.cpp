#include "btn_core.h"
#include "btn_math.h"
#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_blending.h"
#include "btn_regular_bg_ptr.h"
#include "btn_rect_window_actions.h"
#include "btn_sprite_text_generator.h"
#include "btn_rect_window_boundaries_hblank_effect_ptr.h"

#include "btn_sprite_items_circle.h"
#include "btn_regular_bg_items_land.h"
#include "btn_regular_bg_items_clouds.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void window_scene(btn::regular_bg_ptr& clouds_bg, btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: toggle window visibility",
            "",
            "START: go to next scene",
        };

        info info("Window", info_text_lines, text_generator);

        btn::rect_window internal_window = btn::rect_window::internal();
        internal_window.set_boundaries(-48, -96, 48, 96);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                internal_window.set_visible(! internal_window.visible());
            }

            clouds_bg.set_position(clouds_bg.x() + 0.5, clouds_bg.y() + 0.5);
            info.update();
            btn::core::update();
        }

        internal_window.set_boundaries(0, 0, 0, 0);
        internal_window.set_visible(true);
        clouds_bg.set_position(0, 0);
    }

    void window_boundaries_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "PAD: move window boundaries",
            "",
            "START: go to next scene",
        };

        info info("Window boundaries", info_text_lines, text_generator);

        btn::rect_window internal_window = btn::rect_window::internal();
        btn::fixed_point internal_window_position;

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::left_held())
            {
                internal_window_position.set_x(internal_window_position.x() - 1);
            }
            else if(btn::keypad::right_held())
            {
                internal_window_position.set_x(internal_window_position.x() + 1);
            }

            if(btn::keypad::up_held())
            {
                internal_window_position.set_y(internal_window_position.y() - 1);
            }
            else if(btn::keypad::down_held())
            {
                internal_window_position.set_y(internal_window_position.y() + 1);
            }

            internal_window.set_boundaries(internal_window_position.y() - 48, internal_window_position.x() - 48,
                                           internal_window_position.y() + 48, internal_window_position.x() + 48);
            info.update();
            btn::core::update();
        }

        internal_window.set_boundaries(0, 0, 0, 0);
    }

    void sprite_window_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "PAD: move circle sprite",
            "",
            "START: go to next scene",
        };

        info info("Sprite window", info_text_lines, text_generator);

        btn::sprite_ptr circle_sprite = btn::sprite_items::circle.create_sprite(0, 0);
        circle_sprite.set_window_enabled(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::left_held())
            {
                circle_sprite.set_x(circle_sprite.x() - 1);
            }
            else if(btn::keypad::right_held())
            {
                circle_sprite.set_x(circle_sprite.x() + 1);
            }

            if(btn::keypad::up_held())
            {
                circle_sprite.set_y(circle_sprite.y() - 1);
            }
            else if(btn::keypad::down_held())
            {
                circle_sprite.set_y(circle_sprite.y() + 1);
            }

            info.update();
            btn::core::update();
        }
    }

    void window_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Window actions", info_text_lines, text_generator);

        btn::rect_window internal_window = btn::rect_window::internal();
        internal_window.set_boundaries(-48, -96, 48, 96);

        btn::rect_window_move_top_loop_action move_top_action(internal_window, 120, 0);
        btn::rect_window_move_left_loop_action move_left_action(internal_window, 120, 0);
        btn::rect_window_move_bottom_loop_action move_bottom_action(internal_window, 120, 0);
        btn::rect_window_move_right_loop_action move_right_action(internal_window, 120, 0);

        while(! btn::keypad::start_pressed())
        {
            move_top_action.update();
            move_left_action.update();
            move_bottom_action.update();
            move_right_action.update();
            info.update();
            btn::core::update();
        }

        internal_window.set_boundaries(0, 0, 0, 0);
    }

    void window_hblank_effect_scene(btn::regular_bg_ptr& clouds_bg, btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Window H-Blank effect", info_text_lines, text_generator);

        btn::rect_window internal_window = btn::rect_window::internal();
        int amplitude = 56;
        internal_window.set_top(-amplitude);
        internal_window.set_bottom(amplitude);

        btn::array<btn::pair<btn::fixed, btn::fixed>, btn::display::height()> horizontal_boundaries;
        btn::rect_window_boundaries_hblank_effect_ptr horizontal_hblank_effect =
                btn::rect_window_boundaries_hblank_effect_ptr::create_horizontal(internal_window, horizontal_boundaries);
        btn::fixed base_degrees_angle;

        while(! btn::keypad::start_pressed())
        {
            base_degrees_angle += 4;

            if(base_degrees_angle >= 360)
            {
                base_degrees_angle -= 360;
            }

            btn::fixed degrees_angle = base_degrees_angle;

            for(int index = 0; index < amplitude; ++index)
            {
                degrees_angle += 16;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                btn::fixed desp = btn::degrees_sin(degrees_angle) * 8;
                btn::fixed stretch = amplitude - index;
                btn::pair<btn::fixed, btn::fixed> left_right(desp - stretch, desp + stretch);
                horizontal_boundaries[(btn::display::height() / 2) + index] = left_right;
                horizontal_boundaries[(btn::display::height() / 2) - index - 1] = left_right;
            }

            horizontal_hblank_effect.reload_deltas_ref();
            clouds_bg.set_position(clouds_bg.x() + 0.5, clouds_bg.y() + 0.5);
            info.update();
            btn::core::update();
        }

        internal_window.set_top(0);
        internal_window.set_bottom(0);
        clouds_bg.set_position(0, 0);
    }
}

int main()
{
    btn::core::init();

    btn::regular_bg_ptr land_bg = btn::regular_bg_items::land.create_bg(0, 0);
    btn::regular_bg_ptr clouds_bg = btn::regular_bg_items::clouds.create_bg(0, 0);

    btn::window outside_window = btn::window::outside();
    outside_window.set_show_bg(clouds_bg, false);

    btn::blending::set_transparency_alpha(0.5);
    clouds_bg.set_blending_enabled(true);

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);

    while(true)
    {
        window_scene(clouds_bg, text_generator);
        btn::core::update();

        window_boundaries_scene(text_generator);
        btn::core::update();

        sprite_window_scene(text_generator);
        btn::core::update();

        window_actions_scene(text_generator);
        btn::core::update();

        window_hblank_effect_scene(clouds_bg, text_generator);
        btn::core::update();
    }
}
