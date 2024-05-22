/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_pause.h"

#include "bn_music.h"
#include "bn_colors.h"
#include "bn_keypad.h"
#include "bn_rumble.h"
#include "bn_green_swap.h"
#include "bn_sprite_builder.h"

#include "fr_announcer.h"
#include "fr_scene_type.h"
#include "fr_common_stuff.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_pause_background.h"

namespace fr
{

namespace
{
    constexpr bn::fixed text_x = -34;
    constexpr bn::fixed initial_y = -28;
    constexpr bn::fixed light_y = initial_y + 11 + 6;
    constexpr bn::fixed rumble_y = light_y + 11;
    constexpr bn::fixed restart_y = rumble_y + 11 + 6;
    constexpr bn::fixed exit_y = restart_y + 11;

    [[nodiscard]] bn::sprite_ptr _create_cursor_sprite(common_stuff& common_stuff)
    {
        bn::vector<bn::sprite_ptr, 1> output_sprites;
        bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
        text_generator.set_bg_priority(0);
        text_generator.set_one_sprite_per_character(true);
        text_generator.generate(text_x - 10, light_y, ">", output_sprites);
        text_generator.set_one_sprite_per_character(false);
        return bn::move(output_sprites[0]);
    }

    [[nodiscard]] bn::sprite_ptr _create_sprite(bn::fixed x, bn::fixed y, const bn::string_view& text,
                                                common_stuff& common_stuff)
    {
        bn::vector<bn::sprite_ptr, 1> output_sprites;
        bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
        text_generator.generate(x, y, text, output_sprites);
        return bn::move(output_sprites[0]);
    }
}

pause::pause(common_stuff& common_stuff) :
    _background_tiles(bn::sprite_items::pause_background.tiles_item().create_tiles()),
    _background_palette(bn::sprite_items::pause_background.palette_item().create_palette()),
    _cursor_sprite(_create_cursor_sprite(common_stuff)),
    _low_brightness_sprite(_create_sprite(text_x + 55, light_y, "LOW", common_stuff)),
    _medium_brightness_sprite(_create_sprite(text_x + 55, light_y, "MED", common_stuff)),
    _high_brightness_sprite(_create_sprite(text_x + 55, light_y, "HI", common_stuff)),
    _rumble_enabled_sprite(_create_sprite(text_x + 55, rumble_y, "ON", common_stuff)),
    _rumble_disabled_sprite(_create_sprite(text_x + 55, rumble_y, "OFF", common_stuff))
{
    bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
    text_generator.generate(text_x + 17, initial_y, "PAUSE", _text_sprites);
    text_generator.generate(text_x, light_y, "LIGHT:", _text_sprites);
    text_generator.generate(text_x, rumble_y, "RUMBLE:", _text_sprites);
    text_generator.generate(text_x, restart_y, "RESTART", _text_sprites);
    text_generator.generate(text_x, exit_y, "EXIT", _text_sprites);

    _hide_text();
    text_generator.set_bg_priority(1);
}

bn::optional<scene_type> pause::update(common_stuff& common_stuff, announcer& announcer)
{
    bn::optional<scene_type> result;

    if(_bg_fade_action)
    {
        if(_bg_fade_action->done())
        {
            if(! announcer.talking())
            {
                if(_wait_counter)
                {
                    --_wait_counter;
                }
                else
                {
                    _bg_fade_action.reset();
                    _sprite_fade_action.reset();
                    bn::bg_palettes::set_fade_intensity(0);
                    bn::sprite_palettes::set_fade_intensity(0);

                    if(_restart)
                    {
                        result = scene_type::RACE;
                    }
                    else
                    {
                        result = scene_type::TITLE;
                    }
                }
            }
        }
        else
        {
            _bg_fade_action->update();
            _sprite_fade_action->update();
        }
    }
    else if(_animation_inc)
    {
        ++_animation_index;
        _show_animation(common_stuff);
    }
    else if(_animation_dec)
    {
        --_animation_index;
        _show_animation(common_stuff);
    }
    else
    {
        sram_data& sram_data = common_stuff.storage;

        if(bn::keypad::start_pressed() || bn::keypad::b_pressed())
        {
            if(paused())
            {
                _animation_inc = false;
                _animation_dec = true;
                sram_data.write();
                bn::sound_items::blip05.play();
            }
            else if(bn::keypad::start_pressed())
            {
                _rumble = bn::rumble::enabled();
                _green_swap = bn::green_swap::enabled();

                if(_rumble)
                {
                    bn::rumble::set_enabled(false);
                }

                if(_green_swap)
                {
                    bn::green_swap::set_enabled(false);
                }

                if(bn::music::playing())
                {
                    bn::music::pause();
                }

                _animation_inc = true;
                _animation_dec = false;
                _cursor_index = 0;
                ++_animation_index;
                _show_animation(common_stuff);
                announcer.clear_queue();
                bn::sound_items::blip06.play();
            }
        }
        else if(! _background_sprites.empty())
        {
            _menu_keypad.update();

            bool a_pressed = bn::keypad::a_pressed();
            bool left_pressed = _menu_keypad.left();
            bool right_pressed = _menu_keypad.right();

            if(a_pressed || left_pressed || right_pressed)
            {
                switch(_cursor_index)
                {

                case 0:
                    switch(sram_data.brightness())
                    {

                    case brightness_level::LOW:
                        if(a_pressed || right_pressed)
                        {
                            sram_data.set_brightness(brightness_level::MEDIUM);
                            _show_brightness(common_stuff);
                            bn::sound_items::blip04.play();
                        }
                        break;

                    case brightness_level::MEDIUM:
                        if(left_pressed)
                        {
                            sram_data.set_brightness(brightness_level::LOW);
                        }
                        else
                        {
                            sram_data.set_brightness(brightness_level::HIGH);
                        }

                        _show_brightness(common_stuff);
                        bn::sound_items::blip04.play();
                        break;

                    case brightness_level::HIGH:
                        if(a_pressed)
                        {
                            sram_data.set_brightness(brightness_level::LOW);
                            _show_brightness(common_stuff);
                            bn::sound_items::blip04.play();
                        }
                        else if(left_pressed)
                        {
                            sram_data.set_brightness(brightness_level::MEDIUM);
                            _show_brightness(common_stuff);
                            bn::sound_items::blip04.play();
                        }
                        break;

                    default:
                        BN_ERROR("Invalid brightness: ", int(sram_data.brightness()));
                        break;
                    }
                    break;

                case 1:
                    if(sram_data.rumble_allowed())
                    {
                        if(a_pressed || right_pressed)
                        {
                            sram_data.set_rumble_allowed(false);
                            _show_rumble(common_stuff);
                            bn::sound_items::blip04.play();
                        }
                    }
                    else
                    {
                        if(a_pressed || left_pressed)
                        {
                            sram_data.set_rumble_allowed(true);
                            _show_rumble(common_stuff);
                            bn::sound_items::blip04.play();
                        }
                    }
                    break;

                case 2:
                    if(a_pressed)
                    {
                        _exit(true, announcer);
                        sram_data.write();
                    }
                    break;

                case 3:
                    if(a_pressed)
                    {
                        _exit(false, announcer);
                        sram_data.write();
                    }
                    break;

                default:
                    BN_ERROR("Invalid cursor index: ", _cursor_index);
                    break;
                }
            }
            else if(_menu_keypad.up())
            {
                if(_cursor_index > 0)
                {
                    --_cursor_index;
                    _show_cursor();
                    bn::sound_items::blip02.play();
                }
            }
            else if(_menu_keypad.down())
            {
                if(_cursor_index < 3)
                {
                    ++_cursor_index;
                    _show_cursor();
                    bn::sound_items::blip02.play();
                }
            }
        }
    }

    return result;
}

void pause::_show_animation(const common_stuff& common_stuff)
{
    switch(_animation_index)
    {

    case 0:
        if(_rumble)
        {
            bn::rumble::set_enabled(true);
        }

        if(_green_swap)
        {
            bn::green_swap::set_enabled(true);
        }

        if(bn::music::paused())
        {
            bn::music::resume();
        }

        _background_sprites.clear();
        _animation_dec = false;
        break;

    case 1:
        _show_background(1);
        break;

    case 2:
        _show_background(2);
        break;

    case 3:
        _show_background(3);
        break;

    case 4:
        _show_background(4);
        break;

    case 5:
        _show_background(5);
        break;

    case 6:
        _show_background(6);
        break;

    case 7:
        _show_background(7);
        break;

    case 8:
        if(_animation_inc)
        {
            _show_text(common_stuff);
        }
        else
        {
            _hide_text();
        }
        break;

    case 9:
        _menu_keypad.reset();
        _animation_inc = false;
        break;

    default:
        BN_ERROR("Invalid animation index: ", _animation_index);
        break;
    }
}

void pause::_show_background(int size)
{
    bn::sprite_builder builder(bn::sprite_items::pause_background.shape_size(), _background_tiles,
                               _background_palette);
    builder.set_bg_priority(0);
    builder.set_z_order(1);
    _background_sprites.clear();

    int x_size = bn::min(size, 7);
    int y_size = bn::min(size, 5);

    bn::fixed sprite_start_x = (-bn::fixed(x_size - 1) / 2) * 15;
    bn::fixed sprite_y = (-bn::fixed(y_size - 1) / 2) * 15;

    for(int y = 0; y < y_size; ++y)
    {
        bn::fixed sprite_x = sprite_start_x;
        builder.set_y(sprite_y);
        sprite_y += 15;

        for(int x = 0; x < x_size; ++x)
        {
            builder.set_x(sprite_x);
            sprite_x += 15;
            _background_sprites.push_back(builder.build());
        }
    }
}

void pause::_show_cursor()
{
    switch(_cursor_index)
    {

    case 0:
        _cursor_sprite.set_y(light_y);
        break;

    case 1:
        _cursor_sprite.set_y(rumble_y);
        break;

    case 2:
        _cursor_sprite.set_y(restart_y);
        break;

    case 3:
        _cursor_sprite.set_y(exit_y);
        break;

    default:
        BN_ERROR("Invalid cursor index: ", _cursor_index);
        break;
    }

    _cursor_sprite.set_visible(true);
}

void pause::_show_rumble(const common_stuff& common_stuff)
{
    bool rumble_allowed = common_stuff.storage.rumble_allowed();
    _rumble_enabled_sprite.set_visible(rumble_allowed);
    _rumble_disabled_sprite.set_visible(! rumble_allowed);
}

void pause::_show_brightness(const common_stuff& common_stuff)
{
    brightness_level brightness = common_stuff.storage.brightness();

    switch(brightness)
    {

    case brightness_level::LOW:
        _low_brightness_sprite.set_visible(true);
        _medium_brightness_sprite.set_visible(false);
        _high_brightness_sprite.set_visible(false);
        break;

    case brightness_level::MEDIUM:
        _low_brightness_sprite.set_visible(false);
        _medium_brightness_sprite.set_visible(true);
        _high_brightness_sprite.set_visible(false);
        break;

    case brightness_level::HIGH:
        _low_brightness_sprite.set_visible(false);
        _medium_brightness_sprite.set_visible(false);
        _high_brightness_sprite.set_visible(true);
        break;

    default:
        BN_ERROR("Invalid brightness: ", int(brightness));
        break;
    }
}

void pause::_show_text(const common_stuff& common_stuff)
{
    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_visible(true);
    }

    _show_cursor();
    _show_rumble(common_stuff);
    _show_brightness(common_stuff);
}

void pause::_hide_text()
{
    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_visible(false);
    }

    _cursor_sprite.set_visible(false);
    _low_brightness_sprite.set_visible(false);
    _medium_brightness_sprite.set_visible(false);
    _high_brightness_sprite.set_visible(false);
    _rumble_enabled_sprite.set_visible(false);
    _rumble_disabled_sprite.set_visible(false);
}

void pause::_exit(bool restart, announcer& announcer)
{
    _bg_fade_action.emplace(60, 1);
    _sprite_fade_action.emplace(60, 1);
    _restart = restart;
    bn::sound_items::blip09.play();

    if(restart)
    {
        announcer.play_restart_voice(30);
        _wait_counter = 0;
    }
    else
    {
        announcer.play_exit_voice(30);
        _wait_counter = 30;
    }
}

}
