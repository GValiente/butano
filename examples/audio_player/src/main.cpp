/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_music.h"
#include "bn_keypad.h"
#include "bn_format.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_config_audio.h"
#include "bn_sprite_text_generator.h"

#include "bn_music_items_info.h"
#include "bn_sound_items_info.h"

#include "common_variable_8x16_sprite_font.h"

namespace
{
    constexpr bn::fixed title_y = -69;

    constexpr bn::fixed music_title_y = title_y + 24;
    constexpr bn::fixed music_y = music_title_y + 14;
    constexpr bn::fixed music_volume_y = music_y + 14;
    constexpr bn::fixed music_position_y = music_volume_y + 14;

    constexpr bn::fixed sound_title_y = music_position_y + 24;
    constexpr bn::fixed sound_y = sound_title_y + 14;

    constexpr bn::fixed info_y = 67;

    [[nodiscard]] int mixing_rate()
    {
        switch(BN_CFG_AUDIO_MIXING_RATE)
        {

        case BN_AUDIO_MIXING_RATE_8_KHZ:
            return 8;

        case BN_AUDIO_MIXING_RATE_10_KHZ:
            return 10;

        case BN_AUDIO_MIXING_RATE_13_KHZ:
            return 13;

        case BN_AUDIO_MIXING_RATE_16_KHZ:
            return 16;

        case BN_AUDIO_MIXING_RATE_18_KHZ:
            return 18;

        case BN_AUDIO_MIXING_RATE_21_KHZ:
            return 21;

        case BN_AUDIO_MIXING_RATE_27_KHZ:
            return 27;

        case BN_AUDIO_MIXING_RATE_31_KHZ:
            return 31;

        default:
            BN_ERROR("Invalid maxing rate: ", BN_CFG_AUDIO_MIXING_RATE);
        }
    }

    [[nodiscard]] int max_channels()
    {
        return (BN_CFG_AUDIO_MAX_MUSIC_CHANNELS) + (BN_CFG_AUDIO_MAX_SOUND_CHANNELS);
    }

    void update_music_text(bool selected, int music_item_index, bn::sprite_text_generator& text_generator,
                           bn::ivector<bn::sprite_ptr>& music_sprites)
    {
        music_sprites.clear();
        text_generator.set_center_alignment();

        bn::string_view prefix = selected ? "< " : "";
        bn::string_view suffix = selected ? " >" : "";
        int music_items_count = bn::music_items_info::span.size();

        if(music_item_index < music_items_count)
        {
            bn::string<256> text;
            text.append(prefix);
            text.append(bn::format<16>("{}/{}: ", music_item_index + 1, music_items_count));
            text.append(bn::music_items_info::span[music_item_index].second);
            text.append(suffix);
            text_generator.generate(0, music_y, text, music_sprites);
        }
        else
        {
            text_generator.generate(0, music_y, bn::format<16>("{} --- {}", prefix, suffix), music_sprites);
        }
    }

    void update_music_volume_text(bool selected, int music_volume, bn::sprite_text_generator& text_generator,
                                  bn::ivector<bn::sprite_ptr>& music_volume_sprites)
    {
        music_volume_sprites.clear();
        text_generator.set_center_alignment();

        bn::string_view prefix = selected ? "< " : "";
        bn::string_view suffix = selected ? " >" : "";

        bn::string<32> text;
        text.append(prefix);
        text.append("Volume: ");
        text.append(bn::format<16>("{}%", music_volume));
        text.append(suffix);
        text_generator.generate(0, music_volume_y, text, music_volume_sprites);
    }

    void update_music_position_text(bool selected, int music_position, bn::sprite_text_generator& text_generator,
                                    bn::ivector<bn::sprite_ptr>& music_position_sprites)
    {
        music_position_sprites.clear();
        text_generator.set_center_alignment();

        bn::string_view prefix = selected ? "< " : "";
        bn::string_view suffix = selected ? " >" : "";
        bn::string<32> text;
        text.append(prefix);
        text.append("Position: ");

        if(music_position >= 0)
        {
            text.append(bn::to_string<16>(music_position));
        }
        else
        {
            text.append("---");
        }

        text.append(suffix);
        text_generator.generate(0, music_position_y, text, music_position_sprites);
    }

