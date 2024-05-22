/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_music_actions.h"
#include "bn_sound_actions.h"
#include "bn_jingle_actions.h"
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

    void jingle_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: play jingle",
            "LEFT: decrease jingle volume",
            "RIGHT: increase jingle volume",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Jingle", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::music_items::cyberrid.play(0.125);

        while(! bn::keypad::start_pressed())
        {
            if(bn::jingle::playing())
            {
                bn::fixed volume = bn::jingle::volume();

                if(bn::keypad::left_held())
                {
                    bn::jingle::set_volume(bn::max(volume - 0.01, bn::fixed(0)));
                }
                else if(bn::keypad::right_held())
                {
                    bn::jingle::set_volume(bn::min(volume + 0.01, bn::fixed(1)));
                }
            }
            else
            {
                if(bn::keypad::a_pressed())
                {
                    bn::jingle::play(bn::music_items::mj_totsnuk01, 0.6);
                }
            }

            info.update();
            bn::core::update();
        }

        bn::music::stop();

        if(bn::jingle::playing())
        {
            bn::jingle::set_volume(0);
        }
    }

    void jingle_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Jingle actions", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::music_items::cyberrid.play(0.125);
        bn::music_items::mj_totsnuk01.play_jingle(0);

        bn::jingle_volume_loop_action action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            if(bn::jingle::playing())
            {
                action.update();
            }
            else
            {
                bn::music_items::mj_totsnuk01.play_jingle(0);
                action = bn::jingle_volume_loop_action(120, 1);
            }

            info.update();
            bn::core::update();
        }

        bn::music::stop();

        if(bn::jingle::playing())
        {
            bn::jingle::set_volume(0);
        }
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

    void sound_master_volume_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease volume",
            "RIGHT: increase volume",
            "",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Sound master volume", info_text_lines, text_generator);
        unsigned counter = 0;
        info.set_show_always(true);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed volume = bn::sound::master_volume();

            if(bn::keypad::left_held())
            {
                bn::sound::set_master_volume(bn::max(volume - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::sound::set_master_volume(bn::min(volume + 0.01, bn::fixed(1)));
            }

            ++counter;

            if(counter % 64 == 0)
            {
                bn::sound_items::alert.play(0.5);
            }

            info.update();
            bn::core::update();
        }

        bn::sound::set_master_volume(1);
    }

    void sound_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sound actions", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::sound_master_volume_loop_action action(120, 0.25);
        unsigned counter = 0;

        while(! bn::keypad::start_pressed())
        {
            ++counter;

            if(counter % 64 == 0)
            {
                bn::sound_items::alert.play(0.5);
            }

            action.update();
            info.update();
            bn::core::update();
        }

        bn::sound::set_master_volume(1);
    }

    void sound_handle_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: restart sound",
            "UP: increase speed",
            "DOWN: decrease speed",
            "LEFT: decrease panning",
            "RIGHT: increase panning",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Sound handle", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::optional<bn::sound_handle> sound_handle;

        while(! bn::keypad::start_pressed())
        {
            if(! sound_handle || ! sound_handle->active())
            {
                sound_handle = bn::sound_items::victory_3.play(0.5);
            }
            else
            {
                bn::fixed speed = sound_handle->speed();
                bn::fixed panning = sound_handle->panning();

                if(bn::keypad::a_pressed())
                {
                    sound_handle->stop();
                }
                else if(bn::keypad::up_held())
                {
                    sound_handle->set_speed(bn::min(speed + 0.01, bn::fixed(64)));
                }
                else if(bn::keypad::down_held())
                {
                    sound_handle->set_speed(bn::max(speed - 0.01, bn::fixed(0.25)));
                }
                else if(bn::keypad::left_held())
                {
                    sound_handle->set_panning(bn::max(panning - 0.01, bn::fixed(-1)));
                }
                else if(bn::keypad::right_held())
                {
                    sound_handle->set_panning(bn::min(panning + 0.01, bn::fixed(1)));
                }
            }

            info.update();
            bn::core::update();
        }

        bn::sound::stop_all();
    }

    void sound_handle_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sound handle actions", info_text_lines, text_generator);
        info.set_show_always(true);

        bn::optional<bn::sound_speed_to_action> action;

        while(! bn::keypad::start_pressed())
        {
            if(! action || ! action->handle().active())
            {
                bn::sound_handle sound_handle = bn::sound_items::victory_3.play(0.5, 0.25, 0);
                action = bn::sound_speed_to_action(sound_handle, 120, 2);
            }
            else
            {
                if(! action->done())
                {
                    action->update();
                }
            }

            info.update();
            bn::core::update();
        }

        bn::sound::stop_all();
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

        jingle_scene(text_generator);
        bn::core::update();

        jingle_actions_scene(text_generator);
        bn::core::update();

        sound_scene(text_generator);
        bn::core::update();

        sound_panning_scene(text_generator);
        bn::core::update();

        sound_master_volume_scene(text_generator);
        bn::core::update();

        sound_actions_scene(text_generator);
        bn::core::update();

        sound_handle_scene(text_generator);
        bn::core::update();

        sound_handle_actions_scene(text_generator);
        bn::core::update();
    }
}
