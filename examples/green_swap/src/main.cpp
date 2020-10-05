#include "btn_core.h"
#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_regular_bg_ptr.h"
#include "btn_green_swap_actions.h"
#include "btn_sprite_text_generator.h"
#include "btn_green_swap_hblank_effect_ptr.h"

#include "btn_regular_bg_items_village.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void green_swap_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: toggle green swap",
            "",
            "START: go to next scene",
        };

        info info("Green swap", info_text_lines, text_generator);

        btn::green_swap::set_enabled(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                btn::green_swap::set_enabled(! btn::green_swap::enabled());
            }

            info.update();
            btn::core::update();
        }

        btn::green_swap::set_enabled(false);
    }

    void green_swap_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Green swap actions", info_text_lines, text_generator);

        btn::green_swap_toggle_action action(60);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }

        btn::green_swap::set_enabled(false);
    }

    void green_swap_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Green swap H-Blank effect", info_text_lines, text_generator);

        btn::array<bool, btn::display::height()> green_swap_states;
        btn::green_swap_hblank_effect_ptr green_swap_hblank_effect =
                btn::green_swap_hblank_effect_ptr::create(green_swap_states);

        int green_swap_lines = 64;
        int first_green_swap_line_index = 0;
        int green_swap_line_index_inc = 1;

        while(! btn::keypad::start_pressed())
        {
            if(first_green_swap_line_index == 0)
            {
                green_swap_line_index_inc = 1;
            }
            else if(first_green_swap_line_index == btn::display::height() - green_swap_lines)
            {
                green_swap_line_index_inc = -1;
            }

            first_green_swap_line_index += green_swap_line_index_inc;
            green_swap_states.fill(false);

            for(int index = 0; index < green_swap_lines; ++index)
            {
                green_swap_states[first_green_swap_line_index + index] = true;
            }

            green_swap_hblank_effect.reload_states_ref();

            info.update();
            btn::core::update();
        }
    }
}

int main()
{
    btn::core::init();

    btn::regular_bg_ptr village_bg = btn::regular_bg_items::village.create_bg(0, 0);
    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);

    while(true)
    {
        green_swap_scene(text_generator);
        btn::core::update();

        green_swap_actions_scene(text_generator);
        btn::core::update();

        green_swap_hblank_effect_scene(text_generator);
        btn::core::update();
    }
}
