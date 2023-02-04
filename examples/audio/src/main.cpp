/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_music_actions.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_text_generator.h"

#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_bullet.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void music_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease volume",
            "RIGHT: increase volume",
            "A: pause/resume music",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Music", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::music_items::cyberrid.play(0.5);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed volume = bn::music::volume();

            if(bn::keypad::left_held())
            {
                bn::music::set_volume(bn::max(volume - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::music::set_volume(bn::min(volume + 0.01, bn::fixed(1)));
            }

            if(bn::keypad::a_pressed())
            {
                if(bn::music::paused())
                {
                    bn::music::resume();
                }
                else
                {
                    bn::music::pause();
                }
            }

            info.update();
            bn::core::update();
        }

        bn::music::stop();
    }

    void music_tempo_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease tempo",
            "RIGHT: increase tempo",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Music tempo", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::music_items::cyberrid.play(0.5);
        bn::music::set_tempo(0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed tempo = bn::music::tempo();

            if(bn::keypad::left_held())
            {
                bn::music::set_tempo(bn::max(tempo - 0.01, bn::fixed(0.5)));
            }
            else if(bn::keypad::right_held())
            {
                bn::music::set_tempo(bn::min(tempo + 0.01, bn::fixed(2)));
            }

            info.update();
            bn::core::update();
        }

        bn::music::stop();
    }

    void music_pitch_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease pitch",
            "RIGHT: increase pitch",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Music pitch", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::music_items::cyberrid.play(0.5);
        bn::music::set_pitch(0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed pitch = bn::music::pitch();

            if(bn::keypad::left_held())
            {
                bn::music::set_pitch(bn::max(pitch - 0.01, bn::fixed(0.5)));
            }
            else if(bn::keypad::right_held())
            {
                bn::music::set_pitch(bn::min(pitch + 0.01, bn::fixed(2)));
            }

            info.update();
            bn::core::update();
        }

        bn::music::stop();
    }

    void music_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Music actions", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::music_items::cyberrid.play(0);

        bn::music_volume_loop_action action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::music::stop();
    }

    void sound_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: play sound",
            "",
            "",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Sound", info_text_lines, text_generator);
        info.set_show_always(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                bn::sound_items::alert.play(0.5);
            }

            info.update();
            bn::core::update();
        }
    }

    void sound_panning_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sound panning", info_text_lines, text_generator);
        info.set_show_always(true);

        int sprite_x_range = 96;
        bn::sprite_ptr sprite = bn::sprite_items::bullet.create_sprite(-sprite_x_range, 0);
        bn::sprite_move_loop_action bullet_sprite_move_action(sprite, sprite_x_range * 4, sprite_x_range, 0);
        int sound_counter = 1;

        while(! bn::keypad::start_pressed())
        {
            --sound_counter;

            if(! sound_counter)
            {
                bn::fixed sound_panning = sprite.x() / sprite_x_range;
                bn::sound_items::alert.play(0.5, 1, sound_panning);
                sound_counter = 64;
            }

            bullet_sprite_move_action.update();
            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        music_scene(text_generator);
        bn::core::update();

        music_tempo_scene(text_generator);
        bn::core::update();

        music_pitch_scene(text_generator);
        bn::core::update();

        music_actions_scene(text_generator);
        bn::core::update();

        sound_scene(text_generator);
        bn::core::update();

        sound_panning_scene(text_generator);
        bn::core::update();
    }
}