    void update_sound_text(bool selected, int sound_item_index, bn::sprite_text_generator& text_generator,
                           bn::ivector<bn::sprite_ptr>& sound_sprites)
    {
        sound_sprites.clear();
        text_generator.set_center_alignment();

        bn::string_view prefix = selected ? "< " : "";
        bn::string_view suffix = selected ? " >" : "";
        int sound_items_count = bn::sound_items_info::span.size();

        if(sound_item_index < sound_items_count)
        {
            bn::string<256> text;
            text.append(prefix);
            text.append(bn::format<16>("{}/{}: ", sound_item_index + 1, sound_items_count));
            text.append(bn::sound_items_info::span[sound_item_index].second);
            text.append(suffix);
            text_generator.generate(0, sound_y, text, sound_sprites);
        }
        else
        {
            text_generator.generate(0, sound_y, bn::format<16>("{} --- {}", prefix, suffix), sound_sprites);
        }
    }

    void update_cpu_usage_text(int frame_counter, bn::fixed& max_cpu_usage,
                               bn::sprite_text_generator& text_generator, bn::ivector<bn::sprite_ptr>& cpu_sprites)
    {
        max_cpu_usage = bn::max(max_cpu_usage, bn::core::last_cpu_usage());

        if(frame_counter % 64 == 0)
        {
            int max_cpu_usage_pct = (max_cpu_usage * 100).right_shift_integer();
            cpu_sprites.clear();
            text_generator.set_right_alignment();
            text_generator.generate(112, info_y, bn::format<16>("CPU: {}%", max_cpu_usage_pct), cpu_sprites);
            max_cpu_usage = 0;
        }
    }

    void update_music(int music_volume, int& music_item_index, int& playing_music_item_index,
                      bn::sprite_text_generator& text_generator, bn::ivector<bn::sprite_ptr>& music_sprites)
    {
        if(bn::keypad::left_pressed())
        {
            if(music_item_index > 0)
            {
                --music_item_index;
            }
            else
            {
                music_item_index = bn::max(bn::music_items_info::span.size() - 1, 0);
            }

            update_music_text(true, music_item_index, text_generator, music_sprites);
        }
        else if(bn::keypad::right_pressed())
        {
            if(music_item_index < bn::music_items_info::span.size() - 1)
            {
                ++music_item_index;
            }
            else
            {
                music_item_index = 0;
            }

            update_music_text(true, music_item_index, text_generator, music_sprites);
        }
        else if(bn::keypad::a_pressed())
        {
            if(playing_music_item_index >= 0)
            {
                if(playing_music_item_index == music_item_index)
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
                else
                {
                    bn::music_items_info::span[music_item_index].first.play(bn::fixed(music_volume) / 100);
                    playing_music_item_index = music_item_index;
                }
            }
            else
            {
                if(music_item_index < bn::music_items_info::span.size())
                {
                    bn::music_items_info::span[music_item_index].first.play(bn::fixed(music_volume) / 100);
                    playing_music_item_index = music_item_index;
                }
            }
        }
        else if(bn::keypad::b_pressed())
        {
            if(playing_music_item_index >= 0)
            {
                bn::music::stop();
                playing_music_item_index = -1;
            }
        }
    }

    void update_music_volume(int& music_volume, bn::sprite_text_generator& text_generator,
                             bn::ivector<bn::sprite_ptr>& music_volume_sprites)
    {
        int old_music_volume = music_volume;

        if(bn::keypad::left_pressed())
        {
            if(old_music_volume > 5)
            {
                music_volume -= 5;
            }
        }
        else if(bn::keypad::right_pressed())
        {
            if(old_music_volume < 100)
            {
                music_volume += 5;
            }
        }

        if(music_volume != old_music_volume)
        {
            if(bn::music::playing())
            {
                bn::music::set_volume(bn::fixed(music_volume) / 100);
            }

            update_music_volume_text(true, music_volume, text_generator, music_volume_sprites);
        }
    }

    void update_music_position(bool selected, int& last_music_position, bool& last_music_position_selected,
                               bn::sprite_text_generator& text_generator,
                               bn::ivector<bn::sprite_ptr>& music_position_sprites)
    {
        int position;

        if(bn::music::playing())
        {
            position = bn::music::position();

            if(selected)
            {
                if(bn::keypad::left_pressed())
                {
                    if(position > 0)
                    {
                        --position;
                        bn::music::set_position(position);
                    }
                }
                else if(bn::keypad::right_pressed())
                {
                    ++position;
                    bn::music::set_position(position);
                }
            }
        }
        else
        {
            position = -1;
        }

        if(position != last_music_position || selected != last_music_position_selected)
        {
            last_music_position = position;
            last_music_position_selected = selected;
            update_music_position_text(selected, position, text_generator, music_position_sprites);
        }
    }

