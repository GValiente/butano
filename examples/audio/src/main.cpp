/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"
#include "btn_keypad.h"
#include "btn_optional.h"
#include "btn_bg_palettes.h"
#include "btn_music_actions.h"
#include "btn_sprite_actions.h"
#include "btn_sprite_text_generator.h"

#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_sprite_items_bullet.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void music_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease volume",
            "RIGHT: increase volume",
            "A: pause/resume music",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        info info("Music", info_text_lines, text_generator);
        info.set_show_always(true);

        btn::music_items::cyberrid.play(0.5);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed volume = btn::music::volume();

            if(btn::keypad::left_held())
            {
                btn::music::set_volume(btn::max(volume - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::music::set_volume(btn::min(volume + 0.01, btn::fixed(1)));
            }

            if(btn::keypad::a_pressed())
            {
                if(btn::music::paused())
                {
                    btn::music::resume();
                }
                else
                {
                    btn::music::pause();
                }
            }

            info.update();
            btn::core::update();
        }

        btn::music::stop();
    }

    void music_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Music actions", info_text_lines, text_generator);
        info.set_show_always(true);

        btn::music_items::cyberrid.play(0);

        btn::music_volume_loop_action action(120, 1);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }

        btn::music::stop();
    }

    void sound_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: play sound",
            "",
            "",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        info info("Sound", info_text_lines, text_generator);
        info.set_show_always(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                btn::sound_items::alert.play(0.5);
            }

            info.update();
            btn::core::update();
        }
    }

    void sound_panning_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sound panning", info_text_lines, text_generator);
        info.set_show_always(true);

        int sprite_x_range = 96;
        btn::sprite_ptr sprite = btn::sprite_items::bullet.create_sprite(-sprite_x_range, 0);
        btn::sprite_move_loop_action bullet_sprite_move_action(sprite, sprite_x_range * 4, sprite_x_range, 0);
        int sound_counter = 1;

        while(! btn::keypad::start_pressed())
        {
            --sound_counter;

            if(! sound_counter)
            {
                btn::fixed sound_panning = sprite.x() / sprite_x_range;
                btn::sound_items::alert.play(0.5, 1, sound_panning);
                sound_counter = 64;
            }

            bullet_sprite_move_action.update();
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
        music_scene(text_generator);
        btn::core::update();

        music_actions_scene(text_generator);
        btn::core::update();

        sound_scene(text_generator);
        btn::core::update();

        sound_panning_scene(text_generator);
        btn::core::update();
    }
}