    void update_sound(int& sound_item_index, bn::sprite_text_generator& text_generator,
                      bn::ivector<bn::sprite_ptr>& sound_sprites)
    {
        if(bn::keypad::left_pressed())
        {
            if(sound_item_index > 0)
            {
                --sound_item_index;
            }
            else
            {
                sound_item_index = bn::max(bn::sound_items_info::span.size() - 1, 0);
            }

            update_sound_text(true, sound_item_index, text_generator, sound_sprites);
        }
        else if(bn::keypad::right_pressed())
        {
            if(sound_item_index < bn::sound_items_info::span.size() - 1)
            {
                ++sound_item_index;
            }
            else
            {
                sound_item_index = 0;
            }

            update_sound_text(true, sound_item_index, text_generator, sound_sprites);
        }
        else if(bn::keypad::a_pressed())
        {
            if(sound_item_index < bn::sound_items_info::span.size())
            {
                bn::sound_items_info::span[sound_item_index].first.play();
            }
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    bn::vector<bn::sprite_ptr, 16> static_sprites;
    text_generator.set_center_alignment();
    text_generator.generate(0, title_y, "AUDIO PLAYER", static_sprites);
    text_generator.generate(0, music_title_y, "MUSIC", static_sprites);
    text_generator.generate(0, sound_title_y, "SOUND", static_sprites);

    text_generator.set_left_alignment();
    text_generator.generate(-112, info_y, bn::format<16>("{} KHZ", mixing_rate()), static_sprites);

    text_generator.set_center_alignment();
    text_generator.generate(0, info_y, bn::format<16>("{} max ch", max_channels()), static_sprites);

    bn::vector<bn::sprite_ptr, 32> music_sprites;
    int option_index = 0;
    int music_item_index = 0;
    int playing_music_item_index = -1;
    update_music_text(true, music_item_index, text_generator, music_sprites);

    bn::vector<bn::sprite_ptr, 8> music_volume_sprites;
    int music_volume = 50;
    update_music_volume_text(false, music_volume, text_generator, music_volume_sprites);

    bn::vector<bn::sprite_ptr, 8> music_position_sprites;
    int last_music_position = -1;
    bool last_music_position_selected = false;
    update_music_position_text(last_music_position_selected, last_music_position,
                               text_generator, music_position_sprites);

    bn::vector<bn::sprite_ptr, 32> sound_sprites;
    int sound_item_index = 0;
    update_sound_text(false, sound_item_index, text_generator, sound_sprites);

    bn::vector<bn::sprite_ptr, 8> cpu_sprites;
    bn::fixed max_cpu_usage;
    int frame_counter = 0;

    while(true)
    {
        switch(option_index)
        {

        case 0:
            if(bn::keypad::up_pressed())
            {
                option_index = 3;
                update_music_text(false, music_item_index, text_generator, music_sprites);
                update_sound_text(true, sound_item_index, text_generator, sound_sprites);
            }
            else if(bn::keypad::down_pressed())
            {
                ++option_index;
                update_music_text(false, music_item_index, text_generator, music_sprites);
                update_music_volume_text(true, music_volume, text_generator, music_volume_sprites);
            }
            else
            {
                update_music(music_volume, music_item_index, playing_music_item_index,
                             text_generator, music_sprites);
            }
            break;

        case 1:
            if(bn::keypad::up_pressed())
            {
                --option_index;
                update_music_volume_text(false, music_volume, text_generator, music_volume_sprites);
                update_music_text(true, music_item_index, text_generator, music_sprites);
            }
            else if(bn::keypad::down_pressed())
            {
                ++option_index;
                update_music_volume_text(false, music_volume, text_generator, music_volume_sprites);
            }
            else
            {
                update_music_volume(music_volume, text_generator, music_volume_sprites);
            }
            break;

        case 2:
            if(bn::keypad::up_pressed())
            {
                --option_index;
                update_music_volume_text(true, music_volume, text_generator, music_volume_sprites);
            }
            else if(bn::keypad::down_pressed())
            {
                ++option_index;
                update_sound_text(true, sound_item_index, text_generator, sound_sprites);
            }
            break;

        case 3:
            if(bn::keypad::up_pressed())
            {
                --option_index;
                update_sound_text(false, sound_item_index, text_generator, sound_sprites);
            }
            else if(bn::keypad::down_pressed())
            {
                option_index = 0;
                update_sound_text(false, sound_item_index, text_generator, sound_sprites);
                update_music_text(true, music_item_index, text_generator, music_sprites);
            }
            else
            {
                update_sound(sound_item_index, text_generator, sound_sprites);
            }
            break;

        default:
            BN_ERROR("Invalid option index: ", option_index);
            break;
        }

        update_music_position(option_index == 2, last_music_position, last_music_position_selected,
                              text_generator, music_position_sprites);

        update_cpu_usage_text(frame_counter, max_cpu_usage, text_generator, cpu_sprites);

        ++frame_counter;
        bn::core::update();
    }
}
